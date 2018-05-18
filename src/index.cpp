#include <nan.h>
#include "Matrix.hpp"
#include "ExpressionTree.hpp"
#include "BinaryMatrixOperators.hpp"

NAN_MODULE_INIT(InitModule)
{
  Matrix::Init(target);
  ExpressionTree::Init(target);

  BinaryMatrixOperators::Init(target);
}

NODE_MODULE(EigenJs, InitModule);