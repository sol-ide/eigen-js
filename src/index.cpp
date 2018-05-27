#include <nan.h>
#include "Matrix.hpp"
#include "RowVector.hpp"
#include "ColumnVector.hpp"
#include "ExpressionTree.hpp"
#include "BinaryMatrixOperators.hpp"

NAN_MODULE_INIT(InitModule)
{
  Matrix::Init(target);
  RowVector::Init(target);
  ColumnVector::Init(target);
  ExpressionTree::Init(target);

  BinaryMatrixOperators::Init(target);
}

NODE_MODULE(EigenJs, InitModule);