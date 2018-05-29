#include "Matrix.hpp"
#include "MatrixGeneration.hpp"
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
      (rows * columns)*sizeof(EigenMatrix::value_type)
    );
  }
}

/* virtual */ const std::string& Matrix::GetName() const
{
  static const std::string name = "Matrix";
  return name;
}

std::size_t Matrix::GetRows() const
{
	return matrix_->rows();
}

std::size_t Matrix::GetColumns() const
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

  Nan::SetPrototypeMethod(ctor, "toString", ToString);

  Nan::SetMethod(ctor, "Ones", Ones);

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
  if (info.Length() > 2 && (!info[2]->IsFloat64Array() && !Nan::New(Zeros::constructor)->HasInstance(info[2]) && !Nan::New(Ones::constructor)->HasInstance(info[2]))) {
	  return Nan::ThrowError(Nan::New("Matrix::New - expected last argument to be a float 64 array").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance

  std::size_t rows = info[0]->NumberValue();
  std::size_t columns = info[1]->NumberValue();

  Matrix* m = nullptr;
  if(info.Length() > 2 && Nan::New(Ones::constructor)->HasInstance(info[2]))
  {
    m = new Matrix(rows, columns, Generation::Fill<double>(1));
  }
  else if (info.Length() > 2 && Nan::New(Zeros::constructor)->HasInstance(info[2]))
  {
    m = new Matrix(rows, columns, Generation::Fill<double>(0));
  }
  else
  {
    const double* data = nullptr;
    std::size_t length = 0;
    if (info.Length() > 2)
    {
      data = reinterpret_cast<const double*>(info[2].As<v8::Float64Array>()->Buffer()->GetContents().Data());
      length = info[2].As<v8::Float64Array>()->Buffer()->GetContents().ByteLength() / sizeof(double);
    }

    if (length != rows * columns)
    {
      return Nan::ThrowError(Nan::New("Matrix::New - expected array size = rows x columns").ToLocalChecked());
    }

    m = new Matrix(rows, columns, data);
  }

  m->Wrap(info.Holder());

  // return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(Matrix::Ones)
{
  // expect exactly 3 arguments
  if (info.Length() != 2) {
    return Nan::ThrowError(Nan::New("Matrix::Ones - expected arguments rows, columns").ToLocalChecked());
  }

  // expect arguments to be numbers
  if (!info[0]->IsNumber() || !info[1]->IsNumber()) {
    return Nan::ThrowError(Nan::New("Matrix::Ones - expected arguments to be numbers").ToLocalChecked());
  }

  // create a new instance and wrap our javascript instance

  v8::Local<v8::Function> onesFunc = Nan::New(Ones::constructor)->GetFunction();
  v8::Local<v8::Object> onesJs = Nan::NewInstance(onesFunc).ToLocalChecked();

  const int argc = 3;
  v8::Local<v8::Value> argv[argc] = {
    info[0],
    info[1],
    onesJs
  };


  v8::Local<v8::Function> constructorFunc = Nan::New(Matrix::constructor)->GetFunction();
  v8::Local<v8::Object> matrixJs = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();


  info.GetReturnValue().Set(matrixJs);
}

NAN_METHOD(Matrix::ToString)
{
  Matrix* self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
  std::ostringstream oss;
  const auto rows = self->matrix_->rows();
  const auto columns = self->matrix_->cols();

  for (Eigen::Index indexRow = 0; indexRow < rows; ++indexRow)
  {
    if (indexRow == 0)
    {
      oss << self->GetName() << " : ";
    }
    
    oss << std::endl << '\t';

    const auto& row = self->matrix_->row(indexRow);
    for (Eigen::Index indexColumn = 0; indexColumn < columns; ++indexColumn)
    {
      if (indexColumn > 0)
      {
        oss << " ";
      }
      oss << row(indexColumn);
    }
  }
  v8::MaybeLocal<v8::String> string = v8::String::NewFromUtf8(info.GetIsolate(), oss.str().c_str(), v8::NewStringType::kNormal);
  info.GetReturnValue().Set(string.ToLocalChecked());
}

NAN_PROPERTY_GETTER(Matrix::HandleRows)
{
    Matrix* self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), self->GetRows()));
}

NAN_PROPERTY_GETTER(Matrix::HandleColumns)
{
    Matrix* self = Nan::ObjectWrap::Unwrap<Matrix>(info.This());
    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), self->GetColumns()));
}