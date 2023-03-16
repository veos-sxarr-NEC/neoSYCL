#pragma once
#include <cmath>
#include <limits>
#include "vec.hpp"

namespace neosycl::sycl {

namespace detail {
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
typename std::make_unsigned<T>::type abs(const T& x) {
  return (x < 0 ? (-x) : x);
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
typename std::make_unsigned<T>::type abs_diff(const T& i, const T& j) {
  T diff = i - j;
  return abs(diff);
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T add_sat(const T& i, const T& j) {
  if (std::numeric_limits<T>::max() - i <= j)
    return std::numeric_limits<T>::max();
  return i + j;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T hadd(const T& i, const T& j) {
  T sum = add_sat(i, j);
  return (sum >> 1);
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T rhadd(const T& i, const T& j) {
  T sum = add_sat(1, add_sat(i, j));
  return (sum >> 1);
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
int clz(const T& x) {
  using uT          = typename std::make_unsigned<T>::type;
  constexpr uT mask = (uT(1) << (sizeof(T) * 8 - 1));

  uT bits = (uT)x;
  int num = 0;
  while ((bits & mask) == 0) {
    bits <<= 1;
    num++;
  }
  return num;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T mul_hi(const T& i, const T& j) {
  using uT          = typename std::make_unsigned<T>::type;
  constexpr uT mask = ((~(unsigned int)(0)) << (sizeof(T) * 4));
  return (mask & (i * j));
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T mul_sat(const T& i, const T& j) {
  if (std::numeric_limits<T>::max() / i <= j)
    return std::numeric_limits<T>::max();
  return i * j;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T mad_hi(const T& i, const T& j, const T& k) {
  return mul_hi(i, j) + k;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T mad_sat(const T& i, const T& j, const T& k) {
  T prod = mul_sat(i, j);
  return add_sat(prod, k);
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T rotate(const T& i, const T& j) {
  T bits = i;
  bits <<= j;
  bits |= (i >> (sizeof(T) * 8 - j));
  return bits;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T sub_sat(const T& i, const T& j) {
  if (std::numeric_limits<T>::min() + j >= i)
    return std::numeric_limits<T>::min();
  return i - j;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
T popcount(const T& x) {
  int nzb = 0;
  T bits  = x;
  while (bits) {
    nzb += (bits & 0x01);
    bits >>= 1;
  }
  return nzb;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value &&
std::numeric_limits<T>::digits==8>>
unsigned short upsample(const T& i, const T& j) {
  using unsigned_T = typename std::make_unsigned<T>::type;
  int digit = std::numeric_limits<unsigned_T>::digits;
  return (static_cast<unsigned short>(i) << digit) | j;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value &&
std::numeric_limits<T>::digits==7>>
short upsample(const T& i, const T& j) {
  using unsigned_T = typename std::make_unsigned<T>::type;
  int digit = std::numeric_limits<unsigned_T>::digits;
  return (static_cast<short>(i) << digit) | j;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value &&
std::numeric_limits<T>::digits==16>>
unsigned int upsample(const T& i, const T& j) {
  using unsigned_T = typename std::make_unsigned<T>::type;
  int digit = std::numeric_limits<unsigned_T>::digits;
  return (static_cast<unsigned int>(i) << digit) | j;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value &&
std::numeric_limits<T>::digits==15>>
int upsample(const T& i, const T& j) {
  using unsigned_T = typename std::make_unsigned<T>::type;
  int digit = std::numeric_limits<unsigned_T>::digits;
  return (static_cast<int>(i) << digit) | j;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value &&
std::numeric_limits<T>::digits==32>>
unsigned long long upsample(const T& i, const T& j) {
  using unsigned_T = typename std::make_unsigned<T>::type;
  int digit = std::numeric_limits<unsigned_T>::digits;
  return (static_cast<unsigned long long>(i) << digit) | j;
}
template <typename T, typename = std::enable_if_t<std::is_integral<T>::value &&
std::numeric_limits<T>::digits==31>>
long long upsample(const T& i, const T& j) {
  using unsigned_T = typename std::make_unsigned<T>::type;
  int digit = std::numeric_limits<unsigned_T>::digits;
  return (static_cast<long long>(i) << digit) | j;
}
}; // namespace detail

DEFINE_UGEN_FUNC(abs, detail::abs);
DEFINE_UGEN_FUNC2(abs_diff, detail::abs_diff);
DEFINE_GEN_FUNC2(add_sat, detail::add_sat);
/* clamp() is defined in math.hpp */
DEFINE_GEN_FUNC2(hadd, detail::hadd);
DEFINE_GEN_FUNC2(rhadd, detail::rhadd);
DEFINE_GEN_FUNC(clz, detail::clz);
DEFINE_GEN_FUNC3(mad_hi, detail::mad_hi);
DEFINE_GEN_FUNC2(mul_hi, detail::mul_hi);
DEFINE_GEN_FUNC3(mad_sat, detail::mad_sat);
/* max() is defined in math.hpp */
/* min() is defined in math.hpp */
DEFINE_GEN_FUNC2(rotate, detail::rotate);
DEFINE_GEN_FUNC2(sub_sat, detail::sub_sat);
DEFINE_GEN_FUNC2_upsample(upsample, detail::upsample);
DEFINE_GEN_FUNC(popcount, detail::popcount);

} // namespace neosycl::sycl
