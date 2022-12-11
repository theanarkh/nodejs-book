#include <iostream>
#include <node.h>

namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::FunctionTemplate;
using v8::Function;
using v8::Number;
using v8::MaybeLocal;
using v8::Context;
using v8::Int32;
using v8::Global;

void ProtoMethod(const FunctionCallbackInfo<Value>& args) {
    std::cout<<"call ProtoMethod"<<std::endl;
}

// 定义一个加法函数
void InstanceMethod(const FunctionCallbackInfo<Value>& args) {
    std::cout<<"call InstanceMethod"<<std::endl;
}

Local<String> newString(const char * name) {
  return String::NewFromUtf8(Isolate::GetCurrent(), name, v8::NewStringType::kNormal).ToLocalChecked();
}

class Dummy {
  public:
    Dummy(Local<Object> object): jsObject(Isolate::GetCurrent(), object) {
      object->SetAlignedPointerInInternalField(0, static_cast<void*>(this));
      Local<Context> context = Isolate::GetCurrent()->GetCurrentContext();
      (void)object->Set(context, newString("hello"), newString("world"));
    };
    Global<Object> jsObject;
    int dummy_field = 1;
};

// 定义一个加法函数
void New(const FunctionCallbackInfo<Value>& args) {
    new Dummy(args.This());
}

void Method(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    Local<String> hello = newString("hello");
    Local<String> helloValue = args.Holder()->Get(context, hello).ToLocalChecked().As<String>();
    Dummy* dummy = static_cast<Dummy*>(args.Holder()->GetAlignedPointerFromInternalField(0));
    Local<Object> obj = Object::New(isolate);
    (void)obj->Set(context, hello, helloValue);
    (void)obj->Set(context, newString("dummy_field"), Number::New(isolate, dummy->dummy_field));
    std::cout<<(args.Holder() == args.This() ? "same" : "not same" )<<std::endl;
    args.GetReturnValue().Set(obj);
}

void Initialize(
  Local<Object> exports,
  Local<Value> module,
  Local<Context> context
) {
  Isolate* isolate = context->GetIsolate();
  
  // 新建一个函数模版
  Local<FunctionTemplate> funcWithCallback = FunctionTemplate::New(isolate, New);
  funcWithCallback->InstanceTemplate()->SetInternalFieldCount(1);
  funcWithCallback->PrototypeTemplate()->Set(isolate, "method", FunctionTemplate::New(isolate, Method));
  // 新建一个字符串表示函数名
  Local<String> funcWithCallbackName = String::NewFromUtf8(isolate, "FuncWithCallback", v8::NewStringType::kNormal).ToLocalChecked();
  // 设置函数名
  funcWithCallback->SetClassName(funcWithCallbackName);
  Local<Function> funcWithCallbackInstance = funcWithCallback->GetFunction(context).ToLocalChecked();

  // 新建一个函数模版
  Local<FunctionTemplate> parentFunc = FunctionTemplate::New(isolate);
  // 新建一个字符串表示函数名
  Local<String> parentName = String::NewFromUtf8(isolate, "Parent", v8::NewStringType::kNormal).ToLocalChecked();
  // 设置函数名
  parentFunc->SetClassName(parentName);
  // 设置原型属性
  parentFunc->PrototypeTemplate()->Set(isolate, "protoField", Number::New(isolate, 1));
  parentFunc->PrototypeTemplate()->Set(isolate, "protoMethod", FunctionTemplate::New(isolate, ProtoMethod));
  // 设置对象属性
  parentFunc->InstanceTemplate()->Set(isolate, "instanceField", Number::New(isolate, 2));
  parentFunc->InstanceTemplate()->Set(isolate, "instanceMethod", FunctionTemplate::New(isolate, InstanceMethod));

  // 根据函数模版创建一个函数
  Local<Function> parentInstance = parentFunc->GetFunction(context).ToLocalChecked();

  Local<FunctionTemplate> childFunc = FunctionTemplate::New(isolate);
  Local<String> childName = String::NewFromUtf8(isolate, "Child", v8::NewStringType::kNormal).ToLocalChecked();
  childFunc->SetClassName(childName);
  childFunc->Inherit(parentFunc);

  Local<Function> childInstance = childFunc->GetFunction(context).ToLocalChecked();

  // 导出
  exports->Set(context, funcWithCallbackName, funcWithCallbackInstance).Check();
  exports->Set(context, parentName, parentInstance).Check();
  exports->Set(context, childName, childInstance).Check();
}

NODE_MODULE_CONTEXT_AWARE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace demo