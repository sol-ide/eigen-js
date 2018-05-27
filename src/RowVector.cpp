#include "RowVector.hpp"
#include <memory>

Nan::Persistent<v8::FunctionTemplate> RowVector::constructor;

RowVector::RowVector(std::size_t size, const double* data)
  : Matrix( 1, size, data )
{
}

NAN_MODULE_INIT(RowVector::Init)
{
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(RowVector::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("RowVector").ToLocalChecked());

  
  v8::Local<v8::FunctionTemplate> baseCtor = Nan::New<v8::FunctionTemplate>(Matrix::constructor);
  ctor->Inherit(baseCtor);

  target->Set(Nan::New("RowVector").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(RowVector::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("RowVector::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 3 arguments
  if(info.Length() != 1) {
    return Nan::ThrowError(Nan::New("RowVector::New - expected arguments data").ToLocalChecked());
  }

  // expect arguments to be numbers
  if (!info[0]->IsFloat64Array()) {
	  return Nan::ThrowError(Nan::New("RowVector::New - expected last argument to be a float 64 array").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance

  double* data = reinterpret_cast<double*>(info[0].As<v8::Float64Array>()->Buffer()->GetContents().Data());
  std::size_t length = info[0].As<v8::Float64Array>()->Buffer()->GetContents().ByteLength() / sizeof(double);

  RowVector* v = new RowVector(length, data);
  v->Wrap(info.Holder());

  // return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}

/* virtual */ const std::string& RowVector::GetName() const
{
  static const std::string name = "RowVector";
  return name;
}
