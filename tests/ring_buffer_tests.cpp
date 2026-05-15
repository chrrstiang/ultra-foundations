#include "../ring_buffer.cpp"
#include <gtest/gtest.h>

/** Test Cases:
 *
 *
 * push():
 * - empty arr
 * - full arr
 * - float into int arr
 * - positive number
 * - negative number
 * - push when head wraps around
 * - push when tail wraps around
 *
 * pop():
 * - empty arr returns correct nulltpr
 * - full arr returns correct element
 * - pop on tail wrap-around returns correct element
 *
 * peek():
 * - peek every element of full arr
 * - peek into empty arr
 * - peek at out-of-bounds (negative and over capacity)
 *
 * moving_average():
 * - empty array
 * - full array
 * - all positive numbers
 * - all negative
 * - positive & negative
 * - arr of size 1
 * - arr of size 10
 * - when elements wrap around
 */

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