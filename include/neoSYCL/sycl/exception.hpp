#pragma once
#include <exception>

namespace neosycl::sycl {

class context;

class exception : public std::exception {
public:
  exception(const string_class& message) : error_msg(message) {}

  const char* what() const noexcept override {
    return error_msg.c_str();
  }

  bool has_context() const;

  context get_context() const;

private:
  string_class error_msg;
};

//using exception_list = vector_class<exception_ptr_class>;
class exception_list {
public:
  std::size_t size() {
    return ptrlist.size();
  }

  std::vector<std::exception_ptr>::const_iterator begin() {
    return ptrlist.begin();
  }

  std::vector<std::exception_ptr>::const_iterator end() {
    return ptrlist.end();
  }

private:
  friend class queue;
  
  void pushback(const exception_ptr_class &value) {
    ptrlist.emplace_back(value);
  };

  void pushback(exception_ptr_class &&value) {
    ptrlist.emplace_back(std::move(value));
  };

  vector_class<exception_ptr_class> ptrlist;
};

using async_handler  = function_class<void, exception_list>;

class runtime_error : public exception {
  using exception::exception;
};
class kernel_error : public runtime_error {
  using runtime_error::runtime_error;
};
class accessor_error : public runtime_error {
  using runtime_error::runtime_error;
};
class nd_range_error : public runtime_error {
  using runtime_error::runtime_error;
};
class event_error : public runtime_error {
  using runtime_error::runtime_error;
};
class invalid_parameter_error : public runtime_error {
  using runtime_error::runtime_error;
};
class device_error : public exception {
  using exception::exception;
};
class compile_program_error : public device_error {
  using device_error::device_error;
};
class link_program_error : public device_error {
  using device_error::device_error;
};
class invalid_object_error : public device_error {
  using device_error::device_error;
};
class memory_allocation_error : public device_error {
  using device_error::device_error;
};
class platform_error : public device_error {
  using device_error::device_error;
};
class profiling_error : public device_error {
  using device_error::device_error;
};
class feature_not_supported : public device_error {
  using device_error::device_error;
};

class unimplemented : public exception {
public:
  unimplemented() : exception("not implemented") {}
};

} // namespace neosycl::sycl
