//===--------- device.hpp - OpenCL Adapter ---------------------------===//
//
// Copyright (C) 2023 Intel Corporation
//
// Part of the Unified-Runtime Project, under the Apache License v2.0 with LLVM
// Exceptions. See LICENSE.TXT
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#pragma once

#include "common.hpp"
#include "common/ur_ref_count.hpp"
#include "device.hpp"
#include "platform.hpp"

struct ur_device_handle_t_ : ur::opencl::handle_base {
  using native_type = cl_device_id;
  native_type CLDevice;
  ur_platform_handle_t Platform;
  cl_device_type Type = 0;
  ur_device_handle_t ParentDevice = nullptr;
  bool IsNativeHandleOwned = true;
  ur::RefCount RefCount;

  bool UseUnifiedSVM = false;
  std::vector<cl_svm_capabilities_khr> SVMCapabilities;

  ur_device_handle_t_(native_type Dev, ur_platform_handle_t Plat,
                      ur_device_handle_t Parent)
      : handle_base(), CLDevice(Dev), Platform(Plat), ParentDevice(Parent) {
    if (Parent) {
      Type = Parent->Type;
      [[maybe_unused]] auto Res = clRetainDevice(CLDevice);
      assert(Res == CL_SUCCESS);
    } else {
      [[maybe_unused]] auto Res = clGetDeviceInfo(
          CLDevice, CL_DEVICE_TYPE, sizeof(cl_device_type), &Type, nullptr);
      assert(Res == CL_SUCCESS);
    }
    InitUnifiedSVM();
  }

  ~ur_device_handle_t_() {
    if (ParentDevice) {
      // This does not need protected by a lock; this destructor can only run
      // exactly once. However, to prevent issues with the OpenCL handle being
      // reused, CLDevice must still be alive here.
      Platform->SubDevices.erase(CLDevice);
      [[maybe_unused]] auto Res = clReleaseDevice(CLDevice);
      assert(Res == CL_SUCCESS);
    }
    if (ParentDevice && IsNativeHandleOwned) {
      clReleaseDevice(CLDevice);
    }
  }

  ur_result_t getDeviceVersion(oclv::OpenCLVersion &Version) {
    size_t DevVerSize = 0;
    CL_RETURN_ON_FAILURE(
        clGetDeviceInfo(CLDevice, CL_DEVICE_VERSION, 0, nullptr, &DevVerSize));

    std::string DevVer(DevVerSize, '\0');
    CL_RETURN_ON_FAILURE(clGetDeviceInfo(CLDevice, CL_DEVICE_VERSION,
                                         DevVerSize, DevVer.data(), nullptr));

    Version = oclv::OpenCLVersion(DevVer);
    if (!Version.isValid()) {
      return UR_RESULT_ERROR_INVALID_DEVICE;
    }

    return UR_RESULT_SUCCESS;
  }

  bool isIntelFPGAEmuDevice() {
    size_t NameSize = 0;
    CL_RETURN_ON_FAILURE(
        clGetDeviceInfo(CLDevice, CL_DEVICE_NAME, 0, nullptr, &NameSize));
    std::string NameStr(NameSize, '\0');
    CL_RETURN_ON_FAILURE(clGetDeviceInfo(CLDevice, CL_DEVICE_NAME, NameSize,
                                         NameStr.data(), nullptr));

    return NameStr.find("Intel(R) FPGA Emulation Device") != std::string::npos;
  }

  ur_result_t checkDeviceExtensions(const std::vector<std::string> &Exts,
                                    bool &Supported) {
    size_t ExtSize = 0;
    CL_RETURN_ON_FAILURE(
        clGetDeviceInfo(CLDevice, CL_DEVICE_EXTENSIONS, 0, nullptr, &ExtSize));

    std::string ExtStr(ExtSize, '\0');

    CL_RETURN_ON_FAILURE(clGetDeviceInfo(CLDevice, CL_DEVICE_EXTENSIONS,
                                         ExtSize, ExtStr.data(), nullptr));

    Supported = true;
    for (const std::string &Ext : Exts) {
      if (!(Supported = (ExtStr.find(Ext) != std::string::npos))) {
        // The Intel FPGA emulation device does actually support these, even if
        // it doesn't report them.
        if (isIntelFPGAEmuDevice() &&
            (Ext == "cl_intel_device_attribute_query" ||
             Ext == "cl_intel_required_subgroup_size" ||
             Ext == "cl_khr_subgroups")) {
          Supported = true;
          continue;
        }
        break;
      }
    }

    return UR_RESULT_SUCCESS;
  }

  ur_result_t InitUnifiedSVM() {
    if (CheckUseUnifiedSVM) {
      checkDeviceExtensions({"cl_khr_unified_svm"}, UseUnifiedSVM);
    }
    if (UseUnifiedSVM) {
      size_t sz = 0;
      clGetDeviceInfo(CLDevice, CL_DEVICE_SVM_TYPE_CAPABILITIES_KHR, 0, nullptr,
                      &sz);

      SVMCapabilities.resize(sz / sizeof(cl_svm_capabilities_khr));
      clGetDeviceInfo(CLDevice, CL_DEVICE_SVM_TYPE_CAPABILITIES_KHR, sz,
                      SVMCapabilities.data(), nullptr);
    }
    return UR_RESULT_SUCCESS;
  }
};
