#pragma once
#include "neoSYCL/sycl/info/queue.hpp"
#include "neoSYCL/sycl/detail/task_counter.hpp"
#include <semaphore.h>

namespace neosycl::sycl {

///////////////////////////////////////////////////////////////////////////////
class queue {
public:
  explicit queue(const property_list& propList = {})
      : bind_device(device::get_default_device()),
        counter(new detail::task_counter()), ctx(bind_device), prog(ctx) {
    first_sem_set();
  }

  explicit queue(const async_handler& asyncHandler,
                 const property_list& propList = {})
      : bind_device(device::get_default_device()),
        counter(new detail::task_counter()), err_handler(asyncHandler),
        ctx(bind_device), prog(ctx) {
    first_sem_set();
  }

  explicit queue(const device_selector& deviceSelector,
                 const property_list& propList = {})
      : bind_device(deviceSelector.select_device()),
        counter(new detail::task_counter()), ctx(bind_device), prog(ctx) {
    first_sem_set();
  }

  explicit queue(const device_selector& deviceSelector,
                 const async_handler& asyncHandler,
                 const property_list& propList = {})
      : bind_device(deviceSelector.select_device()),
        counter(new detail::task_counter()), err_handler(asyncHandler),
        ctx(bind_device), prog(ctx) {
    first_sem_set();
  }

  explicit queue(const device& syclDevice, const property_list& propList = {})
      : bind_device(syclDevice), counter(new detail::task_counter()),
        ctx(bind_device), prog(ctx) {
    first_sem_set();
  }

  explicit queue(const device& syclDevice, const async_handler& asyncHandler,
                 const property_list& propList = {})
      : bind_device(syclDevice), counter(new detail::task_counter()),
        err_handler(asyncHandler), ctx(bind_device), prog(ctx) {
    first_sem_set();
  }

  explicit queue(const context& syclContext,
                 const device_selector& deviceSelector,
                 const property_list& propList = {});

  explicit queue(const context& syclContext,
                 const device_selector& deviceSelector,
                 const async_handler& asyncHandler,
                 const property_list& propList = {});

  explicit queue(const context& syclContext, const device& syclDevice,
                 const property_list& propList = {});

  explicit queue(const context& syclContext, const device& syclDevice,
                 const async_handler& asyncHandler,
                 const property_list& propList = {});

  //  explicit queue(cl_command_queue clQueue, const context &syclContext,
  //                 const async_handler &asyncHandler = {});

  //  cl_command_queue get() const;

  friend bool operator==(const queue& lhs, const queue& rhs);
  friend bool operator!=(const queue& lhs, const queue& rhs);

  context get_context() const {
    return ctx;
  }

  device get_device() const {
    return bind_device;
  }

  bool is_host() const {
    return bind_device.is_host();
  }

  template <info::queue param>
  typename info::param_traits<info::queue, param>::return_type get_info() const;

#ifndef DISABLE_MULTI_THREAD_SUPPORT
  template <typename T>
  event submit(T cgf) {
    sem_set();
    counter->incr();
    std::thread t([f = cgf, d = bind_device, p = prog, c = counter, k = kernel_listptr, l = exlist]() {
      try {
        handler command_group_handler(d, p, c, k);
        f(command_group_handler);
      }
      catch (std::exception& e) {
        PRINT_ERR("%s", e.what());
	l->pushback(std::current_exception());
      }
      catch (...) {
        PRINT_ERR("unknown exception");
        l->pushback(std::current_exception());
      }
      c->decr();
    });
    t.detach();
    return event(counter, err_handler, exlist);
  }
#else
  /* this may run each command group faster but all command groups will be
   * executed sequencially (i.e. no task parallelism) */
  template <typename T>
  event submit(T cgf) {
    try {
      handler command_group_handler(bind_device, prog, counter);
      cgf(command_group_handler);
    }
    catch (std::exception& e) {
      PRINT_ERR("%s", e.what());
      throw;
    }
    catch (...) {
      PRINT_ERR("unknown exception");
      throw;
    }
    return event();
  }
#endif

  template <typename T>
  event submit(T cgf, const queue& secondaryQueue) {
    throw unimplemented();
  }

  void wait() {
    counter->wait();
  }

  void wait_and_throw() {
    counter->wait();
    throw_asynchronous();
  }

  void throw_asynchronous() {
    if (err_handler == NULL) {
      return;
    }

    exception_list tmp_exlist;
    std::swap(tmp_exlist, *exlist);

    if (tmp_exlist.size()) {
      err_handler(std::move(tmp_exlist));
    }
  }

  virtual ~queue() {
    counter->wait();
  }

private:
  device bind_device;
  shared_ptr_class<detail::task_counter> counter;
  async_handler err_handler;
  std::shared_ptr<exception_list> exlist;
  context ctx;
  program prog;
  std::shared_ptr<kernel_list> head;
  std::shared_ptr<kernel_list> kernel_listptr;

  /*The first kernel does not lock*/
  void first_sem_set () {
    head = std::shared_ptr<kernel_list>(new kernel_list); 
    sem_init(head->fence.get(), 0, 1);
  }

  /*Lock the next submitted kernel in advance*/
  void sem_set () {
    exlist = std::shared_ptr<exception_list>(new exception_list);
    std::shared_ptr<kernel_list> newlist(new kernel_list());
    std::shared_ptr<kernel_list> listptr = head;
    
    while (listptr->next) {
      listptr = listptr->next;
    }
    
    kernel_listptr = listptr;
    listptr->next = newlist;
    sem_init(listptr->next->fence.get(), 0, 0);
  }
};

bool operator==(const queue& lhs, const queue& rhs) {
  return (lhs.ctx == rhs.ctx);
}

bool operator!=(const queue& lhs, const queue& rhs) {
  return !(lhs == rhs);
}

} // namespace neosycl::sycl
