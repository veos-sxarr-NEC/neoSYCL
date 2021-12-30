#ifndef CUSTOM_SYCL_INCLUDE_SYCL_TYPES_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_TYPES_HPP_

#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <iostream>

namespace neosycl::sycl {

template <class T, class Alloc = std::allocator<T>>
using vector_class = std::vector<T, Alloc>;

using string_class = std::string;

template <class R, class... ArgTypes>
using function_class = std::function<R(ArgTypes...)>;

using mutex_class = std::mutex;

template <class T> using shared_ptr_class = std::shared_ptr<T>;

template <class T> using unique_ptr_class = std::unique_ptr<T>;

template <class T> using weak_ptr_class = std::weak_ptr<T>;

template <class T> using hash_class = std::hash<T>;

using exception_ptr_class = std::exception_ptr;

template <class T> using buffer_allocator = std::allocator<T>;

} // namespace neosycl::sycl

#endif // CUSTOM_SYCL_INCLUDE_SYCL_TYPES_HPP_
