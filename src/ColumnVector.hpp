#pragma once

#include "Matrix.hpp"

class ColumnVector : public Matrix
{
public:

  ColumnVector(std::size_t size, const double* data);

  static NAN_MODULE_INIT(Init);

  static Nan::Persistent<v8::FunctionTemplate> constructor;

  static NAN_METHOD(New);

  virtual const std::string& GetName() const override;
};
