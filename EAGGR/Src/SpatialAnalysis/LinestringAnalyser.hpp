//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Spatial Analysis
//
//------------------------------------------------------
/// @file LinestringAnalyser.hpp
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

#pragma once

#include "GeometryAnalyser.hpp"

namespace EAGGR
{
  namespace SpatialAnalysis
  {
    class LinestringAnalyser: public GeometryAnalyser
    {
      public:
        LinestringAnalyser(linestring_type a_linestring);

        ~LinestringAnalyser();

        virtual GeometryType GetGeometryType();

        virtual bool Equals(Geometry a_geometry) const;

        virtual bool Intersects(Geometry a_geometry) const;

        virtual bool Touches(Geometry a_geometry) const;

        virtual bool Contains(Geometry a_geometry) const;

        virtual bool Covers(Geometry a_geometry) const;

        virtual bool Within(Geometry a_geometry) const;

        virtual bool CoveredBy(Geometry a_geometry) const;

        virtual bool Crosses(Geometry a_geometry) const;

        virtual bool Overlaps(Geometry a_geometry) const;

        virtual GeometryType GetGeometryType() const;

      private:
        linestring_type m_linestring;
    };
  }
}
