//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file Aperture3HexagonGrid.cpp
///
/// Implements the EAGGR::Model::Grid::OffsetGrid::Aperture3HexagonGrid class.
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

#include <cmath>
#include <stdlib.h>
#include <sstream>

#include "Aperture3HexagonGrid.hpp"
#include "Src/Utilities/RadianMacros.hpp"
#include "Src/Utilities/Maths.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Grid
    {
      namespace OffsetGrid
      {
        unsigned short Aperture3HexagonGrid::GetResolutionFromAccuracy(
            const double a_accuracy) const
        {
          if (a_accuracy <= 0)
          {
            throw EAGGR::EAGGRException("Accuracy value must be > 0");
          }

          // If the accuracy region is bigger than the first resolution hexagon
          if (a_accuracy > 2.0 / 3.0)
          {
            return 0;
          }

          // Determine the resolution value that exactly matches this accuracy
          static const double RESOLUTION_1_AREA = 2.0 / 3.0; // Resolution 1 hexagon is 2/3 of the face area
          const double inverseAccuracy = RESOLUTION_1_AREA / a_accuracy;

          // Use a log to the base of the aperture.
          // Add 1 to resolution as accuracy of 2/3 (resolution1Area) is resolution 1 not 0.
          const double resolutionForAccuracy = (log(inverseAccuracy) / log(m_APERTURE)) + 1.0;

          const double ceilResolution = ceil(resolutionForAccuracy);
          const double floorResolution = floor(resolutionForAccuracy);

          // Find the accuracies represented by the integer resolutions bounding this resolution value
          const double lowerAccuracyBound = GetAccuracyFromResolution(ceilResolution);
          const double upperAccuracyBound = GetAccuracyFromResolution(floorResolution);

          // Determine which of the integer resolutions most closely matches the requested accuracy
          const double lowerBoundDelta = a_accuracy - lowerAccuracyBound;
          const double upperBoundDelta = upperAccuracyBound - a_accuracy;

          if (upperBoundDelta > lowerBoundDelta)
          {
            return ceilResolution;
          }
          else
          {
            return floorResolution;
          }
        }

        double Aperture3HexagonGrid::GetAccuracyFromResolution(
            const unsigned short a_resolution) const
        {
          if (a_resolution == 0)
          {
            return 1.0;
          }
          else
          {
            return ((2.0 / 3.0) / pow(m_APERTURE, a_resolution - 1));
          }
        }

        void Aperture3HexagonGrid::GetRowAndColumn(
            const unsigned short a_resolution,
            const FaceCoordinate a_locationOnFace,
            long &a_rowId,
            long &a_columnId) const
        {
          // Resolution 0 means the whole face so column and row will always be zero
          /// @todo Ensure what is meant by resolution 0 is the same throughout the code.
          if (a_resolution == 0U)
          {
            a_columnId = 0L;
            a_rowId = 0L;

            return;
          }

          // Equations and variable names are based on:
          // http://www.redblobgames.com/grids/hexagons/#pixel-to-hex

          // Get x and y coordinates on the face
          const double x = a_locationOnFace.GetXOffset();
          const double y = a_locationOnFace.GetYOffset();

          // Get edge length of hexagonal cells based on the resolution
          const double size = GetCellEdgeLengthFromResolution(a_resolution);

          // Orientation of the grid rotates between resolution levels
          const bool isPointyTopGrid = ((a_resolution & 1U) == 0U);

          // Calculate the fractional values for q and r (the axial coordinates)
          double q, r;
          if (isPointyTopGrid)
          {
            q = (x * sqrt(3.0) / 3.0 - y / 3.0) / size;
            r = y * (2.0 / 3.0) / size;
          }
          else
          {
            q = x * (2.0 / 3.0) / size;
            r = (-x / 3.0 + (sqrt(3.0) / 3.0) * y) / size;
          }

          // Round to nearest cube coordinates (using axial coordinates as inputs)
          const CubeCoordinate cubeCoord = RoundToNearestCubeCoordinate(q, (-1.0 * q) - r, r);

          // Convert cube coordinates to offset coordinates
          if (isPointyTopGrid)
          {
            // Use odd-r offset coordinate system for "pointy top" grids
            a_columnId = cubeCoord.m_x + (cubeCoord.m_z - (cubeCoord.m_z & 1L)) / 2L;
            a_rowId = cubeCoord.m_z;
          }
          else
          {
            // Use odd-q offset coordinate system for "flat top" grids
            a_columnId = cubeCoord.m_x;
            a_rowId = cubeCoord.m_z + (cubeCoord.m_x - (cubeCoord.m_x & 1L)) / 2L;
          }
        }

        void Aperture3HexagonGrid::GetFaceOffset(
            const Cell::OffsetCell & a_cell,
            double &a_xOffset,
            double &a_yOffset) const
        {
          unsigned short resolution = a_cell.GetResolution();

          // Resolution 0 means the whole face so the point will always be in the middle of the face
          if (resolution == 0U)
          {
            a_xOffset = 0.0;
            a_yOffset = 0.0;

            return;
          }

          // Equations and variable names are based on:
          // http://www.redblobgames.com/grids/hexagons/#hex-to-pixel

          // Get edge length of hexagonal cells based on the resolution
          const double size = GetCellEdgeLengthFromResolution(resolution);

          // Orientation of the grid rotates between resolution levels
          const bool isPointyTopGrid = ((resolution & 1U) == 0U);

          long columnId = a_cell.GetColumn();
          long rowId = a_cell.GetRow();

          if (isPointyTopGrid)
          {
            // Use odd-r offset coordinate system for "pointy top" grids
            a_xOffset = size * sqrt(3.0) * (columnId + 0.5 * (rowId & 1L));
            a_yOffset = size * (3.0 / 2.0) * rowId;
          }
          else
          {
            // Use odd-q offset coordinate system for "flat top" grids
            a_xOffset = size * (3.0 / 2.0) * columnId;
            a_yOffset = size * sqrt(3.0) * (rowId + 0.5 * (columnId & 1L));
          }
        }

        void Aperture3HexagonGrid::GetParents(
            const Cell::OffsetCell & a_cell,
            std::vector<Grid::OffsetCoordinate>& a_parents) const
        {
          a_parents.clear();

          double faceCoordX;
          double faceCoordY;
          GetFaceOffset(a_cell, faceCoordX, faceCoordY);

          // Determine the unit vector components in directions 10, 130 and 250 degrees
          const double UNIT_VEC_X_10_DEGREES = cos(DEGREES_IN_RAD(10.0));
          const double UNIT_VEC_Y_10_DEGREES = sin(DEGREES_IN_RAD(10.0));
          const double UNIT_VEC_X_130_DEGREES = cos(DEGREES_IN_RAD(130.0));
          const double UNIT_VEC_Y_130_DEGREES = sin(DEGREES_IN_RAD(130.0));
          const double UNIT_VEC_X_250_DEGREES = cos(DEGREES_IN_RAD(250.0));
          const double UNIT_VEC_Y_250_DEGREES = sin(DEGREES_IN_RAD(250.0));

          // Dimension of the cell is proportional to sqrt(accuracy)
          // Division by 10 ensures that it is a small offset within the cell
          const double offsetDistance = sqrt(GetAccuracyFromResolution(a_cell.GetResolution()))
              / 10.0;

          // Determine the coordinates of the centre of the supplied cell offset in each of the 3 directions
          const double faceCoord10DegX = offsetDistance * UNIT_VEC_X_10_DEGREES;
          const double faceCoord10DegY = offsetDistance * UNIT_VEC_Y_10_DEGREES;
          const double faceCoord130DegX = offsetDistance * UNIT_VEC_X_130_DEGREES;
          const double faceCoord130DegY = offsetDistance * UNIT_VEC_Y_130_DEGREES;
          const double faceCoord250DegX = offsetDistance * UNIT_VEC_X_250_DEGREES;
          const double faceCoord250DegY = offsetDistance * UNIT_VEC_Y_250_DEGREES;

          const double accuracy = GetAccuracyFromResolution(a_cell.GetResolution() - 1U);

          // Find the first parent
          long firstParentRow;
          long firstParentColumn;
          GetRowAndColumn(
              a_cell.GetResolution() - 1U,
              FaceCoordinate(
                  a_cell.GetFaceIndex(),
                  faceCoordX + faceCoord10DegX,
                  faceCoordY + faceCoord10DegY,
                  accuracy),
              firstParentRow,
              firstParentColumn);

          OffsetCoordinate parent =
          { firstParentRow, firstParentColumn };
          a_parents.push_back(parent);

          // Find the second parent
          long row;
          long column;
          GetRowAndColumn(
              a_cell.GetResolution() - 1U,
              FaceCoordinate(
                  a_cell.GetFaceIndex(),
                  faceCoordX + faceCoord130DegX,
                  faceCoordY + faceCoord130DegY,
                  accuracy),
              row,
              column);

          // If first and second parents are the same then the cell only has 1 parent
          if (row != firstParentRow || column != firstParentColumn)
          {
            parent =
            { row, column};
            a_parents.push_back(parent);

            // Find the third parent
            GetRowAndColumn(
                a_cell.GetResolution() - 1U,
                FaceCoordinate(a_cell.GetFaceIndex(), faceCoordX + faceCoord250DegX, faceCoordY + faceCoord250DegY, accuracy),
                row,
                column);

            parent =
            { row, column};
            a_parents.push_back(parent);
          }
        }

        unsigned short Aperture3HexagonGrid::GetNumChildren() const
        {
          return 7;
        }

        unsigned short Aperture3HexagonGrid::GetMaximumCellIndex() const
        {
          // Cell index must be less than the number of child cells for the grid
          return GetNumChildren() - 1U;
        }

        void Aperture3HexagonGrid::GetChildren(
            const Cell::OffsetCell & a_cell,
            std::vector<Grid::OffsetCoordinate>& a_children) const
        {
          a_children.clear();

          // Orientation of the grid rotates between resolution levels
          const bool horizontalOrientation = IsHorizontalOrientation(a_cell.GetResolution());

          const long rowId = a_cell.GetRow();
          const long columnId = a_cell.GetColumn();

          // Calculate the cell that is the centre child
          long baseChildRowId;
          long baseChildColumnId;

          if (horizontalOrientation)
          {
            baseChildRowId = (rowId * 2) + (columnId % 2);
            baseChildColumnId = ((columnId - (columnId % 2)) * 3 / 2) + (columnId % 2);
          }
          else
          {
            baseChildRowId = ((rowId - (rowId % 2)) * 3 / 2) + (rowId % 2);
            baseChildColumnId = (columnId * 2) + (rowId % 2);
          }

          OffsetCoordinate child0 =
          { baseChildRowId, baseChildColumnId };
          a_children.push_back(child0);

          // Add the surrounding cells.  Four are the same no matter what the orientation but two depend on orientation/row or column
          OffsetCoordinate child1 =
          { baseChildRowId - 1, baseChildColumnId };
          OffsetCoordinate child2 =
          { baseChildRowId, baseChildColumnId + 1 };
          OffsetCoordinate child3 =
          { baseChildRowId + 1, baseChildColumnId };
          OffsetCoordinate child4 =
          { baseChildRowId, baseChildColumnId - 1 };
          a_children.push_back(child1);
          a_children.push_back(child2);
          a_children.push_back(child3);
          a_children.push_back(child4);

          if (horizontalOrientation)
          {
            if (!(columnId & 1))
            {
              OffsetCoordinate child5 =
              { baseChildRowId + 1, baseChildColumnId - 1 };
              OffsetCoordinate child6 =
              { baseChildRowId - 1, baseChildColumnId - 1 };
              a_children.push_back(child5);
              a_children.push_back(child6);
            }
            else
            {
              OffsetCoordinate child5 =
              { baseChildRowId - 1, baseChildColumnId + 1 };
              OffsetCoordinate child6 =
              { baseChildRowId + 1, baseChildColumnId + 1 };
              a_children.push_back(child5);
              a_children.push_back(child6);
            }
          }
          else
          {
            if (!(rowId & 1))
            {
              OffsetCoordinate child5 =
              { baseChildRowId - 1, baseChildColumnId + 1 };
              OffsetCoordinate child6 =
              { baseChildRowId - 1, baseChildColumnId - 1 };
              a_children.push_back(child5);
              a_children.push_back(child6);
            }
            else
            {
              OffsetCoordinate child5 =
              { baseChildRowId + 1, baseChildColumnId + 1 };
              OffsetCoordinate child6 =
              { baseChildRowId + 1, baseChildColumnId - 1 };
              a_children.push_back(child5);
              a_children.push_back(child6);
            }
          }
        }

        unsigned short Aperture3HexagonGrid::GetAperture() const
        {
          return m_APERTURE;
        }

        void Aperture3HexagonGrid::GetVertices(
            const Cell::ICell & a_cell,
            std::list<FaceCoordinate>& a_cellVertices) const
        {
          a_cellVertices.clear();

          const Cell::OffsetCell* cell = dynamic_cast<const Cell::OffsetCell*>(&a_cell);

          if (cell == NULL)
          {
            throw EAGGRException(
                "Cell in Aperture3HexagonGrid GetCellVertices is not an offset cell.");
          }

          double xOffset;
          double yOffset;

          GetFaceOffset(*cell, xOffset, yOffset);

          Grid::ShapeOrientation shapeOrientation = GetOrientation(*cell);

          FaceCoordinate cellCentre(
              a_cell.GetFaceIndex(),
              xOffset,
              yOffset,
              GetAccuracyFromResolution(a_cell.GetResolution()));

          unsigned short resolution = a_cell.GetResolution();

          if (resolution == 0U)
          {
            // This is the whole face - return the face triangle coordinates
            a_cellVertices.push_back(
                FaceCoordinate(
                    cellCentre.GetFaceIndex(),
                    0.0,
                    sqrt(3.0) / 3.0,
                    cellCentre.GetAccuracy()));

            a_cellVertices.push_back(
                FaceCoordinate(
                    cellCentre.GetFaceIndex(),
                    -0.5,
                    -sqrt(3.0) / 6.0,
                    cellCentre.GetAccuracy()));

            a_cellVertices.push_back(
                FaceCoordinate(
                    cellCentre.GetFaceIndex(),
                    0.5,
                    -sqrt(3.0) / 6.0,
                    cellCentre.GetAccuracy()));
          }
          else
          {
            // This is a hexagonal cell

            // The vertices are at the distance of the edge length from the centre
            // since the hexagon is made up from equilateral triangles
            const double distanceToMove = GetCellEdgeLengthFromResolution(resolution);

            // Bearing of a vertex relative to x axis in anti-clockwise direction
            Utilities::Maths::Radians bearingToAVertex;

            switch (shapeOrientation)
            {
              case STANDARD:
                bearingToAVertex = DEGREES_IN_RAD(0.0);
                break;
              case ROTATED:
                bearingToAVertex = DEGREES_IN_RAD(30.0);
                break;
              default:
                std::stringstream stream;
                stream << "Invalid orientation " << shapeOrientation;
                throw EAGGRException(stream.str());
            }

            // Find the coordinates of each of the vertices at 60 degree steps from the base bearing
            for (unsigned int vertex = 0; vertex < 6; ++vertex)
            {
              double vertexOffsetX = distanceToMove * cos(bearingToAVertex);
              double vertexOffsetY = distanceToMove * sin(bearingToAVertex);

              a_cellVertices.push_back(
                  FaceCoordinate(
                      cellCentre.GetFaceIndex(),
                      cellCentre.GetXOffset() + vertexOffsetX,
                      cellCentre.GetYOffset() + vertexOffsetY,
                      cellCentre.GetAccuracy()));

              bearingToAVertex += DEGREES_IN_RAD(60.0);
            }
          }
        }

        ShapeOrientation Aperture3HexagonGrid::GetOrientation(const Cell::OffsetCell & a_cell) const
        {
          unsigned short resolution = a_cell.GetResolution();

          // Odd resolutions are standard orientation; rotated otherwise
          if (resolution & 1)
          {
            return STANDARD;
          }
          else
          {
            return ROTATED;
          }
        }

        double Aperture3HexagonGrid::GetCellEdgeLengthFromResolution(
            const unsigned short a_resolution) const
        {
          static const double EDGE_LENGTH_AT_FIRST_RES = 1.0 / 3.0;

          return (EDGE_LENGTH_AT_FIRST_RES
              / pow(sqrt(m_APERTURE), static_cast<double>(a_resolution - 1U)));
        }

        Aperture3HexagonGrid::CubeCoordinate Aperture3HexagonGrid::RoundToNearestCubeCoordinate(
            const double a_x,
            const double a_y,
            const double a_z) const
        {
          // Equations and variable names are based on:
          // http://www.redblobgames.com/grids/hexagons/#rounding

          double roundedX = round(a_x);
          double roundedY = round(a_y);
          double roundedZ = round(a_z);

          const double xDiff = std::abs(roundedX - a_x);
          const double yDiff = std::abs(roundedY - a_y);
          const double zDiff = std::abs(roundedZ - a_z);

          if (xDiff > yDiff && xDiff > zDiff)
          {
            roundedX = (-1.0 * roundedY) - roundedZ;
          }
          else if (yDiff > zDiff)
          {
            roundedY = (-1.0 * roundedX) - roundedZ;
          }
          else
          {
            roundedZ = (-1.0 * roundedX) - roundedY;
          }

          CubeCoordinate cubeCoord =
          { static_cast<long>(roundedX), static_cast<long>(roundedY), static_cast<long>(roundedZ) };

          return (cubeCoord);
        }

        bool Aperture3HexagonGrid::IsHorizontalOrientation(const unsigned short a_resolution) const
        {
          return a_resolution & 1;
        }
      }
    }
  }
}
