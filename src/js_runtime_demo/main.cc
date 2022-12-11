#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "libplatform/libplatform.h"
#include "v8.h"

using namespace v8;

int main(int argc, char* argv[]) {
  setvbuf(stdout, nullptr, _IONBF, 0);
  setvbuf(stderr, nullptr, _IONBF, 0);
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<Platform> platform = platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
  Isolate* isolate = Isolate::New(create_params);
  {
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    
    // 创建一个对象模块
    Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
    // 创建一个执行上下文
    Local<Context> context = Context::New(isolate, nullptr, global);
    Context::Scope context_scope(context);
    // 创建一个对象
    Local<Object> A = Object::New(isolate);
    // 获取执行上下文中的全局对象
    Local<Object> globalInstance = context->Global();
    // 给全局对象设置一个属性 A，值是上面创建的 A 对象
    globalInstance->Set(context, String::NewFromUtf8Literal(isolate, "A", NewStringType::kNormal), A);
    {
      // 在代码里访问刚才设置的 A 对象，改成其他字符串试试
      // 打开文件
      int fd = open("main.js", 0, O_RDONLY);
      struct stat info;
      // 取得文件信息
      fstat(fd, &info);
      // 分配内存保存文件内容
      char *ptr = (char *)malloc(info.st_size + 1);
      // ptr[info.st_size] = '\0';
      read(fd, (void *)ptr, info.st_size);
      // 要执行的js代码
      Local<String> source = String::NewFromUtf8(isolate, ptr,
                          NewStringType::kNormal,
                          info.st_size).ToLocalChecked();
      // 编译
      Local<Script> script = Script::Compile(context, source).ToLocalChecked();
      // 执行
      Local<Value> result = script->Run(context).ToLocalChecked();
    }
  }

  isolate->Dispose();
  v8::V8::Dispose();
  delete create_params.array_buffer_allocator;
  return 0;
}