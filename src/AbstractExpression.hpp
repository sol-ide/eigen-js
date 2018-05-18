#pragma once

#include "Matrix.hpp"
#include <memory>

class AbstractExpression
{
public:
  virtual ~AbstractExpression();

  virtual Matrix::EigenMatrixPtr Eval() const = 0;

};

using AbstractExpressionPtr = std::shared_ptr<AbstractExpression>;