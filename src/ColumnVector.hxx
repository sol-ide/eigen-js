template< typename Generator >
ColumnVector::ColumnVector(std::size_t length, Generator generator)
  : Matrix(length, 1, generator)
{

}
