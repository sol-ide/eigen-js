#pragma once

#include <cstddef>

class Matrix;
class ExpressionTree;

class ExpressionType
{
public:
  enum class InnerType
  {
    UNDEFINED,
    SCALAR_BOOLEAN,
    SCALAR_DOUBLE,
    VECTOR_DOUBLE,
    MATRIX_DOUBLE
  };

public:
  ExpressionType(std::size_t rows, std::size_t columns, InnerType type);

  ExpressionType();

  void From(Matrix* m);

  void From(ExpressionTree* tree);

  std::size_t GetRows() const;
  std::size_t GetColumns() const;
  InnerType GetType() const;

private:
  std::size_t rows_;
  std::size_t columns_;
  InnerType type_;
};