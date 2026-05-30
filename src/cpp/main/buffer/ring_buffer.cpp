#include <cstddef>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>

/**
 * RingBuffer acts as a ring buffer, FIFO type of data structure that removes
 * old data for new data in order to keep memory usage low and recyclability
 * high.
 */
template <typename T> class RingBuffer {

public:
  // constructor
  RingBuffer(int capacity)
      : data(new T[capacity]), head(0), tail(0), capacity(capacity), count(0) {
    static_assert(std::is_arithmetic_v<T>,
                  "Ring Buffer only supports numerical types.");

    if (capacity <= 0) {
      throw std::invalid_argument("Capacity must be a postive number.");
    }
  }

  // deconstructor (cleans up resources owned by object on deletion)
  ~RingBuffer() {}

  /**
   * Pushes an element of T type into the data array of the RingBuffer.
   * Updates head of buffer accordingly, as well as the tail if necessary.
   */
  void push(T element) {
    // update tail & count
    if (is_full()) {
      if (wrapAround(tail)) {
        tail = 0;
      } else {
        tail++;
      }
    } else {
      count++;
    }

    data[head] = element;

    // updating head
    if (wrapAround(head)) {
      head = 0;
      return;
    } else {
      head++;
    }
  }

  /**
   * Pops element at the tail from the buffer. Updates tail and count
   * accordingly.
   */
  std::optional<T> pop() {
    if (is_empty()) {
      return std::nullopt;
    }

    T element = data[tail];

    if (wrapAround(tail)) {
      tail = 0;
    } else {
      tail++;
    }

    count--;
    return element;
  }

  // read element at index without popping from buffer
  std::optional<T> peek(int index) {
    if (index >= 0 && index < capacity) {
      return data[index];
    } else {
      return std::nullopt;
    }
  }

  // is buffer full or not
  bool is_full() { return count == capacity; }

  // check if buffer is completely empty
  bool is_empty() { return count == 0; }

  // gets average of the elements in buffer
  T moving_average() {
    if (count == 0) {
      return 0;
    }

    T average = 0;
    int el_count = 0;
    int i = tail;

    while (el_count < count) {
      average += data[i];
      el_count++;
      if (wrapAround(i)) {
        i = 0;
      } else {
        i++;
      }
    }

    return average / count;
  }

private:
  std::unique_ptr<T[]> data;
  int head;
  int tail;
  size_t capacity;
  int count;
  // determines whether the index is about to wrap around to the beginning of
  // the buffer
  bool wrapAround(int index) { return (index + 1) % capacity == 0; }
};