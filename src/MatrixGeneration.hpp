#pragma once

#include <nan.h>

namespace Generation
{
  template< typename T >
  class Fill
  {
  public:
    Fill(T value);

    void operator()(std::size_t rows, std::size_t columns, T* data);

  private:
    T value_;
  };

  template< typename T >
  struct Diagonal {
  private:
    Diagonal(T value);

    void operator()(std::size_t rows, std::size_t columns, T* data);
  private:
    T value_;
  };
}

struct Zeros : public Nan::ObjectWrap
{
  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);
  static Nan::Persistent<v8::FunctionTemplate> constructor;
};

struct Ones : public Nan::ObjectWrap
{
  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);
  static Nan::Persistent<v8::FunctionTemplate> constructor;
};

#include "MatrixGeneration.hxx"