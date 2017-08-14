//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Utilities
//
//------------------------------------------------------
/// @file StringConversion.cpp
/// 
/// Provides functions to help with extracting numeric
/// values from a string.
///
/// This file is part of OpenEAGGR.
///
/// OpenEAGGR is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// OpenEAGGR is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Lesser General Public License for more details.
///
/// A copy of the GNU Lesser General Public License is available in COPYING.LESSER
/// or can be found at <http://www.gnu.org/licenses/>.
//------------------------------------------------------

#include <climits>
#include <cstdlib>
#include <errno.h>

#include "Src/Utilities/StringConversion.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace Utilities
  {
    namespace StringConversion
    {
      static const unsigned short BASE_10 = 10U;

      ConversionResult StringToBase10UnsignedShort(
          const std::string & a_string,
          unsigned short & a_output)
      {
        errno = 0;
        char * pNextChar;

        const unsigned long value = std::strtoul(a_string.c_str(), &pNextChar, BASE_10);

        if ((errno == ERANGE && value == LONG_MAX) || value > USHRT_MAX)
        {
          // Overflow
          return CONVERSION_OUT_OF_RANGE;
        }
        if (a_string.size() == 0 || *pNextChar != '\0')
        {
          // Inconvertible
          return CONVERSION_INCONVERTIBLE;
        }

        // Success
        a_output = static_cast<unsigned short>(value);
        return CONVERSION_SUCCESSFUL;
      }

      ConversionResult StringToBase10Short(const std::string & a_string, short & a_output)
      {
        errno = 0;
        char * pNextChar;

        const long value = std::strtol(a_string.c_str(), &pNextChar, BASE_10);

        if ((errno == ERANGE && value == LONG_MAX) || value > SHRT_MAX)
        {
          // Overflow
          return CONVERSION_OUT_OF_RANGE;
        }
        if ((errno == ERANGE && value == LONG_MIN) || value < SHRT_MIN)
        {
          // Underflow
          return CONVERSION_OUT_OF_RANGE;
        }
        if (a_string.size() == 0 || *pNextChar != '\0')
        {
          // Inconvertible
          return CONVERSION_INCONVERTIBLE;
        }

        // Success
        a_output = static_cast<short>(value);
        return CONVERSION_SUCCESSFUL;
      }
    }
  }
}

