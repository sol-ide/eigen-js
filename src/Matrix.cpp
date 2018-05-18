#include "Matrix.hpp"
#include <memory>

Nan::Persistent<v8::FunctionTemplate> Matrix::constructor;

Matrix::Matrix(std::size_t rows, std::size_t columns, const double* data)
{
  matrix_ = std::make_shared<EigenMatrix>(rows, columns);

  if (data != nullptr)
  {
    std::memcpy(
      matrix_->data(),
      data,
      rows * columns
    );
  }
}

std::uint32_t Matrix::GetRows() const
{
	return matrix_->rows();
}

std::uint32_t Matrix::GetColumns() const
{
	return matrix_->cols();
}

Matrix::EigenMatrixPtr Matrix::InnerMatrix()
{
	return matrix_;
}

const Matrix::EigenMatrixPtr Matrix::InnerMatrix() const
{
  return matrix_;
}

void Matrix::SetInnerMatrix(EigenMatrixPtr other)
{
  matrix_ = other;
}



NAN_MODULE_INIT(Matrix::Init)
{
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(Matrix::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Matrix").ToLocalChecked());

  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("rows").ToLocalChecked(), HandleRows);
  Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("columns").ToLocalChecked(), HandleColumns);

  target->Set(Nan::New("Matrix").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(Matrix::New) {
  // throw an error if constructor is called without new keyword
  if(!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("Matrix::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 3 arguments
  if(info.Length() < 2 || info.Length() > 3) {
    return Nan::ThrowError(Nan::New("Matrix::New - expected arguments rows, columns, data").ToLocalChecked());
  }

  // expect arguments to be numbers
  if(!info[0]->IsNumber() || !info[1]->IsNumber()) {
    return Nan::ThrowError(Nan::New("Matrix::New - expected arguments to be numbers").ToLocalChecked());
  }

  // expect arguments to be numbers
  if (info.Length() > 2 && !info[2]->IsFloat64Array()) {
	  return Nan::ThrowError(Nan::New("Matrix::New - expected last argument to be a float 64 array").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance


  std::size_t rows = info[0]->NumberValue();
  std::size_t columns = info[1]->NumberValue();
  double* data = nullptr;
  std::size_t length = 0;
  if (info.Length() > 2)
  {
    data = reinterpret_cast<double*>(info[2].As<v8::Float64Array>()->Buffer()->GetContents().Data());
    length = info[2].As<v8::Float64Array>()->Buffer()->GetContents().ByteLength() / sizeof(double);
  }

  if (length != rows * columns) {
    return Nan::ThrowError(Nan::New("Matrix::New - expected array size = rows x columns").ToLocalChecked());
  }

  Matrix* m = new Matrix(rows, columns, data);
  m->Wrap(info.Holder());

  // return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}

NAN_PROPERTY_GETTER(Matrix::HandleRows) {
    Matrix* self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
    info.GetReturnValue().Set(self->GetRows());
}

NAN_PROPERTY_GETTER(Matrix::HandleColumns) {
    Matrix* self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
    info.GetReturnValue().Set(self->GetColumns());
}