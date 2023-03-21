#include <cmath>
#include <iostream>
#include <string>

#include "set.h"

// funtori interi
/**
 * @brief funtore numeri uguali

 * @return true se i due numeri sono uguali
 */
struct int_equal {
  bool operator()(int a, int b) { return (a == b); }
};

/**
 * @brief funtore numeri pari
 *
 * @return true se il numero è pari
 */
struct int_even {
  bool operator()(int a) { return (a % 2 == 0); }
};

// funtori stringhe
/**
 * @brief funtore sringhe uguali
 *
 * @return true se le due stringhe sono uguali
 */
struct string_equal {
  bool operator()(const std::string& a, const std::string& b) {
    return (a == b);
  }
};

/**
 * @brief funtore stringhe di lunghezza pari
 *
 * @return true se la lunghezza della stringa è  un numero pari
 */
struct string_evensize {
  bool operator()(const std::string& a) { return (a.size() % 2 == 0); }
};

// funtori punto3D custom
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

/**
 * @brief vengono testati i metodi finamentali della classe
 * - costruttore
 * - distruttore
 * - copy constructor
 * - copy assignment
 * - add
 * - remove
 *
 */
void test_metodi_fondamentali_int() {
  Set<int, int_equal> set_of_integers;  // default constructor

  set_of_integers.add(10);
  set_of_integers.add(9);
  set_of_integers.add(69);
  set_of_integers.add(888);
  set_of_integers.add(777);
  set_of_integers.add(666);
  set_of_integers.add(9);      // esiste già
  set_of_integers.remove(69);  // rimosso in mezzo
  set_of_integers.remove(10);  // rimosso all'inizio
  // il set dovrebbe avere size 4
  std::cout << "size(expected 4): " << set_of_integers.size() << std::endl;

  // elementi inseriti (operator<<)
  // expected 9  888  777  666
  std::cout << "elementi set_of_integers: " << set_of_integers << std::endl;

  // copy contructor
  Set<int, int_equal> copied_set_of_integers(set_of_integers);
  std::cout << "elementi copied_set_of_integers: " << copied_set_of_integers
            << std::endl;

  // copy assginment
  Set<int, int_equal> copy_assigned_set_of_integers(set_of_integers);
  std::cout << "elementi copy_assigned_set_of_integers: "
            << copy_assigned_set_of_integers << std::endl;

}  // ~Set()

/**
 * @brief viene testata la costruzione di un set tramite un sottoset definito
 * da due iteratori
 *
 */
void test_costruttore_iter() {
  // test con interi
  Set<int, int_equal> set_of_integers;  // default constructor

  set_of_integers.add(68);
  set_of_integers.add(678);
  set_of_integers.add(59);
  set_of_integers.add(5);
  set_of_integers.add(6548);
  set_of_integers.add(549);

  Set<int, int_equal>::const_iterator a, b;
  a = ++set_of_integers.begin();
  b = ++set_of_integers.begin();
  b = ++b;
  b = ++b;
  b = ++b;

  // expected result 678 59 5
  Set<int, int_equal> iter_set_of_integers(a, b);
  std::cout << "elementi iter_set_of_integers: " << iter_set_of_integers
            << std::endl;

  int elementi_test[7] = {8, 66, 33, 66, 999, 444, 8};

  // exptected result: 66 33 999
  // nota: non è stato aggiunto due volte 66 come expected da un set
  Set<int, int_equal> iter_set_of_integers_2(elementi_test + 1,
                                             elementi_test + 5);
  std::cout << "elementi iter_set_of_integers_2: " << iter_set_of_integers_2
            << std::endl;

  // test con tipo custom
}

/**
 * @brief viene testato l'accesso ai dati con l'operatore [] con interi,
 * stringhe e dato custom
 *
 */
void test_accesso_indice() {
  int elementi_test[5] = {8, 33, 66, 999, 444};
  Set<int, int_equal> set_of_integers(elementi_test, elementi_test + 5);
  for (int i = 0; i < set_of_integers.size(); ++i) {
    std::cout << "elemendo a indice " << i << " " << set_of_integers[i]
              << std::endl;
  }

  std::string elementi_test_str[5] = {"ciao", "mi", "chiamo", "giovanni",
                                      "wow"};
  Set<std::string, string_equal> set_of_strings(elementi_test_str,
                                                elementi_test_str + 5);
  for (int i = 0; i < set_of_strings.size(); ++i) {
    std::cout << "elemendo a indice " << i << " " << set_of_strings[i]
              << std::endl;
  }

  Point3D elementi_test_p3d[5] = {
      {0, 9, 0}, {1, 2, 3}, {2, 2, 2}, {3, 2, 1}, {8, 5, 9}};
  Set<Point3D, point_equal> set_of_points(elementi_test_p3d,
                                          elementi_test_p3d + 5);
  for (int i = 0; i < set_of_points.size(); ++i) {
    std::cout << "elemendo a indice " << i << " " << set_of_points[i]
              << std::endl;
  }
}

/**
 * @brief vengono testati tutti gli operatori globali (<< == + -) con un set di
 * interi
 *
 */
void test_operatori_globali_int() {
  // set di interi
  int elementi_test[5] = {8, 33, 66, 999, 444};
  int elementi_test_unordered[5] = {33, 8, 999, 66, 444};
  int elementi_test_2[5] = {123, 33, 321, 999, 852};

  Set<int, int_equal> set_of_integers(elementi_test, elementi_test + 5);
  Set<int, int_equal> set_of_integers_eql(elementi_test, elementi_test + 5);
  Set<int, int_equal> set_of_integers_2(elementi_test_2, elementi_test_2 + 5);

  // operator<<
  std::cout << "elementi set_of_integers: " << set_of_integers << std::endl;
  std::cout << "elementi set_of_integers_eql: " << set_of_integers_eql
            << std::endl;
  // operator==
  if (set_of_integers == set_of_integers_eql) {
    std::cout
        << "i due set sono uguali (set_of_integers == set_of_integers_eql)"
        << std::endl;
  }
  set_of_integers_eql.add(69);
  if (!(set_of_integers == set_of_integers_eql)) {
    std::cout
        << "i due set non sono uguali (set_of_integers != set_of_integers_eql)"
        << std::endl;
  }

  // operator + union
  Set<int, int_equal> united_int_set = set_of_integers + set_of_integers_2;
  std::cout << "elementi united_int_set: " << united_int_set << std::endl;
  // operator - intersection
  Set<int, int_equal> inters_int_set = set_of_integers - set_of_integers_2;
  std::cout << "elementi inters_int_set: " << inters_int_set << std::endl;
}

/**
 * @brief vengono testati tutti gli operatori globali (<< == + -) con un set di
 * stringhe
 *
 */
void test_operatori_globali_string() {
  // set di stringhe
  std::string elementi_test[5] = {"ciao", "mi", "chiamo", "giovanni", "wow"};
  std::string elementi_test_unordered[5] = {"wow", "ciao", "giovanni", "mi",
                                            "chiamo"};
  std::string elementi_test_2[5] = {"alberto", "davide", "giovanni", "andrea",
                                    "alessia"};

  Set<std::string, string_equal> set_of_strings(elementi_test,
                                                elementi_test + 5);
  Set<std::string, string_equal> set_of_strings_eql(elementi_test,
                                                    elementi_test + 5);
  Set<std::string, string_equal> set_of_strings_2(elementi_test_2,
                                                  elementi_test_2 + 5);

  // operator<<
  std::cout << "elementi set_of_strings: " << set_of_strings << std::endl;
  std::cout << "elementi set_of_strings_eql: " << set_of_strings_eql
            << std::endl;
  // operator==
  if (set_of_strings == set_of_strings_eql) {
    std::cout << "i due set sono uguali (set_of_strings == set_of_strings_eql)"
              << std::endl;
  }

  // operator + union
  Set<std::string, string_equal> united_string_set =
      set_of_strings + set_of_strings_2;
  std::cout << "elementi united_string_set: " << united_string_set << std::endl;
  // operator - intersection
  Set<std::string, string_equal> inters_string_set =
      set_of_strings - set_of_strings_2;
  std::cout << "elementi inters_string_set: " << inters_string_set << std::endl;
}

/**
 * @brief vengono testati tutti gli operatori globali (<< == + -) con un set di
 * dati custom Punto3D
 *
 */
void test_operatori_globali_custom() {
  Point3D elementi_test[5] = {
      {0, 0, 0}, {1, 1, 1}, {2, 2, 2}, {3, 3, 3}, {4, 4, 4}};
  Point3D elementi_test_unordered[5] = {
      {4, 4, 4}, {3, 3, 3}, {2, 2, 2}, {1, 1, 1}, {0, 0, 0}};
  Point3D elementi_test_2[5] = {
      {0, 0, 0}, {1, 2, 3}, {2, 2, 2}, {3, 2, 1}, {8, 5, 9}};

  Set<Point3D, point_equal> set_of_points(elementi_test, elementi_test + 5);
  Set<Point3D, point_equal> set_of_points_eql(elementi_test, elementi_test + 5);
  Set<Point3D, point_equal> set_of_points_2(elementi_test_2,
                                            elementi_test_2 + 5);

  // operator<<
  std::cout << "elementi set_of_points: " << set_of_points << std::endl;
  std::cout << "elementi set_of_points_eql: " << set_of_points_eql << std::endl;
  // operator==
  if (set_of_points == set_of_points_eql) {
    std::cout << "i due set sono uguali (set_of_points == set_of_points_eql)"
              << std::endl;
  }

  // operator + union
  Set<Point3D, point_equal> united_points_set = set_of_points + set_of_points_2;
  std::cout << "elementi united_string_set: " << united_points_set << std::endl;
  // operator - intersection
  Set<Point3D, point_equal> inters_points_set = set_of_points - set_of_points_2;
  std::cout << "elementi inters_string_set: " << inters_points_set << std::endl;
}

/**
 * @brief viene testato il forward iterator con un set di interi
 *
 */
void test_fw_iterator() {
  int elementi_test[5] = {8, 33, 66, 999, 444};
  Set<int, int_equal> set_of_integers(elementi_test, elementi_test + 5);

  std::cout << "stampa elementi con iteratore" << std::endl;
  Set<int, int_equal>::const_iterator b, e;
  for (b = set_of_integers.begin(), e = set_of_integers.end(); b != e; ++b) {
    std::cout << *b << std::endl;
  }
}

/**
 * @brief viene testato il metodo filter_out con un set di interi
 *
 */
void test_filter_out_int() {
  int elementi_test[5] = {8, 33, 66, 999, 444};
  Set<int, int_equal> set_of_integers(elementi_test, elementi_test + 5);
  int_even int_even_funct;
  Set<int, int_equal> res = filter_out(set_of_integers, int_even_funct);
  // expected 8  66  444
  std::cout << res << std::endl;
}

/**
 * @brief viene testato il metodo filter_out con un set di stringhe
 *
 */
void test_filter_out_string() {
  std::string elementi_test[5] = {"ciao", "mi", "chiamo", "giovanni", "wow"};
  Set<std::string, string_equal> set_of_strings(elementi_test,
                                                elementi_test + 5);
  string_evensize string_evensize_funct;
  Set<std::string, string_equal> res =
      filter_out(set_of_strings, string_evensize_funct);
  // expected ciao  mi  giovanni
  std::cout << res << std::endl;
}

/**
 * @brief viene testato il metodo filter_out con un set di dati custom Punto3D
 *
 */
void test_filter_out_custom() {
  Point3D elementi_test[5] = {
      {0, 0, 0}, {1, 2, 3}, {9, 3, 2}, {3, 2, 1}, {8, 5, 9}};

  Set<Point3D, point_equal> set_of_points(elementi_test, elementi_test + 5);
  point_close_to_center point_close_to_center_funct;
  Set<Point3D, point_equal> res =
      filter_out(set_of_points, point_close_to_center_funct);
  // expected (x: 0, y: 0, z: 0)  (x: 1, y: 2, z: 3)  (x: 3, y: 2, z: 1)
  std::cout << res << std::endl;
}

int main() {
  std::cout << "Test metodi fondamentali (con interi)" << std::endl;
  test_metodi_fondamentali_int();

  std::cout << "Test metodi con iteratore (con interi)" << std::endl;
  test_costruttore_iter();

  std::cout << "Test accesso ai dati tramimte indice (con interi, stringhe e "
               "dato custom Punto3D)"
            << std::endl;
  test_accesso_indice();

  std::cout << "Test operatori globali (con interi)" << std::endl;
  test_operatori_globali_int();
  std::cout << "Test operatori globali (con stringhe)" << std::endl;
  test_operatori_globali_string();
  std::cout << "Test operatori globali (con dato custom Punto3D)" << std::endl;
  test_operatori_globali_custom();

  std::cout << "Test metodi iteratore (con interi)" << std::endl;
  test_fw_iterator();

  std::cout << "Test operatori globali (con interi)" << std::endl;
  test_filter_out_int();
  std::cout << "Test operatori globali (con stringhe)" << std::endl;
  test_filter_out_string();
  std::cout << "Test operatori globali (con dato custom Punto3D)" << std::endl;
  test_filter_out_custom();

  return 0;
}