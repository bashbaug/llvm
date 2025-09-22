/*******************************************************************************
 * Copyright (c) 2025 The Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

// This file is only needed temporarily, until OpenCL headers with support for
// cl_khr_unified_svm are widespread.

#pragma once
#if !defined(cl_khr_unified_svm)

/***************************************************************
* cl_khr_unified_svm (beta)
***************************************************************/
#if defined(CL_ENABLE_BETA_EXTENSIONS)

#if defined(CL_VERSION_2_0)

#define cl_khr_unified_svm 1
#define CL_KHR_UNIFIED_SVM_EXTENSION_NAME \
    "cl_khr_unified_svm"


#define CL_KHR_UNIFIED_SVM_EXTENSION_VERSION CL_MAKE_VERSION(0, 9, 0)

/* cl_platform_info */
#define CL_PLATFORM_SVM_TYPE_CAPABILITIES_KHR               0x0909

/* cl_device_info */
#define CL_DEVICE_SVM_TYPE_CAPABILITIES_KHR                 0x1077

typedef cl_bitfield         cl_svm_capabilities_khr;

/* cl_svm_capabilities_khr - bitfield */
#define CL_SVM_CAPABILITY_SINGLE_ADDRESS_SPACE_KHR          (1 << 0)
#define CL_SVM_CAPABILITY_SYSTEM_ALLOCATED_KHR              (1 << 1)
#define CL_SVM_CAPABILITY_DEVICE_OWNED_KHR                  (1 << 2)
#define CL_SVM_CAPABILITY_DEVICE_UNASSOCIATED_KHR           (1 << 3)
#define CL_SVM_CAPABILITY_CONTEXT_ACCESS_KHR                (1 << 4)
#define CL_SVM_CAPABILITY_HOST_OWNED_KHR                    (1 << 5)
#define CL_SVM_CAPABILITY_HOST_READ_KHR                     (1 << 6)
#define CL_SVM_CAPABILITY_HOST_WRITE_KHR                    (1 << 7)
#define CL_SVM_CAPABILITY_HOST_MAP_KHR                      (1 << 8)
#define CL_SVM_CAPABILITY_DEVICE_READ_KHR                   (1 << 9)
#define CL_SVM_CAPABILITY_DEVICE_WRITE_KHR                  (1 << 10)
#define CL_SVM_CAPABILITY_DEVICE_ATOMIC_ACCESS_KHR          (1 << 11)
#define CL_SVM_CAPABILITY_CONCURRENT_ACCESS_KHR             (1 << 12)
#define CL_SVM_CAPABILITY_CONCURRENT_ATOMIC_ACCESS_KHR      (1 << 13)
#define CL_SVM_CAPABILITY_INDIRECT_ACCESS_KHR               (1 << 14)

typedef cl_properties       cl_svm_alloc_properties_khr;
typedef cl_bitfield         cl_svm_alloc_access_flags_khr;
typedef cl_properties       cl_svm_free_properties_khr;
typedef cl_bitfield         cl_svm_free_flags_khr;
typedef cl_uint             cl_svm_pointer_info_khr;

/* cl_svm_alloc_properties_khr */
#define CL_SVM_ALLOC_ASSOCIATED_DEVICE_HANDLE_KHR           0x2078
#define CL_SVM_ALLOC_ACCESS_FLAGS_KHR                       0x2079
#define CL_SVM_ALLOC_ALIGNMENT_KHR                          0x207A

/* cl_svm_alloc_access_flags_khr */
#define CL_SVM_ALLOC_ACCESS_HOST_NOREAD_KHR                 (1 << 0)
#define CL_SVM_ALLOC_ACCESS_HOST_NOWRITE_KHR                (1 << 1)
#define CL_SVM_ALLOC_ACCESS_DEVICE_NOREAD_KHR               (1 << 8)
#define CL_SVM_ALLOC_ACCESS_DEVICE_NOWRITE_KHR              (1 << 9)

/* cl_svm_pointer_info_khr */
#define CL_SVM_INFO_TYPE_INDEX_KHR                          0x2088
#define CL_SVM_INFO_CAPABILITIES_KHR                        0x2089
#define CL_SVM_INFO_PROPERTIES_KHR                          0x208A
#define CL_SVM_INFO_ACCESS_FLAGS_KHR                        0x208B
#define CL_SVM_INFO_BASE_PTR_KHR                            0x419B
#define CL_SVM_INFO_SIZE_KHR                                0x419C
#define CL_SVM_INFO_ASSOCIATED_DEVICE_HANDLE_KHR            0x419D

/* cl_kernel_exec_info */
#define CL_KERNEL_EXEC_INFO_SVM_INDIRECT_ACCESS_KHR         0x11BB

/* SVM type convenience macros */
#define CL_SVM_TYPE_MACRO_COARSE_GRAIN_BUFFER_KHR \
    (CL_SVM_CAPABILITY_SINGLE_ADDRESS_SPACE_KHR | \
    CL_SVM_CAPABILITY_DEVICE_UNASSOCIATED_KHR   | \
    CL_SVM_CAPABILITY_CONTEXT_ACCESS_KHR        | \
    CL_SVM_CAPABILITY_HOST_MAP_KHR              | \
    CL_SVM_CAPABILITY_DEVICE_READ_KHR           | \
    CL_SVM_CAPABILITY_DEVICE_WRITE_KHR          | \
    CL_SVM_CAPABILITY_DEVICE_ATOMIC_ACCESS_KHR)
#define CL_SVM_TYPE_MACRO_FINE_GRAIN_BUFFER_KHR \
    (CL_SVM_CAPABILITY_SINGLE_ADDRESS_SPACE_KHR | \
    CL_SVM_CAPABILITY_DEVICE_UNASSOCIATED_KHR   | \
    CL_SVM_CAPABILITY_CONTEXT_ACCESS_KHR        | \
    CL_SVM_CAPABILITY_HOST_READ_KHR             | \
    CL_SVM_CAPABILITY_HOST_WRITE_KHR            | \
    CL_SVM_CAPABILITY_HOST_MAP_KHR              | \
    CL_SVM_CAPABILITY_DEVICE_READ_KHR           | \
    CL_SVM_CAPABILITY_DEVICE_WRITE_KHR          | \
    CL_SVM_CAPABILITY_DEVICE_ATOMIC_ACCESS_KHR  | \
    CL_SVM_CAPABILITY_CONCURRENT_ACCESS_KHR)
#define CL_SVM_TYPE_MACRO_DEVICE_KHR \
    (CL_SVM_CAPABILITY_SINGLE_ADDRESS_SPACE_KHR | \
    CL_SVM_CAPABILITY_DEVICE_OWNED_KHR          | \
    CL_SVM_CAPABILITY_DEVICE_READ_KHR           | \
    CL_SVM_CAPABILITY_DEVICE_WRITE_KHR          | \
    CL_SVM_CAPABILITY_DEVICE_ATOMIC_ACCESS_KHR  | \
    CL_SVM_CAPABILITY_INDIRECT_ACCESS_KHR)
#define CL_SVM_TYPE_MACRO_HOST_KHR \
    (CL_SVM_CAPABILITY_SINGLE_ADDRESS_SPACE_KHR | \
    CL_SVM_CAPABILITY_DEVICE_UNASSOCIATED_KHR   | \
    CL_SVM_CAPABILITY_CONTEXT_ACCESS_KHR        | \
    CL_SVM_CAPABILITY_HOST_OWNED_KHR            | \
    CL_SVM_CAPABILITY_HOST_READ_KHR             | \
    CL_SVM_CAPABILITY_HOST_WRITE_KHR            | \
    CL_SVM_CAPABILITY_DEVICE_READ_KHR           | \
    CL_SVM_CAPABILITY_DEVICE_WRITE_KHR          | \
    CL_SVM_CAPABILITY_INDIRECT_ACCESS_KHR)
#define CL_SVM_TYPE_MACRO_SINGLE_DEVICE_SHARED_KHR \
    (CL_SVM_CAPABILITY_SINGLE_ADDRESS_SPACE_KHR | \
    CL_SVM_CAPABILITY_HOST_READ_KHR             | \
    CL_SVM_CAPABILITY_HOST_WRITE_KHR            | \
    CL_SVM_CAPABILITY_DEVICE_READ_KHR           | \
    CL_SVM_CAPABILITY_DEVICE_WRITE_KHR          | \
    CL_SVM_CAPABILITY_INDIRECT_ACCESS_KHR)
#define CL_SVM_TYPE_MACRO_SYSTEM_KHR \
    (CL_SVM_CAPABILITY_SINGLE_ADDRESS_SPACE_KHR | \
    CL_SVM_CAPABILITY_SYSTEM_ALLOCATED_KHR      | \
    CL_SVM_CAPABILITY_DEVICE_UNASSOCIATED_KHR   | \
    CL_SVM_CAPABILITY_CONTEXT_ACCESS_KHR        | \
    CL_SVM_CAPABILITY_HOST_READ_KHR             | \
    CL_SVM_CAPABILITY_HOST_WRITE_KHR            | \
    CL_SVM_CAPABILITY_HOST_MAP_KHR              | \
    CL_SVM_CAPABILITY_DEVICE_READ_KHR           | \
    CL_SVM_CAPABILITY_DEVICE_WRITE_KHR          | \
    CL_SVM_CAPABILITY_DEVICE_ATOMIC_ACCESS_KHR  | \
    CL_SVM_CAPABILITY_CONCURRENT_ACCESS_KHR     | \
    CL_SVM_CAPABILITY_CONCURRENT_ATOMIC_ACCESS_KHR | \
    CL_SVM_CAPABILITY_INDIRECT_ACCESS_KHR)


typedef void* CL_API_CALL
clSVMAllocWithPropertiesKHR_t(
    cl_context context,
    const cl_svm_alloc_properties_khr* properties,
    cl_uint svm_type_index,
    size_t size,
    cl_int* errcode_ret);

typedef clSVMAllocWithPropertiesKHR_t *
clSVMAllocWithPropertiesKHR_fn ;

typedef cl_int CL_API_CALL
clSVMFreeWithPropertiesKHR_t(
    cl_context context,
    const cl_svm_free_properties_khr* properties,
    cl_svm_free_flags_khr flags,
    void* ptr);

typedef clSVMFreeWithPropertiesKHR_t *
clSVMFreeWithPropertiesKHR_fn ;

typedef cl_int CL_API_CALL
clGetSVMPointerInfoKHR_t(
    cl_context context,
    cl_device_id device,
    const void* ptr,
    cl_svm_pointer_info_khr param_name,
    size_t param_value_size,
    void* param_value,
    size_t* param_value_size_ret);

typedef clGetSVMPointerInfoKHR_t *
clGetSVMPointerInfoKHR_fn ;

typedef cl_int CL_API_CALL
clGetSVMSuggestedTypeIndexKHR_t(
    cl_context context,
    cl_svm_capabilities_khr required_capabilities,
    cl_svm_capabilities_khr desired_capabilities,
    const cl_svm_alloc_properties_khr* properties,
    size_t size,
    cl_uint* suggested_svm_type_index);

typedef clGetSVMSuggestedTypeIndexKHR_t *
clGetSVMSuggestedTypeIndexKHR_fn ;

#if !defined(CL_NO_NON_ICD_DISPATCH_EXTENSION_PROTOTYPES)

extern CL_API_ENTRY void* CL_API_CALL
clSVMAllocWithPropertiesKHR(
    cl_context context,
    const cl_svm_alloc_properties_khr* properties,
    cl_uint svm_type_index,
    size_t size,
    cl_int* errcode_ret) ;

extern CL_API_ENTRY cl_int CL_API_CALL
clSVMFreeWithPropertiesKHR(
    cl_context context,
    const cl_svm_free_properties_khr* properties,
    cl_svm_free_flags_khr flags,
    void* ptr) ;

extern CL_API_ENTRY cl_int CL_API_CALL
clGetSVMPointerInfoKHR(
    cl_context context,
    cl_device_id device,
    const void* ptr,
    cl_svm_pointer_info_khr param_name,
    size_t param_value_size,
    void* param_value,
    size_t* param_value_size_ret) ;

extern CL_API_ENTRY cl_int CL_API_CALL
clGetSVMSuggestedTypeIndexKHR(
    cl_context context,
    cl_svm_capabilities_khr required_capabilities,
    cl_svm_capabilities_khr desired_capabilities,
    const cl_svm_alloc_properties_khr* properties,
    size_t size,
    cl_uint* suggested_svm_type_index) ;

#endif /* !defined(CL_NO_NON_ICD_DISPATCH_EXTENSION_PROTOTYPES) */

#endif /* defined(CL_VERSION_2_0) */

#endif /* defined(CL_ENABLE_BETA_EXTENSIONS) */

#endif /* !defined(cl_khr_unified_svm) */