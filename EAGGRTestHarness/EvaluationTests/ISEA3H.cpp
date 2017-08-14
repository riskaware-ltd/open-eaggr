//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Evaluation Framework
//
//------------------------------------------------------
/// @file ISEA3H.cpp
/// 
/// System tests for evaluating the ISEA3H DGGS with
/// offset grid indexing.
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

#include "TestMacros.hpp"
#include "EvaluationTests/Performance.hpp"

#include "Src/Model/DGGS.hpp"
#include "Src/Model/IGrid/IOffsetGrid/Aperture3HexagonGrid.hpp"
#include "Src/Model/IGridIndexer/OffsetGridIndexer.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/Model/IProjection/Snyder.hpp"

using namespace EAGGR;
using namespace EAGGR::Utilities::Maths;

EVALUATION_TEST(ISEA3H, ConvertPointsWorldwide)
{
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder snyder(&globe);
  Model::Grid::OffsetGrid::Aperture3HexagonGrid grid;
  Model::GridIndexer::OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  Model::DGGS dggs(&snyder, &indexer);

  EvaluationFramework::Performance performance(dggs);
  performance.ConvertPointsWorldwide();
}
