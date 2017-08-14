//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Cell
//
//------------------------------------------------------
/// @file HierarchicalCell.cpp
/// 
/// Implements the EAGGR::Model::Cell::HierarchicalCell class.
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

#include "HierarchicalCell.hpp"
#include "Src/Utilities/StringConversion.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::StringConversion;

namespace EAGGR
{
  namespace Model
  {
    namespace Cell
    {
      HierarchicalCell::HierarchicalCell(
          const unsigned short a_faceIndex,
          const std::vector<unsigned short>& a_cellIndices,
          const unsigned short a_maximumFaceIndex,
          const unsigned short a_maximumCellIndex)
          :
              m_faceIndex(a_faceIndex),
              m_resolution(a_cellIndices.size()),
              m_cellIndices(),
              m_maximumCellIndex(a_maximumCellIndex),
              m_orientation(Grid::STANDARD)
      {
        // Check face index is valid
        if (m_faceIndex > a_maximumFaceIndex)
        {
          std::stringstream stream;
          stream << "Face index, '" << m_faceIndex << "', exceeds maximum (maximum = "
              << a_maximumFaceIndex << ")";
          throw EAGGRException(stream.str());
        }

        if (m_resolution > m_MAX_RESOLUTION_LEVEL)
        {
          std::stringstream stream;
          stream << "Unable to create cell at resolution " << m_resolution
              << " as it is greater than the upper limit (" << m_MAX_RESOLUTION_LEVEL << ").";
          throw EAGGRException(stream.str());
        }

        // Loop over the supplied cell indices and ensure that none are invalid
        for (unsigned int index = 0; index < m_resolution; ++index)
        {
          if (a_cellIndices[index] > m_maximumCellIndex)
          {
            std::stringstream stream;
            stream << "Cell index, '" << a_cellIndices[index] << "', exceeds maximum (maximum = "
                << m_maximumCellIndex << ")";
            throw EAGGRException(stream.str());
          }
        }

        m_cellIndices = a_cellIndices;
      }

      HierarchicalCell::HierarchicalCell(
          const DggsCellId& a_cellId,
          const unsigned short a_maximumFaceIndex,
          const unsigned short a_maximumCellIndex)
          : m_faceIndex(0U), // Set face index and resolution to defaults
          m_resolution(0U), // They will be updated inside the constructor
          m_cellIndices(), m_maximumCellIndex(a_maximumCellIndex), m_orientation(Grid::STANDARD)
      {
        // Check cell ID contains at least the face index
        if (a_cellId.length() < m_FACE_INDEX_LENGTH)
        {
          std::stringstream stream;
          stream << "Invalid cell ID, '" << a_cellId << "', must contain at least the face index";
          throw EAGGRException(stream.str());
        }

        // Face index is the first two characters of the string
        std::string faceIndexString = a_cellId.substr(0, m_FACE_INDEX_LENGTH);
        if (StringToBase10UnsignedShort(faceIndexString, m_faceIndex) != CONVERSION_SUCCESSFUL)
        {
          std::stringstream stream;
          stream << "Invalid face index, '" << faceIndexString << "', must be positive integer";
          throw EAGGRException(stream.str());
        }

        // Check face index is valid
        if (m_faceIndex > a_maximumFaceIndex)
        {
          std::stringstream stream;
          stream << "Face index, '" << faceIndexString << "', exceeds maximum (maximum = "
              << a_maximumFaceIndex << ")";
          throw EAGGRException(stream.str());
        }

        // Check resolution is within range
        m_resolution = a_cellId.length() - m_FACE_INDEX_LENGTH;

        if (m_resolution > m_MAX_RESOLUTION_LEVEL)
        {
          std::stringstream stream;
          stream << "Resolution " << m_resolution << " is greater than the upper limit ("
              << m_MAX_RESOLUTION_LEVEL << ").";
          throw EAGGRException(stream.str());
        }

        for (unsigned short resolutionLevel = 0; resolutionLevel < m_resolution; ++resolutionLevel)
        {
          const std::string cellIndexString = a_cellId.substr(
              resolutionLevel + m_FACE_INDEX_LENGTH,
              1);

          // Convert cell index to a number
          unsigned short cellIndex;
          if (StringToBase10UnsignedShort(cellIndexString, cellIndex) != CONVERSION_SUCCESSFUL)
          {
            std::stringstream stream;
            stream << "Invalid cell index value, '" << faceIndexString
                << "', must be positive integer";
            throw EAGGRException(stream.str());
          }

          // Check cell index value is within range
          if (cellIndex > m_maximumCellIndex)
          {
            std::stringstream stream;
            stream << "Cell index, '" << cellIndexString << "', exceeds maximum (maximum = "
                << m_maximumCellIndex << ")";
            throw EAGGRException(stream.str());
          }

          m_cellIndices.push_back(cellIndex);
        }
      }

      DggsCellId HierarchicalCell::GetCellId() const
      {
        std::stringstream cellId;

        // Write the face index
        cellId << std::setw(m_FACE_INDEX_LENGTH) << std::setfill('0') << m_faceIndex;

        if (m_resolution > 0)
        {
          // Write out the index for each resolution level
          for (unsigned short index = 0; index < m_resolution; ++index)
          {
            cellId << m_cellIndices[index];
          }
        }

        return cellId.str();
      }

      unsigned short HierarchicalCell::GetFaceIndex() const
      {
        return m_faceIndex;
      }

      unsigned short HierarchicalCell::GetResolution() const
      {
        return m_resolution;
      }

      std::vector<unsigned short> HierarchicalCell::GetCellIndices() const
      {
        return m_cellIndices;
      }

      unsigned short HierarchicalCell::GetCellIndex(const unsigned int a_resolutionLevel) const
      {
        if (a_resolutionLevel < 1)
        {
          std::stringstream stream;
          stream << "Minimum resolution allowed is 1 (attempted to use " << a_resolutionLevel
              << ")";
          throw EAGGRException(stream.str());
        }

        if (a_resolutionLevel > m_resolution)
        {
          std::stringstream stream;
          stream << "Maximum resolution allowed is " << m_resolution << " (attempted to use "
              << a_resolutionLevel << ")";
          throw EAGGRException(stream.str());
        }

        // Resolution is 1-based but the array is 0 based
        return m_cellIndices[a_resolutionLevel - 1];
      }

      Grid::ShapeOrientation HierarchicalCell::GetOrientation() const
      {
        return m_orientation;
      }

      void HierarchicalCell::SetOrientation(const Grid::ShapeOrientation a_orientation)
      {
        m_orientation = a_orientation;
      }

      CellLocation HierarchicalCell::GetCellLocation() const
      {
        // Hierarchical cells are always contained on a single face
        return FACE;
      }
    }
  }
}
