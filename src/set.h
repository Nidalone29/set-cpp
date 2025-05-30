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
 * Classe Set che implementa logicamente un set matematico.
 * Implementazione effettiva realizzato con una single linked list di elementi
 * generici T
 *
 * @tparam T tipo dei valori contenuti nel set
 * @tparam Eql operatore di confronto == (equivalenza) tra due tipi nel set
 */
template <typename T, typename Eql>
class Set {
 public:
  // Macro per un unsigned int
  typedef unsigned int u_int;
  // Macro per il valore generico T
  typedef T value_type;

  // default operations
  /**
   * @brief Default constructor
   *
   * Creazione di un Set vuoto (0 elementi)
   */
  Set() : _head_set(nullptr), _cardinality(0) {
#ifndef NDEBUG
    std::cout << "Set()" << std::endl;
#endif
  }

  /**
   * @brief Copy constructor
   *
   * Copy contructor che effettua una deep copy da un Set
   *
   * @param other
   * @throws std::bad_alloc possibile eccezione di allocazione dato che la add
   * contiene una new
   */
  Set(const Set& other) : _head_set(nullptr), _cardinality(0) {
    node* current = other._head_set;
    try {
      while (current != nullptr) {
        add(current->node_value);
        current = current->next;
      }
      // la cardinalità viene impostata dalla add
    } catch (...) {
      // viene sempre fatta questa clear + eccezione quando c'è un add
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
   * Copy assignment che effettua una deep copy da un Set
   * con l'operatore =
   *
   * @param other Set da copiare
   * @return Set& reference al Set risultante dalla copia
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
   * @brief Distruttore di un oggetto Set
   *
   * Viene distrutto l'oggetto Set sfruttando la funzione clear()
   */
  ~Set() {
    clear();
#ifndef NDEBUG
    std::cout << "~Set()" << std::endl;
#endif
  }

  /**
   * @brief Costruttore tramite due iteratori generici
   *
   * Costruttore che prende come parametro due iteratori da un altro set
   *
   * @tparam Iter tipo dell'iteratore
   * @param b iteratore di inizio
   * @param e iteratiore di fine
   * @throws std::bad_alloc possibile eccezione di allocazione dato che la add
   * contiene una new
   */
  template <typename Iter>
  Set(Iter begin, Iter end) : _head_set(nullptr), _cardinality(0) {
    try {
      for (; begin != end; ++begin) add(static_cast<T>(*begin));
    } catch (...) {
      clear();
      throw;
    }
  }

  // __________________________________________
  /**
   * @brief Controlla se il set è vuoto
   *
   * @return true se il set è vuoto
   * @return false se il set contiene almeno un elemento
   */
  bool is_empty() const {
    return (_cardinality == 0);
  }

  /**
   * @brief Dimensione della set
   *
   * @note è stato chiamato size() e non cardinality() perchè è più intuitivo
   * per un programmatore
   *
   * @return u_int cardinalità (numero di elementi inseriti) del Set
   */
  u_int size() const {
    return _cardinality;
  }

  /**
   * @brief Aggiunge un elemento (alla fine) del set
   *
   * se l'elemento esiste non succede niente
   *
   * @param toadd elemento da aggiungere
   * @return true sse item aggiunto con successo, false se è stato trovato
   * un duplicato
   */
  bool add(const value_type& toadd) {
    node* current = _head_set;

    // caso set vuoto
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

    // caso set popolato lo aggiungiamo alla fine
    while (current != nullptr) {
      // caso elemento duplicato
      if (_equals(current->node_value, toadd)) {
        // non aggiungiamo l'elemento (non creiamo neanche il nodo)
#ifndef NDEBUG
        std::cout << "add(const value_type&)"
                  << " value already exists " << toadd << std::endl;
#endif
        return false;
      }
      // caso raggiunta fine della lista
      if (current->next == nullptr) {
        // siamo alla fine quindi aggiungiamo
        node* tmp = new node(toadd);
        current->next = tmp;
        _cardinality++;
#ifndef NDEBUG
        std::cout << "add(const value_type&)"
                  << " added value " << toadd << std::endl;
#endif
        return true;
      }
      // incremento
      current = current->next;
    }
  }

  /**
   * @brief Rimuove un elemento dal set
   *
   * se l'elemento non era già presente non succede niente
   *
   * @param toremove elemento da rimuovre
   */
  void remove(const value_type& toremove) {
    node* current = _head_set;
    node* previous = _head_set;

    while (current != nullptr) {
      // caso elemento da rimuovere trovato
      if (_equals(current->node_value, toremove)) {
        // caso inizio lista
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
          // caso generico
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
      // incremento
      previous = current;
      current = current->next;
    }
#ifndef NDEBUG
    std::cout << "remove(const value_type&) "
              << " value not found " << toremove << std::endl;
#endif
  }

  /**
   * @brief Viene svuotato l'oggetto Set dai sui elementi
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

  // forward declarations per const iterator
 private:
  struct node;

 public:
  /**
   * @brief Iteratore che ritorna gli elementi inseriti (non modificabili)
   *
   * Forward iterator che itera su tutti i nodi dell set
   * interna e ritorna (un reference o un pointer) non modificabile
   */
  class const_iterator {
   public:
    // Traits dell'iterator
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;

    /**
     * @brief Costruttore di default
     */
    const_iterator() : _ptr(nullptr) {}

    /**
     * @brief Overload operatore *
     *
     * @return reference reference all'elemento nel set
     */
    reference operator*() const {
      return _ptr->node_value;
    }

    /**
     * @brief Overload operatore ->
     *
     * @return pointer puntatore all'elemento nel set
     */
    pointer operator->() const {
      return &(_ptr->node_value);
    }

    /**
     * @brief Overload post incremento ++
     */
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      _ptr = _ptr->next;
      return tmp;
    }

    /**
     * @brief Overload pre incremento ++
     */
    const_iterator& operator++() {
      _ptr = _ptr->next;
      return *this;
    }

    /**
     * @brief Oveload operatore ==
     */
    bool operator==(const const_iterator& other) const {
      return (_ptr == other._ptr);
    }

    /**
     * @brief Overlaod operatore !=
     */
    bool operator!=(const const_iterator& other) const {
      return !(other == *this);
    }

   private:
    friend class Set;

    const_iterator(const node* x) : _ptr(x) {}

    const node* _ptr;
  };  // classe const_iterator

  /**
   * @brief Ritorna l'iteratore per l'inizio della sequenza di dati
   *
   * @return const_iterator iteratore al primo elemento
   */
  const_iterator begin() const {
    return const_iterator(_head_set);
  }

  /**
   * @brief Ritorna l'iteratore per la fine della sequenza di dati
   *
   * @return const_iterator iteratore a nullptr
   */
  const_iterator end() const {
    return const_iterator(nullptr);
  }

  /**
   * @brief Overload operatore [] per accesso a dati del set tramite indice
   *
   * la posizione i è più o meno intesa per l'ordine di inserimento
   * (più o meno perchè se vengono rimossi elementi la posizione si scala)
   *
   * @param i indice "posizione" dell'elemento
   * @return const value_type& const reference al dato del set in posizione i
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
   * @brief confronto di equivalenza tra due set
   *
   * Vengono confrontati i due set e viene controllato se contengono gli
   * stessi elementi (NON viene contato l'ordine)
   * la logica implementativa si basa sul costruire un altro set ed
   * aggiungerci sia gli elementi di a che gli elementi di b e se il set
   * risultante ha la stessa dimensione di a e b, allora a e b sono
   * equivalenti
   *
   * @param b secondo set
   * @return true se i due set sono equivalenti
   * @return false altrimenti
   * @throws std::bad_alloc possibile eccezione di allocazione dato che la add
   * contiene una new
   */
  bool operator==(const Set<T, Eql>& other) {
    // prima controllo parametri (dim)
    if (this->size() != other.size()) return false;

    Eql predic;
    const_iterator begin_a, end_a, begin_other, end_other;
    for (begin_other = other.begin(), end_other = other.end();
         begin_other != end_other; ++begin_other) {
      // questa var found è una ottimizzazione per non far continuare
      // il controllo dopo aver trovato un elemnto comune nel set b
      for (begin_a = this->begin(), end_a = this->begin(); begin_a != end_a;
           ++begin_a) {
        if (!predic(*begin_a, *begin_other)) {
          return false;
        }
      }
    }

    return true;
  }

  // funzioni globali, impliementate qui per comodità sui dati templati
  /**
   * @brief overload operatore << per tutti gli elementi di un set
   *
   * vengono mandati tutti gli elementi di un set (separati da doppio spazio)
   *
   * @param os output stream
   * @param set il set da mandare allo stream
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
   * @brief implementazione Union ("concatenazione" di due set)
   *
   * viene ritornato un nuovo set che la tutti gli elementi dei due set
   * a e b non duplicati
   *
   * @param a primo set
   * @param b secondo set
   * @return Set<T, Eql> un nuovo set risultante da a unito b
   * @throws std::bad_alloc possibile eccezione di allocazione dato che la add
   * contiene una new
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
   * @brief Intersection Union ("intersezione" di due set)
   *
   * viene ritornato un nuovo set che contiene gli elementi in comune tra i
   * due set
   *
   * @param a primo set
   * @param b secondo set
   * @return Set<T, Eql> un nuovo set risultante da a intersecato b
   * @throws std::bad_alloc possibile eccezione di allocazione dato che la add
   * contiene una new
   */
  friend Set<T, Eql> operator-(const Set<T, Eql>& a, const Set<T, Eql>& b) {
    Set<T, Eql> tmp;
    try {
      Eql predic;
      const_iterator begin_a, end_a, begin_b, end_b;
      for (begin_a = a.begin(), end_a = a.end(); begin_a != end_a; ++begin_a) {
        // questa var found è una ottimizzazione per non far continuare
        // il controllo dopo aver trovato un elemnto comune nel set b
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
   * @brief Struttura dati nodo
   *
   * Struttura dati node interna che viene usata per contenere gli elementi
   * inseriti in dei nodi linkati singoli.
   */
  struct node {
    /**
     * @brief Costruttore di default
     *
     * @post next == nullptr
     */
    node() : next(nullptr) {}

    /**
     * @brief Costruttore secondario
     *
     * @param v elemento
     * @param n puntatore a next
     * @post next == n
     * @post node_value == v
     */
    node(const value_type& v, node* n) : node_value(v), next(n) {}

    /**
     * @brief Costruttore secondario
     *
     * @param v elemento
     * @post next == n
     * @post node_value == v
     */
    explicit node(const value_type& v) : node_value(v), next(nullptr) {}

    // Copy constructor, Operatore Assignment e Destructor possiamo
    // farli generare al compilatore

    // valore contenuto nel nodo
    value_type node_value;
    // Puntatre al prossimo nodo
    node* next;
  };

  // Linked list for set
  node* _head_set;
  // Set size (cardinality)
  u_int _cardinality;
  // equals operator for ==
  Eql _equals;
};

/**
 * @brief funzione globale che ritorna un nuovo set con elementi che rispettano
 * un certo predicato P
 *
 * @tparam T tipo del set
 * @tparam Eql operatore di confronto del set
 * @tparam P predicato
 * @param S il set sui cui elementi viene verificata la corrispondenza
 * @param pred il predicato da applicare agli element del set
 * @return Set<T, Eql> un nuovo set che contiene tutti gli elementi di S che
 * soddisfano il prediato P
 * @throws std::bad_alloc possibile eccezione di allocazione dato che la add
 * contiene una new
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
