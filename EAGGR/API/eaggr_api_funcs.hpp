//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: C API
//
//------------------------------------------------------
/// @file eaggr_api_funcs.hpp
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

#pragma once

#include <memory>

#include "API/eaggr_api.h"
#include "Src/CoordinateConversion/CoordinateConverter.hpp"
#include "Src/LatLong/Wgs84Linestring.hpp"
#include "Src/LatLong/Wgs84Polygon.hpp"
#include "Src/Model/ICell.hpp"
#include "Src/SpatialAnalysis/SpatialAnalysis.hpp"

namespace EAGGR
{
  namespace API
  {

    /// Constant for the terminating character of a C-style string.
    static const char TERMINATING_CHAR = '\0';

    /// Converts a lat/long point and adds the resulting DGGS cell to the array of shapes.
    /// @param a_handle Handle for the DGGS model.
    /// @param a_pConverter Converter for changing WGS84 coordinates to spherical.
    /// @param a_pWgs84Point WGS84 latitude and longitude point with an associated accuracy.
    /// @param a_shapeIndex Specifies the location in the shape array to add the cell.
    /// @param a_pDggsShapes Array of DGGS shapes.
    void ConvertWgs84PointAndAddToDggsShapes(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84AccuracyPoint * a_pWgs84Point,
        const unsigned short a_shapeIndex,
        DGGS_Shape ** a_pDggsShapes);

    /// Converts a lat/long linestring and adds the resulting DGGS cells to a linestring
    /// in the array of shapes.
    /// @param a_handle Handle for the DGGS model.
    /// @param a_pConverter Converter for changing WGS84 coordinates to spherical.
    /// @param a_pWgs84Linestring Linestring of WGS84 latitude and longitude points.
    /// @param a_shapeIndex Specifies the location in the shape array to add the linestring.
    /// @param a_pDggsShapes Array of DGGS shapes.
    void ConvertWgs84LinestringAndAddToDggsShapes(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84Linestring * a_pWgs84Linestring,
        const unsigned short a_shapeIndex,
        DGGS_Shape ** a_pDggsShapes);

    /// Converts a lat/long polygon and adds the resulting DGGS cells to a polygon
    /// in the array of shapes.
    /// @param a_handle Handle for the DGGS model.
    /// @param a_pConverter Converter for changing WGS84 coordinates to spherical.
    /// @param a_pWgs84Polygon Polygon made up of WGS84 linestrings.
    /// @param a_shapeIndex Specifies the location in the shape array to add the polygon.
    /// @param a_pDggsShapes Array of DGGS shapes.
    void ConvertWgs84PolygonAndAddToDggsShapes(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84Polygon * a_pWgs84Polygon,
        const unsigned short a_shapeIndex,
        DGGS_Shape ** a_pDggsShapes);

    /// Check cell ID does not exceed the maximum length.
    /// @param a_cell DGGS cell ID string.
    void CheckCellIdLength(const DGGS_Cell a_cell);

    /// Determines if the supplied cells are unique.
    /// @param a_cells The vector of cells to process.
    /// @return True if any two cells are different; false otherwise
    /// @throws DGGSException if supplied vector is empty
    bool AreCellsDifferent(std::vector<std::unique_ptr<Model::Cell::ICell> >& a_cells);

    /// Finds the most appropriate parent cell from a list of potential parents for the supplied cell.
    /// @note the original cell does not have to be a direct child of the parent cells but
    ///       should be a child at some point down the hierarchy
    /// @param a_handle Handle for the DGGS model.
    /// @param a_parentCells The parent cells to search.
    /// @param a_originalCellShape Shape representing the original cell that the best parent should be found for.
    /// @return The index of the most appropriate parent.
    /// @throws DGGSException if supplied parent cells vector is empty
    unsigned int GetBestCellParentIndex(
        const DGGS_Handle a_handle,
        const std::vector<std::unique_ptr<Model::Cell::ICell> > & a_parentCells,
        const DGGS_Shape a_originalCellShape);

    /// Converts a lat/long point (in WGS84 coordinates) and outputs the corresponding
    /// DGGS cell.
    /// @note Point does not contain location value e.g. ONE_FACE, TWO_FACES, ...
    /// @param a_handle Handle for the DGGS model.
    /// @param a_pConverter Converter for changing WGS84 coordinates to spherical.
    /// @param a_pWgs84Point WGS84 latitude and longitude point with an associated accuracy.
    /// @param a_pCellId DGGS cell ID for the point.
    void GetDggsCellFromWgs84AccuracyPoint(
        const DGGS_Handle a_handle,
        const CoordinateConversion::CoordinateConverter * a_pConverter,
        const LatLong::Wgs84AccuracyPoint * a_pWgs84Point,
        DGGS_Cell * a_pCellId);

    /// Converts from a DGGS_AnalysisType to EAGGR::SpatialAnalysis::AnalysisType enumerations
    /// @param a_analysisType The analysis type to convert
    /// @return The corresponding analysis type in the C++ domain
    /// @throws DGGSException if the supplied analysis type is not valid
    EAGGR::SpatialAnalysis::AnalysisType ConvertAnalysisType(
        const DGGS_AnalysisType a_analysisType);

    /// Converts a DGGS_Linestring to a vector
    /// @param a_handle Handle for the DGGS model.
    /// @param a_linestring The DGGS linestring to convert.
    /// @param a_pDggsLinestring The vector containing the converted linestring.
    void ConvertLinestringToVector(
        const DGGS_Handle a_handle,
        const DGGS_Linestring a_linestring,
        std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_pDggsLinestring);

    /// Converts a DGGS_Polygon into two vectors, one containing the outer ring and the other containing
    /// vectors, each of which contains an inner ring
    /// @param a_handle Handle for the DGGS model.
    /// @param a_polygon The DGGS polygon to convert.
    /// @param a_pDggsPolygonOuterRing The vector the outer ring is to be converted to.
    /// @param a_pDggsPolygonInnerRings The vector the inner rings are to be converted to.
    void ConvertPolygonToVectors(
        const DGGS_Handle a_handle,
        const DGGS_Polygon a_polygon,
        std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_pDggsPolygonOuterRing,
        std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_pDggsPolygonInnerRings);
  }
}
