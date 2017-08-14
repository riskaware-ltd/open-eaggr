//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Latitude / Longitude
//
//------------------------------------------------------
/// @file LatLongShape.hpp
/// 
/// Implements the EAGGR::LatLong::LatLongShape class.
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

#include "Src/LatLong/IShape.hpp"

namespace EAGGR
{
  namespace LatLong
  {
    /// Class to encapsulate the shape with an indication of the shape type.
    class LatLongShape
    {
      public:
        /// Constructor
        /// @param a_shapeType The type of shape being represented.
        /// @param a_pShape The shape object.
        LatLongShape(const LatLongShapeType a_shapeType, const IShape* const a_pShape);

        /// @return The type of shape being represented.
        LatLongShapeType GetShapeType();

        /// @return The shape object.
        const IShape* GetShapeData();

      private:
        LatLongShapeType m_shapeType;
        const IShape* m_pShape;
    };
  }
}
