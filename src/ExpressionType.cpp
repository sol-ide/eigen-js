#pragma once

#include "ExpressionType.hpp"
#include "Matrix.hpp"
#include "ExpressionTree.hpp"

ExpressionType::ExpressionType(std::size_t rows, std::size_t columns, InnerType type)
  : rows_(rows)
  , columns_(columns)
  , type_(type)
{
}

ExpressionType::ExpressionType()
  : rows_(0)
  , columns_(0)
  , type_(InnerType::UNDEFINED)
{
}

std::size_t ExpressionType::GetRows() const
{
  return rows_;
}

std::size_t ExpressionType::GetColumns() const
{
  return columns_;
}

ExpressionType::InnerType ExpressionType::GetType() const
{
  return type_;
}

void ExpressionType::From(double scalar)
{
  rows_ = 1;
  columns_ = 1;
  type_ = InnerType::SCALAR_DOUBLE;
}

void ExpressionType::From(Matrix* m)
{
  rows_ = m->GetRows();
  columns_ = m->GetColumns();
  type_ = InnerType::MATRIX_DOUBLE;
}

void ExpressionType::From(ExpressionTree* tree)
{
  AbstractExpressionPtr expr = tree->GetExpr();
  const ExpressionType& type = expr->GetType();

  rows_ = type.rows_;
  columns_ = type.columns_;
  type_ = type.type_;
}
