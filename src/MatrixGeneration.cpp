#include "MatrixGeneration.hpp"

NAN_MODULE_INIT(Zeros::Init)
{
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(Zeros::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Zeros").ToLocalChecked());

  target->Set(Nan::New("Zeros").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(Zeros::New)
{
  info.GetReturnValue().Set(info.Holder());
}

Nan::Persistent<v8::FunctionTemplate> Zeros::constructor;


NAN_MODULE_INIT(Ones::Init)
{
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(Ones::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Ones").ToLocalChecked());

  target->Set(Nan::New("Ones").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(Ones::New)
{
  info.GetReturnValue().Set(info.Holder());
}

Nan::Persistent<v8::FunctionTemplate> Ones::constructor;