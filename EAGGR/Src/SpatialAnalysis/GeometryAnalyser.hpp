//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Spatial Analysis
//
//------------------------------------------------------
/// @file GeometryAnalyser.hpp
/// 
/// Implements the EAGGR::SpatialAnalysis::GeometryAnalyser class.
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

#include "GeometryTypes.hpp"

namespace EAGGR
{
  namespace SpatialAnalysis
  {
    class Geometry
    {
      public:
        GeometryType m_type;
        point_type m_point;
        linestring_type m_linestring;
        polygon_type m_polygon;
    };

    class GeometryAnalyser
    {
      public:

        virtual ~GeometryAnalyser() = 0;

        virtual GeometryType GetGeometryType() = 0;

        virtual bool Equals(Geometry a_geometry) const = 0;

        virtual bool Intersects(Geometry a_geometry) const = 0;

        virtual bool Touches(Geometry a_geometry) const = 0;

        virtual bool Contains(Geometry a_geometry) const = 0;

        virtual bool Covers(Geometry a_geometry) const = 0;

        virtual bool Within(Geometry a_geometry) const = 0;

        virtual bool CoveredBy(Geometry a_geometry) const = 0;

        virtual bool Crosses(Geometry a_geometry) const = 0;

        virtual bool Overlaps(Geometry a_geometry) const = 0;

        bool Disjoint(Geometry a_geometry) const;

        virtual GeometryType GetGeometryType() const = 0;
    };
  }
}
