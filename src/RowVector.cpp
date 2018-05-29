#include "RowVector.hpp"
#include "MatrixGeneration.hpp"
#include <memory>
#include <cstddef>


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
  Nan::SetMethod(ctor, "Ones", Ones);
  
  v8::Local<v8::FunctionTemplate> baseCtor = Nan::New<v8::FunctionTemplate>(Matrix::constructor);
  ctor->Inherit(baseCtor);

  

  target->Set(Nan::New("RowVector").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(RowVector::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall())
  {
    return Nan::ThrowError(Nan::New("RowVector::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 3 arguments
  if(info.Length() < 1 || info.Length() > 2)
  {
    return Nan::ThrowError(Nan::New("RowVector::New - expected arguments data").ToLocalChecked());
  }

  // expect arguments to be numbers
  if (!info[0]->IsFloat64Array() && !Nan::New(Zeros::constructor)->HasInstance(info[1]) && !Nan::New(Ones::constructor)->HasInstance(info[1]))
  {
	  return Nan::ThrowError(Nan::New("RowVector::New - expected last argument to be a float 64 array").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance

  RowVector* v = nullptr;
  if (info.Length() == 2 && Nan::New(Ones::constructor)->HasInstance(info[1]))
  {
    std::size_t length = info[0]->NumberValue();
    v = new RowVector(length, Generation::Fill<double>(1));
  }
  else if (info.Length() == 2 && Nan::New(Zeros::constructor)->HasInstance(info[1]))
  {
    std::size_t length = info[0]->NumberValue();
    v = new RowVector(length, Generation::Fill<double>(0));
  }
  else
  {
    const double* data = reinterpret_cast<double*>(info[0].As<v8::Float64Array>()->Buffer()->GetContents().Data());
    std::size_t length = info[0].As<v8::Float64Array>()->Buffer()->GetContents().ByteLength() / sizeof(double);

    v = new RowVector(length, data);
  }
  v->Wrap(info.Holder());

  // return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(RowVector::Ones)
{
  // expect exactly 3 arguments
  if (info.Length() != 1) {
    return Nan::ThrowError(Nan::New("RowVector::Ones - expected arguments length").ToLocalChecked());
  }

  // expect arguments to be numbers
  if (!info[0]->IsNumber()) {
    return Nan::ThrowError(Nan::New("RowVector::Ones - expected arguments to be numbers").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance

  v8::Local<v8::Function> onesFunc = Nan::New(Ones::constructor)->GetFunction();
  v8::Local<v8::Object> onesJs = Nan::NewInstance(onesFunc).ToLocalChecked();

  const int argc = 2;
  v8::Local<v8::Value> argv[argc] = {
    info[0],
    onesJs
  };


  v8::Local<v8::Function> constructorFunc = Nan::New(RowVector::constructor)->GetFunction();
  v8::Local<v8::Object> vectorJs = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();


  info.GetReturnValue().Set(vectorJs);
}

/* virtual */ const std::string& RowVector::GetName() const
{
  static const std::string name = "RowVector";
  return name;
}
