//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file FaceCoordinate.hpp
/// 
/// Implements the EAGGR::Model::FaceCoordinate class.
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

#include "Src/Model/FaceTypes.hpp"
#include "Src/Utilities/Maths.hpp"

namespace EAGGR
{
  namespace Model
  {
    /// Stores the position of a point projected on to the face of a polyhedral globe.
    class FaceCoordinate
    {
      public:
        /// Sets the parameters that define the point.
        /// @param a_faceIndex The face on which the point is located.
        /// @param a_xOffset The x coordinate of the point relative to the centre of the face.
        /// @param a_yOffset The y coordinate of the point relative to the centre of the face.
        /// @param a_accuracy The accuracy of the point as a fraction of the face area.
        /// @throws DGGSException if supplied accuracy is negative
        FaceCoordinate(
            const FaceIndex a_faceIndex,
            const double a_xOffset,
            const double a_yOffset,
            const double a_accuracy);

        /// @return The index of the face on which the coordinate is located.
        FaceIndex GetFaceIndex() const;

        /// @return The distance of the point relative to the centre of the face along the x-axis.
        /// @todo Make distance a fraction of the total width of the face.
        double GetXOffset() const;

        /// @return The distance of the point relative to the centre of the face along the y-axis.
        /// @todo Make distance a fraction of the total height of the face.
        double GetYOffset() const;

        /// @return The accuracy of the point as an area. The area is given as a fraction of the
        /// total area of the face.
        double GetAccuracy() const;

      private:
        const FaceIndex m_faceIndex;
        const double m_xOffset;
        const double m_yOffset;
        const double m_accuracy;
    };

  }
}
