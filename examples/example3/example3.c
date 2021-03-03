#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <CL/opencl.h>
#include "poclu.h"


int
main (int argc, char **argv)
{
    cl_context context = NULL;
    cl_device_id device = NULL;
    cl_platform_id platform = NULL;
    cl_command_queue queue = NULL;

    cl_int err = NULL;

    err = poclu_get_any_device2 (&context, &device, &queue, &platform);
    CHECK_OPENCL_ERROR_IN ("clCreateContext");

    size_t device_name_length;
    clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &device_name_length);
    char device_name[device_name_length];
    clGetDeviceInfo(device, CL_DEVICE_NAME, device_name_length, device_name, NULL);
    printf("OpenCL device name: %s\n", device_name);

    cl_device_atomic_capabilities value;
    err = clGetDeviceInfo(&device, CL_DEVICE_ATOMIC_MEMORY_CAPABILITIES, sizeof(cl_device_atomic_capabilities), &value, NULL);
    printf("OpenCL Device Atomic Memort Capability: %d \n", value);
}
