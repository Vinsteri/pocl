/* cl3 - OpenCL 3.0 feature test program based on test_clGetDeviceInfo.c

   Copyright (c) 2021 Väinö Liukko

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include "poclu.h"

#define MAX_PLATFORMS 32
#define MAX_DEVICES   32

int
main(void)
{
  cl_int err;
  cl_platform_id platforms[MAX_PLATFORMS];
  cl_uint nplatforms;
  cl_device_id devices[MAX_DEVICES];
  cl_uint ndevices;
  cl_uint i, j;

  err = clGetPlatformIDs(MAX_PLATFORMS, platforms, &nplatforms);	
  CHECK_OPENCL_ERROR_IN("clGetPlatformIDs");
  if (!nplatforms)
    return EXIT_FAILURE;

  for (i = 0; i < nplatforms; i++)
  {
    err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, MAX_DEVICES,
                         devices, &ndevices);
    CHECK_OPENCL_ERROR_IN("clGetDeviceIDs");

    for (j = 0; j < ndevices; j++)
    {
      cl_long global_memsize, max_mem_alloc_size, min_max_mem_alloc_size;

      err = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE,
                            sizeof(global_memsize), &global_memsize, NULL);
      CHECK_OPENCL_ERROR_IN("clGetDeviceInfo");

      err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                            sizeof(max_mem_alloc_size), &max_mem_alloc_size,
                            NULL);
      CHECK_OPENCL_ERROR_IN("clGetDeviceInfo");

      TEST_ASSERT(global_memsize > 0);

      min_max_mem_alloc_size = 128*1024*1024;
      if (min_max_mem_alloc_size < global_memsize/4)
        min_max_mem_alloc_size = global_memsize/4;

      TEST_ASSERT(max_mem_alloc_size >= min_max_mem_alloc_size);

      /* OpenCl 3.0 queries */ 
      cl_device_atomic_capabilities atomic_memory_capability;
      err = clGetDeviceInfo(devices[j], CL_DEVICE_ATOMIC_MEMORY_CAPABILITIES,
                            sizeof(cl_device_atomic_capabilities),
                            atomic_memory_capability, NULL);
      CHECK_OPENCL_ERROR_IN("clGetDeviceInfo");

      /* atleast minimum mandated capability is reported */
      TEST_ASSERT((CL_DEVICE_ATOMIC_ORDER_RELAXED
                 | CL_DEVICE_ATOMIC_SCOPE_WORK_GROUP) 
                 & atomic_memory_capability ==
                  (CL_DEVICE_ATOMIC_ORDER_RELAXED 
                 | CL_DEVICE_ATOMIC_SCOPE_WORK_GROUP));

      cl_device_atomic_capabilities atomic_fence_capability;
      err = clGetDeviceInfo(devices[j], CL_DEVICE_ATOMIC_FENCE_CAPABILITIES,
                            sizeof(cl_device_atomic_capabilities),
                            atomic_fence_capability, NULL);
      CHECK_OPENCL_ERROR_IN("clGetDeviceInfo");

      /* atleast minimum mandated capability is reported */
      TEST_ASSERT((CL_DEVICE_ATOMIC_ORDER_RELAXED |
                   CL_DEVICE_ATOMIC_ORDER_ACQ_REL |
                   CL_DEVICE_ATOMIC_SCOPE_WORK_GROUP) 
                 & atomic_fence_capability ==
                  (CL_DEVICE_ATOMIC_ORDER_RELAXED |
                   CL_DEVICE_ATOMIC_ORDER_ACQ_REL |
                   CL_DEVICE_ATOMIC_SCOPE_WORK_GROUP));
    }
  }

  CHECK_CL_ERROR (clUnloadCompiler ());

  printf ("OK\n");
  return EXIT_SUCCESS;
}
