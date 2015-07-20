//================================================================================
// @ set_p.h
// 
// Class: set_p
//
// An ordered array. Elements are inserted in order. Uses contiguous memory to 
// store m_data, therefore is best used for very small m_data types (ie literals) 
// and types with cheap assignment operators.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef DG_SET_H
#define DG_SET_H

#include <exception>
#include <assert.h>

#include "impl_container_common.h"

namespace Dg
{
    //--------------------------------------------------------------------------------
    //	@	set_p<T>:	T: m_data type
    //--------------------------------------------------------------------------------
    template<class T>
    class set_p
    {
    public:
      //! Constructor 
      //! If the constructor fails to allocate the set_p, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      set_p();

      //! Initialize set_p with to a capacity. 
      //! If the constructor fails to allocate the set_p, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      set_p(unsigned int);
      ~set_p();

      //! Copy constructor.
      set_p(set_p const &);

      //! Assigns new contents to the container, replacing its current content.
      set_p& operator= (set_p const &);

      //! Returns a reference to the \a i<SUP>th</SUP> element in the set_p. 
      //! This function does not perform a range check.
      T& operator[](unsigned int i)	{ return m_data[i]; }

      //! Returns a const reference to the \a i<SUP>th</SUP> element in the set_p. 
      //! This function does not perform a range check.
      const T& operator[](unsigned int i) const { return m_data[i]; }

      //! Return number of elements in the set_p.
      int size() const	{ return m_currentSize; }

      //! Returns whether the set_p is empty.
      bool empty() const	{ return m_currentSize == 0; }

      //! Returns number of elements the set_p can hold before resizing.
      int max_size() const	{ return m_arraySize; }

      //! Searches the set for an element equivalent to \a t.
      //! \return True if the element was found with \a index being set_p to the 
      //!         index of the element in the set_p. False if not found with \a index
      //!         set_p to one lower to where \a t would be.
      //! \param lower Set a low bound to the search sublist.
      bool find(T const & t, int& index, int lower = 0) const;

      //! Searches the set_p for an element with a key equivalent to \a k.
      //! \return True if the element was found with \a index being set_p to the 
      //!         index of the element in the set_p. False if not found with \a index
      //!         set_p to one lower to where \a t would be.
      //! \param lower Set a low bound to the search sublist.
      //! \param upper Set an upper bound to the search sublist.
      bool find(T const & t, int& index, int lower, int upper) const;

      //! Extends the container by inserting new elements, effectively increasing 
      //! the container size by the number of elements inserted.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      void insert(T const & t);

      //! Extends the container by inserting new elements, effectively increasing 
      //! the container size by the number of elements inserted.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      //! \return False if t already exists in the set.
      bool insert_unique(T const & t);

      //! Removes a single element equal to t from the set.
      void erase(T const & t);

      //! Removes all elements equal to t from the set.
      void erase_all(T const & t);

      //! Clear all items from the set_p, retains allocated memory.
      void clear();

      //! Resize the set_p. The content of the set_p is preserved up 
      //! to the lesser of the new and old sizes.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      void resize(int);

      //! Clears the set_p, reallocates memory to the set_p.
      void reset();

    private:
      //! Doubles the memory allocated to the set_p. Retains all data.
      //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
      void extend();

      void init(const set_p&);

    private:
      //m_data members
      T* m_data;

      int m_arraySize;
      int m_currentSize;

    };


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::set_p()
    //--------------------------------------------------------------------------------
    //		Constructor
    //--------------------------------------------------------------------------------
    template<class T>
    set_p<T>::set_p()
      : m_data(nullptr)
      , m_arraySize(0)
      , m_currentSize(0)
    {
      resize(DG_CONTAINER_DEFAULT_SIZE);
    }	//End: set_p::set_p()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::set_p()
    //--------------------------------------------------------------------------------
    //		Constructor
    //--------------------------------------------------------------------------------
    template<class T>
    set_p<T>::set_p(unsigned a_newSize)
      : m_data(nullptr)
      , m_arraySize(0)
      , m_currentSize(0)
    {
      assert(a_newSize > 0);

      resize(a_newSize);

    }	//End: set_p::set_p()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::~set_p()
    //--------------------------------------------------------------------------------
    //		Destructor
    //--------------------------------------------------------------------------------
    template<class T>
    set_p<T>::~set_p()
    {
      //Free memory
      free( m_data );

    }	//End: set_p::~set_p()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::set_p()
    //--------------------------------------------------------------------------------
    //		Initialise set_p to another.
    //--------------------------------------------------------------------------------
    template<class T>
    void set_p<T>::init(const set_p& other)
    {
      //Resize lists
      int sze = (other.m_arraySize>0) ? other.m_arraySize : 1;
      resize(sze);

      memcpy(m_data, other.m_data, other.m_currentSize * sizeof(T));

      m_currentSize = other.m_currentSize;

    }	//End: set_p::init()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::set_p()
    //--------------------------------------------------------------------------------
    //		Copy constructor
    //--------------------------------------------------------------------------------
    template<class T>
    set_p<T>::set_p(const set_p& other) :
      m_data(nullptr), m_arraySize(0), m_currentSize(0)
    {
      init(other);

    }	//End: set_p::set_p()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::operator=()
    //--------------------------------------------------------------------------------
    //		Assignment
    //--------------------------------------------------------------------------------
    template<class T>
    set_p<T>& set_p<T>::operator=(const set_p& other)
    {
      if (this == &other)
        return *this;

      init(other);

      return *this;

    }	//End: set_p::operator=()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::resize()
    //--------------------------------------------------------------------------------
    //		Resize map
    //--------------------------------------------------------------------------------
    template<class T>
    void set_p<T>::resize(int a_newSize)
    {
      assert(a_newSize > 0);

      //Delete old m_data
      T * tempPtr = static_cast<T *>(realloc(m_data, sizeof(T) * a_newSize));

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

    }	//End: set_p::resize()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::find()
    //--------------------------------------------------------------------------------
    //		Find a value in the list, uses a binary search algorithm
    //--------------------------------------------------------------------------------
    template<class T>
    bool set_p<T>::find(T const & a_item, int& a_index, int a_lower) const
    {
      return find(a_item, a_index, a_lower, (m_currentSize - 1));

    }	//End: set_p::find()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::find()
    //--------------------------------------------------------------------------------
    //		Find a value in the list within a range, uses a binary search algorithm
    //--------------------------------------------------------------------------------
    template<class T>
    bool set_p<T>::find(T const & a_item, int& a_index, int a_lower, int a_upper) const
    {
      //Check bounds
      a_lower = (a_lower > 0) ? a_lower : 0;
      a_upper = (a_upper < m_currentSize - 1) ? a_upper : m_currentSize - 1;

      while (a_lower <= a_upper)
      {
        // calculate the midpoint for roughly equal partition
        a_index = ((a_upper + a_lower) >> 1);

        // determine which subarray to search
        if (m_data[a_index] < a_item)
          // change min index to search upper subarray
          a_lower = a_index + 1;
        else if (m_data[a_index] > a_item)
          // change max index to search lower subarray
          a_upper = a_index - 1;
        else
          // key found at index index
          return true;
      }

      //Set index closest (but lower) to key
      a_index = a_lower - 1;
      return false;

    }	//End: set_p::find()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::extend()
    //--------------------------------------------------------------------------------
    template<class T>
    void set_p<T>::extend()
    {
      //Calculate new size
      int new_size = (m_arraySize << 1);

      //overflow, map_p full
      if (new_size <= m_arraySize)
      {
        throw std::overflow_error("m_arraySize");
      }

      T * tempPtr = static_cast<T*>(realloc(m_data, sizeof(T) * new_size));
      if (tempPtr == nullptr)
      {
        throw std::bad_alloc();
      }

      m_data = tempPtr;
      m_arraySize = new_size;

    }	//End: set_p::extend()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::insert()
    //--------------------------------------------------------------------------------
    //		Insert an element into the list
    //--------------------------------------------------------------------------------
    template<class T>
    void set_p<T>::insert(T const & a_item)
    {
      //Find the index to insert to
      int index;
      find(a_item, index);

      //Range check
      if (m_currentSize == m_arraySize)
        extend();

      //shift all RHS objects to the right by one.
      memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

      index++;

      memcpy(&m_data[index], &a_item, sizeof(T));

      m_currentSize++;

      return true;

    }	//End: set_p::insert()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::insert_unique()
    //--------------------------------------------------------------------------------
    //		Insert an element into the list, only if it does not yet exist
    //--------------------------------------------------------------------------------
    template<class T>
    bool set_p<T>::insert_unique(const T& item)
    {
      //Find the index to insert to
      int index;
      if (find(item, index))
        return false;

      //Range check
      if (m_currentSize == m_arraySize)
        extend();

      //shift all RHS objects to the right by one.
      memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

      index++;

      memcpy(&m_data[index], &item, sizeof(T));

      m_currentSize++;

      return true;

    }	//End: set_p::insert_unique()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::erase()
    //--------------------------------------------------------------------------------
    //		Find and removes one of this element from the list.
    //--------------------------------------------------------------------------------
    template<class T>
    void set_p<T>::erase(const T& a_item)
    {
      //Find the index
      int index;
      if (!find(a_item, index))
        return;	//element not found

      //Remove elements
      memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

      //Adjust m_currentSize
      m_currentSize--;

    }	//End: set_p::erase()
    

    //--------------------------------------------------------------------------------
    //	@	set_p<T>::erase_all()
    //--------------------------------------------------------------------------------
    //		Find and removes all of this element from the list.
    //--------------------------------------------------------------------------------
    template<class T>
    void set_p<T>::erase_all(const T& a_item)
    {
      //Find the index
      int lower, upper;
      if (!find(a_item, lower))
        return false;	//element not found

      //Initial upper bounds
      upper = lower + 1;

      //Find lower bounds
      while (lower >= 0 && m_data[lower] == a_item)
        --lower;

      //Find upper bounds
      while (upper <= m_currentSize && m_data[upper] == a_item)
        ++upper;

      //Number of elements to remove
      lower++;
      int num = upper - lower;

      //Remove elements
      memmove(&m_data[lower], &m_data[lower + num], (m_currentSize - index - 1 - num) * sizeof(T));

      //Adjust m_currentSize
      m_currentSize -= num;

      return true;

    }	//End: set_p::erase_all()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::reset()
    //--------------------------------------------------------------------------------
    //		Reset size to 1
    //--------------------------------------------------------------------------------
    template<class T>
    void set_p<T>::reset()
    {
      clear();
      resize(DG_CONTAINER_DEFAULT_SIZE);

    }	//End: set_p::reset()


    //--------------------------------------------------------------------------------
    //	@	set_p<T>::clear()
    //--------------------------------------------------------------------------------
    //		Set the number of elements to zero
    //--------------------------------------------------------------------------------
    template<class T>
    void set_p<T>::clear()
    {
      m_currentSize = 0;

    }	//End: set_p::clear()
};

#endif