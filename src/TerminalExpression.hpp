#pragma once

#include "Matrix.hpp"
#include "AbstractExpression.hpp"

class TerminalExpression : public AbstractExpression
{
public:

  TerminalExpression(const Matrix::EigenMatrixPtr m);
  TerminalExpression(double scalar);

  virtual ExpressionVariant Eval() const override;

private:
  const ExpressionVariant terminal_;
};