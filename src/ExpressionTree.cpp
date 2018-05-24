
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
  Nan::SetPrototypeMethod(ctor, "then", Then);

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
    result_ = expr_->Eval();
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    v8::Local<v8::Promise::Resolver> resolver = GetFromPersistent(0U).As<v8::Promise::Resolver>();


    std::visit([resolver](auto&& result) {
      using T = std::decay_t< decltype(result) >;
      if constexpr(std::is_same < T, Matrix::EigenMatrixPtr >::value)
      {
        const int argc = 2;
        v8::Local<v8::Value> argv[argc] = { Nan::New(0), Nan::New(0) };

        v8::Local<v8::Function> constructorFunc = Nan::New(Matrix::constructor)->GetFunction();
        v8::Local<v8::Object> matrixJs = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();

        Matrix* matrix = Nan::ObjectWrap::Unwrap<Matrix>(matrixJs);
        matrix->SetInnerMatrix(result);
        resolver->Resolve(Nan::GetCurrentContext(), matrix->handle());
      }
      else if constexpr(std::is_same< T, bool >::value)
      {
        resolver->Resolve(Nan::GetCurrentContext(), v8::Boolean::New(Nan::GetCurrentContext()->GetIsolate(), result) );
      }
      else
      {
        resolver->Resolve(Nan::GetCurrentContext(), v8::Number::New(Nan::GetCurrentContext()->GetIsolate(), result));
      }
    }, result_);

  }

  void HandleErrorCallback() {
    Nan::HandleScope scope;
    v8::Local<v8::Promise::Resolver> resolver = GetFromPersistent(0U).As<v8::Promise::Resolver>();
    resolver->Reject(Nan::GetCurrentContext(), Nan::Undefined());
  }

  AbstractExpressionPtr expr_;
  AbstractExpression::ExpressionVariant result_;
};

NAN_METHOD(ExpressionTree::Eval)
{
  ExpressionTree* expression = Nan::ObjectWrap::Unwrap<ExpressionTree>(info.This());

  AbstractExpressionPtr expr = expression->GetExpr();

  AbstractExpression::ExpressionVariant result = expr->Eval();

  std::visit([&info](auto&& result) {
    using T = std::decay_t< decltype(result) >;
    if constexpr(std::is_same < T, Matrix::EigenMatrixPtr >::value)
    {
      const int argc = 2;
      v8::Local<v8::Value> argv[argc] = { Nan::New(0), Nan::New(0) };

      v8::Local<v8::Function> constructorFunc = Nan::New(Matrix::constructor)->GetFunction();
      v8::Local<v8::Object> matrixJs = Nan::NewInstance(constructorFunc, argc, argv).ToLocalChecked();
      Matrix* matrix = Nan::ObjectWrap::Unwrap<Matrix>(matrixJs);

      matrix->SetInnerMatrix(result);

      info.GetReturnValue().Set(matrix->handle());
    }
    else if constexpr(std::is_same< T, bool >::value)
    {
      info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), result));
    }
    else
    {
      info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), result));
    }
  }, result);


}

NAN_METHOD(ExpressionTree::Then)
{
  if (info.Length() != 1)
  {
    return Nan::ThrowError(Nan::New("ExpressionTree::Then - argument continuation").ToLocalChecked());
  }

  if (!info[0]->IsFunction())
  {
    return Nan::ThrowError(Nan::New("ExpressionTree::Then - argument continuation should be a function").ToLocalChecked());
  }
  
  v8::Local<v8::Function> continuation = info[0].As<v8::Function>();
  ExpressionTree* expression = Nan::ObjectWrap::Unwrap<ExpressionTree>(info.This());

  v8::Local<v8::Promise::Resolver> resolver = v8::Promise::Resolver::New(info.GetIsolate());
  

  auto worker = new EvalTreeWorker(expression->GetExpr(), nullptr);
  worker->SaveToPersistent(0U, resolver);

  Nan::AsyncQueueWorker(worker);
  
  
  v8::MaybeLocal<v8::Promise> resultingPromise = resolver->GetPromise()->Then(Nan::GetCurrentContext(), continuation);
  info.GetReturnValue().Set(resultingPromise.ToLocalChecked());
}