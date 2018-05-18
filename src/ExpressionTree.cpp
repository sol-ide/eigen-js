
#include "ExpressionTree.hpp"
#include <memory>

Nan::Persistent<v8::FunctionTemplate> ExpressionTree::constructor;

void ExpressionTree::SetExpr(AbstractExpressionPtr expr)
{
  tree_ = expr;
}

AbstractExpressionPtr ExpressionTree::GetExpr() const
{
  return tree_;
}

NAN_MODULE_INIT(ExpressionTree::Init)
{
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(ExpressionTree::New);
  constructor.Reset(ctor);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("ExpressionTree").ToLocalChecked());

  Nan::SetPrototypeMethod(ctor, "eval", Eval);

  target->Set(Nan::New("ExpressionTree").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(ExpressionTree::New)
{
  // throw an error if constructor is called without new keyword
  if (!info.IsConstructCall()) {
    return Nan::ThrowError(Nan::New("ExpressionTree::New - called without new keyword").ToLocalChecked());
  }

  // expect exactly 3 arguments
  if (info.Length() != 0) {
    return Nan::ThrowError(Nan::New("ExpressionTree::New - expected no argument").ToLocalChecked());
  }

  ExpressionTree* tree = new ExpressionTree();
  tree->Wrap(info.Holder());

  // return the wrapped javascript instance
  info.GetReturnValue().Set(info.Holder());
}


class EvalTreeWorker : public Nan::AsyncWorker
{
public:
  EvalTreeWorker(AbstractExpressionPtr expr, Nan::Callback *callback)
    : Nan::AsyncWorker(callback)
    , expr_(expr)
  {

  }

private:
  virtual void Execute() override
  {
    resultMatrix_ = expr_->Eval();
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    v8::Local<v8::Promise::Resolver> resolver = GetFromPersistent(0U).As<v8::Promise::Resolver>();

    const int argc = 2;
    v8::Local<v8::Value> argv[argc] = { Nan::New(0), Nan::New(0) };

    v8::Local<v8::Function> constructorFunc = Nan::New(Matrix::constructor)->GetFunction();
    v8::Local<v8::Object> matrixJs = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();
    Matrix* matrix = Nan::ObjectWrap::Unwrap<Matrix>(matrixJs);
    matrix->SetInnerMatrix(resultMatrix_);

    resolver->Resolve(Nan::GetCurrentContext(), matrix->handle());
  }

  void HandleErrorCallback() {
    Nan::HandleScope scope;
    v8::Local<v8::Promise::Resolver> resolver = GetFromPersistent(0U).As<v8::Promise::Resolver>();
    resolver->Reject(Nan::GetCurrentContext(), Nan::Undefined());
  }

  AbstractExpressionPtr expr_;
  Matrix::EigenMatrixPtr resultMatrix_;
};

NAN_METHOD(ExpressionTree::Eval)
{
  auto resolver = v8::Promise::Resolver::New(info.GetIsolate());
  
  ExpressionTree* expression = Nan::ObjectWrap::Unwrap<ExpressionTree>(info.This());
  auto worker = new EvalTreeWorker(expression->GetExpr(), nullptr);
  worker->SaveToPersistent(0U, resolver);

  Nan::AsyncQueueWorker(worker);
  info.GetReturnValue().Set(resolver->GetPromise());
}