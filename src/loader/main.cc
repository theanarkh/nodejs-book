#include "node.h"
#include <fcntl.h>
#include <unistd.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::FunctionTemplate;
using v8::Function;
using v8::MaybeLocal;
using v8::Context;
using v8::ScriptCompiler;

static v8::Local<v8::String> newStringToLcal(v8::Isolate * isolate, const char * str, v8::NewStringType type = v8::NewStringType::kNormal) {
    return v8::String::NewFromUtf8(isolate, str, type, strlen(str)).ToLocalChecked();
}

static void Compile(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    String::Utf8Value filename(isolate, args[0].As<String>());
    int fd = open(*filename, 0 , O_RDONLY);
    std::string content;
    char buffer[4096];
    while (1)
    {
      memset(buffer, 0, 4096);
      int ret = read(fd, buffer, 4096);
      if (ret == -1) {
        return args.GetReturnValue().Set(newStringToLcal(isolate, "read file error"));
      }
      if (ret == 0) {
        break;
      }
      content.append(buffer, ret);
    }
    close(fd);
    ScriptCompiler::Source script_source(newStringToLcal(isolate, content.c_str()));
    Local<String> params[] = {
      newStringToLcal(isolate, "require"),
      newStringToLcal(isolate, "exports"),
      newStringToLcal(isolate, "module"),
    };
    MaybeLocal<Function> fun =
    ScriptCompiler::CompileFunctionInContext(context, &script_source, 3, params, 0, nullptr);
    if (fun.IsEmpty()) {
      args.GetReturnValue().Set(Undefined(isolate));
    } else {
      args.GetReturnValue().Set(fun.ToLocalChecked());
    }
}

void Initialize(
  Local<Object> exports,
  Local<Value> module,
  Local<Context> context
) {
  NODE_SET_METHOD(exports, "compile", Compile);
}
extern "C" __attribute__((visibility("default"))) void node_register_module_v108(
  Local<Object> exports,
  Local<Value> module,
  Local<Context> context
) {
  NODE_SET_METHOD(exports, "compile", Compile);
}
NODE_MODULE_CONTEXT_AWARE(NODE_GYP_MODULE_NAME, Initialize)