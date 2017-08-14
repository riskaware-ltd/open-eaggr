//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Spatial Analysis
//
//------------------------------------------------------
/// @file SpatialAnalysis.cpp
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

#include "SpatialAnalysis.hpp"
#include "LinestringAnalyser.hpp"
#include "PolygonAnalyser.hpp"

#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/LatLong/Wgs84AccuracyPoint.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Model;

namespace EAGGR
{
  namespace SpatialAnalysis
  {
    SpatialAnalysis::SpatialAnalysis(
        const std::unique_ptr<Cell::ICell> a_dggsCell,
        const GridIndexer::IGridIndexer * const a_gridIndexer,
        const EAGGR::Model::Projection::IProjection * const a_projection)
        : m_gridIndexer(a_gridIndexer), m_projection(a_projection)
    {
      polygon_type cellOutline;
      CreateCell(*a_dggsCell, cellOutline);
      m_analyser.reset(new PolygonAnalyser(cellOutline));
      polygon_type latLongCellOutline;
      CreateLatLongCell(*a_dggsCell, latLongCellOutline);
      m_latLongAnalyser.reset(new PolygonAnalyser(latLongCellOutline));

      // Point analysis is always on one face
      m_isOnSingleFace = true;
      m_faceIndex = a_dggsCell->GetFaceIndex();
    }

    SpatialAnalysis::SpatialAnalysis(
        const std::vector<std::unique_ptr<Cell::ICell> > & a_dggsLinestring,
        const GridIndexer::IGridIndexer * const a_gridIndexer,
        const EAGGR::Model::Projection::IProjection * const a_projection)
        : m_gridIndexer(a_gridIndexer), m_projection(a_projection)
    {
      if (a_dggsLinestring.size() == 0)
      {
        throw EAGGRException("Linestring has no cells.");
      }

      linestring_type linestring;
      CreateLinestring(a_dggsLinestring, linestring);
      m_analyser.reset(new LinestringAnalyser(linestring));

      linestring_type latLongLinestring;
      CreateLatLongLinestring(a_dggsLinestring, latLongLinestring);
      m_latLongAnalyser.reset(new LinestringAnalyser(latLongLinestring));

      m_faceIndex = a_dggsLinestring[0]->GetFaceIndex();
      m_isOnSingleFace = IsGeometryContainedWithinFace(a_dggsLinestring, m_faceIndex);
    }

    SpatialAnalysis::SpatialAnalysis(
        const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsPolygonOuterRing,
        const std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_dggsPolygonInnerRings,
        const GridIndexer::IGridIndexer * const a_gridIndexer,
        const EAGGR::Model::Projection::IProjection * const a_projection)
        : m_gridIndexer(a_gridIndexer), m_projection(a_projection)
    {
      if (a_dggsPolygonOuterRing.size() == 0)
      {
        throw EAGGRException("Polygon outer ring has no cells.");
      }

      polygon_type polygon;
      CreatePolygon(a_dggsPolygonOuterRing, a_dggsPolygonInnerRings, polygon);
      m_analyser.reset(new PolygonAnalyser(polygon));

      polygon_type latLongPolygon;
      CreateLatLongPolygon(a_dggsPolygonOuterRing, a_dggsPolygonInnerRings, latLongPolygon);
      m_latLongAnalyser.reset(new PolygonAnalyser(latLongPolygon));

      m_faceIndex = a_dggsPolygonOuterRing[0]->GetFaceIndex();
      m_isOnSingleFace = IsGeometryContainedWithinFace(a_dggsPolygonOuterRing, m_faceIndex);
    }

    bool SpatialAnalysis::Analyse(
        const std::unique_ptr<EAGGR::Model::Cell::ICell> a_dggsCell,
        const AnalysisType a_analysisType) const
    {
      Geometry geometry;
      geometry.m_type = GeometryType::POLYGON;

      // Check the point is on the same face as the geometry used by the analyser
      bool onSameFace = m_isOnSingleFace && IsGeometryContainedWithinFace(a_dggsCell, m_faceIndex);
      if (onSameFace)
      {
        CreateCell(*a_dggsCell, geometry.m_polygon);
        return AnalyseGeometry(*m_analyser, geometry, a_analysisType);
      }
      else
      {
        CreateLatLongCell(*a_dggsCell, geometry.m_polygon);
        return AnalyseGeometry(*m_latLongAnalyser, geometry, a_analysisType);
      }
    }

    bool SpatialAnalysis::Analyse(
        const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsLineString,
        const AnalysisType a_analysisType) const
    {
      Geometry geometry;
      geometry.m_type = GeometryType::LINESTRING;

      bool onSameFace = m_isOnSingleFace
          && IsGeometryContainedWithinFace(a_dggsLineString, m_faceIndex);
      if (onSameFace)
      {
        CreateLinestring(a_dggsLineString, geometry.m_linestring);
        return AnalyseGeometry(*m_analyser, geometry, a_analysisType);
      }
      else
      {
        CreateLatLongLinestring(a_dggsLineString, geometry.m_linestring);
        return AnalyseGeometry(*m_latLongAnalyser, geometry, a_analysisType);
      }
    }

    bool SpatialAnalysis::Analyse(
        const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsPolygonOuterRing,
        const std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_dggsPolygonInnerRings,
        const AnalysisType a_analysisType) const
    {
      Geometry geometry;
      geometry.m_type = GeometryType::POLYGON;

      bool onSameFace = m_isOnSingleFace
          && IsGeometryContainedWithinFace(a_dggsPolygonOuterRing, m_faceIndex);

      if (onSameFace)
      {
        CreatePolygon(a_dggsPolygonOuterRing, a_dggsPolygonInnerRings, geometry.m_polygon);
        return AnalyseGeometry(*m_analyser, geometry, a_analysisType);
      }
      else
      {
        CreateLatLongPolygon(a_dggsPolygonOuterRing, a_dggsPolygonInnerRings, geometry.m_polygon);
        return AnalyseGeometry(*m_latLongAnalyser, geometry, a_analysisType);
      }
    }

    void SpatialAnalysis::CreatePoint(const Cell::ICell & a_dggsCell, point_type& a_point) const
    {
      FaceCoordinate coordinate = m_gridIndexer->GetFaceCoordinate(a_dggsCell);
      a_point.x(coordinate.GetXOffset());
      a_point.y(coordinate.GetYOffset());
    }

    void SpatialAnalysis::CreateLatLongPoint(
        const Cell::ICell & a_dggsCell,
        point_type& a_point) const
    {
      FaceCoordinate coordinate = m_gridIndexer->GetFaceCoordinate(a_dggsCell);
      LatLong::SphericalAccuracyPoint sphericalPoint = m_projection->GetLatLongPoint(coordinate);
      LatLong::Wgs84AccuracyPoint wgs84Point = m_converter.ConvertSphereToWGS84(sphericalPoint);
      a_point.x(wgs84Point.GetLongitude());
      a_point.y(wgs84Point.GetLatitude());
    }

    void SpatialAnalysis::CreateCell(const Cell::ICell & a_dggsCell, polygon_type& a_cell) const
    {
      std::list < FaceCoordinate > vertices;
      m_gridIndexer->GetCellVertices(a_dggsCell, vertices);

      linestring_type outerLine;

      for (std::list<FaceCoordinate>::const_iterator vertexIter = vertices.begin();
          vertexIter != vertices.end(); ++vertexIter)
      {
        point_type point;
        point.x(vertexIter->GetXOffset());
        point.y(vertexIter->GetYOffset());

        boost::geometry::append(outerLine, point);
      }

      boost::geometry::assign_points(a_cell.outer(), outerLine);

      // Use the correct function to ensure it is closed
      boost::geometry::correct(a_cell);
    }

    void SpatialAnalysis::CreateLatLongCell(
        const Cell::ICell & a_dggsCell,
        polygon_type& a_cell) const
    {
      std::list < FaceCoordinate > vertices;
      m_gridIndexer->GetCellVertices(a_dggsCell, vertices);

      linestring_type outerLine;

      for (std::list<FaceCoordinate>::const_iterator vertexIter = vertices.begin();
          vertexIter != vertices.end(); ++vertexIter)
      {
        LatLong::SphericalAccuracyPoint sphericalPoint = m_projection->GetLatLongPoint(*vertexIter);
        LatLong::Wgs84AccuracyPoint wgs84Point = m_converter.ConvertSphereToWGS84(sphericalPoint);

        point_type point;
        point.x(wgs84Point.GetLongitude());
        point.y(wgs84Point.GetLatitude());

        boost::geometry::append(outerLine, point);
      }

      boost::geometry::assign_points(a_cell.outer(), outerLine);

      // Use the correct function to ensure it is closed
      boost::geometry::correct(a_cell);
    }

    void SpatialAnalysis::CreateLinestring(
        const std::vector<std::unique_ptr<Cell::ICell> > & a_dggsLineString,
        linestring_type& a_linestring) const
    {
      for (std::vector<std::unique_ptr<Cell::ICell> >::const_iterator cellIter =
          a_dggsLineString.begin(); cellIter < a_dggsLineString.end(); ++cellIter)
      {
        point_type point;

        Cell::ICell& cell = *(*cellIter).get();
        CreatePoint(cell, point);

        boost::geometry::append(a_linestring, point);
      }
    }

    void SpatialAnalysis::CreateLatLongLinestring(
        const std::vector<std::unique_ptr<Cell::ICell> > & a_dggsLineString,
        linestring_type& a_linestring) const
    {
      for (std::vector<std::unique_ptr<Cell::ICell> >::const_iterator cellIter =
          a_dggsLineString.begin(); cellIter < a_dggsLineString.end(); ++cellIter)
      {
        point_type point;

        Cell::ICell& cell = *(*cellIter).get();
        CreateLatLongPoint(cell, point);

        boost::geometry::append(a_linestring, point);
      }
    }

    void SpatialAnalysis::CreatePolygon(
        const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsPolygonOuterRing,
        const std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_dggsPolygonInnerRings,
        polygon_type& a_polygon) const
    {
      linestring_type outerLine;
      CreateLinestring(a_dggsPolygonOuterRing, outerLine);
      boost::geometry::assign_points(a_polygon.outer(), outerLine);

      a_polygon.inners().resize(a_dggsPolygonInnerRings.size());

      for (unsigned int index = 0; index < a_dggsPolygonInnerRings.size(); ++index)
      {
        linestring_type innerLine;
        CreateLinestring(a_dggsPolygonInnerRings.at(index), innerLine);
        boost::geometry::assign_points(a_polygon.inners().at(index), innerLine);
      }

      // Use the correct function to ensure it is closed
      boost::geometry::correct(a_polygon);
    }

    void SpatialAnalysis::CreateLatLongPolygon(
        const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_dggsPolygonOuterRing,
        const std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > & a_dggsPolygonInnerRings,
        polygon_type& a_polygon) const
    {
      linestring_type outerLine;
      CreateLatLongLinestring(a_dggsPolygonOuterRing, outerLine);
      boost::geometry::assign_points(a_polygon.outer(), outerLine);

      a_polygon.inners().resize(a_dggsPolygonInnerRings.size());

      for (unsigned int index = 0; index < a_dggsPolygonInnerRings.size(); ++index)
      {
        linestring_type innerLine;
        CreateLatLongLinestring(a_dggsPolygonInnerRings.at(index), innerLine);
        boost::geometry::assign_points(a_polygon.inners().at(index), innerLine);
      }

      // Use the correct function to ensure it is closed
      boost::geometry::correct(a_polygon);
    }

    bool SpatialAnalysis::AnalyseGeometry(
        GeometryAnalyser& a_analyser,
        Geometry a_geometry,
        AnalysisType a_analysisType) const
    {
      switch (a_analysisType)
      {
        case EQUALS:
          return a_analyser.Equals(a_geometry);
        case CONTAINS:
          return a_analyser.Contains(a_geometry);
        case WITHIN:
          return a_analyser.Within(a_geometry);
        case TOUCHES:
          return a_analyser.Touches(a_geometry);
        case DISJOINT:
          return a_analyser.Disjoint(a_geometry);
        case INTERSECTS:
          return a_analyser.Intersects(a_geometry);
        case COVERS:
          return a_analyser.Covers(a_geometry);
        case COVERED_BY:
          return a_analyser.CoveredBy(a_geometry);
        case CROSSES:
          return a_analyser.Crosses(a_geometry);
        case OVERLAPS:
          return a_analyser.Overlaps(a_geometry);
        default:
          throw EAGGRException("Unsupported analysis type.");
      }
    }

    GeometryType SpatialAnalysis::GetGeometryType() const
    {
      return m_analyser->GetGeometryType();
    }

    bool SpatialAnalysis::IsOnSingleFace() const
    {
      return m_isOnSingleFace;
    }

    bool SpatialAnalysis::IsGeometryContainedWithinFace(
        const std::unique_ptr<EAGGR::Model::Cell::ICell> & a_cell,
        const unsigned int a_faceIndex) const
    {
      return a_cell->GetFaceIndex() == a_faceIndex;
    }

    bool SpatialAnalysis::IsGeometryContainedWithinFace(
        const std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > & a_cells,
        const unsigned int a_faceIndex) const
    {
      bool cellsOnFace = true;

      for (unsigned int cellIndex = 0; cellIndex < a_cells.size(); ++cellIndex)
      {
        if (!IsGeometryContainedWithinFace(a_cells[cellIndex], a_faceIndex))
        {
          cellsOnFace = false;
          break;
        }
      }

      return cellsOnFace;
    }
  }
}

