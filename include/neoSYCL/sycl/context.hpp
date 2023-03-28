#pragma once
#include "neoSYCL/sycl/property_list.hpp"
#include "neoSYCL/sycl/info/context.hpp"

namespace neosycl::sycl {

namespace detail {
class context_impl;
}

///////////////////////////////////////////////////////////////////////////////
class context {
public:
  explicit context(const property_list& propList = {}) {
    init({device::get_default_device()});
  }

  ~context() = default;

  friend bool operator==(const context& lhs, const context& rhs);
  friend bool operator!=(const context& lhs, const context& rhs);

  context(async_handler asyncHandler, const property_list& propList = {})
      : ahandl_(asyncHandler) {
    init({device::get_default_device()});
  }

  context(const device& dev, const property_list& propList = {}) {
    init({dev});
  }

  context(const device& dev, async_handler asyncHandler,
          const property_list& propList = {})
      : ahandl_(asyncHandler) {
    init({dev});
  }

  context(const platform& plt, const property_list& propList = {});

  context(const platform& plt, async_handler asyncHandler,
          const property_list& propList = {});

  context(const vector_class<device>& deviceList,
          const property_list& propList = {});

  context(const vector_class<device>& deviceList, async_handler asyncHandler,
          const property_list& propList = {});

  context(cl_context clContext, async_handler asyncHandler = {}) {
    throw unimplemented();
  }

  /* -- common interface members -- */
  template <info::context param>
  typename info::param_traits<info::context, param>::return_type
  get_info() const;

  vector_class<device> get_devices() const;

  // INTERNAL USE ONLY
  long use_count_() const {
    return impl_.use_count();
  }

private:
  void init(vector_class<device>);

  shared_ptr_class<detail::context_impl> impl_;
  async_handler ahandl_;
};

bool operator==(const context& lhs, const context& rhs) {
  return (lhs.impl_ == rhs.impl_);
}

bool operator!=(const context& lhs, const context& rhs) {
  return !(lhs == rhs);
}

} // namespace neosycl::sycl
