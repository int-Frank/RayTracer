/*!
* @file vector_p.h
*
* @author Frank Hart
* @date 2/01/2014
*
* Class header: vector_p<>
*/

#ifndef DG_VECTOR_P_H
#define DG_VECTOR_P_H

#include <exception>
#include <assert.h>

#include "impl_container_common.h"

//--------------------------------------------------------------------------------
//	@	vector_p<T>:	T: m_data
//--------------------------------------------------------------------------------
/*!
* @ingroup utility_container
*
* @class vector_p
*
* @brief Contiguous array, similar to std::vector.
*
* http://www.cplusplus.com/reference/vector/vector/
*
* DgArrays are sequence containers representing arrays that can change in size.
* Just like arrays, DgArrays use contiguous storage locations for their elements,
* which means that their elements can also be accessed using offsets on regular
* pointers to its elements, and just as efficiently as in arrays. But unlike arrays,
* their size can change dynamically, with their storage being handled automatically by
* the container.
*
* Internally, DgArrays use a dynamically allocated array to store their elements.
* This array may need to be reallocated in order to grow in size when new elements
* are inserted, which implies allocating a new array and moving all elements to it.
* This is a relatively expensive task in terms of processing time, and thus, DgArrays
* do not reallocate each time an element is added to the container.
*
* Instead, vector_p containers may allocate some extra storage to accommodate for
* possible growth, and thus the container may have an actual capacity greater than
* the storage strictly needed to contain its elements (i.e., its size).
*
*
* @author Frank Hart
* @date 7/01/2014
*/

typedef unsigned int uint32;

namespace Dg
{
  template<class T>
  class vector_p
  {
  public:
    //Constructor / destructor
    vector_p();

    //! Construct with a set size
    vector_p(uint32);
    ~vector_p();

    vector_p(const vector_p&);
    vector_p& operator= (const vector_p&);

    //! Copy both the current elements and the elements in the reserved memory.
    void copy_all(const vector_p& other);

    //! Access element
    T& operator[](uint32 i)				{ return m_data[i]; }

    //! Accessor, no range check.
    const T& operator[](uint32 i) const	{ return m_data[i]; }

    //! Get last element
    /// Calling this function on an empty container causes undefined behavior.
    T& back() { return m_data[m_arraySize - 1]; }

    //! Get last element
    /// Calling this function on an empty container causes undefined behavior.
    const T& back() const { return m_data[m_arraySize - 1]; }

    //! Accessor with range check.
    T& at(uint32);

    //! Accessor with range check.
    const T& at(uint32) const;

    //! Current size of the array
    uint32 size()		const			{ return m_currentSize; }

    //! Is the array empty
    bool empty()		const			{ return m_currentSize == 0; }

    //! Size of the reserved memory.
    uint32 max_size()	const			{ return m_arraySize; }

    //! Get pointer to first element.
    T* data()							{ return m_data; }

    //! Get pointer to first element.
    const T* Data()		const			{ return m_data; }

    //! Add element to the back of the array.
    void push_back(const T&);

    //! Remove element from the back of the array.
    void pop_back();

    //! Add element to the back of the array.
    void push_front(const T&);

    //! Remove element from the back of the array.
    void pop_front();

    //! Current size is flagged as 0. Elements are NOT destroyed.
    void clear();

    //! Set the current size to 0 and the reserve to new_size
    void resize(uint32 new_size);

  private:
    //! Exteneds the total size of the array (current + reserve) by a factor of 2
    void extend();

    void init(const vector_p&);

  private:
    //Data members
    T* m_data;
    uint32 m_arraySize;
    uint32 m_currentSize;
  };


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::vector_p()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class T>
  vector_p<T>::vector_p() 
    : m_data(nullptr)
    , m_arraySize(DG_CONTAINER_DEFAULT_SIZE)
    , m_currentSize(0)
  {
    m_data = static_cast<T*>(malloc(m_arraySize * sizeof(T)));

    if (m_data == nullptr)
    {
      throw std::bad_alloc;
    }

  }	//End: vector_p::vector_p()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::vector_p()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class T>
  vector_p<T>::vector_p(uint32 a_size)
    : m_data(nullptr)
    , m_currentSize(0)
    , m_arraySize(a_size)
  {
    assert(a_size != 0);

    //Initialise pointers
    m_data = static_cast<T*>(malloc(m_arraySize * sizeof(T)));

    if (m_data == nullptr)
    {
      throw std::bad_alloc;
    }

  }	//End: vector_p::vector_p()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::~vector_p()
  //--------------------------------------------------------------------------------
  //		Destructor
  //--------------------------------------------------------------------------------
  template<class T>
  vector_p<T>::~vector_p()
  {
    //Free memory
    free(m_data);

  }	//End: vector_p::~vector_p()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::init()
  //--------------------------------------------------------------------------------
  //		Initialise array to another vector_p
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::init(const vector_p& a_other)
  {
    T * tempPtr = static_cast<T*>(realloc(a_.m_arraySize * sizeof(T)));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc;
    }

    m_data = tempPtr;

    //Set sizes
    m_arraySize = a_other.m_arraySize;
    m_currentSize = a_other.m_currentSize;

    memcpy(m_data, a_other.m_data, a_other.m_currentSize * sizeof(T));

  }	//End: vector_p<T>::init()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::vector_p()
  //--------------------------------------------------------------------------------
  //		Copy constructor
  //--------------------------------------------------------------------------------
  template<class T>
  vector_p<T>::vector_p(const vector_p& other) : m_data(nullptr)
  {
    init(other);

  }	//End: vector_p::vector_p()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::operator=()
  //--------------------------------------------------------------------------------
  //		Assignment
  //--------------------------------------------------------------------------------
  template<class T>
  vector_p<T>& vector_p<T>::operator=(const vector_p& other)
  {
    if (this == &other)
      return *this;

    init(other);

    return *this;
  }	//End: vector_p::operator=()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::CopyAll()
  //--------------------------------------------------------------------------------
  //		Copies entire array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::copy_all(const vector_p<T>& a_other)
  {
    if (m_arraySize != a_other.m_arraySize)
      resize(a_other.m_arraySize);

    memcpy(m_data, a_other.m_data, m_arraySize * sizeof(T));

  }	//End: vector_p<T>::CopyAll()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::at()
  //--------------------------------------------------------------------------------
  //		Accessor with range check
  //--------------------------------------------------------------------------------
  template<class T>
  T& vector_p<T>::at(uint32 index)
  {
    if (index >= m_currentSize)
      throw std::out_of_range("vector_p: range error");

    return m_data[index];

  }	//End: vector_p<T>::at()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::at()
  //--------------------------------------------------------------------------------
  //		const accessor with range check
  //--------------------------------------------------------------------------------
  template<class T>
  const T& vector_p<T>::at(uint32 index) const
  {
    if (index >= m_currentSize)
      throw std::out_of_range("vector_p: range error");

    return m_data[index];

  }	//End: vector_p<T>::at()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::push_back()
  //--------------------------------------------------------------------------------
  //		Add element to the back of the array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::push_back(const T& a_item)
  {
    //Range check
    if (m_currentSize == m_arraySize)
    {
      extend();
    }

    //Set element
    memcpy(&m_data[m_currentSize], &a_item, sizeof(T));

    //increment current size
    ++m_currentSize;

  }	//End: vector_p<T>::push_back()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::pop_back
  //--------------------------------------------------------------------------------
  //		Pop an element from the back of the array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::pop_back()
  {
    //Range check
    if (m_currentSize == 0)
      return;

    //Deincrement current size
    --m_currentSize;

  }	//End: vector_p<T>::pop_back()



  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::push_front()
  //--------------------------------------------------------------------------------
  //		Add element to the front of the array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::push_front(const T& a_item)
  {
    //Range check
    if (m_currentSize == m_arraySize)
    {
      extend();
    }

    memmove(&m_data[1], &m_data[0], m_currentSize * sizeof(T));
    memcpy(&m_data[0], &a_item, sizeof(T));

    //increment current size
    ++m_currentSize;

  }	//End: vector_p<T>::push_back()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::pop_front
  //--------------------------------------------------------------------------------
  //		Pop an element from the front of the array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::pop_front()
  {
    //Range check
    if (m_currentSize == 0)
      return;

    memmove(&m_data[0], &m_data[1], (m_currentSize - 1) * sizeof(T));

    //Deincrement current size
    --m_currentSize;

  }	//End: vector_p<T>::pop_front()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::clear()
  //--------------------------------------------------------------------------------
  //		Clear array
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::clear()
  {
    //Set current size to 0
    m_currentSize = 0;

  }	//End: vector_p::clear()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::resize()
  //--------------------------------------------------------------------------------
  //		Resize array, erases all m_data before resize.
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::resize(uint32 a_size)
  {
    assert(a_size != 0);

    T * tempPtr = static_cast<T*>(realloc(m_data, a_size * sizeof(T)));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc;
    }

    m_data = tempPtr;

    //Set sizes
    m_arraySize = a_size;
    if (a_size < m_currentSize)
    {
      m_currentSize = a_size;
    }

  }	//End: vector_p::resize()


  //--------------------------------------------------------------------------------
  //	@	vector_p<T>::extend()
  //--------------------------------------------------------------------------------
  //		Extend the array by a factor of 2, keeps all m_data.
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_p<T>::extend()
  {
    //Calculate new size 
    uint32 new_size = m_arraySize << 1;

    if (new_size < m_arraySize)
    {
      throw std::overflow_error("m_arraySize");
    }

    T * tempPtr = static_cast<T*>(realloc(m_data, a_size * sizeof(T)));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc;
    }

    m_data = tempPtr;

    //Set sizes
    m_arraySize = a_size;
    if (a_size < m_currentSize)
    {
      m_currentSize = a_size;
    }

  }	//End: vector_p::extend()



  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------


  //--------------------------------------------------------------------------------
  //	@	find()
  //--------------------------------------------------------------------------------
  //		Find a value in the list, returns reference
  //--------------------------------------------------------------------------------
  template<class T>
  T* find(vector_p<T>& container, const T& val)
  {
    for (uint32 i = 0; i < container.size(); ++i)
    {
      if (container[i] == val)
        return &container[i];
    }

    return nullptr;

  }	//End find()


  //--------------------------------------------------------------------------------
  //	@	fill()
  //--------------------------------------------------------------------------------
  //		Fill array with value
  //--------------------------------------------------------------------------------
  template<class T>
  void fill(vector_p<T>& container, const T& val)
  {
    for (uint32 i = 0; i < container.size(); ++i)
    {
      container[i] = val;
    }

  }	//End: fill()

};
#endif