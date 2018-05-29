template< typename Generator >
Matrix::Matrix(std::size_t rows, std::size_t columns, Generator generator)
{
  matrix_ = std::make_shared<EigenMatrix>(rows, columns);

  generator(rows, columns, matrix_->data());
}