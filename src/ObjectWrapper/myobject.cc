#include "myobject.h"

namespace demo {

MyObject::MyObject(): node::ObjectWrap() {
}

MyObject::~MyObject() {
  printf("~MyObject\n");
}

void MyObject::New(const FunctionCallbackInfo<Value>& args) {
  MyObject* obj = new MyObject();
  obj->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}


void MyObject::SetRef(const FunctionCallbackInfo<Value>& args) {
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
  obj->Ref();
}

void MyObject::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<String> name = String::NewFromUtf8(isolate, "MyObject").ToLocalChecked();
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, MyObject::New);
  tpl->SetClassName(name);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(tpl, "ref", MyObject::SetRef);
  exports->Set(context, name, tpl->GetFunction(context).ToLocalChecked());
}


}  // namespace demo