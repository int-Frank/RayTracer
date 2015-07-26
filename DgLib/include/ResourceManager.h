//! @file ResourceManager.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: ResourceManager

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "dg_map_p.h"
#include "singleton.h"
#include "Resource.h"
#include "ResourceKey.h"
#include "utility.h"

namespace Dg
{
  class hResource;

  //! General resource manager.
  //! Use:
  //!       1) Register all resources with RegisterResource()
  //!       2) Request Resources with GetResource()
  class ResourceManager : public Singleton<ResourceManager>
  {
    friend class hResource;
    friend class Singleton < ResourceManager > ;

  public:

    enum Option
    {
      DEFAULT         = 0,
      AutoDeinit      = 1,  // DeInitialise a resource once number of registered users equals 0.
    };

  public:

    //! Set an option.
    void SetOptions(uint32_t);

    //! Check an option.
    bool CheckOption(Option);

    //! Register a new resource.
    //! @param a_key Associate a unique key with this resource
    //! @param a_file Associate a file with this resource
    //! @param a_init Initialise resource?
    template<typename ResourceType>
    Dg_Result RegisterResource(DgRKey a_key, bool a_init = false);

    //! Get a pointer to a resource. Will fail if the resouce has not been 
    //! successfully registed first with RegisterResource().
    Dg_Result GetResource(DgRKey, hResource &);

    //! Initialise a particular resource. 
    Dg_Result InitResource(DgRKey);

    //! Initialises all resources.
    Dg_Result InitAll();

    //! Deinitialise a particular resource. 
    void DeinitResource(DgRKey, bool a_force = false);

    //! Deinitialises all resources.
    void DeinitAll(bool a_force = false);

  private:

    ResourceManager() : m_options(DEFAULT){}
    ~ResourceManager();

    //! Only the hResource class should be calling this function.
    void DeregisterUser(DgRKey);

    //! Will initialised resource if not initialised.
    //! Only the hResource class should be calling this function.
    Resource * RegisterUser(DgRKey);

  private:

    struct ResourceContainer
    {
      Resource * m_resource;
      unsigned m_nUsers;
    };

    uint32_t m_options;
    Dg::map_p<DgRKey, ResourceContainer> m_resourceList;
  };


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::RegisterResource()
  //--------------------------------------------------------------------------------
  template <typename ResourceType>
  Dg_Result ResourceManager::RegisterResource(DgRKey a_key, 
                                              bool a_init)
  {
    if (!a_key.IsValid())
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
    rc.m_resource = new ResourceType(a_key);
    rc.m_resource.SetFile(a_file);

    if (a_init)
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
