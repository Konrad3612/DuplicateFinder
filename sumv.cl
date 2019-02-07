__kernel void SumV(__global uchar* input, __global float* output1,__global float* output2,int width,int thresh)
{
	const int y = get_global_id(0);
	int pixeldiff;
	int sum=0;
	int threshcount=0;
	int channels=3;
	for(int x=0;x<width;x++)
	{
		pixeldiff=0;
		for(int i=0;i<3;i++)
		{
			pixeldiff+=abs_diff(input[x*channels+i+y*channels*width],input[x*channels+i+(y+1)*channels*width]);
		}
		if(pixeldiff>thresh)
		{
			threshcount+=1;
		}
		sum+=pixeldiff;
	}
	sum=sum/(width*3);
	output1[y]=(float)sum/255.0f;
	output2[y]=(float)threshcount/(float)width;
}
__kernel void DiffSquare(__global float* input,__global float* output,float average)
{
	const int i = get_global_id(0);
	float diff = input[i]-average;
	output[i]=diff*diff;
}
__kernel void Score(__global float* input1,__global float* input2,__global float* output,const float average,const float stddev,const float scale)
{
	const int i = get_global_id(0);
	float score;
	score=input2[i]*(input1[i]-average)/stddev;
	score=score*scale;
	output[i]=score/(1.0f+fabs(score));
}