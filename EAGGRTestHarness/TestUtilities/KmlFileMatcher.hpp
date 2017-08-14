//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file KmlFileMatcher.hpp
/// 
/// Implements the EAGGR::TestUtilities::KmlFileMatcher class for use in KML testing
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

#include <istream>

namespace EAGGR
{
  namespace TestUtilities
  {
    /// Provides comparison between kml output for use in test classes
    class KmlFileMatcher
    {
      public:
        /// Constructor
        KmlFileMatcher();

        /// Compares the contents of two streams
        /// @param a_expectedStream The expected stream of data
        /// @param a_actualStream The actual stream of data
        /// @return true if the streams have identical contents; false otherwise
        bool Compare(std::istream& a_expectedStream, std::istream& a_actualStream);

        /// @return The error resulting from the most recent call to Compare
        std::string GetLastErrorMessage() const;

      private:
        std::string m_lastErrorMessage;

        static void RemoveNewLineChars(std::string& str);
    };
  }
}
