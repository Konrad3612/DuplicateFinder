#include "HashDatabase.h"



HashDatabase::HashDatabase(const wchar_t *databaseNamePathin, const wchar_t *databaseHashPathin)
{
	databaseNamePath = wcsdup(databaseNamePathin);
	databaseHashPath = wcsdup(databaseHashPathin);
	AddFromFile();
}


HashDatabase::~HashDatabase()
{
}

void HashDatabase::AddFromCollection(HashCollection *collection)
{
	if (collection == nullptr)
		return;
	for (int i = 0; i < collection->size; i++)
	{
		Add(wcsrchr(collection->GetFilename(i),L'\\')+1, collection->GetHash(i));
	}
}
void HashDatabase::AddFromFile()
{
	wchar_t buffer[512];
	char hash[9];
	std::wifstream fp1;
	std::ifstream fp2;
	fp1.open(databaseNamePath, std::ios::in);
	fp2.open(databaseHashPath, std::ios::in | std::ios::binary);
	if (!fp1||!fp2)
		return;
	while (fp1.getline(buffer, 512))
	{
		if (wcscmp(buffer, L"") != 0)
		{
			paths.push_back(wcsdup(buffer));
		}
	}
	fp1.close();
	for(int i=0;i<paths.size();i++)
	{
		int hashSize = 8;
		fp2.read(hash, hashSize);
		hash[hashSize] = '\0';
		cv::Mat *hashMat = new cv::Mat(1, hashSize, CV_8U);
		for (int i=0; i < hashSize; i++)
			hashMat->at<uchar>(0, i) = (uchar)hash[i];
		hashes.push_back(hashMat);
	}
	fp2.close();
}
void HashDatabase::Add(const wchar_t *filename, cv::Mat hash)
{
	/*if (paths.size() != 0)
	{
		if (wcscmp(filename, paths.at(paths.size() - 1)) != 0)
		{
			for (int i = 0; i < paths.size(); i++)
				if (wcscmp(filename, paths.at(i)) == 0)
					return;
		}
	}*/
	for (int i = 0; i < paths.size(); i++)
		if (wcscmp(paths.at(i), filename) == 0)
		{
			char *hash1 = GetHashFromMat(hash);
			char *hash2 = GetHashFromMat(*hashes.at(i));
				if (strcmp(hash1, hash2) == 0)
					return;
		}
	paths.push_back(wcsdup(filename));
	cv::Mat *newHash = new cv::Mat();
	*newHash = hash;
	hashes.push_back(newHash);
}

char *HashDatabase::GetHashFromMat(cv::Mat mat)
{
	char *output = new char[9];
	for (int i = 0; i < mat.cols; i++)
		output[i] = mat.at<char>(0, i);
	output[mat.cols] = '\0';
	return output;
}

void HashDatabase::Save()
{
	std::wofstream fp1;
	std::ofstream fp2;
	fp1.open(L"temp1",std::ios::out);
	fp2.open(L"temp2", std::ios::out | std::ios::binary);
	bool fail = false;
	char buffer[9];
	for (int i = 0; i < paths.size(); i++)
	{
		fp1 << paths.at(i) << std::endl;
		for (int j = 0; j < hashes.at(i)->cols; j++)
			buffer[j] = hashes.at(i)->at<char>(0, j);
		buffer[8] = '\0';
		fp2.write(buffer,8);
		if (fp1.fail()||fp2.fail())
		{
			paths.erase(paths.begin() + i);
			hashes.erase(hashes.begin() + i);
			fp1.close();
			fp2.close();
			Save();
			return;
		}
	}
	fp1.close();
	fp2.close();
	_wremove(databaseNamePath);
	_wremove(databaseHashPath);
	_wrename(L"temp1", databaseNamePath);
	_wrename(L"temp2", databaseHashPath);
}

std::vector<cv::Mat*> HashDatabase::FindHash(const wchar_t *filename)
{
	std::vector<cv::Mat*> output;
	if (wcsrchr(filename, L'\\') != nullptr)
		filename = wcsrchr(filename, L'\\') + 1;
	for (int i = 0; i < paths.size(); i++)
		if (wcscmp(paths.at(i), filename) == 0)
			output.push_back(hashes.at(i));
	return output;
}