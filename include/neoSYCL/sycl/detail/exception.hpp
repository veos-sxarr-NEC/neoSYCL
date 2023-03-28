namespace neosycl::sycl {
exception::exception(const string_class& message, context* err_ctx)
    : error_msg(message), ctx(new context(*err_ctx)) {}

context exception::get_context() const {
  if (has_context() != true) {
    throw exception("get_context() failed");
  }
  return *ctx;
}
} // namespace neosycl::sycl
