//! @file ResourcePointer.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: hResource

#ifndef RESOURCEHANDLE_H
#define RESOURCEHANDLE_H

#include "ResourceManager.h"

namespace Dg
{
  //! Reference counted pointer to a resource.
  class hResource
  {
    friend class ResourceManager;

  public:

    hResource() : m_resource(nullptr) {}

    ~hResource() { ResourceManager::Instance()->DeregisterUser(m_rKey); }

    hResource(hResource const & a_other) : m_rKey(a_other.m_rKey)
      , m_resource(ResourceManager::Instance()->RegisterUser(a_other.m_rKey))
    {}

    hResource & operator=(hResource const & a_other)
    {
      ResourceManager::Instance()->DeregisterUser(m_rKey);
      m_rKey = a_other.m_rKey;
      m_resource = ResourceManager::Instance()->RegisterUser(m_rKey);

      return *this;
    }

    Resource * operator->()
    {
      return m_resource;
    }

    Resource & operator*()
    {
      return *m_resource;
    }

  private:

    Resource * m_resource;
    DgRKey m_rKey;

  };
}

#endif