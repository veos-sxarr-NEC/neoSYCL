#pragma once
#include "neoSYCL/sycl/detail/container/array_nd.hpp"

namespace neosycl::sycl {

template <int dimensions = 1>
struct id {
  id() = default;

  template <int D = dimensions, typename = std::enable_if_t<D == 1>>
  id(size_t dim0) : i_0(dim0), i_1(0), i_2(0) {}

  template <int D = dimensions, typename = std::enable_if_t<D == 2>>
  id(size_t dim0, size_t dim1) : i_0(dim0), i_1(dim1), i_2(0) {}

  template <int D = dimensions, typename = std::enable_if_t<D == 3>>
  id(size_t dim0, size_t dim1, size_t dim2) : i_0(dim0), i_1(dim1), i_2(dim2) {}

  id(const range<dimensions>& range) {
    if (dimensions == 1) {
      this->i_0 = range.get(0);
    }
    else if (dimensions == 2) {
      this->i_0 = range.get(0);
      this->i_1 = range.get(1);
    }
    else { // dimensions ==3
      this->i_0 = range.get(0);
      this->i_1 = range.get(1);
      this->i_2 = range.get(2);
    }
  }

  id(const item<dimensions>& item) {
    if (dimensions == 1) {
      this->i_0 = item[0];
    }
    else if (dimensions == 2) {
      this->i_0 = item[0];
      this->i_1 = item[1];
    }
    else { // dimensions ==3
      this->i_0 = item[0];
      this->i_1 = item[1];
      this->i_2 = item[2];
    }
  }

  template <int LD, int RD>
  friend bool operator==(const id<LD>& lhs, const id<RD>& rhs);
  friend bool operator!=(const id<>& lhs, const id<>& rhs);

  size_t get(int dimension) const {
    if (dimension == 0) {
      return i_0;
    }
    else if (dimension == 1) {
      return i_1;
    }
    else {
      return i_2;
    } // dimension ==2
  }

  size_t& operator[](int dimension) {
    if (dimension == 0) {
      return i_0;
    }
    else if (dimension == 1) {
      return i_1;
    }
    else {
      return i_2;
    } // dimension ==2
  }

  size_t operator[](int dimension) const {
    if (dimension == 0) {
      return i_0;
    }
    else if (dimension == 1) {
      return i_1;
    }
    else {
      return i_2;
    } // dimension ==2
  }

#if 0
  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST(id, +);
  DEFINE_OP_CONST(id, -);
  DEFINE_OP_CONST(id, *);
  DEFINE_OP_CONST(id, /);
  DEFINE_OP_CONST(id, %);
  DEFINE_OP_CONST(id, <<);
  DEFINE_OP_CONST(id, >>);
  DEFINE_OP_CONST(id, &);
  DEFINE_OP_CONST(id, |);
  DEFINE_OP_CONST(id, ^);
  DEFINE_OP_CONST(id, &&);
  DEFINE_OP_CONST(id, ||);
  DEFINE_OP_CONST(id, <);
  DEFINE_OP_CONST(id, >);
  DEFINE_OP_CONST(id, <=);
  DEFINE_OP_CONST(id, >=);

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST_SIZE_T(id, +);
  DEFINE_OP_CONST_SIZE_T(id, -);
  DEFINE_OP_CONST_SIZE_T(id, *);
  DEFINE_OP_CONST_SIZE_T(id, /);
  DEFINE_OP_CONST_SIZE_T(id, %);
  DEFINE_OP_CONST_SIZE_T(id, <<);
  DEFINE_OP_CONST_SIZE_T(id, >>);
  DEFINE_OP_CONST_SIZE_T(id, &);
  DEFINE_OP_CONST_SIZE_T(id, |);
  DEFINE_OP_CONST_SIZE_T(id, ^);
  DEFINE_OP_CONST_SIZE_T(id, &&);
  DEFINE_OP_CONST_SIZE_T(id, ||);
  DEFINE_OP_CONST_SIZE_T(id, <);
  DEFINE_OP_CONST_SIZE_T(id, >);
  DEFINE_OP_CONST_SIZE_T(id, <=);
  DEFINE_OP_CONST_SIZE_T(id, >=);

  // Where OP is: +=, -=,*=, /=, %=, <<=, >>=, &=, |=, ˆ=.
  DEFINE_OP(id, +=);
  DEFINE_OP(id, -=);
  DEFINE_OP(id, *=);
  DEFINE_OP(id, /=);
  DEFINE_OP(id, %=);
  DEFINE_OP(id, <<=);
  DEFINE_OP(id, >>=);
  DEFINE_OP(id, &=);
  DEFINE_OP(id, |=);
  DEFINE_OP(id, ^=);

  // Where OP is: +=, -=,*=, /=, %=, <<=, >>=, &=, |=, ˆ=.
  DEFINE_OP_SIZE_T(id, +);
  DEFINE_OP_SIZE_T(id, -=);
  DEFINE_OP_SIZE_T(id, *=);
  DEFINE_OP_SIZE_T(id, /=);
  DEFINE_OP_SIZE_T(id, %=);
  DEFINE_OP_SIZE_T(id, <<=);
  DEFINE_OP_SIZE_T(id, >>=);
  DEFINE_OP_SIZE_T(id, &=);
  DEFINE_OP_SIZE_T(id, |=);
  DEFINE_OP_SIZE_T(id, ^=);

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST_SIZE_T_LEFT(id, +);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, -);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, *);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, /);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, %);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, <<);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, >>);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, &);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, |);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, ^);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, &&);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, ||);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, <);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, >);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, <=);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, >=);

  DEFINE_COMMON_BY_VALUE_SEMANTICS(id);
#endif

  size_t i_0;
  size_t i_1;
  size_t i_2;
};

template <int LD, int RD>
bool operator==(const id<LD>& lhs, const id<RD>& rhs) {
  if (LD != RD) {
    return false;
  }

  if (LD == 1) {
    return (lhs.i_0 == rhs.i_0);
  }

  if (LD == 2) {
    return (lhs.i_0 == rhs.i_0 && lhs.i_1 == rhs.i_1);
  }

  if (LD == 3) {
    return (lhs.i_0 == rhs.i_0 && lhs.i_1 == rhs.i_1 && lhs.i_2 == rhs.i_2);
  }
  return false;
}

bool operator!=(const id<>& lhs, const id<>& rhs) {
  return !(lhs == rhs);
}

} // namespace neosycl::sycl
