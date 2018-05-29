#pragma once

#include "Matrix.hpp"

class RowVector : public Matrix
{
public:

  template< typename Generator >
  RowVector(std::size_t length, Generator generator);

  RowVector(std::size_t size, const double* data);

  static NAN_MODULE_INIT(Init);

  static Nan::Persistent<v8::FunctionTemplate> constructor;

  static NAN_METHOD(New);
  static NAN_METHOD(Ones);

  virtual const std::string& GetName() const override;
};

#include "RowVector.hxx"
