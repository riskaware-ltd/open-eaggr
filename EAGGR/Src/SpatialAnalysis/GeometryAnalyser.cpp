//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Spatial Analysis
//
//------------------------------------------------------
/// @file GeometryAnalyser.cpp
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

#include "GeometryAnalyser.hpp"

namespace EAGGR
{
  namespace SpatialAnalysis
  {
    GeometryAnalyser::~GeometryAnalyser()
    {
    }

    bool GeometryAnalyser::Disjoint(Geometry a_geometry) const
    {
      return !Intersects(a_geometry);
    }
  }
}

