#include "AbstractExpression.hpp"

AbstractExpression::AbstractExpression(ExpressionType type)
  : type_( type )
{

}

/* virtual */ AbstractExpression::~AbstractExpression()
{
}

const ExpressionType& AbstractExpression::GetType() const
{
  return type_;
}