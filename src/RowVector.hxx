template< typename Generator >
RowVector::RowVector(std::size_t length, Generator generator)
  : Matrix(1, length, generator)
{

}
