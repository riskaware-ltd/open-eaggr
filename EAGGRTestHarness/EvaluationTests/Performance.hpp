//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Evaluation Framework
//
//------------------------------------------------------
/// @file Performance.hpp
/// 
/// Implements the EvaluationFramework::Performance class.
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

#include "TestUtilities/RunningAverage.hpp"
#include "TestUtilities/TestTimer.hpp"

#include "Src/Model/DGGS.hpp"
#include "Src/Utilities/Maths.hpp"

namespace EvaluationFramework
{
  /// Class for evaluating the performance of a DGGS.
  class Performance
  {
    public:
      /// Constructor
      /// @param a_dggs The DGGS to be evaluated.
      Performance(const EAGGR::Model::DGGS & a_dggs);

      /// Runs a test where lat / long coordinates for points around the world
      /// are converted to DGGS cells and back again. The conversions are timed
      /// and the results included in the XML file output by Google Test.
      void ConvertPointsWorldwide() const;

    private:
      /// Timer objects to store the timings for each type of calculation.
      struct Timers
      {
          TestTimer m_convertWgs84ToSpherical;
          TestTimer m_convertSphericalToWgs84;
          TestTimer m_convertLatLongPoints;
          TestTimer m_generateCellIds;
          TestTimer m_createCells;
          TestTimer m_convertCells;
          TestTimer m_getChildCells;
          TestTimer m_getParentCells;
      };

      const EAGGR::Model::DGGS & m_dggs;

      /// Calculates the step size between points in the latitude direction.
      EAGGR::Utilities::Maths::Degrees CalculateLatitudeStep(
          const EAGGR::Utilities::Maths::Degrees latLimitsMargin,
          const EAGGR::Utilities::Maths::Degrees latitudeSpacing) const;

      /// Calculates the tolerances so that the worst-case scenarios for all DGGS are covered.
      void CalculateTolerances(
          const double accuracyArea,
          EAGGR::Utilities::Maths::Degrees & conversionTolerance,
          EAGGR::Utilities::Maths::Degrees & accuracyTolerance) const;

      /// Calculates the distance in metres along the specified line of latitude
      double CircumferenceAtLatitude(const EAGGR::Utilities::Maths::Degrees a_latitude) const;

      /// Stores the error data using the supplied error name as a key
      void StoreErrorValues(
          const std::string & a_errorName,
          const unsigned short a_accuracyLevel,
          const RunningAverage & a_average) const;

      /// Stores timing data
      void StoreTimerValues(const Timers & timers) const;
  };

}
