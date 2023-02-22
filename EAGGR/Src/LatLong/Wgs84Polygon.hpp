//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file Wgs84Polygon.hpp
/// 
/// Implements the EAGGR::LatLong::Wgs84Polygon class.
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

#include <vector>

#include "Src/LatLong/Wgs84Linestring.hpp"

namespace EAGGR
{
  namespace LatLong
  {
    /// Class representing a polygon made up of WGS84 linestrings defining its
    /// outer and inner rings.
    class Wgs84Polygon: public IShape
    {
      public:
        /// Polygon is initialised with no inner rings and no points in its outer ring.
        Wgs84Polygon();
        /// Frees up memory allocated to the linestrings that make up the polygon.
        virtual ~Wgs84Polygon();

        /// Adds a point to the linestring that defines the polygon's outer ring.
        /// @param a_latitude WGS84 latitude of the point.
        /// @param a_longitude WGS84 longitude of the point.
        /// @param a_accuracy Accuracy of the point in metres squared.
        void AddAccuracyPointToOuterRing(
            const Utilities::Maths::Degrees a_latitude,
            const Utilities::Maths::Degrees a_longitude,
            const double a_accuracy);

        /// @return Constant pointer to the outer ring of the polygon.
        const Wgs84Linestring * GetOuterRing() const;

        /// Adds a new empty inner ring to the polygon.
        void CreateInnerRing();

        /// Adds a point to one of the linestrings that define the polygon's inner rings.
        /// @param a_ringIndex Specifies which inner ring to add the point to.
        /// @param a_latitude WGS84 latitude of the point.
        /// @param a_longitude WGS84 longitude of the point.
        /// @param a_accuracy Accuracy of the point in metres squared.
        /// @throws DGGSException If the index does not specify a valid inner ring.
        void AddAccuracyPointToInnerRing(
            const unsigned short a_ringIndex,
            const Utilities::Maths::Degrees a_latitude,
            const Utilities::Maths::Degrees a_longitude,
            const double a_accuracy);

        /// @return Number of inner rings that make up the polygon.
        unsigned short GetNumberOfInnerRings() const;

        /// @param a_ringIndex Specifies which inner ring to add the point to.
        /// @return Constant pointer to the inner ring specified by the index.
        /// @throws DGGSException If the index does not specify a valid inner ring.
        const Wgs84Linestring * GetInnerRing(const unsigned short a_ringIndex) const;

        /// Equal to operator, used for comparing two Wgs84Polygon objects.
        /// @param a_otherPolygon Wgs84Polygon object to compare against.
        /// @return True if the polygons have the same outer ring points and inner rings.
        bool operator==(const Wgs84Polygon & a_otherPolygon) const;

        /// @param a_otherPolygon Wgs84Polygon object to compare against.
        /// @return Inverse of the == operator.
        bool operator!=(const Wgs84Polygon & a_otherPolygon) const;

      private:
        Wgs84Linestring m_outerRing;
        std::vector<Wgs84Linestring *> m_innerRings;

        void CheckInnerRingIsValid(const unsigned short a_ringIndex) const;
    };
  }
}
