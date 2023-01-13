namespace neosycl::sycl {
  context exception::get_context() const {
    if(has_context() != true) {
     throw exception("get_context() failed");
    }
    return *ctx;
  }
}
