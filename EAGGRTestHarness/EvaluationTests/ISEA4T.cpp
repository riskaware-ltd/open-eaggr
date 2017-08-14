//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Evaluation Framework
//
//------------------------------------------------------
/// @file ISEA4T.cpp
/// 
/// System tests for evaluating the ISEA4T DGGS with
/// hierarchical grid indexing.
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
#include "TestMacros.hpp"
#include "EvaluationTests/Performance.hpp"

#include "Src/Model/DGGS.hpp"
#include "Src/Model/IGrid/IHierarchicalGrid/Aperture4TriangleGrid.hpp"
#include "Src/Model/IGridIndexer/HierarchicalGridIndexer.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/Model/IProjection/Snyder.hpp"

using namespace EAGGR;
using namespace EAGGR::Utilities::Maths;

EVALUATION_TEST(ISEA4T, ConvertPointsWorldwide)
{
  Model::PolyhedralGlobe::Icosahedron globe;
  Model::Projection::Snyder snyder(&globe);
  Model::Grid::HierarchicalGrid::Aperture4TriangleGrid grid;
  Model::GridIndexer::HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  Model::DGGS dggs(&snyder, &indexer);

  EvaluationFramework::Performance performance(dggs);
  performance.ConvertPointsWorldwide();
}
