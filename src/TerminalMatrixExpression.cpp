#include "TerminalMatrixExpression.hpp"

TerminalMatrixExpression::TerminalMatrixExpression(const Matrix::EigenMatrixPtr m)
  : AbstractExpression(ExpressionType(m->rows(), m->cols(), ExpressionType::InnerType::MATRIX_DOUBLE))
  , m_(m)
{
}

/* virtual */ TerminalMatrixExpression::ExpressionVariant TerminalMatrixExpression::Eval() const
{
  return m_;
}