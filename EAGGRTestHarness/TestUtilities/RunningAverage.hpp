//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Test Utilities
//
//------------------------------------------------------
/// @file RunningAverage.hpp
/// 
/// Implements the SystemTests::RunningAverage class.
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

/// Class for calculating a running average from a set of values.
/// @note Equations used in the class are taken from:
///       http://dsp.stackexchange.com/a/1187
class RunningAverage
{
  public:
    RunningAverage();

    void Update(const double a_value);
    double GetMaximum() const;
    double GetMeanAverage() const;
    double GetStandardDeviation() const;

  private:
    unsigned long m_noOfValues;
    double m_maximum;
    double m_mean;
    double m_prevMean;
    double m_stdDeviation;
};
