
namespace Generation
{
  template< typename T >
  Fill<T>::Fill(T value)
    : value_( value )
  {

  }

  template< typename T >
  void Fill<T>::operator()(std::size_t rows, std::size_t columns, T* data)
  {
    std::fill(
      data,
      data + (rows * columns),
      value_
    );
  }

  template< typename T >
  Diagonal<T>::Diagonal(T value)
    : value_( value )
  {

  }

  template< typename T >
  void Diagonal<T>::operator()(std::size_t rows, std::size_t columns, T* data)
  {
    std::size_t index = 0;
    for( std::size_t i = 0; i < rows - 1; ++i )
    {
      data[index++] = value_;
      std::fill(
        data + index,
        data + std::min( index + columns, columns*rows ),
        0
      );
    }
  }
}