#ifndef DEMO_H
#define DEMO_H

#include <node.h>
#include <uv.h>
#include <node_object_wrap.h>

using namespace v8;

class Demo: public node::ObjectWrap {
   public:
      Demo(): node::ObjectWrap() {
         uv_timer_init(uv_default_loop(), &timer);
         timer.data = this;
         uv_timer_start(&timer, [] (uv_timer_t *timer) {
            Demo *demo = static_cast<Demo*>(timer->data);
            printf("timeout%d\n", demo->i);
         }, 1000, 1000);
      };
      ~Demo() override {
         printf("~Demo\n");
      }
      static void New(const FunctionCallbackInfo<Value>& args) {
         Demo * demo = new Demo();
         // 把 JS 的 new Demo() 对象和 C++ 的 Demo 对象互相关联起来
         demo->Wrap(args.This());
         // demo->Ref();
         args.GetReturnValue().Set(args.This());
      }

      int i = 1;
   private:
      uv_timer_t timer;
};
#endif