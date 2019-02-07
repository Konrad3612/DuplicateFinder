#pragma once
#include <windows.h>
#include <opencv2\core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\img_hash.hpp>
#include "vsplitter.h"
#include "SimilarityList.h"
class HashCollection
{
protected:
	std::vector<wchar_t*> paths;
	std::vector<cv::Mat*> hashes;
private:
	cv::Mat ReadImage(const wchar_t* filename);
	vsplitter *splitter;
	cv::Ptr<cv::img_hash::ImgHashBase> algo;
	HashCollection *database = nullptr;
public:
	int size;
	cv::Mat GetHash(int index) { return *hashes.at(index); }
	wchar_t* GetFilename(int index) { return paths.at(index); }
	SimilarityList *DetectDuplicatesOneFolder(double threshold);
	SimilarityList *DetectDuplicatesTwoFolders(double threshold, HashCollection *collection);
	void Add(const wchar_t *filename);
	HashCollection(OpenCLDevice *device);
	HashCollection();
	~HashCollection();
	bool isImage(const wchar_t *filename);
	void AddDatabase(HashCollection *databasein) { database = databasein; }
	virtual std::vector<cv::Mat*> FindHash(const wchar_t* filename) { return std::vector<cv::Mat*>(); }
};

