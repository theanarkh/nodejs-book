#include <iostream>
#include <node.h>

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

Local<String> newString(const char * name) {
  return String::NewFromUtf8(Isolate::GetCurrent(), name, v8::NewStringType::kNormal).ToLocalChecked();
}

void ProtoMethod(const FunctionCallbackInfo<Value>& args) {
    std::cout<<"call ProtoMethod"<<std::endl;
}

// 定义一个加法函数
void InstanceMethod(const FunctionCallbackInfo<Value>& args) {
    std::cout<<"call InstanceMethod"<<std::endl;
}

void Initialize(
  Local<Object> exports,
  Local<Value> module,
  Local<Context> context
) {
        Isolate* isolate = context->GetIsolate();
        // Parent
        // 新建一个函数模版
        Local<FunctionTemplate> parentFunc = FunctionTemplate::New(isolate);
        // 新建一个字符串表示函数名
        Local<String> parentName = String::NewFromUtf8(isolate, "Parent", v8::NewStringType::kNormal).ToLocalChecked();
        // 设置函数名
        parentFunc->SetClassName(parentName);
        // 设置原型属性
        parentFunc->PrototypeTemplate()->Set(isolate, "protoField", Number::New(isolate, 1));
        // 设置原型函数，JS 调 protoMethod 时就会调 ProtoMethod 函数
        parentFunc->PrototypeTemplate()->Set(isolate, "protoMethod", FunctionTemplate::New(isolate, ProtoMethod));
        // 设置对象属性
        parentFunc->InstanceTemplate()->Set(isolate, "instanceField", Number::New(isolate, 2));
        parentFunc->InstanceTemplate()->Set(isolate, "instanceMethod", FunctionTemplate::New(isolate, InstanceMethod));
        
        // Child
        Local<FunctionTemplate> childFunc = FunctionTemplate::New(isolate);
        Local<String> childName = String::NewFromUtf8(isolate, "Child", v8::NewStringType::kNormal).ToLocalChecked();
        childFunc->SetClassName(childName);
        childFunc->PrototypeTemplate()->Set(isolate, "childProtoField", Number::New(isolate, 1));
        childFunc->InstanceTemplate()->Set(isolate, "childInstanceField", Number::New(isolate, 2));
        // 继承 parentFunc 函数模版
        childFunc->Inherit(parentFunc);

        Local<Function> parentInstance = parentFunc->GetFunction(context).ToLocalChecked();
        Local<Function> childInstance = childFunc->GetFunction(context).ToLocalChecked();
        exports->Set(context, parentName, parentInstance).Check();
        exports->Set(context, childName, childInstance).Check();
}

// addon 定义
NODE_MODULE_CONTEXT_AWARE(NODE_GYP_MODULE_NAME, Initialize)