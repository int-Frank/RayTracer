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
  //! Reference counted pointer to a resource. It essentially wraps access to the
  //! ResourceManager. Avoid excessive copying as this Registers and Deregisters 
  //! Resources in the ResourceManger.
  class hResource
  {
    friend class ResourceManager;

  public:

    hResource() : m_resource(nullptr) {}
    ~hResource();
    hResource(hResource const & a_other);
    hResource & operator=(hResource const & a_other);

    //! Conversion operator
    Resource * operator->();

    //! Conversion operator
    Resource & operator*();

  private:

    Resource * m_resource;
  };
}

#endif