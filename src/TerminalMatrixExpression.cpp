#include "TerminalMatrixExpression.hpp"

TerminalMatrixExpression::TerminalMatrixExpression(const Matrix::EigenMatrixPtr m)
  : m_( m )
{
}

/* virtual */ Matrix::EigenMatrixPtr TerminalMatrixExpression::Eval() const
{
  return m_;
}