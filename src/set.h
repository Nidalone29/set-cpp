/**
 * @file set.h
 * @author Nidal Guerouaja
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 */

#ifndef SET_H
#define SET_H

#include <algorithm>  // std::swap
#include <cassert>    // assert
#include <cstddef>    // std::ptrdiff_t
#include <iostream>   // std::cout (per debug)
#include <iterator>   // std::forward_iterator_tag

/**
 * @brief Implementation of an unordered Set
 *
 * Set Class that implements a mathematical Set of generic elements T
 * https://en.wikipedia.org/wiki/Set_(mathematics)
 *
 * @tparam T Type of values contained in the Set
 * @tparam Eql equivalence operator between two values in the Set
 */
template <typename T, typename Eql>
class Set {
 public:
  // unsigned int macro
  typedef unsigned int u_int;
  // generic value T macro
  typedef T value_type;

  // default operations
  /**
   * @brief Default constructor
   *
   * Construct and empty Set
   */
  Set() : _head_set(nullptr), _cardinality(0) {
#ifndef NDEBUG
    std::cout << "Set()" << std::endl;
#endif
  }

  /**
   * @brief Copy constructor
   *
   * Copy constructor that performs a DEEP copy of the elements
   *
   * @param other
   * @throws std::bad_alloc possible allocation exception caused by new
   */
  Set(const Set& other) : _head_set(nullptr), _cardinality(0) {
    node* current = other._head_set;
    try {
      while (current != nullptr) {
        add(current->node_value);
        current = current->next;
      }
      // note that the cardinality is Set by add()
    } catch (...) {
      // always clean before throwing because of add()
      clear();
      throw;
    }

#ifndef NDEBUG
    std::cout << "Set(const Set&)" << std::endl;
#endif
  }

  /**
   * @brief Copy assignment operator
   *
   * Copy assignment operator that performs a DEEP copy
   *
   * @param other Set to copy
   * @return Set& reference to the resulting Set
   */
  Set& operator=(const Set& other) {
    if (this != &other) {
      Set tmp(other);
      std::swap(this->_head_set, tmp._head_set);
      std::swap(this->_cardinality, tmp._cardinality);
    }
    return *this;
#ifndef NDEBUG
    std::cout << "Set& operator=(const Set&)" << std::endl;
#endif
  }

  /**
   * @brief Destructor
   *
   * The destruction of the Set is performed using the helper function clear()
   */
  ~Set() {
    clear();
#ifndef NDEBUG
    std::cout << "~Set()" << std::endl;
#endif
  }

  /**
   * @brief Construction with iterators
   *
   * Constructor from two iterators
   *
   * @tparam Iter Type of the Iterator
   * @param begin begin iterator
   * @param end end iterator
   * @throws std::bad_alloc possible allocation exception caused by new
   * (contained in add())
   */
  template <typename Iter>
  Set(Iter begin, Iter end) : _head_set(nullptr), _cardinality(0) {
    try {
      for (; begin != end; ++begin) {
        add(static_cast<T>(*begin));
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  /**
   * @brief Checks for an empty Set
   *
   * @return true if the Set is empty, false otherwise
   */
  bool is_empty() const {
    return (_cardinality == 0);
  }

  /**
   * @brief Number of elements in the Set
   *
   * @note The function name is size() instead of cardinality() because it's
   * more intuitive for a programmer
   *
   * @return u_int number of elements in the Set
   */
  u_int size() const {
    return _cardinality;
  }

  /**
   * @brief Add an element at the end of the Set
   *
   * Function that adds an element to the Set. If the element already exists the
   * function returns false and the Set is not changed.
   *
   * @param toadd Element to add
   * @return true iif the element has been added successfully, false otherwise
   */
  bool add(const value_type& toadd) {
    node* current = _head_set;

    // empty Set
    if (this->is_empty()) {
      node* tmp = new node(toadd);
      _head_set = tmp;
      _cardinality++;
#ifndef NDEBUG
      std::cout << "add(const value_type&)"
                << " added first value " << toadd << std::endl;
#endif
      return true;
    }

    // non empty Set, aka adding the element to the end
    while (current != nullptr) {
      // duplicated element
      if (_equals(current->node_value, toadd)) {
        // we don't add the element
#ifndef NDEBUG
        std::cout << "add(const value_type&)"
                  << " value already exists " << toadd << std::endl;
#endif
        return false;
      }
      // adding to the end of the list
      if (current->next == nullptr) {
        node* tmp = new node(toadd);
        current->next = tmp;
        _cardinality++;
#ifndef NDEBUG
        std::cout << "add(const value_type&)"
                  << " added value " << toadd << std::endl;
#endif
        return true;
      }
      current = current->next;
    }
  }

  /**
   * @brief Remove an element from the Set
   *
   * Function that removes an element from the Set. If the element to remove was
   * not found the Set is not changed.
   *
   *  @param toremove element to remove
   */
  void remove(const value_type& toremove) {
    node* current = _head_set;
    node* previous = _head_set;

    while (current != nullptr) {
      // check if the element to remove has been found
      if (_equals(current->node_value, toremove)) {
        // first element of the list
        if (current == previous) {
          _head_set = current->next;
          delete current;
          _cardinality--;
#ifndef NDEBUG
          std::cout << "remove(const value_type&)"
                    << " removed value at start " << toremove << std::endl;
#endif
          return;
        } else {
          // generic case
          previous->next = current->next;
          delete current;
          _cardinality--;
#ifndef NDEBUG
          std::cout << "remove(const value_type&)"
                    << " removed value in the middle " << toremove << std::endl;
#endif
          return;
        }
      }

      previous = current;
      current = current->next;
    }
#ifndef NDEBUG
    std::cout << "remove(const value_type&)"
              << " value not found " << toremove << std::endl;
#endif
  }

  /**
   * @brief Function that eliminates every element in the Set
   *
   * @post _cardinality == 0
   * @post _head_set == nullptr
   */
  void clear() {
    node* current = _head_set;
    while (current != nullptr) {
      node* cnext = current->next;
      delete current;
      current = cnext;
    }
    _cardinality = 0;
    _head_set = nullptr;
#ifndef NDEBUG
    std::cout << "clear()"
              << " set got cleared " << std::endl;
#endif
  }

  // forward declarations for const iterator
 private:
  struct node;

 public:
  /**
   * @brief Const iterator
   *
   * Forward iterator that iterates over all the nodes in the Set and returns
   * a non modifiable reference or pointer. The order of the elements in the Set
   * is defined by their insertion order.
   */
  class const_iterator {
   public:
    // Iterator Traits
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;

    /**
     * @brief Default Constructor
     */
    const_iterator() : _ptr(nullptr) {}

    /**
     * @brief Overload operator *
     *
     * @return reference to the element
     */
    reference operator*() const {
      return _ptr->node_value;
    }

    /**
     * @brief Overload operator ->
     *
     * @return pointer to the element
     */
    pointer operator->() const {
      return &(_ptr->node_value);
    }

    /**
     * @brief Overload post increment ++
     */
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      _ptr = _ptr->next;
      return tmp;
    }

    /**
     * @brief Overload pre increment ++
     */
    const_iterator& operator++() {
      _ptr = _ptr->next;
      return *this;
    }

    /**
     * @brief Overload operator ==
     */
    bool operator==(const const_iterator& other) const {
      return (_ptr == other._ptr);
    }

    /**
     * @brief Overload operator !=
     */
    bool operator!=(const const_iterator& other) const {
      return !(other == *this);
    }

   private:
    friend class Set;

    const_iterator(const node* x) : _ptr(x) {}

    const node* _ptr;
  };  // class const_iterator

  /**
   * @brief Function that returns the iterator to the first element inserted in
   * the Set
   *
   * @return const_iterator iterator to the first element
   */
  const_iterator begin() const {
    return const_iterator(_head_set);
  }

  /**
   * @brief Function that represents the end of the data
   *
   * @return const_iterator iterator to nullptr
   */
  const_iterator end() const {
    return const_iterator(nullptr);
  }

  /**
   * @brief Overload operator [] to access elements with an index
   *
   * The index is loosely based on the insertion order (loosely because elements
   * can be removed and added)
   *
   * @param i index index of teh value
   * @return const value_type& const reference to the value stored in position i
   */
  const value_type& operator[](const int i) const {
    assert(i >= 0);
    assert(i < _cardinality);

    node* current = _head_set;
    for (int x = 0; x < i; ++x) {
      current = current->next;
    }

    return (*current).node_value;
  }

  /**
   * @brief Equivalency comparison between two Sets
   *
   * The function compares two Sets by checking if all the elements of a Set are
   * present in the other Set (in ay arbitrary order). This is implemented by
   * making a local copy of the other Set adding all the elements of the current
   * Set into the copy. If there is a duplicate the add function returns true,
   * therefore the Set are not equal.
   *
   * @param other The Set that the current Set is compared to
   * @return true iif the two Sets have the same element
   */
  bool operator==(const Set<T, Eql>& other) {
    if (this->size() != other.size()) {
      return false;
    }

    Set local_copy(other);
    const_iterator begin_a, end_a;
    for (begin_a = this->begin(), end_a = this->end(); begin_a != end_a;
         ++begin_a) {
      if (local_copy.add(*begin_a)) {
        return false;
      }
    }

    return true;
  }

  /**
   * @brief Overload operator <<
   *
   * This function returns a stream of all the elements in the Set separated by
   * two spaces
   *
   * @param os output stream
   * @param set input Set
   * @return std::ostream& output stream
   */
  friend std::ostream& operator<<(std::ostream& os, const Set& set) {
    node* current = set._head_set;
    while (current != nullptr) {
      os << current->node_value << "  ";
      current = current->next;
    }
    return os;
  }

  /**
   * @brief Implementation of the Union operation ("concatenation" of two Sets)
   *
   * The function returns a new Set that contains all the elements of the input
   * Sets, without any duplicates
   *
   * @param a first input Set
   * @param b second input Set
   * @return Set<T, Eql> a new Set defined by the union of the Sets a and b
   * @throws std::bad_alloc possible allocation exception caused by new
   * (contained in add())
   */
  friend Set<T, Eql> operator+(const Set<T, Eql>& a, const Set<T, Eql>& b) {
    Set<T, Eql> tmp(a);
    try {
      node* current_b = b._head_set;
      while (current_b != nullptr) {
        tmp.add(current_b->node_value);
        current_b = current_b->next;
      }
    } catch (...) {
      tmp.clear();
      throw;
    }
    return tmp;
  }

  /**
   * @brief Implementation of the Intersection operation
   *
   * The function returns a new Set that contains all the elements in common
   * between the two input Sets
   *
   * @param a first input Set
   * @param b second input Set
   * @return Set<T, Eql> a new Set defined by the intersection of the Sets a and
   * b
   * @throws std::bad_alloc possible allocation exception caused by new
   * (contained in add())
   */
  friend Set<T, Eql> operator-(const Set<T, Eql>& a, const Set<T, Eql>& b) {
    Set<T, Eql> tmp;
    try {
      Eql predic;
      const_iterator begin_a, end_a, begin_b, end_b;
      for (begin_a = a.begin(), end_a = a.end(); begin_a != end_a; ++begin_a) {
        // the found variable is used as an optimization to interrupt the loop
        // when you find the value in the other Set
        bool found = false;
        for (begin_b = b.begin(), end_b = b.end(); (begin_b != end_b) && !found;
             ++begin_b) {
          if (predic(*begin_a, *begin_b)) {
            found = true;
            tmp.add(*begin_a);
          }
        }
      }
    } catch (...) {
      tmp.clear();
      throw;
    }
    return tmp;
  }

 private:
  /**
   * @brief Data structure for the Node
   *
   * Internal DS for the underlying Linked List
   */
  struct node {
    /**
     * @brief Default constructor
     *
     * @post next == nullptr
     */
    node() : next(nullptr) {}

    /**
     * @brief Constructor with value and next
     *
     * @param v element
     * @param n pointer to next
     * @post next == n
     * @post node_value == v
     */
    node(const value_type& v, node* n) : node_value(v), next(n) {}

    /**
     * @brief Constructor with value
     *
     * @param v element
     * @post next == n
     * @post node_value == v
     */
    explicit node(const value_type& v) : node_value(v), next(nullptr) {}

    // Value contained in the node
    value_type node_value;
    // Pointer to next node
    node* next;
  };

  // Linked list for Set
  node* _head_set;
  // Set size (cardinality)
  u_int _cardinality;
  // Equals operator for ==
  Eql _equals;
};

/**
 * @brief Global function that returns a new Set with all the elements that
 * satisfy a predicate P
 *
 * @tparam T Type of the Set
 * @tparam Eql Equality operator fo the Set
 * @tparam P Type of the Predicate
 * @param S input Set
 * @param pred the predicate that will be applied to all the element in the Set
 * @return Set<T, Eql> a new Set with all the elements that satisfy the input
 * predicate
 * @throws std::bad_alloc possible allocation exception caused by new
 * (contained in add())
 */
template <typename T, typename Eql, typename P>
Set<T, Eql> filter_out(const Set<T, Eql>& S, P pred) {
  Set<T, Eql> tmp;
  try {
    typename Set<T, Eql>::const_iterator b, e;
    for (b = S.begin(), e = S.end(); b != e; ++b) {
      if (pred(*b)) tmp.add(*b);
    }
  } catch (...) {
    tmp.clear();
    throw;
  }
  return tmp;
}

#endif  // SET_H
