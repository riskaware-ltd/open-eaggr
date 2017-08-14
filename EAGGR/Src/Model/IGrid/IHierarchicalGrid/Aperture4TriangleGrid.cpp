//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Grid
//
//------------------------------------------------------
/// @file Aperture4TriangleGrid.cpp
///
/// Implements the EAGGR::Model::Grid::HierarchicalGrid::Aperture4TriangleGrid class.
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

#include <vector>
#include <string>
#include <sstream>

#include "Aperture4TriangleGrid.hpp"
#include "Src/Model/CartesianPoint.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace Model
  {
    namespace Grid
    {
      namespace HierarchicalGrid
      {
        unsigned short Aperture4TriangleGrid::GetResolutionFromAccuracy(
            const double a_accuracy) const
        {
          if (a_accuracy <= 0)
          {
            throw EAGGR::EAGGRException("Accuracy value must be > 0");
          }

          // If the accuracy region is bigger than the size of the face then we will put it at the lowest resolution
          if (a_accuracy >= 1.0)
          {
            return 0;
          }

          // Determine the resolution value that exactly matches this accuracy
          const double inverseAccuracy = 1.0 / a_accuracy;
          const double resolutionForAccuracy = log(inverseAccuracy) / log(m_APERTURE); // log to base 4

          // Find the accuracies represented by the integer resolutions bounding this resolution value
          const double lowerAccuracyBound = GetAccuracyFromResolution(ceil(resolutionForAccuracy));
          const double upperAccuracyBound = GetAccuracyFromResolution(floor(resolutionForAccuracy));

          // Determine which of the integer resolutions most closely matches the requested accuracy
          const double lowerBoundDelta = a_accuracy - lowerAccuracyBound;
          const double upperBoundDelta = upperAccuracyBound - a_accuracy;

          if (upperBoundDelta > lowerBoundDelta)
          {
            return ceil(resolutionForAccuracy);
          }
          else
          {
            return floor(resolutionForAccuracy);
          }
        }

        double Aperture4TriangleGrid::GetAccuracyFromResolution(
            const unsigned short a_resolution) const
        {
          return (1.0 / pow(m_APERTURE, a_resolution));
        }

        void Aperture4TriangleGrid::GetFacePartition(
            const CellPartition a_cellPartition,
            const short a_resolution,
            const FaceCoordinate a_locationOnFace,
            CellPartition* a_pSubCellPartition) const
        {
          // Get the width and height of the parent triangle
          const double cellSizeAtResolution = 1.0
              / pow(2.0, static_cast<double>(a_resolution) - 1.0);
          const double triangleWidth = cellSizeAtResolution;
          const double triangleHeight = (m_HEIGHT_TO_EDGE_RATIO) * triangleWidth;

          short shapeOrientation;
          switch (a_cellPartition.GetPartitionOrientation())
          {
            case STANDARD:
              shapeOrientation = 1;
              break;
            case ROTATED:
              shapeOrientation = -1;
              break;
            default:
              std::stringstream stream;
              stream << "Invalid shape orientation " << a_cellPartition.GetPartitionOrientation();
              throw EAGGRException(stream.str());
          }

          // Calculate the centre of sub-triangles
          std::vector < CartesianPoint > subTriangleCentres;

          CartesianPoint shapeCentre = a_cellPartition.GetPartitionCentre();

          // Middle triangle (same as parent)
          const CartesianPoint middleTriangleCentre(shapeCentre.GetX(), shapeCentre.GetY());
          subTriangleCentres.push_back(middleTriangleCentre);

          // Top triangle
          const CartesianPoint topTriangleCentre(
              shapeCentre.GetX(),
              shapeCentre.GetY() + (shapeOrientation * triangleHeight / 3.0));
          subTriangleCentres.push_back(topTriangleCentre);

          // Left triangle
          const CartesianPoint leftTriangleCentre(
              shapeCentre.GetX() - (0.25 * triangleWidth),
              shapeCentre.GetY() - (shapeOrientation * triangleHeight / 6.0));
          subTriangleCentres.push_back(leftTriangleCentre);

          // Right triangle
          const CartesianPoint rightTriangleCentre(
              shapeCentre.GetX() + (0.25 * triangleWidth),
              shapeCentre.GetY() - (shapeOrientation * triangleHeight / 6.0));
          subTriangleCentres.push_back(rightTriangleCentre);

          // Create a Cartesian point from the face coordinate (used for distance calculations)
          const CartesianPoint location(
              a_locationOnFace.GetXOffset(),
              a_locationOnFace.GetYOffset());

          // Find the closest sub-triangle centre
          unsigned short closestSubTriangle = 0U;
          double distanceToClosestCentre = HUGE_VAL;
          for (std::vector<CartesianPoint>::size_type triangleIndex = 0;
              triangleIndex != subTriangleCentres.size(); triangleIndex++)
          {
            const double distance = subTriangleCentres[triangleIndex].GetDistanceToPoint(location);
            if (distance < distanceToClosestCentre)
            {
              closestSubTriangle = triangleIndex;
              distanceToClosestCentre = distance;
            }
          }

          // Store the details of the closest sub-triangle
          a_pSubCellPartition->SetId(closestSubTriangle);
          a_pSubCellPartition->SetPartitionOrientation(a_cellPartition.GetPartitionOrientation());
          a_pSubCellPartition->SetPartitionCentre(subTriangleCentres[closestSubTriangle]);

          // Middle sub-triangle (index 0) is upside-down
          if (a_pSubCellPartition->GetId() == 0U)
          {
            switch (a_pSubCellPartition->GetPartitionOrientation())
            {
              case STANDARD:
                a_pSubCellPartition->SetPartitionOrientation(ROTATED);
                break;
              case ROTATED:
                a_pSubCellPartition->SetPartitionOrientation(STANDARD);
                break;
              default:
                std::stringstream stream;
                stream << "Invalid shape orientation "
                    << a_pSubCellPartition->GetPartitionOrientation();
                throw EAGGRException(stream.str());
            }
          }
        }

        void Aperture4TriangleGrid::GetFaceOffset(
            const Cell::HierarchicalCell & a_cell,
            double &a_xOffset,
            double &a_yOffset) const
        {
          double xOffset = 0.0;
          double yOffset = 0.0;

          // Initial triangle height and width
          double triangleWidth = 1.0;
          double triangleHeight = m_HEIGHT_TO_EDGE_RATIO * triangleWidth;

          // Orientation 1 for standard; -1 for inverted
          short orientation = 1;

          for (unsigned short resolutionLevel = 0; resolutionLevel < a_cell.GetResolution();
              ++resolutionLevel)
          {
            const unsigned short cellIndex = a_cell.GetCellIndex(resolutionLevel + 1);
            switch (cellIndex)
            {
              case 0:
                // Child cell centre is the same as the parent but orientation changes
                orientation *= -1;
                break;
              case 1:
                // Child X coordinate is the same as the parent; y coordinate is offset by 1/3 * triangle height
                yOffset += orientation * triangleHeight / 3.0;
                break;
              case 2:
                // Child X coordinate is a quarter of the triangle width to the left
                // Child Y coordinate is 1/6 of the triangle height below the parent
                xOffset -= triangleWidth / 4.0;
                yOffset -= orientation * triangleHeight / 6.0;
                break;
              case 3:
                // Child X coordinate is a quarter of the triangle width to the right
                // Child Y coordinate is 1/6 of the triangle height below the parent
                xOffset += triangleWidth / 4.0;
                yOffset -= orientation * triangleHeight / 6.0;
                break;
              default:
                std::stringstream stream;
                stream << "Invalid partition index " << cellIndex;
                throw EAGGRException(stream.str());
            }

            // Width and height scale by 2 on each resolution level
            triangleWidth /= 2.0;
            triangleHeight /= 2.0;
          }

          a_xOffset = xOffset;
          a_yOffset = yOffset;
        }

        unsigned short Aperture4TriangleGrid::GetNumChildren() const
        {
          // All cells have four children in this grid
          return 4;
        }

        unsigned short Aperture4TriangleGrid::GetMaximumCellIndex() const
        {
          // Cell index must be less than the number of child cells for the grid
          return GetNumChildren() - 1U;
        }

        unsigned short Aperture4TriangleGrid::GetAperture() const
        {
          return m_APERTURE;
        }

        void Aperture4TriangleGrid::GetVertices(
            const Cell::ICell & a_cell,
            std::list<FaceCoordinate>& a_cellVertices) const
        {
          a_cellVertices.clear();

          const Cell::HierarchicalCell* cell = dynamic_cast<const Cell::HierarchicalCell*>(&a_cell);

          if (cell == NULL)
          {
            throw EAGGRException(
                "Cell in Aperture4TriangleGrid GetCellVertices is not a hierarchical cell.");
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

          // Calculate the width and height of the triangle at the correct resolution
          double triangleWidth = 1.0 / pow(2.0, a_cell.GetResolution());
          double triangleHeight = m_HEIGHT_TO_EDGE_RATIO * triangleWidth;

          double orientation;

          switch (shapeOrientation)
          {
            case STANDARD:
              orientation = 1.0;
              break;
            case ROTATED:
              orientation = -1.0;
              break;
            default:
              std::stringstream stream;
              stream << "Invalid orientation " << shapeOrientation;
              throw EAGGRException(stream.str());
          }

          // Point of the triangle
          xOffset = cellCentre.GetXOffset();
          yOffset = cellCentre.GetYOffset() + (orientation * triangleHeight * 2.0 / 3.0);
          a_cellVertices.push_back(
              FaceCoordinate(
                  cellCentre.GetFaceIndex(),
                  xOffset,
                  yOffset,
                  cellCentre.GetAccuracy()));

          // Left base of the triangle
          xOffset = cellCentre.GetXOffset() - (triangleWidth / 2.0);
          yOffset = cellCentre.GetYOffset() - (orientation * triangleHeight / 3.0);
          a_cellVertices.push_back(
              FaceCoordinate(
                  cellCentre.GetFaceIndex(),
                  xOffset,
                  yOffset,
                  cellCentre.GetAccuracy()));

          // Right base of the triangle
          xOffset = cellCentre.GetXOffset() + (triangleWidth / 2.0);
          yOffset = cellCentre.GetYOffset() - (orientation * triangleHeight / 3.0);
          a_cellVertices.push_back(
              FaceCoordinate(
                  cellCentre.GetFaceIndex(),
                  xOffset,
                  yOffset,
                  cellCentre.GetAccuracy()));
        }

        ShapeOrientation Aperture4TriangleGrid::GetOrientation(
            const Cell::HierarchicalCell & a_cell) const
        {
          ShapeOrientation orientation = STANDARD;

          for (unsigned short resolutionLevel = 0; resolutionLevel < a_cell.GetResolution();
              ++resolutionLevel)
          {
            const unsigned short cellIndex = a_cell.GetCellIndex(resolutionLevel + 1);
            switch (cellIndex)
            {
              case 0:
                // Child cell orientation changes
                switch (orientation)
                {
                  case STANDARD:
                    orientation = ROTATED;
                    break;
                  case ROTATED:
                    orientation = STANDARD;
                    break;
                  default:
                    std::stringstream stream;
                    stream << "Invalid orientation " << orientation;
                    throw EAGGRException(stream.str());
                }
                break;
              case 1:
              case 2:
              case 3:
                // All other cell partitions do not change the orientation
                break;
              default:
                std::stringstream stream;
                stream << "Invalid cell partition " << cellIndex << " at resolution level "
                    << (resolutionLevel + 1);
                throw EAGGRException(stream.str());
            }
          }

          return orientation;
        }
      }
    }
  }
}
