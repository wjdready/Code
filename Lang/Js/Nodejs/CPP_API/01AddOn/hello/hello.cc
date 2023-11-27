
#include <node.h>
#include "add.h"
#include <stdio.h>

namespace demo
{

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Method(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    
    char buf[256];
    snprintf(buf, sizeof(buf), "Hello World 1 + 2 = %d", add(1, 2));
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, buf).ToLocalChecked());
}

void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
