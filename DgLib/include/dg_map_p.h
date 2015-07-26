//! @file dg_pod_map_p.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: map_p

#ifndef DG_MAP_P_H
#define DG_MAP_P_H

#include <exception>
#include <assert.h>

#include "impl_container_common.h"

namespace Dg
{
  //! @ingroup Containers
  //!
  //! @class map_p
  //!
  //! Ordered map_pped list. 
  //!
  //! Assumed types are POD, so no construction / assignment operators called
  //!
  //! @author Frank B. Hart
  //! @date 2/5/2015
  template<typename U, typename T>
  class map_p
  {
    //Internal container which stores the m_data
    struct Container
    {
      U key;
      T item;
    };

  public:

    //! Constructor 
    //! If the constructor fails to allocate the map_p, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    map_p();

    //! Initialize map_p with to a capacity. 
    //! If the constructor fails to allocate the map_p, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    map_p(unsigned int);
    ~map_p();

    //! Copy constructor.
    map_p(map_p const &);

    //! Assigns new contents to the container, replacing its current content.
    map_p& operator= (map_p const &);

    //! Returns a reference to the \a i<SUP>th</SUP> element in the map_p. 
    //! This function does not perform a range check.
    T& operator[](unsigned int i)	{ return m_data[i].item; }

    //! Returns a const reference to the \a i<SUP>th</SUP> element in the map_p. 
    //! This function does not perform a range check.
    const T& operator[](unsigned int i) const { return m_data[i].item; }

    //! Return number of elements in the map_p.
    int size() const	{ return m_currentSize; }

    //! Returns whether the map_p is empty.
    bool empty() const	{ return m_currentSize == 0; }

    //! Returns number of elements the map_p can hold before resizing.
    int max_size() const	{ return m_arraySize; }

    //! Returns the key of the ith element in the map_p.
    //! This function does not perform a range check.
    U query_key(int i)	const { return m_data[i].key; }

    //! Searches the map_p for an element with a key equivalent to \a k.
    //! \return True if the element was found with \a index being set to the 
    //!         index of the element inthe map_p. False if not found with \a index
    //!         set to one lower to where \a k would be.
    //! \param lower Set a low bound to the search sublist.
    bool find(U k, int& index, int lower = 0) const;			 //Use binary search

    //! Searches the map_p for an element with a key equivalent to \a k.
    //! \return True if the element was found with \a index being set to the 
    //!         index of the element inthe map_p. False if not found with \a index
    //!         set to one lower to where \a k would be.
    //! \param lower Set a low bound to the search sublist.
    //! \param upper Set an upper bound to the search sublist.
    bool find(U k, int& index, int lower, int upper) const;	//Use binary search

    //! Extends the container by inserting new elements, effectively increasing 
    //! the container size by the number of elements inserted.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    //! \return False if key already exists in the map_p.
    bool insert(U k, T const & t);

    //! Set element with key \a k, with value \a t.
    //! \return True if key found.
    bool set(U k, T t);

    //! Removes the item in the map_p with key \a k.
    void erase(U k);

    //! Removes the item in the map_p at position \a i.
    void erase_at_position(int);

    //! Clear all items from the map_p, retains allocated memory.
    void clear();

    //! Resize the map_p. The content of the map_p is preserved up 
    //! to the lesser of the new and old sizes.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void resize(int);

    //! Clears the map_p, reallocates memory to the map_p.
    void reset();

  private:

    //! Doubles the memory allocated to the map_p. Retains all data.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void extend();

    void init(map_p const &);

  private:
    //Data members
    Container* m_data;

    int m_arraySize;
    int m_currentSize;
  };


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::map_p()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map_p<U, T>::map_p()
    : m_data(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: map_p::map_p()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::map_p()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map_p<U, T>::map_p(unsigned int a_size)
    : m_data(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    assert(a_size > 0);

    Container * tempPtr = static_cast<Container *>(malloc(sizeof(Container) * a_size));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc;
    }

    m_data = tempPtr;
    m_arraySize = a_size;
    m_currentSize = 0;

  }	//End: map_p::map_p()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::~map_p()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map_p<U, T>::~map_p()
  {
    free(m_data);

  }	//End: map_p::~map_p()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::init()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map_p<U, T>::init(map_p const & a_other)
  {
    resize(a_other.m_arraySize);

    memcpy(m_data, a_other.m_data, a_other.m_currentSize * sizeof(Container));

    m_currentSize = a_other.m_currentSize;

  }	//End: map_p::init()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::map_p()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map_p<U, T>::map_p(map_p const & a_other) :
    m_data(nullptr), m_arraySize(0), m_currentSize(0)
  {
    init(a_other);

  }	//End: map_p::map_p()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::operator=()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  map_p<U, T>& map_p<U, T>::operator=(map_p const & a_other)
  {
    if (this == &a_other)
      return *this;

    init(a_other);

    return *this;

  }	//End: map_p::operator=()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::resize()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map_p<U, T>::resize(int a_newSize)
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

  }	//End: map_p::resize()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::find()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  bool map_p<U, T>::find(U a_key, int& a_index, int a_lower) const
  {
    return find(a_key, a_index, a_lower, (m_currentSize - 1));

  }	//End: map_p::find()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::find()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  bool map_p<U, T>::find(U a_key, int& a_index, int a_lower, int a_upper) const
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

  }	//End: map_p::find()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::extend()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map_p<U, T>::extend()
  {
    //Calculate new size
    int new_size = (m_arraySize << 1);

    //overflow, map_p full
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

  }	//End: map_p::extend()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::insert()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  bool map_p<U, T>::insert(U a_key, T const & a_item)
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

    memcpy(&m_data[index].key, &a_key, sizeof(a_key));
    memcpy(&m_data[index].item, &a_item, sizeof(a_item));

    m_currentSize++;

    return true;

  }	//End: map_p::insert()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::erase()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map_p<U, T>::erase(U a_key)
  {
    //Find the index
    int index;
    if (!find(a_key, index))
    {
      return;
    }

    memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(Container));

    m_currentSize--;

  }	//End: map_p::erase()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::erase_at_position()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map_p<U, T>::erase_at_position(int a_i)
  {
    if (a_i > 0 && a_i < m_currentSize)
    {

      memmove(&m_data[a_i], &m_data[a_i + 1], (m_currentSize - a_i - 1) * sizeof(Container));

      m_currentSize--;
    }

  }	//End: map_p::erase_at_position()


  //--------------------------------------------------------------------------------
  //	@	map_p<U,T>::set()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  bool map_p<U, T>::set(U a_key, T a_item)
  {
    //Find the index to insert to
    int index;
    if (!find(a_key, index))
    {
      return false;	//element does not exist
    }
        
    memcpy(&m_data[index].item, &a_item, sizeof(a_item));

    return true;

  }	//End: map_p::set()


  //--------------------------------------------------------------------------------
  //	@	Dgmap_p<U,T>::reset()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map_p<U, T>::reset()
  {
    clear();
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: map_p::reset()


  //--------------------------------------------------------------------------------
  //	@	Dgmap_p<U,T>::clear()
  //--------------------------------------------------------------------------------
  template<typename U, typename T>
  void map_p<U, T>::clear()
  {
    m_currentSize = 0;

  }	//End: map_p::clear()
};

#endif