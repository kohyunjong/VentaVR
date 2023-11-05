//#include "OpenCL_interface.h"
//
//void OpenCLTest::CLInit()
//{
//	int i, j;
//	char* value;
//	size_t valueSize;
//	cl_uint platformCount;
//	cl_platform_id* platforms;
//	cl_uint deviceCount;
//	cl_device_id* devices;
//	cl_uint maxComputeUnits;
//
//	// get all platforms
//	clGetPlatformIDs(0, NULL, &platformCount);
//	platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)* platformCount);
//	clGetPlatformIDs(platformCount, platforms, NULL);
//
//	for (i = 0; i < platformCount; i++) {
//
//		// get all devices
//		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
//		devices = (cl_device_id*)malloc(sizeof(cl_device_id)* deviceCount);
//		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);
//
//		// for each device print critical attributes
//		for (j = 0; j < deviceCount; j++) {
//
//			// print device name
//			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
//			value = (char*)malloc(valueSize);
//			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
//			printf("platform %d. Device %d: %s\n", i + 1, j + 1, value);
//			free(value);
//
//			// print hardware device version
//			clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
//			value = (char*)malloc(valueSize);
//			clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, NULL);
//			printf(" %d.%d Hardware version: %s\n", i + 1, 1, value);
//			free(value);
//
//			// print software driver version
//			clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
//			value = (char*)malloc(valueSize);
//			clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, NULL);
//			printf(" %d.%d Software version: %s\n", i + 1, 2, value);
//			free(value);
//
//			// print c version supported by compiler for device
//			clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
//			value = (char*)malloc(valueSize);
//			clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
//			printf(" %d.%d OpenCL C version: %s\n", i + 1, 3, value);
//			free(value);
//
//			// print parallel compute units
//			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
//				sizeof(maxComputeUnits), &maxComputeUnits, NULL);
//			printf(" %d.%d Parallel compute units: %d\n", i + 1, 4, maxComputeUnits);
//		}
//	}
//	int platformNum;
//	int deviceNum;
//	printf("\n\nSELECT PLATFORM('1' ~ '%d') : ", platformCount);
//	scanf("%d", &platformNum);
//	printf("\n");
//	printf("SELECT DEVICE('1' ~ '%d') : ", deviceCount);
//	scanf("%d", &deviceNum);
//	printf("\n");
//	clGetDeviceIDs(platforms[platformNum - 1], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);
//
//	device = devices[deviceNum - 1];
//
//	//create context
//	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
//
//	//create command queue
//	queue = clCreateCommandQueue(context, device, 0, NULL);
//
//	// kernel source
//	char* source =
//		"__kernel "
//		"void simpleKernel("
//		"    __global int* inputArray_A,"
//		"    __global int* inputArray_B,"
//		"    __global int* outputArray)"
//		"{"
//		"    uint dstYStride = get_global_size(0);"
//		"    uint dstIndex = get_global_id(1) * dstYStride + get_global_id(0);"
//		"    uint globalRow = get_global_id(1);"
//		"    uint globalCol = get_global_id(0);"
//		"	 outputArray[dstIndex] = inputArray_A[dstIndex] + inputArray_B[dstIndex];"
//		"}";
//
//	// compile program
//	program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);
//	cl_int build_status;
//	build_status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
//
//	//커널 포인터 생성
//	simpleKernel = clCreateKernel(program, "simpleKernel", NULL);
//
//}
//
//void OpenCLTest::bufferWrite()
//{
//
//
//	// 메모리 버퍼 생성
//
//	d_inputArray_A = clCreateBuffer(context, CL_MEM_READ_WRITE, 10 * sizeof(int), NULL, NULL);
//	d_inputArray_B = clCreateBuffer(context, CL_MEM_READ_WRITE, 10 * sizeof(int), NULL, NULL);
//	d_outputArray = clCreateBuffer(context, CL_MEM_READ_WRITE, 10 * sizeof(int), NULL, NULL);
//
//	int inputArray_A[10] = { 2, 2, 2, 2, 2, 3, 3, 3, 3, 3 };
//	int inputArray_B[10] = { 1, 1, 1, 1, 1, 4, 4, 4, 4, 4 };
//
//
//	int i;
//	printf("Array A : ");
//	for (i = 0; i < 10; i++)
//	{
//		printf("%d ", inputArray_A[i]);
//	}
//	printf("\n");
//	printf("Array B : ");
//	for (i = 0; i < 10; i++)
//	{
//		printf("%d ", inputArray_B[i]);
//	}
//	printf("\n");
//
//	clEnqueueWriteBuffer(queue, d_inputArray_A, CL_TRUE, 0, 10 * sizeof(int), inputArray_A, 0, NULL, NULL);
//	clEnqueueWriteBuffer(queue, d_inputArray_B, CL_TRUE, 0, 10 * sizeof(int), inputArray_B, 0, NULL, NULL);
//
//}
//
//void OpenCLTest::runKernel()
//{
//	int i;
//
//	int totalWorkItemsX = 10;
//	int totalWorkItemsY = 1;
//
//	size_t globalSize[2] = { totalWorkItemsX, totalWorkItemsY };
//
//	float *minVal, *maxVal;
//
//	// 커널 매개변수 설정 
//	clSetKernelArg(simpleKernel, 0, sizeof(cl_mem), &d_inputArray_A);
//	clSetKernelArg(simpleKernel, 1, sizeof(cl_mem), &d_inputArray_B);
//	clSetKernelArg(simpleKernel, 2, sizeof(cl_mem), &d_outputArray);
//
//
//	clEnqueueNDRangeKernel(queue, simpleKernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);
//
//	// 완료 대기 
//	clFinish(queue);
//
//	int outputArray[10];
//	clEnqueueReadBuffer(queue, d_outputArray, CL_TRUE, 0, 10 * sizeof(int), outputArray, 0, NULL, NULL);
//
//
//	printf("output  : ");
//	for (i = 0; i < 10; i++)
//	{
//		printf("%d ", outputArray[i]);
//	}
//	printf("\n");
//}
//
//void OpenCLTest::runWarpKernel()
//{
//}
//void OpenCLTest::Release()
//{
//	// 릴리즈
//	clReleaseProgram(program);
//	clReleaseCommandQueue(queue);
//	clReleaseContext(context);
//}
//int OpenCLTest::runTest() {
//	QueryPerformanceFrequency(&tot_clockFreq);
//
//	// OpenCL 디바이스, 커널 셋업
//	CLInit();
//
//	QueryPerformanceCounter(&tot_beginClock); //시간측정 시작
//
//	//디바이스 쪽 버퍼 생성 및 write
//	bufferWrite();
//
//	//커널 실행
//	runKernel();
//
//	QueryPerformanceCounter(&tot_endClock);
//	double totalTime = (double)(tot_endClock.QuadPart - tot_beginClock.QuadPart) / tot_clockFreq.QuadPart;
//	printf("Total processing Time : %f ms\n", totalTime * 1000);
//
//	Release();
//	return 0;
//}