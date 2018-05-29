#pragma once

#include "Matrix.hpp"

class ColumnVector : public Matrix
{
public:

  template< typename Generator >
  ColumnVector(std::size_t length, Generator generator);

  ColumnVector(std::size_t size, const double* data);

  static NAN_MODULE_INIT(Init);

  static Nan::Persistent<v8::FunctionTemplate> constructor;

  static NAN_METHOD(New);
  static NAN_METHOD(Ones);

  virtual const std::string& GetName() const override;
};

#include "ColumnVector.hxx"