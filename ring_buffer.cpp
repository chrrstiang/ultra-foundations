/**
 * RingBuffer acts as a ring buffer, FIFO type of data structure that rewmoves
 * old data for new data in order to keep memory usage low and recyclability
 * high.
 */
template <typename T> class RingBuffer {

public:
  std::unique_ptr<T[]> data;
  int head;
  int tail;
  int capacity;
  int count;

  // constructor
  RingBuffer(int head, int capacity)
      : data(new T[capacity]), head(0), tail(0), capacity(capacity), count(0) {}

  // deconstructor (cleans up resources owned by object on deletion)
  ~RingBuffer() {}

  /**
   * Pushes an element of T type into the data array of the RingBuffer.
   * Updates head of buffer accordingly, as well as the tail if necessary.
   */
  void push(T element) {
    // update tail & count
    if (this->is_full()) {
      if (this->tail == this->capacity - 1) {
        this->tail = 0;
      } else {
        this->tail++;
      }
    } else {
      this->count++;
    }

    this->data[head] = element;

    // updating head
    if (this->head == this->capacity - 1) {
      this->head = 0;
      return;
    } else {
      this->head++;
    }
  }

  /**
   * Pops element at the tail from the buffer. Updates tail and count
   * accordingly.
   */
  std::optional<T> pop() {
    if (this->is_empty()) {
      return std::nullopt;
    }

    T element = this->data[tail];

    if (this->tail == this->capacity - 1) {
      this->tail = 0;
    } else {
      this->tail++;
    }

    this->count--;
    return element;
  }

  // read element at index without popping from buffer
  T peek(int index) { return this->data[index]; }

  // is buffer full or not
  bool is_full() { return this->count == this->capacity; }

  // check if buffer is completely empty
  bool is_empty() { return this->count == 0; }

  // gets average of the elements in buffer
  int moving_average() {}
};