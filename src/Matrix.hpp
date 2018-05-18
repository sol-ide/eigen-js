#pragma once

#include <nan.h>
#include <Eigen/Dense>
#include <memory>

class Matrix : public Nan::ObjectWrap
{
public:
  using EigenMatrix = Eigen::MatrixXd;
  using EigenMatrixPtr = std::shared_ptr<EigenMatrix>;
public:

  Matrix(std::size_t rows, std::size_t columns, const double* data);

  static NAN_MODULE_INIT(Init);

  static NAN_GETTER(HandleRows);
  static NAN_GETTER(HandleColumns);

  static NAN_METHOD(New);

  static Nan::Persistent<v8::FunctionTemplate> constructor;

  std::uint32_t GetRows() const;
  std::uint32_t GetColumns() const;

  EigenMatrixPtr InnerMatrix();
  const EigenMatrixPtr InnerMatrix() const;

  void SetInnerMatrix(EigenMatrixPtr innerMatrix);

private:
  EigenMatrixPtr matrix_;
};
