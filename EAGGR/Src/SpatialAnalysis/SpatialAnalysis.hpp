//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Spatial Analysis
//
//------------------------------------------------------
/// @file SpatialAnalysis.hpp
/// 
/// Implements the EAGGR::SpatialAnalysis::SpatialAnalysis class.
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

#include "Src/Model/ICell.hpp"
#include "Src/Model/IGridIndexer.hpp"
#include "GeometryAnalyser.hpp"
#include "Src/Model/IProjection.hpp"
#include "Src/CoordinateConversion/CoordinateConverter.hpp"

#include <memory>
#include <vector>
#include <map>

namespace EAGGR
{
  namespace SpatialAnalysis
  {
    enum AnalysisType
    {
      CONTAINS, COVERED_BY, COVERS, CROSSES, DISJOINT, EQUALS, INTERSECTS, OVERLAPS, TOUCHES, WITHIN
    };

    class SpatialAnalysis
    {
      public:
        SpatialAnalysis(
            const std::unique_ptr<EAGGR::Model::Cell::ICell> a_dggsCell,
            const EAGGR::Model::GridIndexer::IGridIndexer * const a_gridIndexer,
            const EAGGR::Model::Projection::IProjection * const a_projection);

        SpatialAnalysis(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsLineString,
            const EAGGR::Model::GridIndexer::IGridIndexer * const a_gridIndexer,
            const EAGGR::Model::Projection::IProjection * const a_projection);

        SpatialAnalysis(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsPolygonOuterRing,
            const std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_dggsPolygonInnerRings,
            const EAGGR::Model::GridIndexer::IGridIndexer * const a_gridIndexer,
            const EAGGR::Model::Projection::IProjection * const a_projection);

        bool Analyse(
            const std::unique_ptr<EAGGR::Model::Cell::ICell> a_dggsCell,
            const AnalysisType a_analysisType) const;

        bool Analyse(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsLinestring,
            const AnalysisType a_analysisType) const;

        bool Analyse(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsPolygonOuterRing,
            const std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_dggsPolygonInnerRings,
            const AnalysisType a_analysisType) const;

        GeometryType GetGeometryType() const;

        bool IsOnSingleFace() const;

      private:

        const EAGGR::Model::GridIndexer::IGridIndexer * const m_gridIndexer;

        std::unique_ptr<GeometryAnalyser> m_analyser;
        std::unique_ptr<GeometryAnalyser> m_latLongAnalyser;

        const EAGGR::Model::Projection::IProjection * const m_projection;
        const EAGGR::CoordinateConversion::CoordinateConverter m_converter;
        unsigned int m_faceIndex;
        bool m_isOnSingleFace;

        void CreatePoint(const EAGGR::Model::Cell::ICell & a_dggsCell, point_type& a_point) const;

        void CreateLatLongPoint(
            const EAGGR::Model::Cell::ICell & a_dggsCell,
            point_type& a_point) const;

        void CreateCell(const EAGGR::Model::Cell::ICell & a_dggsCell, polygon_type& a_cell) const;

        void CreateLatLongCell(
            const EAGGR::Model::Cell::ICell & a_dggsCell,
            polygon_type& a_cell) const;

        void CreateLinestring(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsLineString,
            linestring_type& a_linestring) const;

        void CreateLatLongLinestring(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsLineString,
            linestring_type& a_linestring) const;

        void CreatePolygon(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsPolygonOuterRing,
            const std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_dggsPolygonInnerRings,
            polygon_type& a_polygon) const;

        void CreateLatLongPolygon(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsPolygonOuterRing,
            const std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_dggsPolygonInnerRings,
            polygon_type& a_polygon) const;

        bool AnalyseGeometry(
            GeometryAnalyser& a_analyser,
            Geometry a_geometry,
            AnalysisType a_analysisType) const;

        bool IsGeometryContainedWithinFace(
            const std::unique_ptr<EAGGR::Model::Cell::ICell> & a_cell,
            const unsigned int a_faceIndex) const;

        bool IsGeometryContainedWithinFace(
            const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_cells,
            const unsigned int a_faceIndex) const;
    };
  }
}
