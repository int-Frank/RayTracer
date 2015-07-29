//! @file Resource.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: Resource

#ifndef RESOURCE_H
#define RESOURCE_H

#include "utility.h"
#include "ResourceKey.h"

namespace Dg
{
  //! Base class for all Resources
  class Resource
  {
  public:

    Resource(RKey a_key) : m_key(a_key) {}

    virtual ~Resource() = 0;

    virtual bool IsInitialised() = 0;

    //! The key is used to init the object.
    virtual Dg_Result Init() = 0;
    virtual Dg_Result DeInit() = 0;

    RKey GetKey() const { return m_key; }

  private:
    RKey m_key;
  };
}

#endif