//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file OffsetCell.cpp
/// 
/// Implements the EAGGR::Model::OffsetCell class.
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

#include <cstdlib>
#include <iomanip>
#include <sstream>

#include "OffsetCell.hpp"
#include "Src/Utilities/StringConversion.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::StringConversion;

namespace EAGGR
{
  namespace Model
  {
    namespace Cell
    {
      OffsetCell::OffsetCell(
          const unsigned short a_faceIndex,
          const unsigned short a_resolution,
          const long a_rowCoordinate,
          const long a_columnCoordinate,
          const CellLocation a_cellLocation,
          const unsigned short a_maximumFaceIndex)
          :
              m_faceIndex(a_faceIndex),
              m_resolution(a_resolution),
              m_rowCoordinate(a_rowCoordinate),
              m_columnCoordinate(a_columnCoordinate),
              m_orientation(Grid::STANDARD),
              m_cellLocation(a_cellLocation)
      {
        if (a_faceIndex > a_maximumFaceIndex)
        {
          std::stringstream errorStream;
          errorStream << "Face index, '" << a_faceIndex << "', exceeds maximum (maximum = "
              << a_maximumFaceIndex << ")";
          throw EAGGRException(errorStream.str());
        }
        if (a_resolution > m_MAX_RESOLUTION_LEVEL)
        {
          std::stringstream stream;
          stream << "Resolution " << a_resolution << " is greater than the upper limit ("
              << m_MAX_RESOLUTION_LEVEL << ").";
          throw EAGGRException(stream.str());
        }
      }

      OffsetCell::OffsetCell(const DggsCellId& a_cellId, const unsigned short a_maximumFaceIndex)
          :
              m_faceIndex(0U), // Set to default values
              m_resolution(0U),
              m_rowCoordinate(0L),
              m_columnCoordinate(0L),
              m_orientation(Grid::STANDARD),
              m_cellLocation(UNKNOWN)
      {
        // Index to track the current location in the string
        short index = 0;

        // Face index is the first two characters of the string
        const std::string faceIndexString = a_cellId.substr(index, m_FACE_INDEX_LENGTH);
        index += m_FACE_INDEX_LENGTH;

        // Convert face index from string and check its value
        if (StringToBase10UnsignedShort(faceIndexString, m_faceIndex) != CONVERSION_SUCCESSFUL)
        {
          std::stringstream errorStream;
          errorStream << "Invalid face index, '" << faceIndexString
              << "', must be positive integer";
          throw EAGGRException(errorStream.str());
        }
        if (m_faceIndex > a_maximumFaceIndex)
        {
          std::stringstream errorStream;
          errorStream << "Face index, '" << faceIndexString << "', exceeds maximum (maximum = "
              << a_maximumFaceIndex << ")";
          throw EAGGRException(errorStream.str());
        }

        // Resolution is the next two characters of the string
        const std::string resolutionString = a_cellId.substr(index, m_RESOLUTION_LENGTH);
        index += m_RESOLUTION_LENGTH;

        // Convert resolution from string and check its value
        if (StringToBase10UnsignedShort(resolutionString, m_resolution) != CONVERSION_SUCCESSFUL)
        {
          std::stringstream errorStream;
          errorStream << "Invalid resolution, '" << faceIndexString
              << "', must be positive integer";
          throw EAGGRException(errorStream.str());
        }
        if (m_resolution > m_MAX_RESOLUTION_LEVEL)
        {
          std::stringstream stream;
          stream << "Resolution " << m_resolution << ", exceeds upper limit (limit = "
              << m_MAX_RESOLUTION_LEVEL << ").";
          throw EAGGRException(stream.str());
        }

        // Create a stream of the remaining characters of the cell id
        std::string coordinates = a_cellId.substr(index, a_cellId.size() - index);
        std::istringstream coordinateStream(coordinates);

        // Read the x and y coordinates from the stream
        char separator;
        coordinateStream >> m_rowCoordinate >> separator >> m_columnCoordinate;
        if (coordinateStream.fail())
        {
          std::stringstream errorStream;
          errorStream << "Invalid offset coordinates, '" << coordinates
              << "', must be two integer values separated by '" << m_SEPARATOR << "'";
          throw EAGGRException(errorStream.str());
        }
      }

      DggsCellId OffsetCell::GetCellId() const
      {
        std::stringstream cellId;

        // Write the face index
        cellId << std::setw(m_FACE_INDEX_LENGTH) << std::setfill('0') << m_faceIndex;

        // Write the resolution
        cellId << std::setw(m_RESOLUTION_LENGTH) << std::setfill('0') << m_resolution;

        // Write out the index for x coordinate
        cellId << m_rowCoordinate;

        // Write coordinate separator
        cellId << m_SEPARATOR;

        // Write out the index for y coordinate
        cellId << m_columnCoordinate;

        return cellId.str();
      }

      unsigned short OffsetCell::GetFaceIndex() const
      {
        return m_faceIndex;
      }

      unsigned short OffsetCell::GetResolution() const
      {
        return m_resolution;
      }

      long OffsetCell::GetRow() const
      {
        return m_rowCoordinate;
      }

      long OffsetCell::GetColumn() const
      {
        return m_columnCoordinate;
      }

      Grid::ShapeOrientation OffsetCell::GetOrientation() const
      {
        return m_orientation;
      }

      void OffsetCell::SetOrientation(const Grid::ShapeOrientation a_orientation)
      {
        m_orientation = a_orientation;
      }

      CellLocation OffsetCell::GetCellLocation() const
      {
        return m_cellLocation;
      }
    }
  }
}
