#include <node.h>
#include "myobject.h"

namespace demo {

using v8::Local;
using v8::Object;


void InitAll(Local<Object> exports, Local<Object> module) {
  MyObject::Init(exports);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, InitAll)

}  // namespace demo