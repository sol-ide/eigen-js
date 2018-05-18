#pragma once

#include "Matrix.hpp"
#include "AbstractExpression.hpp"

class BinaryMatrixExpression : public AbstractExpression
{
public:
  using BinaryEvaluator = std::function<Matrix::EigenMatrix(const Matrix::EigenMatrix& lhs, const Matrix::EigenMatrix& rhs)>;
public:
  BinaryMatrixExpression(AbstractExpressionPtr lhs, AbstractExpressionPtr rhs, BinaryEvaluator evaluator);

  virtual Matrix::EigenMatrixPtr Eval() const override;

private:
  AbstractExpressionPtr lhs_;
  AbstractExpressionPtr rhs_;

  BinaryEvaluator evaluator_;
};