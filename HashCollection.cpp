#include "HashCollection.h"



HashCollection::HashCollection(OpenCLDevice *device)
{
	algo = cv::img_hash::PHash::create();
	splitter = new vsplitter(device);
	size = 0;
}

HashCollection::HashCollection()
{

}

HashCollection::~HashCollection()
{
	for (int i = 0; i < paths.size(); i++)
		delete paths.at(i);
	for (int i = 0; i < hashes.size(); i++)
		delete hashes.at(i);
	delete splitter;
}

//cv::Mat *computeHash(cv::Mat image, cv::Ptr<cv::img_hash::ImgHashBase> algo)
//{
//	cv::Mat *hash = new cv::Mat();
//	algo->compute(image, *hash);
//	return hash;
//}
//
//double compareHash(cv::Mat hash1, cv::Mat hash2, cv::Ptr<cv::img_hash::ImgHashBase> algo)
//{
//	return algo->compare(hash1, hash2);
//}

void HashCollection::Add(const wchar_t *filename)
{
	std::vector<cv::Mat*> databaseHashes;
	if (database != nullptr)
		databaseHashes = database->FindHash(filename);
	if(databaseHashes.size()!=0)
		for (int i = 0; i < databaseHashes.size(); i++)
		{
			paths.push_back(wcsdup(filename));
			cv::Mat *newMat = new cv::Mat();
			*newMat = *databaseHashes.at(i);
			hashes.push_back(newMat);
			size++;
		}
	else
	{
		cv::Mat img = ReadImage(filename);
		if (img.empty())
			return;
		splitter->Analyze(&img);
		if (splitter->output.size() == 0)
		{
			cv::Mat *hash = new cv::Mat;
			algo->compute(img, *hash);
			paths.push_back(wcsdup(filename));
			hashes.push_back(hash);
			size++;
		}
		else
		{
			int lowerBound = 0;
			int upperBound = 0;
			for (int i = 0; i < splitter->output.size(); i++)
			{
				cv::Mat part;
				upperBound = splitter->output.at(i);
				part = img(cv::Rect(0, lowerBound, img.cols, upperBound - lowerBound));
				cv::Mat *hash = new cv::Mat;
				algo->compute(part, *hash);
				paths.push_back(wcsdup(filename));
				hashes.push_back(hash);
				size++;
				lowerBound = upperBound;
			}
		}
	}
}

SimilarityList *HashCollection::DetectDuplicatesOneFolder(double threshold)
{
	SimilarityList *similarityList = new SimilarityList();
	for(int i=0;i<hashes.size()-1;i++)
		for (int j = i + 1; j < hashes.size(); j++)
		{
			double similarity = algo->compare(*hashes.at(i), *hashes.at(j));
			if (similarity < threshold)
				similarityList->Add(paths.at(i), paths.at(j), similarity);
		}
	return similarityList;
}

SimilarityList *HashCollection::DetectDuplicatesTwoFolders(double threshold,HashCollection *collection)
{
	SimilarityList *similarityList = new SimilarityList();
	for (int i = 0; i < hashes.size(); i++)
		for (int j = 0; j < collection->size; j++)
		{
			double similarity = algo->compare(*hashes.at(i), collection->GetHash(j));
			if (similarity < threshold)
				similarityList->Add(paths.at(i), collection->GetFilename(j), similarity);
		}
	return similarityList;
}

cv::Mat HashCollection::ReadImage(const wchar_t* filename)
{
	if (!isImage(filename))
		return cv::Mat();
	FILE* fp = _wfopen(filename, L"rb");
	if (!fp)
	{
		return cv::Mat();
	}
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	char* buf = new char[sz];
	fseek(fp, 0, SEEK_SET);
	long n = fread(buf, 1, sz, fp);
	cv::_InputArray arr(buf, sz);
	cv::Mat img = cv::imdecode(arr, cv::IMREAD_COLOR);
	delete[] buf;
	fclose(fp);
	return img;
}

bool HashCollection::isImage(const wchar_t* filename)
{
	bool output = false;
	FILE *fp = _wfopen(filename, L"rb");
	if (fp == nullptr)
		return false;
	unsigned char byte = fgetc(fp);
	if (byte == 0x89)
		if (fgetc(fp) == 'P')
			if (fgetc(fp) == 'N')
				if (fgetc(fp) == 'G')
					if (fgetc(fp) == 0x0D)
						output = true;
	if (byte == 'G')
		if (fgetc(fp) == 'I')
			if (fgetc(fp) == 'F')
				output = false;
	fseek(fp, 0, SEEK_SET);
	if (fgetc(fp) == 0xFF)
		if (fgetc(fp) == 0xD8)
			if (fgetc(fp) == 0xFF)
				 output = true;
	fclose(fp);
	return output;
}