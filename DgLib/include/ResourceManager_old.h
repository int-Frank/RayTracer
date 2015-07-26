#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <mutex>
#include <atomic>

#include "utility.h"

#define RESOURCEMANAGER_DEFAULT_ARRAY_SIZE 64

namespace Dg
{
  template<typename>
  class ResourceManager;

  class ResourceBase;

  class ResourceKey
  {
    friend class ResourceBase;
    ResourceKey(){}
  };

  class ResourceBase
  {
  public:

    ResourceBase() { m_inUse = true; }
    virtual ~ResourceBase() {}

    virtual Dg_Result Init(const void * a_data) {}

    virtual void Update() {};

    bool IsInUse()
    {
      return m_inUse;
    }

    void SetUse(bool a_bool)
    {
      m_inUse = a_bool;
    }

    //! Hands ownership of this object to the Resource manager.
    template<typename InstanceType>
    Dg_Result Register()
    {
      return ResourceManager<InstanceType>::Register(this, ResourceKey());
    }

  private:

    //Disallow Copy operations
    ResourceBase(const ResourceBase&);
    ResourceBase& operator=(const ResourceBase&);

  private:

    std::atomic_bool m_inUse;
  };


  //A separate thread should be polling Update()
  template<typename>
  class ResourceManager
  {
  public:

    static void Update()
    {
      m_mutex.lock();
      for (int i = m_currentSize - 1; i >= 0; i--)
      {
        if (!m_data[i]->IsInUse())
        {
          delete m_data[i];
          memcpy(&m_data[i], &m_data[i + 1], (m_currentSize - index - 1) * sizeof(ResourceBase*));
          m_currentSize--;
        }
        else
        {
          m_data.Update();
        }
      }
      m_mutex.unlock();
    }

    static Dg_Result Register(ResourceBase* a_ptr, ResourceKey)
    {
      m_mutex.lock();

      for (int i = 0; i < m_currentSize; i++)
      {
        if (m_data[i] == a_ptr)
        {
          return DgR_Duplicate;
        }
      }

      if (m_currentSize == m_arraySize)
      {
        m_arraySize <<= 1;
        ResourceBase * newrb = (ResourceBase*)malloc(ResourceBase[m_arraySize] * sizeof(ResourceBase*));
        memcpy(newrb, m_data, m_currentSize * sizeof(ResourceBase*));
        free(m_data);
        m_data = newrb;
      }

      m_data[m_currentSize] = a_ptr;
      m_currentSize++;

      m_mutex.unlock();
      return DgR_Success;
    }

  private:

    static std::mutex m_mutex;
    static ResourceBase** m_data;
    uint32_t m_arraySize;
    uint32_t m_currentSize;
  };

  template<typename T>
  std::mutex ResourceManager<T>::m_mutex;

  template<typename T>
  ResourceBase** ResourceManager<T>::m_data(nullptr);

  template<typename T>
  uint32_t ResourceManager<T>::m_arraySize(RESOURCEMANAGER_DEFAULT_ARRAY_SIZE);

  template<typename T>
  uint32_t ResourceManager<T>::m_currentSize(0);

}

#endif