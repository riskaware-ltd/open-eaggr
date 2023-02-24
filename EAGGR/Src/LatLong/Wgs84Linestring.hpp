//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file Wgs84Linestring.hpp
/// 
/// Implements the EAGGR::LatLong::Wgs84Linestring class.
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

#include "Src/Utilities/Maths.hpp"
#include "Src/LatLong/Wgs84AccuracyPoint.hpp"

namespace EAGGR
{
  namespace LatLong
  {
    /// Class representing a linestring made up of WGS84 points with associated
    /// accuracies in metres squared.
    class Wgs84Linestring: public IShape
    {
      public:
        /// Linestring is initialised with no points.
        Wgs84Linestring();
        /// Frees up memory allocated to the points in the linestring.
        virtual ~Wgs84Linestring();

        /// Adds a point to the linestring.
        /// @param a_latitude WGS84 latitude of the point.
        /// @param a_longitude WGS84 longitude of the point.
        /// @param a_accuracy Accuracy of the point in metres squared.
        void AddAccuracyPoint(
            const Utilities::Maths::Degrees a_latitude,
            const Utilities::Maths::Degrees a_longitude,
            const double a_accuracy);

        /// @return Number of points that make up the linestring.
        unsigned short GetNumberOfPoints() const;

        /// @param a_pointIndex Specifies a point in the linestring.
        /// @return Point in the linestring at the specified index.
        /// @throws DGGSException If the index does not specify a valid point in the linestring.
        const Wgs84AccuracyPoint * GetAccuracyPoint(const unsigned short a_pointIndex) const;

        /// Equal to operator, used for comparing two Wgs84Linestring objects.
        /// @param a_otherLinestring Wgs84Linestring object to compare against.
        /// @return True if the linestrings have the same number of points, with the same values.
        bool operator==(const Wgs84Linestring & a_otherLinestring) const;

        /// @param a_otherLinestring Wgs84Linestring object to compare against.
        /// @return Inverse of the == operator.
        bool operator!=(const Wgs84Linestring & a_otherLinestring) const;

      private:
        std::vector<Wgs84AccuracyPoint *> m_points;

        void CheckPointIsValid(const unsigned short a_pointIndex) const;
    };
  }
}
