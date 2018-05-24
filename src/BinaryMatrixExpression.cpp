#include "BinaryMatrixExpression.hpp"

BinaryMatrixExpression::BinaryMatrixExpression(AbstractExpressionPtr lhs, AbstractExpressionPtr rhs, ExpressionType type, BinaryEvaluator evaluator)
  : AbstractExpression( type )
  , lhs_( lhs )
  , rhs_( rhs )
  , evaluator_( evaluator )
{
}

/* virtual */ BinaryMatrixExpression::ExpressionVariant BinaryMatrixExpression::Eval() const
{
  auto lhs = lhs_->Eval();
  auto rhs = rhs_->Eval();

  return evaluator_(lhs, rhs);
}