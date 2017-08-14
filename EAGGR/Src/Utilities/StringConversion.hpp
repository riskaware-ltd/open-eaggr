//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Utilities
//
//------------------------------------------------------
/// @file StringConversion.hpp
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

#pragma once

#include <string>

namespace EAGGR
{
  namespace Utilities
  {
    namespace StringConversion
    {
      /// Return type of the functions in this namespace. Indicates if the function
      /// ran successfully, or the reason why the function failed.
      typedef enum
      {
        CONVERSION_SUCCESSFUL, CONVERSION_OUT_OF_RANGE, CONVERSION_INCONVERTIBLE
      } ConversionResult;

      /// Converts a string to an unsigned short integer.
      /// @param a_string String containing a positive base-10 integer.
      /// @param a_output Converted value.
      /// @return Enumerated type indicating whether the function ran successfully.
      ConversionResult StringToBase10UnsignedShort(
          const std::string & a_string,
          unsigned short & a_output);

      /// Converts a string to a signed short integer.
      /// @param a_string String containing a positive or negative base-10 integer.
      /// @param a_output Converted value.
      /// @return Enumerated type indicating whether the function ran successfully.
      ConversionResult StringToBase10Short(const std::string & a_string, short & a_output);
    }
  }
}
;
