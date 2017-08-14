//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Test Utilities
//
//------------------------------------------------------
/// @file RunningAverage.cpp
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

#include <cmath>

#include "RunningAverage.hpp"

RunningAverage::RunningAverage()
    : m_noOfValues(0UL), m_maximum(0.0), m_mean(0.0), m_prevMean(0.0), m_stdDeviation(0.0)
{
}

void RunningAverage::Update(const double a_value)
{
  // Increment the counter for the number of values
  m_noOfValues++;

  // Update the maximum value, if necessary
  if (a_value > m_maximum)
  {
    m_maximum = a_value;
  }

  // Store the mean average from the previous iteration
  m_prevMean = m_mean;

  // Calculate the new mean average
  m_mean += (a_value - m_mean) / m_noOfValues;

  // Update the standard deviation
  m_stdDeviation += (a_value - m_mean) * (a_value - m_prevMean);
}

double RunningAverage::GetMaximum() const
{
  return (m_maximum);
}

double RunningAverage::GetMeanAverage() const
{
  return (m_mean);
}

double RunningAverage::GetStandardDeviation() const
{
  return (sqrt(m_stdDeviation / m_noOfValues));
}
