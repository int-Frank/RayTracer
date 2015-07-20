//! @file dg_pod_map_p.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: map

#ifndef DG_MAP_H
#define DG_MAP_H

#include <exception>
#include <assert.h>

#include "impl_container_common.h"

namespace Dg
{
  //! @ingroup Containers
  //!
  //! @class map
  //!
  //! Ordered map_pped list. 
  //!
  //! Assumed types are POD, so no construction / assignment operators called
  //!
  //! @author Frank B. Hart
  //! @date 2/5/2015
  template<typename U, typename T>
  class map
  {
    //Internal container which stores the m_data
    struct Container
    {
      U key;
      T item;
    };

  public:

    //! Constructor 
    //! If the constructor fails to allocate the map, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    map();

    //! Initialize map with to a capacity. 
    //! If the constructor fails to allocate the map, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    map(unsigned int);
    ~map();

    //! Copy constructor.
    map(map const &);

    //! Assigns new contents to the container, replacing its current content.
    map& operator= (map const &);

    //! Returns a reference to the \a i<SUP>th</SUP> element in the map. 
    //! This function does not perform a range check.
    T& operator[](unsigned int i)	{ return m_data[i].item; }

    //! Returns a const reference to the \a i<SUP>th</SUP> element in the map. 
    //! This function does not perform a range check.
    const T& operator[](unsigned int i) const { return m_data[i].item; }

    //! Return number of elements in the map.
    int size() const	{ return m_currentSize; }

    //! Returns whether the map is empty.
    bool empty() const	{ return m_currentSize == 0; }

    //! Returns number of elements the map can hold before resizing.
    int max_size() const	{ return m_arraySize; }

    //! Returns the key of the ith element in the map.
    //! This function does not perform a range check.
    U query_key(int i)	const { return m_data[i].key; }

    //! Searches the map for an element with a key equivalent to \a k.
    //! \return True if the element was found with \a index being set to the 
    //!         index of the element inthe map. False if not found with \a index
    //!         set to one lower to where \a k would be.
    //! \param lower Set a low bound to the search sublist.
    bool find(U k, int& index, int lower = 0) const;			 //Use binary search

    //! Searches the map for an element with a key equivalent to \a k.
    //! \return True if the element was found with \a index being set to the 
    //!         index of the element inthe map. False if not found with \a index
    //!         set to one lower to where \a k would be.
    //! \param lower Set a low bound to the search sublist.
    //! \param upper Set an upper bound to the search sublist.
    bool find(U k, int& index, int lower, int upper) const;	//Use binary search

    //! Extends the container by inserting new elements, effectively increasing 
    //! the container size by the number of elements inserted.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    //! \return False if key already exists in the map.
    bool insert(U k, T t);

    //! Set element with key \a k, with value \a t.
    //! \return True if key found.
    bool set(U k, T t);

    //! Removes the item in the map with key \a k.
    void erase(U k);

    //! Clear all items from the map, retains allocated memory.
    void clear();

    //! Resize the map. The content of the map is preserved up 
    //! to the lesser of the new and old sizes.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void resize(int);

    //! Clears the map, reallocates memory to the map.
    void reset();

  private:

    //! Doubles the memory allocated to the map. Retains all data.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void extend();

    void init(map const &);

  private:
    //Data members
    Container* m_data;

    int m_arraySize;
    int m_currentSize;
  };


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::map()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map<U, T>::map()
    : m_data(nullptr), m_arraySize(0), m_currentSize(0)
  {
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::map()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map<U, T>::map(unsigned int a_size)
    : m_data(nullptr), m_arraySize(0), m_currentSize(0)
  {
    assert(a_size > 0);

    T * tempPtr = static_cast<Container *>(malloc(sizeof(Container) * a_size));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc;
    }

    m_data = tempPtr;
    m_arraySize = a_size;
    m_currentSize = 0;

  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::~map()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map<U, T>::~map()
  {
    for (int i = 0; i < m_currentSize; ++i)
    {
      m_data[i].key.~U();
      m_data[i].item.~T();
    }

    free(m_data);

  }	//End: map::~map()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::init()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map<U, T>::init(map const & a_other)
  {
    clear();
    resize(a_other.m_arraySize);

    for (int i = 0; i < a_other.m_currentSize; ++i)
    {
      new (&m_data[i].key) U(a_other.m_data[i].key);
      new (&m_data[i].item) T(a_other.m_data[i].item);
    }

    m_currentSize = a_other.m_currentSize;

  }	//End: map::init()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::map()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map<U, T>::map(map const & a_other) :
    m_data(nullptr), m_arraySize(0), m_currentSize(0)
  {
    init(a_other);

  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::operator=()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map<U, T>& map<U, T>::operator=(map const & a_other)
  {
    if (this == &a_other)
      return *this;

    init(a_other);

    return *this;

  }	//End: map::operator=()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::resize()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map<U, T>::resize(int a_newSize)
  {
    assert(a_newSize > 0);

    Container * tempPtr = static_cast<Container *>(realloc(m_data, sizeof(Container) * a_newSize));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc();
    }

    m_data = tempPtr;
    m_arraySize = a_newSize;
    if (a_newSize < m_currentSize)
    {
      m_currentSize = a_newSize;
    }

  }	//End: map::resize()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::find()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  bool map<U, T>::find(U a_key, int& a_index, int a_lower) const
  {
    return find(a_key, a_index, a_lower, (m_currentSize - 1));

  }	//End: map::find()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::find()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  bool map<U, T>::find(U a_key, int& a_index, int a_lower, int a_upper) const
  {
    //Check bounds
    a_lower = (a_lower > 0) ? a_lower : 0;
    a_upper = (a_upper < m_currentSize - 1) ? a_upper : m_currentSize - 1;

    while (a_lower <= a_upper)
    {
      // calculate the midpoint for roughly equal partition
      a_index = ((a_upper + a_lower) >> 1);

      // determine which subarray to search
      if (m_data[a_index].key < a_key)
        // change min index to search upper subarray
        a_lower = a_index + 1;
      else if (m_data[a_index].key > a_key)
        // change max index to search lower subarray
        a_upper = a_index - 1;
      else
        // key found at index index
        return true;
    }

    //Set index closest (but lower) to key
    a_index = a_lower - 1;
    return false;

  }	//End: map::find()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::extend()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map<U, T>::extend()
  {
    //Calculate new size
    int new_size = (m_arraySize << 1);

    //overflow, map full
    if (new_size <= m_arraySize)
    {
      throw std::overflow_error("m_arraySize");
    }

    Container * tempPtr = static_cast<Container*>(realloc(m_data, sizeof(Container) * new_size));
    if (tempPtr == nullptr)
    {
      throw std::bad_alloc();
    }

    m_data = tempPtr;
    m_arraySize = new_size;

  }	//End: map::extend()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::insert()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  bool map<U, T>::insert(U a_key, T a_item)
  {
    //Find the index to insert to
    int index;
    if (find(a_key, index))
      return false;	//element already exists

    //Range check
    if (m_currentSize == m_arraySize)
      extend();

    //shift all RHS objects to the right by one.
    memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(Container));

    index++;

    //Construct new element.
    new (&m_data[index].key) U(a_key);
    new (&m_data[index].item) T(a_item);

    m_currentSize++;

    return true;

  }	//End: map::insert()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::erase()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map<U, T>::erase(U a_key)
  {
    //Find the index
    int index;
    if (!find(a_key, index))
    {
      return;
    }

    //Destroy element
    m_data[index].key.~U();
    m_data[index].item.~T();

    memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(Container));

    m_currentSize--;

  }	//End: map::erase()


  //--------------------------------------------------------------------------------
  //	@	map<U,T>::set()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  bool map<U, T>::set(U a_key, T a_item)
  {
    //Find the index to insert to
    int index;
    if (!find(a_key, index))
    {
      return false;	//element does not exist
    }

    m_data[index].item = a_item;

    return true;

  }	//End: map::set()


  //--------------------------------------------------------------------------------
  //	@	Dgmap_p<U,T>::reset()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map<U, T>::reset()
  {
    clear();
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: map::reset()


  //--------------------------------------------------------------------------------
  //	@	Dgmap_p<U,T>::clear()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map<U, T>::clear()
  {
    for (int i = 0; i < m_currentSize; ++i)
    {
      m_data[i].key.~U();
      m_data[i].item.~T();
    }

    m_currentSize = 0;

  }	//End: map::clear()
};

#endif