//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Spatial Analysis
//
//------------------------------------------------------
/// @file LinestringAnalyser.cpp
/// 
/// Implements the EAGGR::SpatialAnalysis::LinestringAnalyser class.
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

#include "LinestringAnalyser.hpp"

namespace EAGGR
{
  namespace SpatialAnalysis
  {
    LinestringAnalyser::LinestringAnalyser(linestring_type a_linestring)
        : m_linestring(a_linestring)
    {
    }

    LinestringAnalyser::~LinestringAnalyser()
    {
    }

    GeometryType LinestringAnalyser::GetGeometryType()
    {
      return GeometryType::LINESTRING;
    }

    bool LinestringAnalyser::Equals(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case LINESTRING:
          return boost::geometry::equals(m_linestring, a_geometry.m_linestring);
        default:
          return false;
      }
    }

    bool LinestringAnalyser::Intersects(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POINT:
          return boost::geometry::intersects(m_linestring, a_geometry.m_point);
        case LINESTRING:
          return boost::geometry::intersects(m_linestring, a_geometry.m_linestring);
        case POLYGON:
          return boost::geometry::intersects(m_linestring, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool LinestringAnalyser::Touches(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POINT:
          return boost::geometry::touches(m_linestring, a_geometry.m_point);
        case LINESTRING:
          return boost::geometry::touches(m_linestring, a_geometry.m_linestring);
        case POLYGON:
          return boost::geometry::touches(m_linestring, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool LinestringAnalyser::Contains(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        // Use the boost within method but switch the arguments (this is equivalent)
        // contains(a, b) equivalent to within(b, a)
        case POINT:
          return boost::geometry::within(a_geometry.m_point, m_linestring);
        case LINESTRING:
          return boost::geometry::within(a_geometry.m_linestring, m_linestring);
        default:
          return false;
      }
    }

    bool LinestringAnalyser::Covers(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        // Use the boost covered_by method but switch the arguments (this is equivalent)
        // covers(a, b) equivalent to covered_by(b, a)
        case POINT:
          return boost::geometry::covered_by(a_geometry.m_point, m_linestring);
        case LINESTRING:
          return boost::geometry::covered_by(a_geometry.m_linestring, m_linestring);
        default:
          return false;
      }
    }

    bool LinestringAnalyser::Within(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case LINESTRING:
          return boost::geometry::within(m_linestring, a_geometry.m_linestring);
        case POLYGON:
          return boost::geometry::within(m_linestring, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool LinestringAnalyser::CoveredBy(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case LINESTRING:
          return boost::geometry::covered_by(m_linestring, a_geometry.m_linestring);
        case POLYGON:
          return boost::geometry::covered_by(m_linestring, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool LinestringAnalyser::Crosses(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case POINT:
          return boost::geometry::crosses(m_linestring, a_geometry.m_point);
        case LINESTRING:
          return boost::geometry::crosses(m_linestring, a_geometry.m_linestring);
        case POLYGON:
          return boost::geometry::crosses(m_linestring, a_geometry.m_polygon);
        default:
          return false;
      }
    }

    bool LinestringAnalyser::Overlaps(Geometry a_geometry) const
    {
      switch (a_geometry.m_type)
      {
        case LINESTRING:
          return boost::geometry::overlaps(m_linestring, a_geometry.m_linestring);
        default:
          return false;
      }
    }

    GeometryType LinestringAnalyser::GetGeometryType() const
    {
      return GeometryType::LINESTRING;
    }
  }
}
