#pragma once
#include <vector>
class SimilarityList
{
private:
	struct Similarity
	{
		wchar_t *image1;
		wchar_t *image2;
		double similarity;
	};
public:
	std::vector<Similarity*> list;
	void Sort();
	std::vector<int> Remove(const wchar_t *image);
	void Add(const wchar_t *image1, const wchar_t *image2, double similarity);
	SimilarityList();
	~SimilarityList();
};

