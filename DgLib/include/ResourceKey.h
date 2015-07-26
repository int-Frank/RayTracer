//! @file ResourceKey.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: DgRKey

#ifndef RESOURCEKEY_H
#define RESOURCEKEY_H

#include <stdint.h>

namespace Dg
{
  //! A key associated with Resources.
  //! A key is a uint32_t. The following information is encoded in the key:
  //!        Type             Unique tag
  //!     0000 0000 | 0000 0000 0000 0000 0000 0000
  class DgRKey
  {
  public:

    DgRKey() : m_value(0) {}
    DgRKey(uint32_t a_value) : m_value(a_value) {}
    DgRKey(uint32_t a_type, uint32_t a_tag) 
    {
      m_value = ((a_type << 24) | (a_tag & 0x00FFFFFF));
    }

    ~DgRKey();

    DgRKey(DgRKey const & a_other) : m_value(a_other.m_value) {}
    DgRKey & operator=(DgRKey const & a_other)
    {
      m_value = a_other.m_value;
      return *this;
    }

    bool operator==(DgRKey const & a_other) const { return m_value == a_other.m_value; }
    bool operator!=(DgRKey const & a_other) const { return m_value != a_other.m_value; }

    operator uint32_t() { return m_value; }

    //! Get the type of resource the key is associated with.
    uint32_t GetType() { return (m_value >> 24); }

    //! Get the unique tag of the resource the key is associated with.
    uint32_t GetTag() { return m_value & 0x00FFFFFF; }

    //! Is this a valid key?
    bool IsValid() { return m_value != 0; }

  private:

    uint32_t m_value;
  };
}

#endif