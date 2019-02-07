#include "vsplitter.h"

std::string LoadKernel(const char* name)
{
	std::ifstream in(name);
	std::string result((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	return result;
}

vsplitter::vsplitter(OpenCLDevice *devicein)
{
	device = devicein;
	CreateProgram(LoadKernel("sumv.cl"));
}

vsplitter::~vsplitter()
{
}

void vsplitter::Analyze(cv::Mat *image_in)
{
	Clear();
	image = image_in;
	if (image->rows*image->cols * 3 > device->maxBufferSize)
		return;
	SumDiffV(25);
	PickPoints(0.25);
	FindHarmonics();
	Interpretation(0.5);
}

void CheckError(cl_int error)
{
	if (error != CL_SUCCESS) {
		std::cerr << "OpenCL call failed with error " << error << std::endl;
	}
}

void vsplitter::CreateProgram(const std::string& source)
{
	size_t lengths[1] = { source.size() };
	const char* sources[1] = { source.data() };
	cl_int error = 0;
	program = new cl_program;
	*program = clCreateProgramWithSource(*device->context, 1, sources, lengths, &error);
	CheckError(error);
	CheckError(clBuildProgram(*program, device->deviceIdCount, device->deviceIds->data(), nullptr, nullptr, nullptr));
	size_t size;
	clGetProgramBuildInfo(*program, device->deviceIds->at(device->deviceIndex), CL_PROGRAM_BUILD_LOG, 0, nullptr, &size);
	char *log = (char*)malloc(size);
	clGetProgramBuildInfo(*program, device->deviceIds->at(device->deviceIndex), CL_PROGRAM_BUILD_LOG, size, log, nullptr);
}

void vsplitter::SumDiffV(int thresh)
{
	cl_int error;
	cl_kernel kernel = clCreateKernel(*program, "SumV", &error);
	CheckError(error);
	cl_mem imgBuffer = clCreateBuffer(*device->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uchar)*image->cols*image->rows * 3, (void*)image->data, &error);
	cl_mem diffBuffer = clCreateBuffer(*device->context, CL_MEM_READ_WRITE, sizeof(float) * (image->rows), nullptr, &error);
	cl_mem threshBuffer = clCreateBuffer(*device->context, CL_MEM_READ_WRITE, sizeof(float)*image->rows, nullptr, &error);
	CheckError(error);
	int width = image->cols;
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &imgBuffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &diffBuffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &threshBuffer);
	clSetKernelArg(kernel, 3, sizeof(width), &width);
	clSetKernelArg(kernel, 4, sizeof(thresh), &thresh);
	const size_t globalWorkSize[] = { image->rows-1, 0, 0 };
	cl_command_queue queue = clCreateCommandQueue(*device->context, device->deviceIds->at(device->deviceIndex), 0, &error);
	CheckError(error);
	CheckError(clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
	clReleaseMemObject(imgBuffer);
	diffList = new std::vector<float> (image->rows);
	threshList = new std::vector<float> (image->rows);
	CheckError(clEnqueueReadBuffer(queue, diffBuffer, CL_TRUE, 0, sizeof(float) * image->rows, diffList->data(), 0, nullptr, nullptr));
	CheckError(clEnqueueReadBuffer(queue, threshBuffer, CL_TRUE, 0, sizeof(float) * image->rows, threshList->data(), 0, nullptr, nullptr));

	kernel = clCreateKernel(*program, "DiffSquare", &error);
	average=0;
	cl_mem diffSqrBuffer = clCreateBuffer(*device->context, CL_MEM_READ_WRITE, sizeof(float)*image->rows, nullptr, &error);
	for (int i = 0; i < image->rows; i++)
		average += diffList->at(i);
	average = average / (float)image->rows;
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &diffBuffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &diffSqrBuffer);
	clSetKernelArg(kernel, 2, sizeof(average), &average);
	const size_t globalWorkSize2[] = { image->rows, 0, 0 };
	CheckError(clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, globalWorkSize2, nullptr, 0, nullptr, nullptr));
	std::vector<float> diffsqr(image->rows);
	clEnqueueReadBuffer(queue, diffSqrBuffer, CL_TRUE, 0, sizeof(float)*image->rows, diffsqr.data(), 0, nullptr, nullptr);
	stddev = 0;
	for (int i = 0; i < image->rows; i++)
		stddev += diffsqr.at(i);
	stddev = sqrt(stddev / (float)(image->rows - 1));

	float scale = 0.33;
	kernel = clCreateKernel(*program, "Score", &error);
	cl_mem scoreBuffer = clCreateBuffer(*device->context, CL_MEM_READ_WRITE, sizeof(float)*image->rows, nullptr, &error);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &diffBuffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &threshBuffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &scoreBuffer);
	clSetKernelArg(kernel, 3, sizeof(average), &average);
	clSetKernelArg(kernel, 4, sizeof(stddev), &stddev);
	clSetKernelArg(kernel, 5, sizeof(scale), &scale);
	const size_t globalWorkSize3[] = { image->rows,0,0 };
	CheckError(clEnqueueNDRangeKernel(queue,kernel,1,nullptr,globalWorkSize3,nullptr,0,nullptr,nullptr));
	scoreList = new std::vector<float>(image->rows);
	clEnqueueReadBuffer(queue, scoreBuffer, CL_TRUE, 0, sizeof(float)*image->rows, scoreList->data(), 0, nullptr, nullptr);

	clReleaseCommandQueue(queue);
	clReleaseMemObject(imgBuffer);
	clReleaseMemObject(scoreBuffer);
	clReleaseMemObject(diffBuffer);
	clReleaseMemObject(threshBuffer);
	clReleaseMemObject(diffSqrBuffer);
	clReleaseKernel(kernel);
}

//void vsplitter::InitializeContext()
//{
//	cl_uint platformIdCount = 0;
//	clGetPlatformIDs(0, nullptr, &platformIdCount);
//	std::vector<cl_platform_id> platformIds(platformIdCount);
//	clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);
//	deviceIdCount = 0;
//	int platformId = platformIdCount - 1;
//	clGetDeviceIDs(platformIds[platformId], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceIdCount);
//	deviceIds = new std::vector<cl_device_id>(deviceIdCount);
//	clGetDeviceIDs(platformIds[platformId], CL_DEVICE_TYPE_ALL, deviceIdCount, deviceIds->data(), nullptr);
//	for (int i = 0; i < deviceIdCount; i++)
//	{
//		std::cout << "Device nr: " << i << std::endl;
//		size_t pointerSize = 0;
//		char *pointer;
//		clGetDeviceInfo(deviceIds->at(i), CL_DEVICE_NAME, 0, nullptr, &pointerSize);
//		pointer = (char*)malloc(pointerSize);
//		clGetDeviceInfo(deviceIds->at(i), CL_DEVICE_NAME, pointerSize, pointer, nullptr);
//		std::cout << pointer << std::endl;
//		delete[] pointer;
//	}
//	const cl_context_properties contextProperties[] =
//	{
//		CL_CONTEXT_PLATFORM,
//		reinterpret_cast<cl_context_properties> (platformIds[platformId]),
//		0, 0
//	};
//	cl_int error;
//	context = new cl_context;
//	*context = clCreateContext(
//		contextProperties, deviceIdCount,
//		deviceIds->data(), nullptr,
//		nullptr, &error);
//}

void vsplitter::PickPoints(float thresh)
{
	std::vector<int> pointstemp;
	std::vector<float> scorestemp;
	for (int i = 0; i < image->rows; i++)
	{
		if (scoreList->at(i) > thresh)
		{
			pointstemp.push_back(i);
			scorestemp.push_back(scoreList->at(i));
		}
	}

	for (int i = 0; i < pointstemp.size(); i++)
	{
		int k=0;
		for (k; k + i < pointstemp.size()-1;)
		{
			if (pointstemp.at(i + k + 1) - pointstemp.at(i + k) <= 2)
				k++;
			else
				break;
		}
		if (k != 0)
		{
			int topIndex = -1;
			double topScore = 0;
			for (int j = 0; j < k; j++)
			{
				if (scorestemp.at(i + j) > topScore)
				{
					topIndex = j + i;
					topScore = scorestemp.at(topIndex);
				}
			}
			points.push_back(pointstemp.at(topIndex));
			scores.push_back(scorestemp.at(topIndex));
			i += k;
		}
		else
		{
			points.push_back(pointstemp.at(i));
			scores.push_back(scorestemp.at(i));
		}
	}
	points.push_back(image->rows - 1);
	scores.push_back(0.75);
}

int FindNearestIndex(std::vector<int> input,int value)
{
	int i;
	for (i = 0; i < input.size(); i++)
	{
		if (input.at(i) >= value)
			if (i > 0)
			{
				if (abs(value - input[i - 1]) < abs(input[i] - value))
					return i-1;
				else
					return i;
			}
			else
				return i;
	}
	return i-1;
}

int FindUpperIndex(std::vector<int> input, int value)
{
	int i;
	for (i = 0; i < input.size(); i++)
	{
		if (input.at(i) > value)
			return i;
	}
	return -1;
}

int FindLowerIndex(std::vector<int> input, int value)
{
	int i;
	for (i = 0; i < input.size(); i++)
	{
		if (input.at(i) > value)
		{
			if (i == 0)
				return -1;
			else
				return i - 1;
		}
	}
	return i - 1;
}

float MyStandardDeviation(std::vector<int> arr)
{
	double stddev = 0;
	double average = 0;
	int k=0;
	for (int i = 0; i < arr.size(); i++)
		if (arr.at(i) != 0)
		{
			average += arr.at(i);
			k++;
		}
	if (k < 2)
		return -1;
	average = average / k;
	for (int i=0;i<arr.size();i++)
		if (arr.at(i) != 0)
		{
			double diff = arr.at(i) - average;
			stddev += diff * diff;
			stddev = sqrt(stddev / (k - 1));
		}
	return stddev;
}

int HowMany(std::vector<int> arr,int number)
{
	int k = 0;
	for (int i = 0; i < arr.size(); i++)
		if (arr.at(i) == number)
			k++;
	return k;
}

double MySum(std::vector<double> arr)
{
	double score = 0;
	int k = 0;
	for (int i = 0; i < arr.size(); i++)
	{
		score += arr.at(i);
		if (arr.at(i) == 0)
			k++;
	}
	if (arr.size() == 0)
		return 0;
	score = score * (1 - (k / arr.size()));
	score = score * 0.33;
	score = score / (1 + fabs(score));
	return score;
}

void vsplitter::FindHarmonics()
{
	double maxratio = 2.5;
	double minHarmonic = floor((double)image->rows / (image->cols*maxratio));
	double maxHarmonic = ceil((double)image->rows / ((double)image->cols / maxratio));
	for (int i = 0; i < minHarmonic; i++)
		harmonicScores.push_back(0);
	for (int harmonic = minHarmonic; harmonic < maxHarmonic; harmonic++)
	{
		double increment = (double)image->rows / harmonic;
		double harmonicScore1 = 0;
		double harmonicScore2 = 0;
		std::vector<int> dstUp;
		std::vector<int> dstDown;
		std::vector<double> dstScores;
		for (int i = 1; i < harmonic; i++)
		{
			int height = increment * i;
			int nearestIndex = FindNearestIndex(points, height);
			if (nearestIndex != -1)
				harmonicScore1 += (2 * scores.at(nearestIndex) / cv::max(abs((double)(points.at(nearestIndex) - height))/3, 1.0) - scores.at(nearestIndex)) / (harmonic-1);
 			int upIndex = FindUpperIndex(points, height);
			int downIndex = FindLowerIndex(points, height);
			if (downIndex != -1)
				dstDown.push_back(height - points.at(downIndex));
			else
				dstDown.push_back(0);
			if (upIndex != -1)
				dstUp.push_back(points.at(upIndex) - height);
			else
				dstUp.push_back(0);
			if (downIndex != -1 && upIndex != -1)
				dstScores.push_back(scores.at(downIndex) + scores.at(upIndex));
			else
				dstScores.push_back(0);
		}
		for (int i = 0; i < dstScores.size(); i++)
		{
			int thresh = image->cols / 10;
			if (dstDown.at(i) > thresh)
			{
				dstDown.at(i) = 0;
				dstScores.at(i) = 0;
			}
			if (dstUp.at(i) > thresh)
			{
				dstUp.at(i) = 0;
				dstScores.at(i) = 0;
			}
		}
		double stddevDown = MyStandardDeviation(dstDown);
		double stddevUp = MyStandardDeviation(dstUp);
		double score = MySum(dstScores);
		int zeros = 0;
		zeros += HowMany(dstDown, 0);
		zeros += HowMany(dstUp, 0);
		if (stddevDown != -1 || stddevUp != -1)
			harmonicScore2 = score / cv::max(1.0, (stddevDown + stddevUp)/3) - (double)zeros / (2*(harmonic - 1));
		else
			if (harmonic == 2 && dstDown.at(0) != 0 && dstUp.at(0) != 0)
				harmonicScore2 = 1;
		harmonicScores.push_back(cv::max(harmonicScore1, harmonicScore2));
	}
}
void vsplitter::Interpretation(float thresh)
{
	int harmonic = -1;
	for (int i = 0; i < harmonicScores.size(); i++)
		if (harmonicScores.at(i) > thresh)
			harmonic = i;
	if (harmonic != -1)
	{
		for (int i = 0; i < harmonic; i++)
			output.push_back((double)image->rows / harmonic * (i + 1));
	}
	else
	{
		double maxratio = 2;
		int height = 0;
		int upperThreshold = height + maxratio * image->cols;
		int lowerThreshold = height + image->cols / maxratio;
		while (lowerThreshold < image->rows)
		{
			upperThreshold = height + maxratio * image->cols;
			lowerThreshold = height + image->cols / maxratio;
			int k=0,startIndex=-1;
			for (int i = 0; i < points.size(); i++)
				if (points.at(i) >= lowerThreshold && points.at(i) <= upperThreshold)
				{
					if (startIndex == -1)
						startIndex = i;
					k++;
				}
			if (k != 0)
			{
				double maxScore = 0;
				int topIndex = -1;
				for (int i = 0; i < k; i++)
				{
					if (scores.at(startIndex + i) > maxScore)
					{
						maxScore = scores.at(startIndex + i);
						topIndex = startIndex + i;
					}
				}
				output.push_back(points.at(topIndex));
				height = points.at(topIndex);
			}
			else if (upperThreshold < image->rows)
			{
				output.clear();
				return;
			}
			else
				return;
		}
		
	}
}
void vsplitter::Clear()
{
	if (diffList != nullptr)
		delete diffList;
	if (threshList != nullptr)
		delete threshList;
	if (scoreList != nullptr)
		delete scoreList;
	points.clear();
	scores.clear();
	harmonicScores.clear();
	output.clear();
}
