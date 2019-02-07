#pragma once
#include "HashCollection.h"
#include <iostream>
#include <fstream>
class HashDatabase :
	public HashCollection
{
private:
	void Add(const wchar_t *filename, cv::Mat hash);
	wchar_t *databaseHashPath;
	wchar_t *databaseNamePath;
public:
	HashDatabase(const wchar_t *databaseNamePathin, const wchar_t *databaseHashPathin);
	~HashDatabase();
	void AddFromCollection(HashCollection *collection);
	void AddFromFile();
	std::vector<cv::Mat*> FindHash(const wchar_t *filename);
	char *GetHashFromMat(cv::Mat mat);
	void Save();
};

