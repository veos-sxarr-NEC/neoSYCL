#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_HIGHLIGHT_FUNC_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_HIGHLIGHT_FUNC_HPP_

namespace neosycl::sycl::detail {

template <typename KernelName, typename Kernel, int Dimensions>
void HIGHLIGHT_KERNEL_PARALLEL(Kernel k,
                               const range<Dimensions> &num_work_items) {}

template <typename KernelName, typename Kernel>
void HIGHLIGHT_KERNEL_SINGLE_TASK(Kernel k) {}

} // namespace neosycl::sycl::detail

#endif // SYCL_INCLUDE_CL_SYCL_KERNEL_HIGHLIGHT_FUNC_HPP_
