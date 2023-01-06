namespace neosycl::sycl {
  context exception::test() const {
    if(has_context() != true) {
     throw exception("get_context() failed");
    }
    return *ctx;
  }
}
