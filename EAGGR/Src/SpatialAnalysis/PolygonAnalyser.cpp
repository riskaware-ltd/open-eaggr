//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Spatial Analysis
//
//------------------------------------------------------
/// @file PolygonAnalyser.cpp
/// 
/// Implements the EAGGR::SpatialAnalysis::PolygonAnalyser class.
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

#include "PolygonAnalyser.hpp"

namespace EAGGR
{
  namespace SpatialAnalysis
  {
    PolygonAnalyser::PolygonAnalyser(polygon_type a_polygon)
        : m_polygon(a_polygon)
    {
    }

    PolygonAnalyser::~PolygonAnalyser()
    {
    }

    GeometryType PolygonAnalyser::GetGeometryType()
    {
      return GeometryType::POLYGON;
    }

    bool PolygonAnalyser::Equals(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POLYGON:
          return boost::geometry::equals(m_polygon, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool PolygonAnalyser::Intersects(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POINT:
          return boost::geometry::intersects(m_polygon, a_geometry.m_point);
        case LINESTRING:
          return boost::geometry::intersects(m_polygon, a_geometry.m_linestring);
        case POLYGON:
          return boost::geometry::intersects(m_polygon, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool PolygonAnalyser::Touches(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POINT:
          return boost::geometry::touches(m_polygon, a_geometry.m_point);
        case LINESTRING:
          return boost::geometry::touches(m_polygon, a_geometry.m_linestring);
        case POLYGON:
          return boost::geometry::touches(m_polygon, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool PolygonAnalyser::Contains(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        // Use the boost within method but switch the arguments (this is equivalent)
        // contains(a, b) equivalent to within(b, a)
        case POINT:
          return boost::geometry::within(a_geometry.m_point, m_polygon);
        case LINESTRING:
          return boost::geometry::within(a_geometry.m_linestring, m_polygon);
        case POLYGON:
          return boost::geometry::within(a_geometry.m_polygon, m_polygon);
        default:
          return false;
      }
    }

    bool PolygonAnalyser::Covers(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        // Use the boost within method but switch the arguments (this is equivalent)
        // covers(a, b) equivalent to covered_by(b, a)
        case POINT:
          return boost::geometry::covered_by(a_geometry.m_point, m_polygon);
        case LINESTRING:
          return boost::geometry::covered_by(a_geometry.m_linestring, m_polygon);
        case POLYGON:
          return boost::geometry::covered_by(a_geometry.m_polygon, m_polygon);
        default:
          return false;
      }
    }

    bool PolygonAnalyser::Within(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POLYGON:
          return boost::geometry::within(m_polygon, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool PolygonAnalyser::CoveredBy(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POLYGON:
          return boost::geometry::covered_by(m_polygon, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool PolygonAnalyser::Crosses(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POINT:
          return boost::geometry::crosses(m_polygon, a_geometry.m_point);
        case LINESTRING:
          return boost::geometry::crosses(a_geometry.m_linestring, m_polygon);
        default:
          return false;
      }
    }

    bool PolygonAnalyser::Overlaps(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POLYGON:
          return boost::geometry::overlaps(m_polygon, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    GeometryType PolygonAnalyser::GetGeometryType() const
    {
      return GeometryType::POLYGON;
    }
  }
}
