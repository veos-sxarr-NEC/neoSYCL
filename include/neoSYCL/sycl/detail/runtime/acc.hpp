#pragma once

namespace neosycl::sycl::rt {

using namespace neosycl::sycl;

template <typename T>
struct acc_ {
  T* p_;
  size_t r_[6];

  inline T& operator[](size_t i) {
    return p_[i];
  }
  inline T& operator[](const id<3> i) {
    return p_[(i.i_0 * r_[1] + i.i_1) * r_[2] + i.i_2];
  }
  inline T& operator[](const id<2> i) {
    return p_[i.i_0 * r_[1] + i.i_1];
  }
  inline T& operator[](const id<1> i) {
    return p_[i.i_0];
  }

  template <int dimensions>
  T& operator[](item<dimensions>& i) {
    return this->operator[](i.get_id());
  }

  T* get() const {
    return p_;
  }
};

inline item<1> id2item(size_t s[6], size_t i0) {
  return item<1>(range<1>{s[0]}, detail::container::ArrayND<1>{i0},
                 detail::container::ArrayND<1>{s[3]});
}

inline item<2> id2item(size_t s[6], size_t i0, size_t i1) {
  return item<2>(range<2>{s[0], s[1]}, detail::container::ArrayND<2>{i0, i1},
                 detail::container::ArrayND<2>{s[3], s[4]});
}

inline item<3> id2item(size_t s[6], size_t i0, size_t i1, size_t i2) {
  return item<3>(range<3>{s[0], s[1], s[2]},
                 detail::container::ArrayND<3>{i0, i1, i2},
                 detail::container::ArrayND<3>{s[3], s[4], s[5]});
}

inline void id2item_1_1(item<1>& itm, size_t i) {
  itm.data[0] = i;
}

inline void id2item_2_1(item<2>& itm, size_t i) {
  itm.data[0] = i;
}
inline void id2item_2_2(item<2>& itm, size_t i) {
  itm.data[1] = i;
}

inline void id2item_3_1(item<3>& itm, size_t i) {
  itm.data[0] = i;
}
inline void id2item_3_2(item<3>& itm, size_t i) {
  itm.data[1] = i;
}
inline void id2item_3_3(item<3>& itm, size_t i) {
  itm.data[2] = i;
}

} // namespace neosycl::sycl::rt

#ifdef ___NEOSYCL_KERNEL_RUNTIME_ONLY___
#include "neoSYCL/sycl/access.hpp"

namespace neosycl::sycl {
template <typename dataT, int dimensions, access::mode accessMode,
          access::target accessTarget       = access::target::global_buffer,
          access::placeholder isPlaceholder = access::placeholder::false_t>
using accessor = rt::acc_<dataT>;
}
#endif
