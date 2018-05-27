#include "TerminalExpression.hpp"

TerminalExpression::TerminalExpression(const Matrix::EigenMatrixPtr m)
  : AbstractExpression(ExpressionType(m->rows(), m->cols(), ExpressionType::InnerType::MATRIX_DOUBLE))
  , terminal_(m)
{
}

TerminalExpression::TerminalExpression(double scalar)
  : AbstractExpression(ExpressionType(1, 1, ExpressionType::InnerType::SCALAR_DOUBLE))
  , terminal_(scalar)
{
}

/* virtual */ TerminalExpression::ExpressionVariant TerminalExpression::Eval() const
{
  return terminal_;
}