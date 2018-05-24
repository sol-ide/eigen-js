#pragma once

#include "Matrix.hpp"
#include "ExpressionType.hpp"
#include <memory>
#include <variant>

class AbstractExpression
{
public:
  using ExpressionVariant = std::variant<bool, double, Matrix::EigenMatrixPtr>;

  AbstractExpression(ExpressionType type);

  virtual ~AbstractExpression();

  virtual ExpressionVariant Eval() const = 0;

  const ExpressionType& GetType() const;

private:
  ExpressionType type_;
};

using AbstractExpressionPtr = std::shared_ptr<AbstractExpression>;