#include <cmath>
#include <climits>
#include <iostream>
#include <tuple>
#include <typeinfo>
#include <vector>
#include <array>

#include "../src/set.h"
#include "gtest/gtest.h"

/**
 * @brief struct that represents a 3D Point
 *
 */
struct Point3D {
  int x, y, z;
};

/**
 * @brief Functor for equal numbers

 * @return true iif the numbers are equal
 */
struct int_equal {
  bool operator()(int a, int b) {
    return (a == b);
  }
};

/**
 * @brief Functor for even numbers
 *
 * @return true iif the number is even
 */
struct int_even {
  bool operator()(int a) {
    return (a % 2 == 0);
  }
};

/**
 * @brief Functor for equal strings
 *
 * @return true true iif the strings are equal
 */
struct string_equal {
  bool operator()(const std::string& a, const std::string& b) {
    return (a == b);
  }
};

/**
 * @brief Functor for strings of even length
 *
 * @return true iif the length of the string is even
 */
struct string_evensize {
  bool operator()(const std::string& a) {
    return (a.size() % 2 == 0);
  }
};

/**
 * @brief Functor for equal Points
 *
 * @return true iif the points are equal (have the same x y z)
 */
struct point_equal {
  bool operator()(Point3D a, Point3D b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z);
  }
};

/**
 * @brief Functor for Point close to the center
 *
 * @return true iif the distance of the point from the center is less than or
 * equal to 5
 */
struct point_close_to_center {
  bool operator()(Point3D a) {
    return (sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2)) <= 5);
  }
};

/**
 * @brief Operator for pretty print a 3D Point
 */
std::ostream& operator<<(std::ostream& os, const Point3D& p) {
  std::cout << "(x: " << p.x << ", y: " << p.y << ", z: " << p.z << ")";
  return os;
}

using testing::Types;

template <typename T, typename Eql, typename Pred>
struct TypeDefinitions {
  typedef T My_type;
  typedef Eql My_type_eql;
  typedef Pred My_type_comp;
};

typedef ::testing::Types<
    TypeDefinitions<int, int_equal, int_even>,
    TypeDefinitions<std::string, string_equal, string_evensize>,
    TypeDefinitions<Point3D, point_equal, point_close_to_center>>
    Implementations;

template <class C>
class SetTest : public testing::Test {
 protected:
  Set<typename C::My_type, typename C::My_type_eql> set;
};

constexpr int DATASET_SIZE = 5;
std::array<int, DATASET_SIZE> elements_test_int = {0, 2, INT_MAX, INT_MIN, -1};
std::array<std::string, DATASET_SIZE> elements_test_string = {
    "Alberto", "Andrea", "Giovanni", "Riccardo", "Lorenzo"};
std::array<Point3D, DATASET_SIZE> elements_test_points = {
    {{0, 0, 0}, {-1, -2, -3}, {-2, 2, -2}, {6, 0, -8}, {6, 0, 1}}};

template <typename T>
T getvalue(const int index) {
  throw std::runtime_error(
      "Unsupported type! There isn't any sample data to use in the test for "
      "this type.");
}

template <>
int getvalue<int>(const int index) {
  assert(index < DATASET_SIZE && index >= 0);
  return elements_test_int[index];
}

template <>
std::string getvalue<std::string>(const int index) {
  assert(index < DATASET_SIZE && index >= 0);
  return elements_test_string[index];
}

template <>
Point3D getvalue<Point3D>(const int index) {
  assert(index < DATASET_SIZE && index >= 0);
  return elements_test_points[index];
}

TYPED_TEST_SUITE(SetTest, Implementations);

TYPED_TEST(SetTest, Constructor) {
  EXPECT_TRUE(this->set.is_empty());
}

TYPED_TEST(SetTest, CopyConstructor) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));

  Set<typename TypeParam::My_type, typename TypeParam::My_type_eql> copied_set(
      this->set);

  EXPECT_EQ(this->set.size(), copied_set.size());
}

TYPED_TEST(SetTest, CopyAssignemnt) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));
  Set<typename TypeParam::My_type, typename TypeParam::My_type_eql> other =
      this->set;

  EXPECT_EQ(this->set.size(), other.size());
}

TYPED_TEST(SetTest, AddElement) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));
  EXPECT_EQ(this->set.size(), 2);
  this->set.add(getvalue<typename TypeParam::My_type>(1));
  EXPECT_EQ(this->set.size(), 2);
}

TYPED_TEST(SetTest, RemoveElement) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));
  EXPECT_EQ(this->set.size(), 2);
  this->set.remove(getvalue<typename TypeParam::My_type>(1));
  EXPECT_EQ(this->set.size(), 1);
  this->set.add(getvalue<typename TypeParam::My_type>(2));
  this->set.remove(getvalue<typename TypeParam::My_type>(0));
  EXPECT_EQ(this->set.size(), 1);
}

TYPED_TEST(SetTest, IsEmpty) {
  EXPECT_TRUE(this->set.is_empty());
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));
  this->set.add(getvalue<typename TypeParam::My_type>(2));
  this->set.add(getvalue<typename TypeParam::My_type>(3));
  EXPECT_FALSE(this->set.is_empty());
}

TYPED_TEST(SetTest, Clear) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));
  this->set.add(getvalue<typename TypeParam::My_type>(2));
  this->set.add(getvalue<typename TypeParam::My_type>(3));
  this->set.clear();
  EXPECT_EQ(this->set.size(), 0);
}

TYPED_TEST(SetTest, OperatorEquals) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));
  this->set.add(getvalue<typename TypeParam::My_type>(2));
  this->set.add(getvalue<typename TypeParam::My_type>(3));

  Set<typename TypeParam::My_type, typename TypeParam::My_type_eql> other;
  other.add(getvalue<typename TypeParam::My_type>(2));
  other.add(getvalue<typename TypeParam::My_type>(0));
  other.add(getvalue<typename TypeParam::My_type>(3));
  other.add(getvalue<typename TypeParam::My_type>(1));

  EXPECT_TRUE(this->set == other);
  other.remove(getvalue<typename TypeParam::My_type>(3));
  EXPECT_FALSE(this->set == other);
  other.add(getvalue<typename TypeParam::My_type>(4));
  EXPECT_FALSE(this->set == other);
}

TYPED_TEST(SetTest, Union) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));

  Set<typename TypeParam::My_type, typename TypeParam::My_type_eql> other;
  other.add(getvalue<typename TypeParam::My_type>(1));
  other.add(getvalue<typename TypeParam::My_type>(2));
  other.add(getvalue<typename TypeParam::My_type>(3));

  Set<typename TypeParam::My_type, typename TypeParam::My_type_eql> result =
      this->set + other;

  EXPECT_EQ(result.size(), 4);
}

TYPED_TEST(SetTest, Intersection) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));

  Set<typename TypeParam::My_type, typename TypeParam::My_type_eql> other;
  other.add(getvalue<typename TypeParam::My_type>(2));
  other.add(getvalue<typename TypeParam::My_type>(3));

  Set<typename TypeParam::My_type, typename TypeParam::My_type_eql> result =
      this->set - other;

  EXPECT_TRUE(result.is_empty());

  other.add(getvalue<typename TypeParam::My_type>(1));
  result = this->set - other;
  EXPECT_EQ(result.size(), 1);
}

TYPED_TEST(SetTest, FilterOut) {
  this->set.add(getvalue<typename TypeParam::My_type>(0));
  this->set.add(getvalue<typename TypeParam::My_type>(1));
  this->set.add(getvalue<typename TypeParam::My_type>(2));
  this->set.add(getvalue<typename TypeParam::My_type>(3));
  this->set.add(getvalue<typename TypeParam::My_type>(4));

  Set<typename TypeParam::My_type, typename TypeParam::My_type_eql> result =
      filter_out(this->set, typename TypeParam::My_type_comp{});

  EXPECT_EQ(result.size(), 3);
}
