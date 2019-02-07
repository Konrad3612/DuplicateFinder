#pragma once
#include <vector>
#include <iostream>
extern "C"
{
#include <CL/cl.h>
}
class OpenCLDevice
{
public:
	OpenCLDevice();
	~OpenCLDevice();
	void InitializeDevice();
	bool isDeviceInitialized=false;
	cl_context *context;
	cl_uint deviceIdCount;
	std::vector<cl_device_id> *deviceIds;
	size_t maxBufferSize = 0;
	int deviceIndex=0;
};

