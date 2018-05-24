#pragma once

#include "Matrix.hpp"
#include "AbstractExpression.hpp"
#include <variant>

class BinaryMatrixExpression : public AbstractExpression
{
public:
  using BinaryEvaluator = std::function<ExpressionVariant(const ExpressionVariant& lhs, const ExpressionVariant& rhs)>;

public:
  BinaryMatrixExpression(AbstractExpressionPtr lhs, AbstractExpressionPtr rhs, ExpressionType type, BinaryEvaluator evaluator);

  virtual ExpressionVariant Eval() const override;

private:
  AbstractExpressionPtr lhs_;
  AbstractExpressionPtr rhs_;

  BinaryEvaluator evaluator_;
};