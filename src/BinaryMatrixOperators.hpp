#pragma once

#include <nan.h>

namespace BinaryMatrixOperators
{
  NAN_MODULE_INIT(Init);
}

NAN_METHOD(Add);
NAN_METHOD(Multiply);
