#include <nan.h>
#include "Matrix.hpp"
#include "BinaryMatrixOperators.hpp"
#include "BinaryMatrixExpression.hpp"
#include "TerminalMatrixExpression.hpp"
#include "ExpressionTree.hpp"
#include "ExpressionType.hpp"

namespace BinaryMatrixOperators
{
  NAN_MODULE_INIT(Init)
  {
    target->Set(Nan::New("add").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Add)->GetFunction());
    target->Set(Nan::New("product").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Product)->GetFunction());
    target->Set(Nan::New("equals").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Equals)->GetFunction());
  }
}

struct VisitAdd
{
  AbstractExpression::ExpressionVariant operator()(const bool& a, const bool& b) const
  {
    return nullptr;
  }

  template< typename T >
  AbstractExpression::ExpressionVariant operator()(const T& a, const T& b) const
  {
    return a + b;
  }

  template< typename T >
  AbstractExpression::ExpressionVariant operator()(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) const
  {
    if (!a || !b)
    {
      return nullptr;
    }
    return std::make_shared<T>(*a + *b);
  }

  template< typename U, typename V >
  AbstractExpression::ExpressionVariant operator()(const U& a, const V& b) const
  {
    return nullptr;
  }
};

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


  std::shared_ptr<AbstractExpression> lhs;
  ExpressionType lhsType;

  if (Nan::New(Matrix::constructor)->HasInstance(info[0]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    lhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
    lhsType.From(m);
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[0]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[0]->ToObject());
    lhs = tree->GetExpr();
    lhsType.From(tree);
  }

  std::shared_ptr<AbstractExpression> rhs;
  ExpressionType rhsType;
  if (Nan::New(Matrix::constructor)->HasInstance(info[1]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    rhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
    rhsType.From(m);
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[1]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[1]->ToObject());
    rhs = tree->GetExpr();
    rhsType.From(tree);
  }

  if( lhsType.GetRows() == rhsType.GetRows() && lhsType.GetColumns() == rhsType.GetColumns() && lhsType.GetType() == rhsType.GetType() )
  {
    AbstractExpressionPtr exp = std::make_shared<BinaryMatrixExpression>(
      lhs,
      rhs,
      lhsType,
      [](const auto& lhs, const auto& rhs) {
        return std::visit(VisitAdd(), lhs, rhs);
    });

  
    v8::Local<v8::Function> constructorFunc = Nan::New(ExpressionTree::constructor)->GetFunction();
    v8::Local<v8::Object> expJs = Nan::NewInstance(constructorFunc).ToLocalChecked();
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(expJs);
    tree->SetExpr(exp);

    info.GetReturnValue().Set(expJs);
  }
  else
  {
    return Nan::ThrowError(Nan::New("EigenJs.Add - expected lhs and rhs matrix to be of same size and same type").ToLocalChecked());
  }
}

struct VisitProduct
{
  AbstractExpression::ExpressionVariant operator()(const bool& a, const bool& b) const
  {
    return nullptr;
  }

  template< typename T >
  AbstractExpression::ExpressionVariant operator()(const T& a, const T& b) const
  {
    return a * b;
  }

  template< typename T >
  AbstractExpression::ExpressionVariant operator()(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) const
  {
    if (!a || !b)
    {
      return nullptr;
    }
    return std::make_shared<T>(*a * *b);
  }

  template< typename U, typename V >
  AbstractExpression::ExpressionVariant operator()(const U& a, const V& b) const
  {
    return nullptr;
  }
};

NAN_METHOD(Product)
{
  if (info.Length() != 2) {
    return Nan::ThrowError(Nan::New("EigenJs.Prod - expected arguments lhs, rhs").ToLocalChecked());
  }

  if (!Nan::New(Matrix::constructor)->HasInstance(info[0]) && !Nan::New(ExpressionTree::constructor)->HasInstance(info[0])) {
    return Nan::ThrowError(Nan::New("EigenJs.Prod - expected lhs argument to be instance of Matrix").ToLocalChecked());
  }

  if (!Nan::New(Matrix::constructor)->HasInstance(info[1]) && !Nan::New(ExpressionTree::constructor)->HasInstance(info[1])) {
    return Nan::ThrowError(Nan::New("EigenJs.Prod - expected rhs argument to be instance of Matrix").ToLocalChecked());
  }

  std::shared_ptr<AbstractExpression> lhs;
  ExpressionType lhsType;

  if (Nan::New(Matrix::constructor)->HasInstance(info[0]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    lhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
    lhsType.From(m);
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[0]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[0]->ToObject());
    lhs = tree->GetExpr();
    lhsType.From(tree);
  }

  std::shared_ptr<AbstractExpression> rhs;
  ExpressionType rhsType;
  if (Nan::New(Matrix::constructor)->HasInstance(info[1]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    rhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
    rhsType.From(m);
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[1]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[1]->ToObject());
    rhs = tree->GetExpr();
    rhsType.From(tree);
  }

  
  if (lhsType.GetColumns() == rhsType.GetRows() && lhsType.GetType() == rhsType.GetType())
  {
    ExpressionType type(lhsType.GetRows(), rhsType.GetColumns(), lhsType.GetType());

    AbstractExpressionPtr exp = std::make_shared<BinaryMatrixExpression>(
      lhs,
      rhs,
      type,
      [](const auto& lhs, const auto& rhs) {
        return std::visit(VisitProduct(), lhs, rhs);
    });

    v8::Local<v8::Function> constructorFunc = Nan::New(ExpressionTree::constructor)->GetFunction();
    v8::Local<v8::Object> expJs = Nan::NewInstance(constructorFunc).ToLocalChecked();
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(expJs);
    tree->SetExpr(exp);

    info.GetReturnValue().Set(expJs);
  }
  else
  {
    return Nan::ThrowError(Nan::New("EigenJs.Prod - expected lhs columns and rhs rows matrix to be of same size and same type").ToLocalChecked());
  }
}

struct VisitEquals
{
  template< typename T >
  AbstractExpression::ExpressionVariant operator()(const T& a, const T& b) const
  {
    return a == b;
  }

  template< typename T >
  AbstractExpression::ExpressionVariant operator()(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) const
  {
    if (!a || !b)
    {
      return false;
    }
    return *a == *b;
  }

  template< typename U, typename V >
  AbstractExpression::ExpressionVariant operator()(const U& a, const V& b) const
  {
    return false;
  }
};

NAN_METHOD(Equals)
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

  std::shared_ptr<AbstractExpression> lhs;
  ExpressionType lhsType;

  if (Nan::New(Matrix::constructor)->HasInstance(info[0]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
    lhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
    lhsType.From(m);
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[0]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[0]->ToObject());
    lhs = tree->GetExpr();
    lhsType.From(tree);
  }

  std::shared_ptr<AbstractExpression> rhs;
  ExpressionType rhsType;
  if (Nan::New(Matrix::constructor)->HasInstance(info[1]))
  {
    Matrix* m = Nan::ObjectWrap::Unwrap<Matrix>(info[1]->ToObject());
    rhs = std::make_shared<TerminalMatrixExpression>(m->InnerMatrix());
    rhsType.From(m);
  }
  else if (Nan::New(ExpressionTree::constructor)->HasInstance(info[1]))
  {
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(info[1]->ToObject());
    rhs = tree->GetExpr();
    rhsType.From(tree);
  }


  if (lhsType.GetRows() == rhsType.GetRows() && lhsType.GetColumns() == rhsType.GetColumns()  && lhsType.GetType() == rhsType.GetType())
  {
    ExpressionType type(1, 1, ExpressionType::InnerType::SCALAR_BOOLEAN);

    AbstractExpressionPtr exp = std::make_shared<BinaryMatrixExpression>(
      lhs,
      rhs,
      type,
      [](const auto& lhs, const auto& rhs) {
        return std::visit(VisitEquals(), lhs, rhs);
    });

    v8::Local<v8::Function> constructorFunc = Nan::New(ExpressionTree::constructor)->GetFunction();
    v8::Local<v8::Object> expJs = Nan::NewInstance(constructorFunc).ToLocalChecked();
    ExpressionTree* tree = Nan::ObjectWrap::Unwrap<ExpressionTree>(expJs);
    tree->SetExpr(exp);

    info.GetReturnValue().Set(expJs);
  }
  else
  {
    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), false));
  }
}
