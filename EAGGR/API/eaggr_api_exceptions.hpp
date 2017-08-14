//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: C API
//
//------------------------------------------------------
/// @file eaggr_api_exceptions.hpp
///
/// Exception classes used internally by the API code.
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

#include <Src/EAGGRException.hpp>

namespace EAGGR
{
  namespace API
  {
    /// Exception thrown if null is returned by the malloc() or realloc() functions.
    class MemoryAllocationException: public EAGGRException
    {
      public:
        MemoryAllocationException(const std::string & a_message)
            : EAGGRException(a_message)
        {
        }
    };

    /// Exception thrown if a cell ID exceeds the maximum allowed length.
    class MaxCellIdLengthException: public EAGGRException
    {
      public:
        MaxCellIdLengthException(const std::string & a_message)
            : EAGGRException(a_message)
        {
        }
    };
  }
}
