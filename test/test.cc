#include <limits.h>

#include <cmath>
#include <iostream>
#include <tuple>
#include <typeinfo>
#include <vector>

#include "../src/set.h"
#include "gtest/gtest.h"

// funtori interi
/**
 * @brief funtore numeri uguali

 * @return true sse i due numeri sono uguali
 */
struct int_equal {
  bool operator()(int a, int b) { return (a == b); }
};

/**
 * @brief funtore numeri pari
 *
 * @return true sse il numero è pari
 */
struct int_even {
  bool operator()(int a) { return (a % 2 == 0); }
};

// funtori stringhe
/**
 * @brief funtore sringhe uguali
 *
 * @return true sse le due stringhe sono uguali
 */
struct string_equal {
  bool operator()(const std::string& a, const std::string& b) {
    return (a == b);
  }
};

/**
 * @brief funtore stringhe di lunghezza pari
 *
 * @return true sse la lunghezza della stringa è un numero pari
 */
struct string_evensize {
  bool operator()(const std::string& a) { return (a.size() % 2 == 0); }
};

/**
 * @brief struct che rappresenta un punto 3D
 *
 */
struct Point3D {
  int x, y, z;
};

/**
 * @brief funtore punti uguali
 *
 * @return true se due punti sono uguali (hanno la stessa coordianta x y z)
 */
struct point_equal {
  bool operator()(Point3D a, Point3D b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z);
  }
};

/**
 * @brief funtore punto vicino al centro
 *
 * @return true se il punto ha una distanza <= 5 dal centro
 */
struct point_close_to_center {
  bool operator()(Point3D a) {
    return (sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2)) <= 5);
  }
};

/**
 * @brief operatore per la stampa di un punto 3D
 */
std::ostream& operator<<(std::ostream& os, const Point3D& p) {
  std::cout << "(x: " << p.x << ", y: " << p.y << ", z: " << p.z << ")";
  return os;
}

using testing::Types;

template <typename T, typename Eql>
struct TypeDefinitions {
  typedef T My_type;
  typedef Eql My_type_eql;
};

typedef ::testing::Types<TypeDefinitions<int, int_equal>,
                         TypeDefinitions<std::string, string_equal>,
                         TypeDefinitions<Point3D, point_equal>>
    Implementations;

template <class C>
class SetTest : public testing::Test {
 protected:
  SetTest() : set(new Set<typename C::My_type, typename C::My_type_eql>()) {}

  ~SetTest() override { delete set; }

  Set<typename C::My_type, typename C::My_type_eql>* const set;
};

TYPED_TEST_SUITE(SetTest, Implementations);

// empty set
TYPED_TEST(SetTest, SetDefaultConstructor) {
  // typename TypeParam::MyA someA;
  EXPECT_EQ(0, this->set->size());
  EXPECT_TRUE(this->set->is_empty());
}

class SetTestInt : public SetTest<TypeDefinitions<int, int_equal>>,
                   public testing::WithParamInterface<int> {
 protected:
};

class SetTestString
    : public SetTest<TypeDefinitions<std::string, string_equal>>,
      public testing::WithParamInterface<std::string> {};

class SetTestPoint3D : public SetTest<TypeDefinitions<Point3D, point_equal>>,
                       public testing::WithParamInterface<Point3D> {};

TEST_P(SetTestInt, SetAddInt) {
  EXPECT_TRUE(this->set->add(GetParam()));
  EXPECT_EQ(1, this->set->size());
  EXPECT_FALSE(this->set->add(GetParam()));
  EXPECT_EQ(1, this->set->size());
}

TEST_P(SetTestString, SetAddString) {
  EXPECT_TRUE(this->set->add(GetParam()));
  EXPECT_EQ(1, this->set->size());
  EXPECT_FALSE(this->set->add(GetParam()));
  EXPECT_EQ(1, this->set->size());
}

TEST_P(SetTestPoint3D, SetAddPoint3D) {
  EXPECT_TRUE(this->set->add(GetParam()));
  EXPECT_EQ(1, this->set->size());
  EXPECT_FALSE(this->set->add(GetParam()));
  EXPECT_EQ(1, this->set->size());
}

// test remove
TEST_P(SetTestInt, SetRemoveInt) {
  EXPECT_TRUE(this->set->add(GetParam()));
  this->set->remove(GetParam());
  EXPECT_TRUE(this->set->is_empty());
}

int elements_test_int[] = {0, 1, INT_MAX, INT_MIN, -1};

std::string elements_test_string[] = {"alberto", ""};

Point3D elements_test_points[] = {{0, 0, 0}, {-1, -2, -3}, {-2, 2, -2}};

INSTANTIATE_TEST_SUITE_P(A, SetTestInt, testing::ValuesIn(elements_test_int));

INSTANTIATE_TEST_SUITE_P(B, SetTestString,
                         testing::ValuesIn(elements_test_string));

INSTANTIATE_TEST_SUITE_P(C, SetTestPoint3D,
                         testing::ValuesIn(elements_test_points));

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}