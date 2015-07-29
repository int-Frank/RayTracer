//! @file ResourceManager.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: ResourceManager

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <stdint.h>

#include "dg_map_p.h"
#include "singleton.h"
#include "Resource.h"
#include "ResourceKey.h"
#include "utility.h"

namespace Dg
{
  class hResource;

  //! Options for individual resources
  enum class rOption
  {
    DEFAULT    = 0,
    AutoInit   = 1,  // Initialise the resource on registration
    AutoDeinit = 2   // Deinitialise a resource once number of registered users equals 0.
  };

  //! ResourceManger options
  enum class rmOption
  {
    DEFAULT    = 0,
  };

  //! General resource manager.
  //! Use:
  //!       1) Register all resources with RegisterResource()
  //!       2) Request Resources with GetResource()
  class ResourceManager : public Singleton<ResourceManager>
  {
    friend class hResource;
    friend class Singleton < ResourceManager > ;

  public:

    //! Set an option.
    void SetOptions(uint32_t);

    //! Check an option.
    bool CheckOption(rmOption);

    //! Register a new resource.
    //! @param a_key Associate a unique key with this resource
    //! @param a_file Associate a file with this resource
    //! @param a_options Options for this resource
    template<typename ResourceType>
    Dg_Result RegisterResource(RKey a_key, uint32_t a_options);

    //! Get a pointer to a resource. Will fail if the resouce has not been 
    //! successfully registed first with RegisterResource().
    Dg_Result GetResourceHandle(RKey, hResource &);

    //! Initialise a particular resource. 
    Dg_Result InitResource(RKey);

    //! Initialises all resources.
    Dg_Result InitAll();

    //! Deinitialise a particular resource. 
    void DeinitResource(RKey, bool a_force = false);

    //! Deinitialises all resources.
    void DeinitAll(bool a_force = false);

  private:

    ResourceManager() : m_options(static_cast<uint32_t>(rmOption::DEFAULT)){}
    ~ResourceManager();

    //! Only the hResource class should be calling this function.
    void DeregisterUser(RKey);

    //! Will initialised resource if not initialised.
    //! Only the hResource class should be calling this function.
    Resource * RegisterUser(RKey);

  private:

    struct ResourceContainer
    {
      Resource * m_resource;
      unsigned   m_nUsers;
      uint32_t   m_opts;
    };

    uint32_t m_options;
    Dg::map_p<RKey, ResourceContainer> m_resourceList;
  };


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::RegisterResource()
  //--------------------------------------------------------------------------------
  template <typename ResourceType>
  Dg_Result ResourceManager::RegisterResource(RKey a_key, 
                                              uint32_t a_options)
  {
    if (a_key != RKey_INVALID)
    {
      return DgR_Failure;
    }

    int index(0);
    if (m_resourceList.find(a_key, index))
    {
      return DgR_Duplicate;
    }

    ResourceContainer rc;
    rc.m_nUsers = 0;
    rc.m_opts = a_options;
    rc.m_resource = new ResourceType(a_key);

    if (a_options & static_cast<uint32_t>(rOption::AutoInit))
    {
      if (rc.m_resource->Init() != DgR_Success)
      {
        return DgR_Failure;
      }
    }

    m_resourceList.insert(a_key, rc);

    return DgR_Success;
  }
}// End: ResourceManager::RegisterResource()


#endif
