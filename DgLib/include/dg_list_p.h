//================================================================================
// @ list_p.h
// 
// Class: list_p
//
// Pre-allocated Linked list. Similar to std::list with similarly named methods
// and functionality. The underlying arrays are preallocated and only change in
// size if extending list past that allocated, or manually resizing. This makes
// for fast insertion/erasing of elements.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DG_LIST_S_H
#define DG_LIST_S_H

#include <exception>

#include "impl_container_common.h"

namespace Dg
{

  //--------------------------------------------------------------------------------
  //	@	list_p<T>:	T: m_data type
  //--------------------------------------------------------------------------------
  template<typename T>
  class list_p
  {
  private:

	  //--------------------------------------------------------------------------------
	  //		Container to hold the object and pointers
	  //--------------------------------------------------------------------------------
	  struct DataContainer
	  {
		  DataContainer(): next(nullptr), previous(nullptr) {}
		
		  DataContainer* next;
		  DataContainer* previous;
		  T element;
	  };

  public:

	  //--------------------------------------------------------------------------------
	  //		Constant Iterator
	  //--------------------------------------------------------------------------------
	  class const_iterator
	  {
	  private:
		  friend class list_p;
		
	  private:
		  //Special constructor, not for external use
		  const_iterator(const DataContainer* _ptr) {ptr = _ptr;}

	  public:
		  //Constructor / destructor
		  const_iterator(){}
		  ~const_iterator(){}

		  //Copy operations
		  const_iterator(const const_iterator& it): ptr(it.ptr){}
		  const_iterator& operator= (const const_iterator&);
		
		  //Comparison
		  bool operator==(const const_iterator& it) const {return ptr == it.ptr;}
		  bool operator!=(const const_iterator& it) const {return ptr != it.ptr;}

		  //operators
		  const_iterator& operator++();
		  const_iterator operator++(int);
		  const_iterator& operator--();
		  const_iterator operator--(int);

		  //Conversion
		  const T* operator->() const {return &(ptr->element);}
		  const T& operator*() const {return ptr->element;}

	  private:
		  //Data members
		  const DataContainer *ptr;

	  };


	  //--------------------------------------------------------------------------------
	  //		Iterator
	  //--------------------------------------------------------------------------------
	  class iterator
	  {
		  friend class list_p;

	  private:
		  //Special constructor, not for external use
		  iterator(DataContainer* _ptr) {ptr = _ptr;}

	  public:
		  //Constructor / destructor
		  iterator(){}
		  ~iterator(){}

		  //Copy operations
		  iterator(const iterator& it): ptr(it.ptr){}
		  iterator& operator= (const iterator&);

		  //Comparison
		  bool operator==(const iterator& it) const {return ptr == it.ptr;}
		  bool operator!=(const iterator& it) const {return ptr != it.ptr;}

		  //operators
		  iterator& operator++();
		  iterator operator++(int);
		  iterator& operator--();
		  iterator operator--(int);

		  //Conversion
		  operator const_iterator() const {return const_iterator(ptr);}
		  T* operator->() const {return &(ptr->element);}
		  T& operator*() const {return ptr->element;}

	  private:
		  //Data members
		  DataContainer *ptr;

	  };


  public:
	  //Constructor / destructor
	  list_p(size_t a_size = 1);
	  ~list_p();

	  //Copy operations
	  list_p(const list_p&);
	  list_p& operator= (const list_p&);

	  //Accessors
	  iterator			  begin()		  const {return iterator(m_rootContainer.next);}
    iterator			  end()		    const {return iterator(const_cast<DataContainer*>(&m_endContainer)); }
	  const_iterator	cbegin()	  const {return const_iterator(m_rootContainer.next);}
    const_iterator	cend()		  const {return const_iterator(const_cast<DataContainer*>(&m_endContainer)); }
	  size_t			    size()		  const {return m_currentSize;}
	  size_t 			    max_size()	const {return m_arraySize;}
	  bool 			      empty()		  const {return m_currentSize == 0;}
	  T&				      back()		  const {return m_endContainer.previous->element;}
	  T&				      front()		  const {return m_rootContainer.next->element;}

	  //Manipulators
    void push_back(T const &);
	  bool push_back();
    void push_front(T const &);
    bool push_front();
    void insert(iterator const &, T const &);
	  void pop_back();
	  void pop_front();
	  void erase(iterator&); //TODO Needs to return iterator
	  void clear();
	  void resize(size_t);

  private:
    //Functions
    void extend();
    void init(size_t new_size);

  private:
	  //Data members

	  //Pre-allocated block of memory to hold elements
	  DataContainer *m_data;

	  //Root and end objects, and pointers
	  DataContainer m_rootContainer;
	  DataContainer m_endContainer;

	  //Next free element in the list;
	  DataContainer* m_nextFree;		

	  //Sizes
	  size_t m_currentSize;
	  size_t m_arraySize;

  };


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::iterator::operator=()
  //--------------------------------------------------------------------------------
  //		iterator Assignment
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::iterator& list_p<T>::iterator::operator=
	  (const typename list_p<T>::iterator& other)
  {
	  ptr = other.ptr;

	  return *this;
  }	//End:: list_p<T>::iterator::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  //		iterator pre increment
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::iterator& list_p<T>::iterator::operator++()
  {
	  ptr = ptr->next;

	  return *this;
  }	//End: list_p<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  //		iterator post increment
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::iterator list_p<T>::iterator::operator++(int)
  {
	  iterator result(*this);	// make a copy for result
      ++(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_p<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  //		iterator pre decrement
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::iterator& list_p<T>::iterator::operator--()
  {
	  ptr = ptr->previous;

	  return *this;

  }	//End: list_p<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  //		iterator post decrement
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::iterator list_p<T>::iterator::operator--(int)
  {
	  iterator result(*this);	// make a copy for result
      --(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_p<T>::iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  //		const_iterator Assignment
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::const_iterator& list_p<T>::const_iterator::operator=
	  (const typename list_p<T>::const_iterator& other)
  {
	  ptr = other.ptr;

	  return *this;

  }	//End::list_p<T>::const_iterator::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::const_iterator::operator++()
  //--------------------------------------------------------------------------------
  //		const_iterator pre increment
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::const_iterator& list_p<T>::const_iterator::operator++()
  {
	  ptr = ptr->next;

	  return *this;

  }	//End: list_p<T>::const_iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  //		const_iterator post increment
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::const_iterator list_p<T>::const_iterator::operator++(int)
  {
	  const_iterator result(*this);	// make a copy for result
      ++(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_p<T>::const_iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  //		const_iterator pre decrement
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::const_iterator& list_p<T>::const_iterator::operator--()
  {
	  ptr = ptr->previous;

	  return *this;

  }	//End: list_p<T>::const_iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  //		const_iterator post decrement
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::const_iterator list_p<T>::const_iterator::operator--(int)
  {
	  const_iterator result(*this);	// make a copy for result
      --(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_p<T>::const_iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::init()
  //--------------------------------------------------------------------------------
  //		General initialise function
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::init(size_t a_size)
  {
    assert(a_size > 0);

    T * tempPtr = static_cast<DataContainer *>(realloc(m_data, a_size * sizeof(DataContainer)));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc;
    }

    m_data = tempPtr;

	  //Assign sizes
    m_arraySize = a_size;
	  m_currentSize = 0;

	  //Initialise m_data
	  m_nextFree = &m_data[0];

	  //Set outer container pointers
    m_rootContainer.next = &m_endContainer;
    m_endContainer.previous = &m_rootContainer;

	  //Only need to assign forward pointers
	  for (size_t i = 0; i < m_arraySize-1; i++)
	  {
		  m_data[i].next = &m_data[i+1];
	  }

  }	//End: list_p::init()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::list_p<T>()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class T>
  list_p<T>::list_p() : m_data(nullptr), m_nextFree(nullptr)
  {
	  //Set m_data
	  init(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: list_p::DgLingedList()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::list_p<T>()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class T>
  list_p<T>::list_p(size_t a_size): m_data(nullptr), m_nextFree(nullptr)
  {
	  //Size must be at least 1
    assert(a_size > 0);

	  //Set up the list
    init(a_size);

  }	//End: list_p::DgLingedList()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::~list_p<T>()
  //--------------------------------------------------------------------------------
  //		Destructor
  //--------------------------------------------------------------------------------
  template<class T>
  list_p<T>::~list_p()
  {
	  free(m_data)

  }	//End: list_p::~DgLingedList()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::list_p<T>()
  //--------------------------------------------------------------------------------
  //		Copy constructor
  //--------------------------------------------------------------------------------
  template<class T>
  list_p<T>::list_p(const list_p& other)
  {
	  //Initialise m_data
	  init(other.m_arraySize);

	  //Assign m_data
	  list_p<T>::const_iterator it = other.begin();
	  for (it; it != other.end(); ++it)
	  {
		  push_back(*it);
	  }

  }	//End: list_p::list_p()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::operator()
  //--------------------------------------------------------------------------------
  //		Assignment
  //--------------------------------------------------------------------------------
  template<class T>
  list_p<T>& list_p<T>::operator=(const list_p& other)
  {
	  if (this == &other)
		  return *this;

	  //resize array
	  resize(other.m_arraySize);

	  //Assign m_data
	  list_p<T>::const_iterator it = other.begin();
	  for (it; it != other.end(); ++it)
	  {
		  push_back(*it);
	  }

	  return *this;
  }	//End: list_p::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::clear()
  //--------------------------------------------------------------------------------
  //		Clear the list, retains allocated memory.
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::clear()
  {
	  //Reset next free
	  m_nextFree = &m_data[0];

	  //Set outer container pointers
    m_rootContainer.next = &m_endContainer;
    m_endContainer.previous = &m_rootContainer;

	  //close the last element in the list
	  m_data[m_arraySize-1].next = nullptr;

	  //Assign pointers
	  for (size_t i = 0; i < m_arraySize-1; i++)
	  {
		  m_data[i].next = &m_data[i+1];
	  }

	  m_currentSize = 0;

  }	//End: list_p<T>::clear()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::resize()
  //--------------------------------------------------------------------------------
  //		Resize the list, wipes all m_data.
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::resize(size_t a_newSize)
  {
	  //Size must be at least 1
    assert(a_newSize > 0);

	  //Initialise m_data
	  init(new_size);

  }	//End: list_p<T>::clear()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::push_back()
  //--------------------------------------------------------------------------------
  //		Add an element to the back of the list
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::push_back(const T& a_item)
  {
	  //Is the list full?
	  if (m_currentSize == m_arraySize)
		  extend();

	  //Get the list node to work on
	  DataContainer *new_element = m_nextFree;
	
	  //Move m_nextFree pointer to the next DataContainer
	  m_nextFree = m_nextFree->next;

	  //Assign the element
    memcpy(new_element->element, &a_item, sizeof(T));

	  //Add the current element to the back of the active list
	  m_endContainer.previous->next = new_element;
	  new_element->previous = m_endContainer.previous;
    new_element->next = &m_endContainer;
	  m_endContainer.previous = new_element;

	  //Increment m_currentSize
	  m_currentSize++;

  }	//End: list_p::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::push_back()
  //--------------------------------------------------------------------------------
  //		Add an element to the back of the list, but does not assign, nor
  //		resize the array.
  //--------------------------------------------------------------------------------
  template<class T>
  bool list_p<T>::push_back()
  {
	  //Is the list full?
	  if (m_currentSize == m_arraySize)
		  return false;

	  //Get the list node to work on
	  DataContainer *new_element = m_nextFree;

	  //Move m_nextFree pointer to the next DataContainer
	  m_nextFree = m_nextFree->next;

	  //Add the current element to the back of the active list
	  m_endContainer.previous->next = new_element;
	  new_element->previous = m_endContainer.previous;
    new_element->next = &m_endContainer;
	  m_endContainer.previous = new_element;

	  //Increment m_currentSize
	  m_currentSize++;

	  return true;

  }	//End: list_p::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::push_front()
  //--------------------------------------------------------------------------------
  //		Add an element to the front of the list, but does not assign, nor
  //		resize the array.
  //--------------------------------------------------------------------------------
  template<class T>
  bool list_p<T>::push_front()
  {
      //Is the list full?
      if (m_currentSize == m_arraySize)
          return false;

      //Get the list node to work on
      DataContainer *new_element = m_nextFree;

      //Move m_nextFree pointer to the next DataContainer
      m_nextFree = m_nextFree->next;

      //Add the current element to the back of the active list
      m_rootContainer.next->previous = new_element;
      new_element->previous = &m_rootContainer;
      new_element->next = m_rootContainer.next;
      m_rootContainer.next = new_element;

      //Increment m_currentSize
      m_currentSize++;

      return true;

  }	//End: list_p::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::push_front()
  //--------------------------------------------------------------------------------
  //		Add an element to the front of the list
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::push_front(const T& val)
  {
	  //Is the list full?
	  if (m_currentSize == m_arraySize)
		  extend();

	  //Get the list node to work on
	  DataContainer *new_element = m_nextFree;
	
	  //Move m_nextFree pointer to the next DataContainer
	  m_nextFree = m_nextFree->next;

	  //Assign the element
    memcpy(new_element->element, &a_item, sizeof(T));

	  //Add the current element to the back of the active list
	  m_rootContainer.next->previous = new_element;
	  new_element->previous = &m_rootContainer;
	  new_element->next = m_rootContainer.next;
	  m_rootContainer.next = new_element;

	  //Increment m_currentSize
	  m_currentSize++;

  }	//End: list_p::push_front()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::pop_back()
  //--------------------------------------------------------------------------------
  //		Erase last element
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::pop_back()
  {
	  //Range check
	  assert(m_currentSize != 0);
	
	  //Get new last element
    DataContainer* last = m_endContainer.previous->previous;

	  //Assign next free
	  m_endContainer.previous->next = m_nextFree;
	  m_nextFree = m_endContainer.previous;

	  //Break element from chain
    last->next = &m_endContainer;			//prev points to next
	  m_endContainer.previous = last;	//next points to previous

	  //Deincrement m_currentSize
	  m_currentSize--;

  }	//End: list_p::pop_back()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::pop_front()
  //--------------------------------------------------------------------------------
  //		Erase first element
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::pop_front()
  {
	  //Range check
	  assert(m_currentSize != 0);
	
	  //Get new first element
    DataContainer* first = m_rootContainer.next->next;

	  //Assign next free
	  m_rootContainer.next->next = m_nextFree;
	  m_nextFree = m_rootContainer.next;

	  //Break element from chain
    first->previous = &m_rootContainer;		//prev points to next
	  m_rootContainer.next = first;	//next points to previous

	  //Deincrement m_currentSize
	  m_currentSize--;

  }	//End: list_p::pop_front()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::insert()
  //--------------------------------------------------------------------------------
  //		Add an element to the list at position. 
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::insert(iterator const & it, const T& a_item)
  {
    assert(it.ptr != &m_rootContainer);

	  //Is the list full?
	  if (m_currentSize == m_arraySize)
		  extend();
	
	  //Get the list node to work on
	  DataContainer *new_element = m_nextFree;
	
	  //Move m_nextFree pointer to the next DataContainer
	  m_nextFree = m_nextFree->next;

	  //Insert next free
	  it.ptr->previous->next = new_element;
	  new_element->previous = it.ptr->previous;
	  it.ptr->previous = new_element;
	  new_element->next = it.ptr;

	  //Set the element
    memcpy(new_element->element, &a_item, sizeof(T));

	  //Increment m_currentSize
	  m_currentSize++;

  }	//End: list_p::insert()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::erase()
  //--------------------------------------------------------------------------------
  //		Erase an element from the list
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::erase(iterator& it)
  {
	  //Remember previous element
	  DataContainer* next = it.ptr->next;

	  //Break element from chain
	  it.ptr->previous->next = it.ptr->next;		//prev points to next
	  it.ptr->next->previous = it.ptr->previous;	//next points to previous

	  //Add this broken item to the begining of the free list
	  it.ptr->next = m_nextFree;				//put item in between m_nextFree and end of the list
	
	  //Reset m_nextFree
	  m_nextFree = it.ptr;
	
	  //Deincrement m_currentSize
	  m_currentSize--;

	  //Return iterator to the next container
	  it = iterator(next);

  }	//End: list_p::erase()


  //--------------------------------------------------------------------------------
  //	@	list_p<T>::extend()
  //--------------------------------------------------------------------------------
  //		Increases the size of the underlying arrays by a factor of 1.5
  //--------------------------------------------------------------------------------
  template<class T>
  void list_p<T>::extend()
  {
	  //Calculate new size
	  size_t new_size = m_arraySize << 1;
	
    if (new_size < m_arraySize)
    {
      throw std::overflow_error("m_arraySize");
    }

	  //Create new array
	  DataContainer* new_data = static_cast<DataContainer *>(malloc(new_size * sizeof(DataContainer)));

    if (new_data == nullptr)
    {
      throw std::bad_alloc;
    }

	  //Assign pointers
	  for (size_t i = 0; i < new_size-1; i++)
	  {
		  new_data[i].next = &new_data[i+1];
	  }
	  for (size_t i = 1; i < m_currentSize; i++)
	  {
		  new_data[i].previous = &new_data[i-1];
	  }

	  //Assign values
	  iterator it = begin();
	  for (size_t i = 0; it != end(); ++it, ++i)
	  {
		  new_data[i].element = *it;
	  }

	  //Assign m_data pointer
	  free(m_data);
	  m_data = new_data;
	
	  //Assign next free pointer
	  m_nextFree = &new_data[m_currentSize];
	
	  //Adjust sizes
	  m_arraySize = new_size;

	  //Determine root and end pointers
	  if (m_currentSize == 0)
	  {
      m_rootContainer.next = &m_endContainer;
      m_endContainer.previous = &m_rootContainer;
	  }
	  else
	  {
		  m_rootContainer.next = &m_data[0];
		  m_endContainer.previous = &m_data[m_currentSize-1];
      new_data[0].previous = &m_rootContainer;
      new_data[m_currentSize - 1].next = &m_endContainer;
	  }
	
  }	//End: list_p<T>::extend()


  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------


  //--------------------------------------------------------------------------------
  //	@	find()
  //--------------------------------------------------------------------------------
  //		Find a value in the list, returns iterator
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::iterator find (
	  typename list_p<T>::iterator first, 
	  typename list_p<T>::iterator last, 
	  const T& val)
  {
    while (first!=last) 
    {
      if (*first==val) 
		    return first;

      ++first;
    }

    return last;

  }	//End find()


  //--------------------------------------------------------------------------------
  //	@	find()
  //--------------------------------------------------------------------------------
  //		Find a value in the list, returns const_iterator
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_p<T>::const_iterator find (
	  typename list_p<T>::const_iterator first, 
	  typename list_p<T>::const_iterator last, 
	  const T& val)
  {
    while (first!=last) 
    {
      if (*first==val) 
		    return first;

      ++first;
    }

    return last;

  }	//End find()

};
#endif