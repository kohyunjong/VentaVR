//#ifndef __LHC_OPENCL_INTERFACE_H__
//#define __LHC_OPENCL_INTERFACE_H__
//#include <stdio.h>
//#include <stdlib.h>
//#include <CL/cl.h>
//#include <Windows.h>
//#include <string.h>
//#include <tchar.h>
//#include <stdbool.h>
//#include <math.h>
//
//class OpenCLTest
//{
//public:
//	OpenCLTest(){};
//	~OpenCLTest();
//
//	cl_mem				d_inputArray_A;
//	cl_mem				d_inputArray_B;
//	cl_mem				d_outputArray;
//	cl_platform_id		platform;
//	cl_context          context;
//	cl_device_id        device;
//	cl_command_queue    queue;
//	cl_program			program;
//	cl_kernel			simpleKernel;
//	cl_kernel			warpKernel;
//	LARGE_INTEGER		beginClock, endClock, clockFreq;
//	LARGE_INTEGER		tot_beginClock, tot_endClock, tot_clockFreq;
//
//	void	CLInit();
//	void	bufferWrite();
//	void	runKernel();
//	void	Release();
//	int		runTest();
//	void	runWarpKernel();
//};
//
//
//
//
//#endif  //__LHC_OPENCL_INTERFACE_H__