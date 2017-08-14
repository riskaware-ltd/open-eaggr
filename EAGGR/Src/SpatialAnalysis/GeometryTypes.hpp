//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Spatial Analysis
//
//------------------------------------------------------
/// @file GeometryTypes.hpp
/// 
/// Definition of types for geometry spatial analysis
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

// Boost code introducing warnings when we use boost::geometry::overlaps from PolygonAnalyser
// Disabling warnings for just these files
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#include "boost/geometry/geometry.hpp"
#include "boost/geometry/geometries/point_xy.hpp"
#pragma GCC diagnostic pop

namespace EAGGR
{
  namespace SpatialAnalysis
  {
    typedef boost::geometry::model::d2::point_xy<double> point_type;
    typedef boost::geometry::model::linestring<point_type> linestring_type;
    typedef boost::geometry::model::polygon<point_type> polygon_type;

    enum GeometryType
    {
      POINT, LINESTRING, POLYGON
    };
  }
}
