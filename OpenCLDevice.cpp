#include "OpenCLDevice.h"



OpenCLDevice::OpenCLDevice()
{
}


OpenCLDevice::~OpenCLDevice()
{
}

void OpenCLDevice::InitializeDevice()
{
	cl_uint platformIdCount = 0;
	clGetPlatformIDs(0, nullptr, &platformIdCount);
	std::vector<cl_platform_id> platformIds(platformIdCount);
	clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);
	deviceIdCount = 0;
	int platformId = platformIdCount - 1;
	clGetDeviceIDs(platformIds[platformId], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceIdCount);
	deviceIds = new std::vector<cl_device_id>(deviceIdCount);
	clGetDeviceIDs(platformIds[platformId], CL_DEVICE_TYPE_ALL, deviceIdCount, deviceIds->data(), nullptr);
	for (int i = 0; i < deviceIdCount; i++)
	{
		std::cout << "Device nr: " << i << std::endl;
		size_t pointerSize = 0;
		char *pointer;
		clGetDeviceInfo(deviceIds->at(i), CL_DEVICE_NAME, 0, nullptr, &pointerSize);
		pointer = (char*)malloc(pointerSize);
		clGetDeviceInfo(deviceIds->at(i), CL_DEVICE_NAME, pointerSize, pointer, nullptr);
		std::cout << pointer << std::endl;
		delete[] pointer;
	}
	const cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM,
		reinterpret_cast<cl_context_properties> (platformIds[platformId]),
		0, 0
	};
	cl_int error;
	context = new cl_context;
	*context = clCreateContext(
		contextProperties, deviceIdCount,
		deviceIds->data(), nullptr,
		nullptr, &error);
	isDeviceInitialized = true;
	cl_ulong pointerSize=0;
	int test = sizeof(pointerSize);
	clGetDeviceInfo(deviceIds->at(deviceIndex), CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(pointerSize), &pointerSize, nullptr);
	maxBufferSize = pointerSize / 3;
}
