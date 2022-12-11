#include "myobject.h"

void Initialize(
  Local<Object> exports,
  Local<Value> module,
  Local<Context> context
) {
  Isolate *isolate = context->GetIsolate();
  Local<FunctionTemplate> demo = FunctionTemplate::New(isolate, Demo::New);
  const char * str = "Demo";
  Local<String> name = String::NewFromUtf8(isolate, str, NewStringType::kNormal, strlen(str)).ToLocalChecked();
  demo->InstanceTemplate()->SetInternalFieldCount(1);
  exports->Set(context, name, demo->GetFunction(context).ToLocalChecked()).Check();
}

NODE_MODULE_CONTEXT_AWARE(NODE_GYP_MODULE_NAME, Initialize)