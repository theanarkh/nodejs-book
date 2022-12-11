#include "uv.h"

int main() {
  uv_async_t async_handle;
  uv_thread_t thread;
  
  uv_async_init(uv_default_loop(), &async_handle, [](uv_async_t* handle) {
    printf("done\n");
    uv_close((uv_handle_t*) handle, nullptr);
  });

  uv_thread_create(&thread, [] (void* args) {
   // uv_async_send((uv_async_t*)args);
  }, &async_handle);
  
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  uv_thread_join(&thread);
  return 0;
}
