//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: EAGGR
//
//------------------------------------------------------
/// @file DGGSException.hpp
/// 
/// Implements the EAGGR::DGGSException class.
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

#include <iostream>
#include <stdexcept>

namespace EAGGR
{
  /// Exception thrown by the EAGGR library.
  class EAGGRException: public std::exception
  {
    public:
      /// Constructor.
      /// @param a_message The error message.
      EAGGRException(const std::string & a_message)
          : m_message("EAGGR Exception: " + a_message)
      {
      }

      /// Destructor
      ~EAGGRException() throw ()
      {
      }

      /// @return The error message.
      const char * what() const throw ()
      {
        return m_message.c_str();
      }

    private:
      const std::string m_message;
  };
}
