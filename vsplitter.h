#pragma once
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "OpenCLDevice.h"

extern "C" 
{
	#include <CL/cl.h>
}

class vsplitter
{
private:
	cv::Mat *image;
	cl_program *program;
	float stddev;
	float average;
	std::vector<float>* diffList=nullptr;
	std::vector<float>* threshList=nullptr;
	std::vector<float>* scoreList=nullptr;
	std::vector<int> points;
	std::vector<float> scores;
	std::vector<double> harmonicScores;
	OpenCLDevice *device = nullptr;
	void Clear();
	void SumDiffV(int thresh);
	void PickPoints(float thresh);
	void CreateProgram(const std::string& source);
	void FindHarmonics();
	void Interpretation(float thresh);
public:
	vsplitter(OpenCLDevice *device);
	std::vector<int> output;
	~vsplitter();
	void Analyze(cv::Mat *image_in);
};

