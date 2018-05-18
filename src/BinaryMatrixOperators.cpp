#include <nan.h>
#include "Matrix.hpp"
#include "BinaryMatrixOperators.hpp"
#include "BinaryMatrixExpression.hpp"
#include "TerminalMatrixExpression.hpp"
#include "ExpressionTree.hpp"

namespace BinaryMatrixOperators
{
  NAN_MODULE_INIT(Init)
  {
    target->Set(Nan::New("add").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Add)->GetFunction());
    target->Set(Nan::New("multiply").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Multiply)->GetFunction());
  }
}

NAN_METHOD(Add)
{
  if (info.Length() != 2) {
    return Nan::ThrowError(Nan::New("EigenJs.Add - expected arguments lhs, rhs").ToLocalChecked());
  }

  if (!Nan::New(Matrix::constructor)->HasInstance(info[0]) && !Nan::New(ExpressionTree::constructor)->HasInstance(info[0])) {
    return Nan::ThrowError(Nan::New("EigenJs.Add - expected lhs argument to be instance of Matrix").ToLocalChecked());
  }

  if (!Nan::New(Matrix::constructor)->HasInstance(info[1]) && !Nan::New(ExpressionTree::constructor)->HasInstance(info[1])) {
    return Nan::ThrowError(Nan::New("EigenJs.Add - expected rhs argument to be instance of Matrix").ToLocalChecked());
  }

  AbstractExpressionPtr lhs;
  if (Nan::New(Matrix::constructor)->HasInstance(info[0]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    lhs = std::make_shared<TerminalMatrixExpression>( m->InnerMatrix() );
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[0]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[0]->ToObject());
    lhs = tree->GetExpr();
  }

  AbstractExpressionPtr rhs;
  if (Nan::New(Matrix::constructor)->HasInstance(info[1]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    rhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[1]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[1]->ToObject());
    rhs = tree->GetExpr();
  }

  AbstractExpressionPtr exp = std::make_shared<BinaryMatrixExpression>(lhs, rhs, [](const auto& lhs, const auto& rhs) {
    return lhs + rhs;
  });
  
  v8::Local<v8::Function> constructorFunc = Nan::New(ExpressionTree::constructor)->GetFunction();
  v8::Local<v8::Object> expJs = Nan::NewInstance(constructorFunc).ToLocalChecked();
  ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(expJs);
  tree->SetExpr(exp);

  info.GetReturnValue().Set(expJs);
}

NAN_METHOD(Multiply)
{
  if (info.Length() != 2) {
    return Nan::ThrowError(Nan::New("EigenJs.Add - expected arguments lhs, rhs").ToLocalChecked());
  }

  if (!Nan::New(Matrix::constructor)->HasInstance(info[0]) && !Nan::New(ExpressionTree::constructor)->HasInstance(info[0])) {
    return Nan::ThrowError(Nan::New("EigenJs.Add - expected lhs argument to be instance of Matrix").ToLocalChecked());
  }

  if (!Nan::New(Matrix::constructor)->HasInstance(info[1]) && !Nan::New(ExpressionTree::constructor)->HasInstance(info[1])) {
    return Nan::ThrowError(Nan::New("EigenJs.Add - expected rhs argument to be instance of Matrix").ToLocalChecked());
  }

  AbstractExpressionPtr lhs;
  if (Nan::New(Matrix::constructor)->HasInstance(info[0]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    lhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[0]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[0]->ToObject());
    lhs = tree->GetExpr();
  }

  AbstractExpressionPtr rhs;
  if (Nan::New(Matrix::constructor)->HasInstance(info[1]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    rhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[1]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[1]->ToObject());
    rhs = tree->GetExpr();
  }

  AbstractExpressionPtr exp = std::make_shared<BinaryMatrixExpression>(lhs, rhs, [](const auto& lhs, const auto& rhs) {
    return lhs * rhs;
  });

  v8::Local<v8::Function> constructorFunc = Nan::New(ExpressionTree::constructor)->GetFunction();
  v8::Local<v8::Object> expJs = Nan::NewInstance(constructorFunc).ToLocalChecked();
  ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(expJs);
  tree->SetExpr(exp);

  info.GetReturnValue().Set(expJs);
}
