#pragma once

#include "neoSYCL/sycl/detail/device_type.hpp"

namespace neosycl::sycl::detail {
class context_info;

struct device_info {
  virtual bool is_host() = 0;

  virtual bool is_cpu() = 0;

  virtual bool is_gpu() = 0;

  virtual bool is_accelerator() = 0;

  virtual info::device_type type() = 0;

  virtual context_info* create_context_info(device d) const = 0;
};

struct cpu_device_info : public device_info {
  bool is_host() override { return true; }

  bool is_cpu() override { return true; }
  bool is_gpu() override { return false; }
  bool is_accelerator() override { return false; }

  info::device_type type() override { return info::device_type::cpu; }

  context_info* create_context_info(device d) const override;
};

using default_device_info = cpu_device_info;

} // namespace neosycl::sycl::detail
