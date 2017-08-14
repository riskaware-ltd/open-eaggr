//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file KmlFileMatcher.cpp
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

#include "KmlFileMatcher.hpp"

#include <sstream>
#include <cstring>
#include <algorithm>

namespace EAGGR
{
  namespace TestUtilities
  {
    KmlFileMatcher::KmlFileMatcher()
        : m_lastErrorMessage("")
    {
    }

    bool KmlFileMatcher::Compare(std::istream& a_expectedStream, std::istream& a_actualStream)
    {
      bool filesMatch = true;
      m_lastErrorMessage = "";

      std::string expectedLine;
      std::string actualLine;

      // Read the first line of the expected stream
      std::getline(a_expectedStream, expectedLine);

      unsigned int lineNumber = 1;

      // Continue while the expected stream has more data
      while (!a_expectedStream.eof())
      {
        // Read a line from the actual stream
        std::getline(a_actualStream, actualLine);

        // If the actual stream is at the end while the first stream has more data
        if (a_actualStream.eof())
        {
          m_lastErrorMessage = "Actual stream finished while expected stream has further data.";
          filesMatch = false;
          break;
        }

        // Remove the end of line characters (could be different on different platforms)
        RemoveNewLineChars(actualLine);
        RemoveNewLineChars(expectedLine);

        // Compare the lines from the two streams
        if (expectedLine.compare(actualLine) != 0)
        {
          // Create the error message
          std::stringstream failureMessage;
          failureMessage << "Failed on lineNumber " << lineNumber;
          m_lastErrorMessage = failureMessage.str();
          filesMatch = false;
          break;
        }

        // Read the next line from the expected stream
        std::getline(a_expectedStream, expectedLine);
        ++lineNumber;
      }

      // Only perform further checks if the files are still marked as matching
      if (filesMatch)
      {
        // Once the expected stream is finished check that the actual stream is also finished
        std::getline(a_actualStream, actualLine);
        if (!a_actualStream.eof())
        {
          m_lastErrorMessage = "Expected stream finished while actual stream has further data.";
          filesMatch = false;
        }
      }

      return filesMatch;
    }

    std::string KmlFileMatcher::GetLastErrorMessage() const
    {
      return m_lastErrorMessage;
    }

    /// Removes \n and \r characters from the end of a string
    void KmlFileMatcher::RemoveNewLineChars(std::string& str)
    {
      str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
      str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    }
  }
}
