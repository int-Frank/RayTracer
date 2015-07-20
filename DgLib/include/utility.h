#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>


namespace Dg
{
  //! @ingroup utility_constants
  enum Dg_Result
  {
    DgR_Success = 0,
    DgR_Failure,
    DgR_Undefined,
    DgR_OutOfBounds,
    DgR_Duplicate
  };

  //! @ingroup utility_functions
  //!
  //! @brief Converts a string to a number.
  //!
  //! @param[out] t Output number.
  //! @param[in] s Input string.
  //! @param[in] f The base fo the number to read. (eg std::dec)
  //!
  //! @return True if a number was loaded into the output.
  template <typename Real>
  bool StringToNumber(Real& t,
    const std::string& s,
    std::ios_base& (*f)(std::ios_base&))
  {
    std::istringstream iss(s);
    return !(iss >> f >> t).fail();

  }	//End: StringToNumber()

}

#endif