#ifndef CIRCULARVECTOR_H
#define CIRCULARVECTOR_H

#include <cstdlib>
#include <vector>

// this is one part of LOAM algorithm
// this code is to design a data structure that stores dynamic datas

/** \brief Simple circular buffer implementation for storing data history.
 *
 * @tparam T The buffer element type.
 */
template <class T>
class CircularVector {

public:
  //constructor
  CircularVector(const size_t& capacity = 200)
                         : _capacity(capacity),
                                      _size(0),
                                  _startIdx(0){
    _buffer = new T[capacity];

  };
  //destructor
  ~CircularVector()
  {
    delete[] _buffer;
    _buffer = NULL;
  }

  /** \brief Retrieve the buffer size.
   *
   * @return the buffer size
   */
  const size_t& size() {
    return _size;
  }

  /** \brief Retrieve the buffer capacity.
   *
   * @return the buffer capacity
   */
  const size_t& capacity() {
    return _capacity;
  }

  /** \brief Ensure that this buffer has at least the required capacity.
   *
   * @param reqCapacity the minimum required capacity
   */
  void ensureCapacity(const int& reqCapacity) {
    if (reqCapacity > 0 && _capacity < reqCapacity) {
      // create new buffer and copy (valid) entries
      T* newBuffer = new T[reqCapacity];
      for (size_t i = 0; i < _size; i++) {
        newBuffer[i] = (*this)[i];
      }

      // switch buffer pointers and delete old buffer
      T* oldBuffer = _buffer;
      _buffer = newBuffer;
      _startIdx = 0;

      delete[] oldBuffer;
    }
  }

  /** \brief Check if the buffer is empty.
   *
   * @return true if the buffer is empty, false otherwise
   */
  bool empty() {
    return _size == 0;
  }

  /** \brief Retrieve the i-th element of the buffer.
   *
   *
   * @param i the buffer index
   * @return the element at the i-th position
   */
  const T& operator[](const size_t& i) {
    return _buffer[(_startIdx + i) % _capacity];
  }

  /** \brief Retrieve the first (oldest) element of the buffer.
   *
   * @return the first element
   */
  const T& first() {
    return _buffer[_startIdx];
  }

  /** \brief show the current index of query element.
  * this function is for test
  * @return the current index
  */
  int curIdx() {
	  return _startIdx;
  }

  /** \brief Retrieve the last (latest) element of the buffer.
   *
   * @return the last element
   */
  const T& last() {
    size_t idx = _size == 0 ? 0 : (_startIdx + _size - 1) % _capacity;
    return _buffer[idx];
  }

  /** \brief Push a new element to the buffer.
   *
   * If the buffer reached its capacity, the oldest element is overwritten.
   *
   * @param element the element to push
   */
  void push(const T& element) {
    if (_size < _capacity) {
      _buffer[_size] = element;
      _size++;
    } else {
      _buffer[_startIdx] = element;
      _startIdx = (_startIdx + 1) % _capacity;
    }
  }

private:
  size_t _capacity;   ///< buffer capacity
  size_t _size;       ///< current buffer size
  size_t _startIdx;   ///< current start index
  T* _buffer;         ///< internal element buffer
};


#endif //CIRCULARVECTOR_H
