#pragma once

#include "Matrix.hpp"
#include "AbstractExpression.hpp"

class TerminalMatrixExpression : public AbstractExpression
{
public:

  TerminalMatrixExpression(const Matrix::EigenMatrixPtr m);

  virtual ExpressionVariant Eval() const override;

private:
  const Matrix::EigenMatrixPtr m_;
};