#pragma once
#include <nan.h>
#include "ExpressionTree.hpp"
#include "AbstractExpression.hpp"

class ExpressionTree : public Nan::ObjectWrap
{
public:

  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);
  static NAN_METHOD(Eval);

  void SetExpr(AbstractExpressionPtr expr);
  AbstractExpressionPtr GetExpr() const;

  static Nan::Persistent<v8::FunctionTemplate> constructor;

private:
  AbstractExpressionPtr tree_;
};

