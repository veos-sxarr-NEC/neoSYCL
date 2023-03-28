#pragma once

#include "neoSYCL/sycl/info/program.hpp"

namespace neosycl::sycl {

namespace detail {
class program_impl;
class program_data;
}; // namespace detail

class handler;

enum class program_state { none, compiled, linked };

///////////////////////////////////////////////////////////////////////////////
class program {
public:
  friend class handler;
  using data = detail::program_data;

  program() = delete;

  explicit program(const context& context, const property_list& propList = {}) {
    init_(context, context.get_devices());
  }

  program(const context& context, vector_class<device> deviceList,
          const property_list& propList = {}) {
    init_(context, deviceList);
  }

  program(vector_class<program>& programList,
          const property_list& propList = {});

  program(vector_class<program>& programList, string_class linkOptions,
          const property_list& propList = {});

  //  program(const context &context, cl_program clProgram);

  friend bool operator==(const program& lhs, const program& rhs);
  friend bool operator!=(const program& lhs, const program& rhs);

  /* -- common interface members -- */
  cl_program get() const {
    throw unimplemented();
  }

  bool is_host() const;

  template <typename kernelT>
  void compile_with_kernel_type(string_class compileOptions = "");

  void compile_with_source(string_class kernelSource,
                           string_class compileOptions = "");
  template <typename kernelT>
  void build_with_kernel_type(string_class buildOptions = "");

  void build_with_source(string_class kernelSource,
                         string_class buildOptions = "");

  void link(string_class linkOptions = "");

  template <typename kernelT>
  bool has_kernel() const;

  bool has_kernel(string_class kernelName) const;

  template <typename kernelT>
  kernel get_kernel() const;

  kernel get_kernel(string_class kernelName) const;

  template <info::program param>
  typename info::param_traits<info::program, param>::return_type
  get_info() const;

  vector_class<vector_class<char>> get_binaries() const;

  context get_context() const;

  vector_class<device> get_devices() const;

  string_class get_compile_options() const;

  string_class get_link_options() const;

  string_class get_build_options() const;

  program_state get_state() const;

private:
  shared_ptr_class<detail::program_impl> impl_;

  shared_ptr_class<detail::program_data> get_data(device dev) const;

  void init_(context c, vector_class<device> deviceList);
};

bool operator==(const program& lhs, const program& rhs) {
  return (lhs.impl_ == rhs.impl_);
}

bool operator!=(const program& lhs, const program& rhs) {
  return !(lhs == rhs);
}

} // namespace neosycl::sycl
