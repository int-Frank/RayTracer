#ifndef SINGLETON_H
#define SINGLETON_H

namespace Dg
{
  template<typename T>
  class Singleton
  {
  public:

    static T * Instance() 
    {
      static T s_instance;
      return &s_instance;
    }
   
  protected:

    Singleton(); // Prevent construction
    Singleton(Singleton const &); // Prevent construction by copying
    Singleton & operator=(Singleton const &); // Prevent assignment
    virtual ~Singleton(); // Prevent unwanted destruction

  };


}

#endif