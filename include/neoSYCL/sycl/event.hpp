#pragma once

namespace neosycl::sycl {

class event {
public:
  event() {}

  explicit event(shared_ptr_class<detail::task_counter> counter, async_handler err_handler, exception_list* exlist)
    : c(counter), h(err_handler), l(exlist) {}

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
    c->wait();
    if (h == NULL) {
      return;
    }

    exception_list tmp_exlist;
    std::swap(tmp_exlist, *l);

    if (tmp_exlist.size()) {
      h(std::move(tmp_exlist));
    }
  }

  static void wait_and_throw(const vector_class<event>& eventList) {
    throw unimplemented();
  }

private:
  shared_ptr_class<detail::task_counter> c;
  async_handler h;
  exception_list* l;
};

} // namespace neosycl::sycl
