#include "../main/buffer/ring_buffer.h"
#include <gtest/gtest.h>

/** Test Cases:
 *
 * Constructor__Test:
 * - ZeroCapacityThrows [failure]: capacity 0 throws invalid_argument
 * - NegativeCapacityThrows [failure]: negative capacity throws invalid_argument
 *
 * push():
 * - EmptyBuffer [success]: push to empty buffer stores value correctly
 * - FullBuffer [success]: push to full buffer overwrites oldest element
 * - FloatIntoIntBuffer [edge]: float truncated to int on push
 * - PositiveInt [success]: positive int pushed and peeked correctly
 * - NegativeInt [success]: negative int pushed and peeked correctly
 * - HeadWrapAround [success]: head wraps around when buffer is full and overflows
 * - TailWrapAround [success]: tail wraps around after pop empties and refills buffer
 *
 * pop():
 * - EmptyBufferReturnsNullopt [edge]: pop on empty buffer returns nullopt
 * - FullBufferReturnsCorrectElement [success]: pop returns oldest element
 * - TailWrapAroundReturnsCorrectElement [success]: pop at wrap-around index correct
 *
 * peek():
 * - FullBufferAllElements [success]: peek at each index of full buffer
 * - EmptyBuffer [edge]: peek at index 0 of empty buffer returns a value
 * - OutOfBounds [failure]: peek at negative or out-of-capacity index returns nullopt
 *
 * moving_average():
 * - EmptyBuffer [edge]: average of empty buffer is 0
 * - FullBuffer [success]: average of full buffer is correct
 * - AllPositive [success]: average of all positive elements
 * - AllNegative [success]: average of all negative elements
 * - PositiveAndNegative [success]: average of mixed positive/negative elements
 * - SingleElement [edge]: average of single-element buffer is that element
 * - TenElements [success]: average over 10 elements (integer division)
 * - ElementsWrapAround [success]: average computed correctly after overwrite wrap
 * - PartiallyFilled [edge]: average over only filled portion of buffer
 */

// --- Constructor__Test ---

TEST(Constructor__Test, ZeroCapacityThrows) {
  EXPECT_THROW(RingBuffer<int>(0), std::invalid_argument);
}

TEST(Constructor__Test, NegativeCapacityThrows) {
  EXPECT_THROW(RingBuffer<int>(-3), std::invalid_argument);
}

// --- Push__Test ---

TEST(Push__Test, EmptyBuffer) {
  RingBuffer<int> buffer(5);

  buffer.push(1);

  EXPECT_FALSE(buffer.is_empty());
  EXPECT_FALSE(buffer.is_full());
  EXPECT_EQ(buffer.pop().value(), 1);
}

TEST(Push__Test, FullBuffer) {
  RingBuffer<int> buffer(5);

  buffer.push(1);
  buffer.push(2);
  buffer.push(3);
  buffer.push(4);
  buffer.push(5);

  EXPECT_TRUE(buffer.is_full());

  buffer.push(6);

  EXPECT_EQ(buffer.peek(0), 6);
  EXPECT_TRUE(buffer.is_full());
  EXPECT_FALSE(buffer.is_empty());
}

TEST(Push__Test, PositiveInt) {
  RingBuffer<int> buffer(2);

  buffer.push(5);

  EXPECT_EQ(buffer.peek(0), 5);
}

TEST(Push__Test, NegativeInt) {
  RingBuffer<int> buffer(2);

  buffer.push(-5);

  EXPECT_EQ(buffer.peek(0), -5);
}

TEST(Push__Test, HeadWrapAround) {
  RingBuffer<int> buffer(3);

  buffer.push(1);
  buffer.push(2);
  buffer.push(3);

  EXPECT_EQ(buffer.peek(2), 3);

  buffer.push(10);

  EXPECT_EQ(buffer.peek(0), 10);
  EXPECT_TRUE(buffer.is_full());
  EXPECT_FALSE(buffer.is_empty());
}

TEST(Push__Test, TailWrapAround) {
  RingBuffer<int> buffer(3);

  buffer.push(1);
  buffer.push(2);
  buffer.push(3);

  EXPECT_EQ(buffer.peek(2), 3);
  EXPECT_TRUE(buffer.is_full());

  buffer.pop();
  buffer.pop();
  buffer.pop();

  EXPECT_TRUE(buffer.is_empty());

  buffer.push(1);

  EXPECT_EQ(buffer.peek(0), 1);
}

TEST(Push__Test, FloatIntoIntBuffer) {
  RingBuffer<int> buffer(2);

  buffer.push(3.7f);

  EXPECT_EQ(buffer.peek(0), 3);
}

// --- pop() ---

TEST(Pop__Test, EmptyBufferReturnsNullopt) {
  RingBuffer<int> buffer(5);

  EXPECT_FALSE(buffer.pop().has_value());
}

TEST(Pop__Test, FullBufferReturnsCorrectElement) {
  RingBuffer<int> buffer(3);

  buffer.push(1);
  buffer.push(2);
  buffer.push(3);

  EXPECT_TRUE(buffer.is_full());
  EXPECT_EQ(buffer.pop().value(), 1);
  EXPECT_FALSE(buffer.is_full());
}

TEST(Pop__Test, TailWrapAroundReturnsCorrectElement) {
  RingBuffer<int> buffer(3);

  buffer.push(1);
  buffer.push(2);
  buffer.push(3);

  buffer.pop(); // tail: 0 -> 1
  buffer.pop(); // tail: 1 -> 2

  // third pop triggers wrapAround at index 2
  EXPECT_EQ(buffer.pop().value(), 3);
  EXPECT_TRUE(buffer.is_empty());
}

// --- peek() ---

TEST(Peek__Test, FullBufferAllElements) {
  RingBuffer<int> buffer(3);

  buffer.push(10);
  buffer.push(20);
  buffer.push(30);

  EXPECT_EQ(buffer.peek(0).value(), 10);
  EXPECT_EQ(buffer.peek(1).value(), 20);
  EXPECT_EQ(buffer.peek(2).value(), 30);
}

TEST(Peek__Test, EmptyBuffer) {
  RingBuffer<int> buffer(3);

  // Valid index on empty buffer is in-bounds, returns a value (not nullopt)
  EXPECT_TRUE(buffer.peek(0).has_value());
}

TEST(Peek__Test, OutOfBounds) {
  RingBuffer<int> buffer(3);

  EXPECT_FALSE(buffer.peek(-1).has_value());
  EXPECT_FALSE(buffer.peek(3).has_value());
  EXPECT_FALSE(buffer.peek(100).has_value());
}

// --- moving_average() ---

TEST(MovingAverage__Test, EmptyBuffer) {
  RingBuffer<int> buffer(5);

  EXPECT_EQ(buffer.moving_average(), 0);
}

TEST(MovingAverage__Test, FullBuffer) {
  RingBuffer<int> buffer(5);

  buffer.push(1);
  buffer.push(2);
  buffer.push(3);
  buffer.push(4);
  buffer.push(5);

  EXPECT_EQ(buffer.moving_average(), 3); // 15 / 5
}

TEST(MovingAverage__Test, AllPositive) {
  RingBuffer<int> buffer(4);

  buffer.push(2);
  buffer.push(4);
  buffer.push(6);
  buffer.push(8);

  EXPECT_EQ(buffer.moving_average(), 5); // 20 / 4
}

TEST(MovingAverage__Test, AllNegative) {
  RingBuffer<int> buffer(3);

  buffer.push(-3);
  buffer.push(-6);
  buffer.push(-9);

  EXPECT_EQ(buffer.moving_average(), -6); // -18 / 3
}

TEST(MovingAverage__Test, PositiveAndNegative) {
  RingBuffer<int> buffer(4);

  buffer.push(-5);
  buffer.push(5);
  buffer.push(-10);
  buffer.push(10);

  EXPECT_EQ(buffer.moving_average(), 0); // 0 / 4
}

TEST(MovingAverage__Test, SingleElement) {
  RingBuffer<int> buffer(1);

  buffer.push(42);

  EXPECT_EQ(buffer.moving_average(), 42);
}

TEST(MovingAverage__Test, TenElements) {
  RingBuffer<int> buffer(10);

  for (int i = 1; i <= 10; i++) {
    buffer.push(i);
  }

  EXPECT_EQ(buffer.moving_average(), 5); // 55 / 10 (integer division)
}

TEST(MovingAverage__Test, ElementsWrapAround) {
  RingBuffer<int> buffer(3);

  buffer.push(1);
  buffer.push(2);
  buffer.push(3);
  buffer.push(4); // overwrites 1; logical contents: [2, 3, 4]

  EXPECT_EQ(buffer.moving_average(), 3); // 9 / 3
}

TEST(MovingAverage__Test, PartiallyFilled) {
  // buffer capacity 5, only 3 elements pushed — average over 3, not 5
  RingBuffer<int> buffer(5);

  buffer.push(10);
  buffer.push(20);
  buffer.push(30);

  EXPECT_EQ(buffer.moving_average(), 20); // 60 / 3
}