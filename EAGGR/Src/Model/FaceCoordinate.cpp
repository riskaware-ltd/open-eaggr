//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Model
//
//------------------------------------------------------
/// @file FaceCoordinate.cpp
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

#include "FaceCoordinate.hpp"
#include "Src/EAGGRException.hpp"

namespace EAGGR
{
  namespace Model
  {
    FaceCoordinate::FaceCoordinate(
        const FaceIndex a_faceIndex,
        const double a_xOffset,
        const double a_yOffset,
        const double a_accuracy)
        :
            m_faceIndex(a_faceIndex),
            m_xOffset(a_xOffset),
            m_yOffset(a_yOffset),
            m_accuracy(a_accuracy)
    {
      if (a_accuracy < 0.0)
      {
        throw EAGGR::EAGGRException("Accuracy cannot be negative.");
      }
    }

    FaceIndex FaceCoordinate::GetFaceIndex() const
    {
      return (m_faceIndex);
    }

    double FaceCoordinate::GetXOffset() const
    {
      return (m_xOffset);
    }

    double FaceCoordinate::GetYOffset() const
    {
      return (m_yOffset);
    }

    double FaceCoordinate::GetAccuracy() const
    {
      return (m_accuracy);
    }
  }
}
