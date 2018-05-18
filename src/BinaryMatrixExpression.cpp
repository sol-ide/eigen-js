#include "BinaryMatrixExpression.hpp"

BinaryMatrixExpression::BinaryMatrixExpression(AbstractExpressionPtr lhs, AbstractExpressionPtr rhs, BinaryEvaluator evaluator)
  : lhs_( lhs )
  , rhs_( rhs )
  , evaluator_( evaluator )
{
}

/* virtual */ Matrix::EigenMatrixPtr BinaryMatrixExpression::Eval() const
{
  auto lhs = lhs_->Eval();
  auto rhs = rhs_->Eval();

  if (lhs && rhs)
  {
    auto result = evaluator_(*lhs, *rhs);
    return std::make_shared<Matrix::EigenMatrix>(std::move(result));
  }
  return nullptr;
}