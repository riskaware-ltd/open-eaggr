//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: C API
//
//------------------------------------------------------
/// @file eaggr_api_funcs.cpp
///
/// Contains functions used by only the C API interface.
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

#include <memory>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <set>

#include "API/eaggr_api_funcs.hpp"

#include "API/eaggr_api_exceptions.hpp"
#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/Model/DGGS.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace API
  {
    static void UpdateDggsShapesMemoryAllocation(
        DGGS_Shape ** a_pDggsShapes,
        const unsigned short a_maxShapeIndex);

    static void GetDggsLinestringFromWgs84Linestring(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84Linestring * a_pWgs84Linestring,
        DGGS_Linestring * a_pDggsLinestring);

    void ConvertWgs84PointAndAddToDggsShapes(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84AccuracyPoint * a_pWgs84Point,
        const unsigned short a_shapeIndex,
        DGGS_Shape ** a_pDggsShapes)
    {
      // Allocate memory for the shape
      UpdateDggsShapesMemoryAllocation(a_pDggsShapes, a_shapeIndex);

      // Convert to spherical coordinates (expected by the DGGS class)
      const LatLong::SphericalAccuracyPoint sphericalPoint = a_pConverter->ConvertWGS84ToSphere(
          *a_pWgs84Point);

      // Convert the point to a cell
      std::unique_ptr < Model::Cell::ICell > pCell =
          static_cast<Model::DGGS *>(a_handle)->ConvertLatLongPointToCell(sphericalPoint);

      // Check cell ID does not exceed the maximum length
      const Model::Cell::DggsCellId cellId = pCell->GetCellId();
      CheckCellIdLength(cellId.c_str());

      // Store the cell data in the output array
      (*a_pDggsShapes)[a_shapeIndex].m_type = DGGS_CELL;
      static_cast<void>(strncpy(
          (*a_pDggsShapes)[a_shapeIndex].m_data.m_cell,
          cellId.c_str(),
          EAGGR_MAX_CELL_STRING_LENGTH));

      // Determine the cells location in relation to the edges of the polyhedron
      switch (pCell->GetCellLocation())
      {
        case Model::Cell::FACE:
          (*a_pDggsShapes)[a_shapeIndex].m_location = DGGS_ONE_FACE;
          break;
        case Model::Cell::EDGE:
          (*a_pDggsShapes)[a_shapeIndex].m_location = DGGS_TWO_FACES;
          break;
        case Model::Cell::VERTEX:
          (*a_pDggsShapes)[a_shapeIndex].m_location = DGGS_MANY_FACES;
          break;
        default:
          std::stringstream stream;
          stream << "Invalid cell location '" << pCell->GetCellLocation() << "'";
          throw EAGGR::EAGGRException(stream.str());
      }
    }

    void ConvertWgs84LinestringAndAddToDggsShapes(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84Linestring * a_pWgs84Linestring,
        const unsigned short a_shapeIndex,
        DGGS_Shape ** a_pDggsShapes)
    {
      // Allocate memory for the shape
      UpdateDggsShapesMemoryAllocation(a_pDggsShapes, a_shapeIndex);

      // Set the shape type
      (*a_pDggsShapes)[a_shapeIndex].m_type = DGGS_LINESTRING;

      // Convert linestring to a DGGS linestring
      GetDggsLinestringFromWgs84Linestring(
          a_handle,
          a_pConverter,
          a_pWgs84Linestring,
          &((*a_pDggsShapes)[a_shapeIndex].m_data.m_linestring));

      // Cell location only applies to cells, and not linestrings
      (*a_pDggsShapes)[a_shapeIndex].m_location = DGGS_NO_LOCATION;
    }

    void ConvertWgs84PolygonAndAddToDggsShapes(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84Polygon * a_pWgs84Polygon,
        const unsigned short a_shapeIndex,
        DGGS_Shape ** a_pDggsShapes)
    {
      // Allocate memory for the shape
      UpdateDggsShapesMemoryAllocation(a_pDggsShapes, a_shapeIndex);

      // Set the shape type
      (*a_pDggsShapes)[a_shapeIndex].m_type = DGGS_POLYGON;

      // Convert outer ring to a DGGS linestring
      GetDggsLinestringFromWgs84Linestring(
          a_handle,
          a_pConverter,
          a_pWgs84Polygon->GetOuterRing(),
          &((*a_pDggsShapes)[a_shapeIndex].m_data.m_polygon.m_outerRing));

      // Set the number of inner rings in the polygon
      const unsigned short noOfInnerRings = a_pWgs84Polygon->GetNumberOfInnerRings();
      (*a_pDggsShapes)[a_shapeIndex].m_data.m_polygon.m_noOfInnerRings = noOfInnerRings;

      // Allocate memory for the linestrings of the inner rings
      (*a_pDggsShapes)[a_shapeIndex].m_data.m_polygon.m_innerRings =
          static_cast<DGGS_Linestring *>(malloc(noOfInnerRings * sizeof(DGGS_Linestring)));

      // Check memory allocation was successful
      if ((*a_pDggsShapes)[a_shapeIndex].m_data.m_polygon.m_innerRings == NULL)
      {
        throw MemoryAllocationException(
            "Failed to allocate memory for the inner rings of the DGGS polygon");
      }

      // Loop through and convert each inner ring of the polygon
      for (unsigned short ringIndex = 0U; ringIndex < noOfInnerRings; ringIndex++)
      {
        // Convert inner ring to a DGGS linestring
        GetDggsLinestringFromWgs84Linestring(
            a_handle,
            a_pConverter,
            a_pWgs84Polygon->GetInnerRing(ringIndex),
            &((*a_pDggsShapes)[a_shapeIndex].m_data.m_polygon.m_innerRings[ringIndex]));
      }

      // Cell location only applies to cells, and not polygons
      (*a_pDggsShapes)[a_shapeIndex].m_location = DGGS_NO_LOCATION;
    }

    void CheckCellIdLength(const DGGS_Cell a_cell)
    {
      unsigned short cellIdLength = strlen(a_cell) + sizeof(TERMINATING_CHAR);
      if (cellIdLength > EAGGR_MAX_CELL_STRING_LENGTH)
      {
        std::stringstream stream;
        stream << "Cell ID exceeds maximum length (by "
            << cellIdLength - EAGGR_MAX_CELL_STRING_LENGTH
            << " characters) - reduce required accuracy";
        throw MaxCellIdLengthException(stream.str());
      }
    }

    bool AreCellsDifferent(std::vector<std::unique_ptr<Model::Cell::ICell> >& a_cells)
    {
      if (a_cells.size() == 0)
      {
        throw EAGGRException("No cells provided to determine uniqueness");
      }

      std::string firstCellId = a_cells[0]->GetCellId();

      bool areDifferent = false;

      for (unsigned short cellIndex = 1U; cellIndex < a_cells.size(); ++cellIndex)
      {
        std::string cellId = a_cells[cellIndex]->GetCellId();

        if (firstCellId.compare(cellId) != 0)
        {
          areDifferent = true;
          break;
        }
      }

      return areDifferent;
    }

    unsigned int GetBestCellParentIndex(
        const DGGS_Handle a_handle,
        const std::vector<std::unique_ptr<Model::Cell::ICell> > & a_parentCells,
        const DGGS_Shape a_originalCellShape)
    {
      if (a_parentCells.size() == 0)
      {
        throw EAGGRException("No parent cells supplied to GetBestCellParentIndex.");
      }

      // If there is only one parent then that is the best parent
      if (a_parentCells.size() == 1)
      {
        return 0;
      }
      else
      {
        // Otherwise we need to check the relationships between the parent and original cells
        // For the hexagonal cells, child cells can have 3 parent cells which can cause increasing
        // error in the parent cell as the cell hierarchy is traversed.
        // This algorithm selects a parent cell that fully covers the original child cell if one is
        // available, otherwise it uses the first parent.

        // Loop over all of the potential parent cells
        for (unsigned int parentIndex = 0; parentIndex < a_parentCells.size(); ++parentIndex)
        {
          // Construct the DGGS Shape representing the parent cell
          DGGS_Shape parentCellShape;
          parentCellShape.m_type = DGGS_ShapeType::DGGS_CELL;
          strncpy(
              parentCellShape.m_data.m_cell,
              a_parentCells[parentIndex]->GetCellId().c_str(),
              EAGGR_MAX_CELL_STRING_LENGTH);

          // Determine if the potential parent cell covers the child cell
          bool parentContainsOriginalCell;
          DGGS_ReturnCode returnCode = EAGGR_CompareShapes(
              a_handle,
              DGGS_AnalysisType::DGGS_CONTAINS,
              &parentCellShape,
              &a_originalCellShape,
              &parentContainsOriginalCell);

          if (returnCode != DGGS_SUCCESS)
          {
            throw EAGGRException("Failed to compare parent and child cells.");
          }

          // If the parent conatins the original child cell then this is the best parent and we return this index
          if (parentContainsOriginalCell)
          {
            return parentIndex;
          }
        }

        // If there is no parent that covers the original cell then it doesn't matter
        // which parent cell is chosen - use the first one
        return 0;
      }
    }

    static void UpdateDggsShapesMemoryAllocation(
        DGGS_Shape ** a_pDggsShapes,
        const unsigned short a_maxShapeIndex)
    {
      // Update the memory allocation
      *a_pDggsShapes = static_cast<DGGS_Shape *>(realloc(
          *a_pDggsShapes,
          (a_maxShapeIndex + 1U) * sizeof(DGGS_Shape)));

      // Check memory allocation was successful
      if (*a_pDggsShapes == NULL)
      {
        throw MemoryAllocationException("Failed to allocate memory for the DGGS shape array");
      }
    }

    static void GetDggsLinestringFromWgs84Linestring(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84Linestring * a_pWgs84Linestring,
        DGGS_Linestring * a_pDggsLinestring)
    {
      // Set the number of cells in the linestring
      const unsigned short noOfCells = a_pWgs84Linestring->GetNumberOfPoints();
      a_pDggsLinestring->m_noOfCells = noOfCells;

      // Allocate memory for the cells in the linestring
      a_pDggsLinestring->m_cells = static_cast<DGGS_Cell *>(malloc(noOfCells * sizeof(DGGS_Cell)));

      // Check memory allocation was successful
      if (a_pDggsLinestring->m_cells == NULL)
      {
        throw MemoryAllocationException(
            "Failed to allocate memory for the cells in a DGGS linestring");
      }

      // Convert and add each cell to the DGGS linestring
      const unsigned short noOfPoints = a_pWgs84Linestring->GetNumberOfPoints();
      for (unsigned short pointIndex = 0U; pointIndex < noOfPoints; pointIndex++)
      {
        GetDggsCellFromWgs84AccuracyPoint(
            a_handle,
            a_pConverter,
            a_pWgs84Linestring->GetAccuracyPoint(pointIndex),
            &(a_pDggsLinestring->m_cells[pointIndex]));
      }
    }

    void GetDggsCellFromWgs84AccuracyPoint(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84AccuracyPoint * a_pWgs84Point,
        DGGS_Cell * a_pCellId)
    {
      // Convert to spherical coordinates (expected by the DGGS class)
      const LatLong::SphericalAccuracyPoint sphericalPoint = a_pConverter->ConvertWGS84ToSphere(
          *a_pWgs84Point);

      // Convert the point to a cell
      std::unique_ptr < Model::Cell::ICell > pCell =
          static_cast<Model::DGGS *>(a_handle)->ConvertLatLongPointToCell(sphericalPoint);

      // Check cell ID does not exceed the maximum length
      const Model::Cell::DggsCellId cellId = pCell->GetCellId();
      CheckCellIdLength(cellId.c_str());

      // Store the cell data in the output array
      static_cast<void>(strncpy(*a_pCellId, cellId.c_str(), EAGGR_MAX_CELL_STRING_LENGTH));
    }

    EAGGR::SpatialAnalysis::AnalysisType ConvertAnalysisType(const DGGS_AnalysisType a_analysisType)
    {
      switch (a_analysisType)
      {
        case DGGS_AnalysisType::DGGS_CONTAINS:
          return EAGGR::SpatialAnalysis::AnalysisType::CONTAINS;
        case DGGS_AnalysisType::DGGS_COVERED_BY:
          return EAGGR::SpatialAnalysis::AnalysisType::COVERED_BY;
        case DGGS_AnalysisType::DGGS_COVERS:
          return EAGGR::SpatialAnalysis::AnalysisType::COVERS;
        case DGGS_AnalysisType::DGGS_CROSSES:
          return EAGGR::SpatialAnalysis::AnalysisType::CROSSES;
        case DGGS_AnalysisType::DGGS_DISJOINT:
          return EAGGR::SpatialAnalysis::AnalysisType::DISJOINT;
        case DGGS_AnalysisType::DGGS_EQUALS:
          return EAGGR::SpatialAnalysis::AnalysisType::EQUALS;
        case DGGS_AnalysisType::DGGS_INTERSECTS:
          return EAGGR::SpatialAnalysis::AnalysisType::INTERSECTS;
        case DGGS_AnalysisType::DGGS_OVERLAPS:
          return EAGGR::SpatialAnalysis::AnalysisType::OVERLAPS;
        case DGGS_AnalysisType::DGGS_TOUCHES:
          return EAGGR::SpatialAnalysis::AnalysisType::TOUCHES;
        case DGGS_AnalysisType::DGGS_WITHIN:
          return EAGGR::SpatialAnalysis::AnalysisType::WITHIN;
        default:
          throw EAGGRException("Unrecognised analysis type.");
      }
    }

    void ConvertLinestringToVector(
        const DGGS_Handle a_handle,
        const DGGS_Linestring a_linestring,
        std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_pDggsLinestring)
    {

      for (unsigned short cellIndex = 0U; cellIndex < a_linestring.m_noOfCells; cellIndex++)
      {

        //Check cell ID length does not exceed the maximum length
        CheckCellIdLength(a_linestring.m_cells[cellIndex]);

        std::unique_ptr < EAGGR::Model::Cell::ICell > cell =
            static_cast<Model::DGGS *>(a_handle)->CreateCell(a_linestring.m_cells[cellIndex]);

        a_pDggsLinestring.push_back(std::move(cell));
      }
    }

    void ConvertPolygonToVectors(
        const DGGS_Handle a_handle,
        const DGGS_Polygon a_polygon,
        std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_pDggsPolygonOuterRing,
        std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_pDggsPolygonInnerRings)
    {

      ConvertLinestringToVector(a_handle, a_polygon.m_outerRing, a_pDggsPolygonOuterRing);

      for (unsigned short ringIndex = 0U; ringIndex < a_polygon.m_noOfInnerRings; ringIndex++)
      {
        a_pDggsPolygonInnerRings.push_back(
            std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> >());

        ConvertLinestringToVector(
            a_handle,
            a_polygon.m_innerRings[ringIndex],
            a_pDggsPolygonInnerRings[ringIndex]);
      }
    }
  }
}
