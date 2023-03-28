#pragma once
#include <cstring>

namespace neosycl::sycl {

namespace detail {

struct device_impl {
  device dev_;
  device_impl(device d) : dev_(d) {}
  virtual ~device_impl() = default;

  virtual bool is_host()                           = 0;
  virtual bool is_cpu()                            = 0;
  virtual bool is_gpu()                            = 0;
  virtual bool is_accelerator()                    = 0;
  virtual const void* get_info(info::device) const = 0;
  virtual info::device_type type()                 = 0;
  virtual program_data* create_program(device)     = 0;

  template <info::device param>
  typename info::param_traits<info::device, param>::return_type get_info() {
    typename info::param_traits<info::device, param>::return_type ret =
        *(typename info::param_traits<info::device, param>::return_type*)
            get_info(param);
    return ret;
  }
};

struct device_impl_host : public device_impl {
  device_impl_host(device d) : device_impl(d) {}

  bool is_host() override {
    return true;
  }
  bool is_cpu() override {
    return false;
  }
  bool is_gpu() override {
    return false;
  }
  bool is_accelerator() override {
    return false;
  }

  const void* get_info(info::device param) const override {
    switch (param) {
    case info::device::device_type:
    default:
      PRINT_ERR("device::get_info(%d) not implemented", (int)param);
      throw unimplemented();
    }
  }
  info::device_type type() override {
    return info::device_type::host;
  }
  virtual program_data* create_program(device d) override {
    return new program_data_host(d);
  }
};

using default_device_impl = device_impl_host;

} // namespace detail

device device::get_default_device() {
#if defined(USE_VE) && defined(BUILD_VE)
  return platform::get_default_platform().get_devices()[2];
#else
#if defined(USE_CPU)
  return platform::get_default_platform().get_devices()[1];
#else
  return platform::get_default_platform().get_devices()[0];
#endif
#endif
}

bool device::is_host() const {
  return impl_->is_host();
}

bool device::is_cpu() const {
  return impl_->is_cpu();
}

bool device::is_gpu() const {
  return impl_->is_gpu();
}

bool device::is_accelerator() const {
  return impl_->is_accelerator();
}

info::device_type device::type() const {
  return impl_->type();
}

detail::program_data* device::create_program() const {
  return impl_->create_program(*this);
}

vector_class<device> device::get_devices(info::device_type t) {
  vector_class<device> ret;
  platform plt = platform::register_all_devices();
  ret = plt.get_devices(t);
  return ret;
}

template <info::device param>
typename info::param_traits<info::device, param>::return_type
device::get_info() const {
  return impl_->get_info<param>();
}

} // namespace neosycl::sycl
