#pragma once

namespace neosycl::sycl {

class event {
public:
  event() {}

  explicit event(shared_ptr_class<detail::task_counter> counter)
    : c(counter) {}

  ~event() {}

  vector_class<event> get_wait_list() {
    throw unimplemented();
  }

  void wait() {
    c->wait();
  }

  static void wait(const vector_class<event>& eventList) {
    throw unimplemented();
  }

  void wait_and_throw() {
    throw unimplemented();
  }

  static void wait_and_throw(const vector_class<event>& eventList) {
    throw unimplemented();
  }

private:
  shared_ptr_class<detail::task_counter> c;
};

} // namespace neosycl::sycl
