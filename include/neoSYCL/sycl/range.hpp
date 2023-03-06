#pragma once
#include "neoSYCL/sycl/detail/container/array_nd.hpp"

namespace neosycl::sycl {

template <int dimensions = 1>
struct range {
  template <int D = dimensions, typename = std::enable_if_t<D == 1>>
  range(size_t dim0) : data(dim0) {}

  template <int D = dimensions, typename = std::enable_if_t<D == 2>>
  range(size_t dim0, size_t dim1) : data(dim0, dim1) {}

  template <int D = dimensions, typename = std::enable_if_t<D == 3>>
  range(size_t dim0, size_t dim1, size_t dim2) : data(dim0, dim1, dim2) {}

  template <int LD, int RD>
  friend bool operator==(const range<LD>& lhs, const range<RD>& rhs);

  template <int LD, int RD>
  friend bool operator!=(const range<LD>& lhs, const range<RD>& rhs);


  size_t get(int dimension) const {
    return data[dimension];
  }

  size_t& operator[](int dimension) {
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    return data[dimension];
  }

  size_t size() const {
    return data.get_liner();
  }

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST(range, +);
  DEFINE_OP_CONST(range, -);
  DEFINE_OP_CONST(range, *);
  DEFINE_OP_CONST(range, /);
  DEFINE_OP_CONST(range, %);
  DEFINE_OP_CONST(range, <<);
  DEFINE_OP_CONST(range, >>);
  DEFINE_OP_CONST(range, &);
  DEFINE_OP_CONST(range, |);
  DEFINE_OP_CONST(range, ^);
  DEFINE_OP_CONST(range, &&);
  DEFINE_OP_CONST(range, ||);
  DEFINE_OP_CONST(range, <);
  DEFINE_OP_CONST(range, >);
  DEFINE_OP_CONST(range, <=);
  DEFINE_OP_CONST(range, >=);

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST_SIZE_T(range, +);
  DEFINE_OP_CONST_SIZE_T(range, -);
  DEFINE_OP_CONST_SIZE_T(range, *);
  DEFINE_OP_CONST_SIZE_T(range, /);
  DEFINE_OP_CONST_SIZE_T(range, %);
  DEFINE_OP_CONST_SIZE_T(range, <<);
  DEFINE_OP_CONST_SIZE_T(range, >>);
  DEFINE_OP_CONST_SIZE_T(range, &);
  DEFINE_OP_CONST_SIZE_T(range, |);
  DEFINE_OP_CONST_SIZE_T(range, ^);
  DEFINE_OP_CONST_SIZE_T(range, &&);
  DEFINE_OP_CONST_SIZE_T(range, ||);
  DEFINE_OP_CONST_SIZE_T(range, <);
  DEFINE_OP_CONST_SIZE_T(range, >);
  DEFINE_OP_CONST_SIZE_T(range, <=);
  DEFINE_OP_CONST_SIZE_T(range, >=);

  // Where OP is: +=, -=,*=, /=, %=, <<=, >>=, &=, |=, ˆ=.
  DEFINE_OP(range, +=);
  DEFINE_OP(range, -=);
  DEFINE_OP(range, *=);
  DEFINE_OP(range, /=);
  DEFINE_OP(range, %=);
  DEFINE_OP(range, <<=);
  DEFINE_OP(range, >>=);
  DEFINE_OP(range, &=);
  DEFINE_OP(range, |=);
  DEFINE_OP(range, ^=);

  // Where OP is: +=, -=,*=, /=, %=, <<=, >>=, &=, |=, ˆ=.
  DEFINE_OP_SIZE_T(range, +);
  DEFINE_OP_SIZE_T(range, -=);
  DEFINE_OP_SIZE_T(range, *=);
  DEFINE_OP_SIZE_T(range, /=);
  DEFINE_OP_SIZE_T(range, %=);
  DEFINE_OP_SIZE_T(range, <<=);
  DEFINE_OP_SIZE_T(range, >>=);
  DEFINE_OP_SIZE_T(range, &=);
  DEFINE_OP_SIZE_T(range, |=);
  DEFINE_OP_SIZE_T(range, ^=);

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST_SIZE_T_LEFT(range, +);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, -);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, *);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, /);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, %);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, <<);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, >>);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, &);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, |);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, ^);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, &&);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, ||);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, <);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, >);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, <=);
  DEFINE_OP_CONST_SIZE_T_LEFT(range, >=);

  detail::container::ArrayND<dimensions> data;
};

template <int LD, int RD>
bool operator==(const range<LD>& lhs, const range<RD>& rhs) {
  if (LD != RD) {
    return false;
  }

  if (LD == 1) {
    return (lhs.data[0] == rhs.data[0]);
  }

  if (LD == 2) {
    return (lhs.data[0] == rhs.data[0] && lhs.data[1] == rhs.data[1]);
  }

  if (LD == 3) {
    return (lhs.data[0] == rhs.data[0] && lhs.data[1] == rhs.data[1] && lhs.data[2] == rhs.data[2]);
  }
  return false;
}

template <int LD, int RD>
bool operator!=(const range<LD>& lhs, const range<RD>& rhs) {
  return !(lhs == rhs);
}

} // namespace neosycl::sycl
