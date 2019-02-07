#include "SimilarityList.h"



SimilarityList::SimilarityList()
{
}

void SimilarityList::Add(const wchar_t *image1, const wchar_t *image2, double similarity)
{
	if (wcscmp(image1, image2) == 0)
		return;
	for (int i = 0; i < list.size(); i++)
	{
		if (wcscmp(list.at(i)->image1, image1) == 0)
			if (wcscmp(list.at(i)->image2, image2) == 0)
				return;
		if (wcscmp(list.at(i)->image2, image2) == 0)
			if (wcscmp(list.at(i)->image1, image1) == 0)
				return;
	}
	Similarity *newCell = new Similarity;
	newCell->image1 = wcsdup(image1);
	newCell->image2 = wcsdup(image2);
	newCell->similarity = similarity;
	list.push_back(newCell);
}

std::vector<int> SimilarityList::Remove(const wchar_t *image)
{
	std::vector<int> indexList;
	for (int i = 0; i < list.size(); i++)
		if (wcscmp(list.at(i)->image1, image) == 0 || wcscmp(list.at(i)->image2, image) == 0)
		{
			list.erase(list.begin() + i);
			indexList.push_back(i);
		}
	return indexList;
}

void SimilarityList::Sort()
{
	if (list.size() == 0)
		return;
	bool over;
	do
	{
		over = true;
		for(int i=0;i<list.size()-1;i++)
			if (list.at(i + 1)->similarity < list.at(i)->similarity)
			{
				Similarity* placeholder = list.at(i + 1);
				list.at(i + 1) = list.at(i);
				list.at(i) = placeholder;
				over = false;
			}
	} while (!over);
}

SimilarityList::~SimilarityList()
{
	for (int i = 0; i < list.size(); i++)
	{
		delete list.at(i)->image1;
		delete list.at(i)->image2;
	}
}
