//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file SpatialAnalysisTest.cpp
/// 
/// Tests for the EAGGR::SpatialAnalysis::SpatialAnalysis class.
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

#include "Src/SpatialAnalysis/SpatialAnalysis.hpp"
#include "Src/Model/IGridIndexer/HierarchicalGridIndexer.hpp"
#include "Src/Model/IGridIndexer/OffsetGridIndexer.hpp"
#include "Src/Model/IGrid/IHierarchicalGrid/Aperture4TriangleGrid.hpp"
#include "Src/Model/IGrid/IOffsetGrid/Aperture3HexagonGrid.hpp"
#include "Src/Model/IProjection/Snyder.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"

using namespace EAGGR::Model::Cell;
using namespace EAGGR::Model::Grid;
using namespace EAGGR::Model::GridIndexer;
using namespace EAGGR::Model::Projection;
using namespace EAGGR::Model::PolyhedralGlobe;
using namespace EAGGR::SpatialAnalysis;

static const unsigned short MAX_FACE_INDEX = 19U;

class TestDataTriangle
{
  public:
    TestDataTriangle(IGridIndexer & a_indexer)
    {
      // Populate a linestring for comparing with
      linestring.push_back(a_indexer.CreateCell("0001"));
      linestring.push_back(a_indexer.CreateCell("0002"));
      linestring.push_back(a_indexer.CreateCell("0003"));

      // Populate a linestring that touches a cell vertex, for comparing with
      touchingCellVertexLinestring.push_back(a_indexer.CreateCell("0003"));
      touchingCellVertexLinestring.push_back(a_indexer.CreateCell("0012"));

      // Populate the outer ring of a polygon for comparing with
      outerLinestring.push_back(a_indexer.CreateCell("00111"));
      outerLinestring.push_back(a_indexer.CreateCell("00022"));
      outerLinestring.push_back(a_indexer.CreateCell("00011"));
      outerLinestring.push_back(a_indexer.CreateCell("00333"));

      // Populate the inner ring of a polygon for comparing with
      innerRings.at(0).push_back(a_indexer.CreateCell("0010"));
      innerRings.at(0).push_back(a_indexer.CreateCell("0003"));
      innerRings.at(0).push_back(a_indexer.CreateCell("0000"));
      innerRings.at(0).push_back(a_indexer.CreateCell("00023"));
      innerRings.at(0).push_back(a_indexer.CreateCell("00123"));

      innerRings.at(1).push_back(a_indexer.CreateCell("00003"));
      innerRings.at(1).push_back(a_indexer.CreateCell("00031"));
      innerRings.at(1).push_back(a_indexer.CreateCell("00302"));

      // Populate the outer ring of a polygon for comparing with that touches the vertex of a cell
      touchingCellVertexOuterLinestring.push_back(a_indexer.CreateCell("0013"));
      touchingCellVertexOuterLinestring.push_back(a_indexer.CreateCell("0002"));
      touchingCellVertexOuterLinestring.push_back(a_indexer.CreateCell("00011"));
      touchingCellVertexOuterLinestring.push_back(a_indexer.CreateCell("00333"));

      // Populate the inner ring of a polygon for comparing with that touches the vertex of a cell
      touchingCellVertexInnerRings.at(0).push_back(a_indexer.CreateCell("000031"));
      touchingCellVertexInnerRings.at(0).push_back(a_indexer.CreateCell("000032"));
      touchingCellVertexInnerRings.at(0).push_back(a_indexer.CreateCell("000033"));

      // Populate linestrings for cell/linestring comparison
      // Populate a linestring that is contained within the cell
      linestringWithinCell.push_back(a_indexer.CreateCell("00001"));
      linestringWithinCell.push_back(a_indexer.CreateCell("00002"));
      linestringWithinCell.push_back(a_indexer.CreateCell("00003"));

      // Populate a linestring constructed using the cell at an end
      linestringWithCellEnd.push_back(a_indexer.CreateCell("0000"));
      linestringWithCellEnd.push_back(a_indexer.CreateCell("0001"));
      linestringWithCellEnd.push_back(a_indexer.CreateCell("0002"));

      // Populate a linestring constructed using the cell in the middle
      linestringWithCellInMiddle.push_back(a_indexer.CreateCell("0003"));
      linestringWithCellInMiddle.push_back(a_indexer.CreateCell("0000"));
      linestringWithCellInMiddle.push_back(a_indexer.CreateCell("0001"));

      // Populate a linestring that crosses the cell, but not through its centre
      linestringCrossesCell.push_back(a_indexer.CreateCell("0001"));
      linestringCrossesCell.push_back(a_indexer.CreateCell("0010"));

      // Populate a disjoint linestring
      linestringDisjointFromCell.push_back(a_indexer.CreateCell("0022"));
      linestringDisjointFromCell.push_back(a_indexer.CreateCell("0021"));
      linestringDisjointFromCell.push_back(a_indexer.CreateCell("0023"));

      // Populate a linestring that touches the vertex of a cell
      linestringTouchesCellVertex.push_back(a_indexer.CreateCell("0003"));
      linestringTouchesCellVertex.push_back(a_indexer.CreateCell("0012"));

      // Populate outer and inner rings for cell/polygon comparison

      // Populate a polygon that has no inner rings and has the cell on its outer ring

      polygonWithCellInBoundaryNoInnerOuterLinestring.push_back(a_indexer.CreateCell("0000"));
      polygonWithCellInBoundaryNoInnerOuterLinestring.push_back(a_indexer.CreateCell("0001"));
      polygonWithCellInBoundaryNoInnerOuterLinestring.push_back(a_indexer.CreateCell("0002"));

      // Populate a polygon that has inner rings and has the cell on its outer ring

      polygonWithCellInBoundaryWithInnerOuterLinestring.push_back(a_indexer.CreateCell("0000"));
      polygonWithCellInBoundaryWithInnerOuterLinestring.push_back(a_indexer.CreateCell("00011"));
      polygonWithCellInBoundaryWithInnerOuterLinestring.push_back(a_indexer.CreateCell("00333"));
      polygonWithCellInBoundaryWithInnerOuterLinestring.push_back(a_indexer.CreateCell("00111"));

      polygonWithCellInBoundaryWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00003"));
      polygonWithCellInBoundaryWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00013"));
      polygonWithCellInBoundaryWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00031"));

      // Populate a polygon that has the cell on its inner ring
      polygonWithCellOnInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00111"));
      polygonWithCellOnInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonWithCellOnInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00333"));

      polygonWithCellOnInnerInnerRings.at(0).push_back(a_indexer.CreateCell("0000"));
      polygonWithCellOnInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00013"));
      polygonWithCellOnInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00031"));

      // Populate a polygon that has no inner rings and is within the cell
      polygonWithinCellNoInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00001"));
      polygonWithinCellNoInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00002"));
      polygonWithinCellNoInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00003"));

      // Populate a polygon that has an inner ring and is within the cell
      polygonWithinCellWithInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00001"));
      polygonWithinCellWithInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00002"));
      polygonWithinCellWithInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00003"));

      polygonWithinCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("000001"));
      polygonWithinCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("000002"));
      polygonWithinCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("000003"));

      // Populate a polygon that has no inner rings and covers the cell
      polygonCoversCellNoInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00111"));
      polygonCoversCellNoInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonCoversCellNoInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00333"));

      // Populate a polygon that has an inner ring and covers the cell
      polygonCoversCellWithInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00111"));
      polygonCoversCellWithInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonCoversCellWithInnerRingOuterLinestring.push_back(a_indexer.CreateCell("00333"));

      polygonCoversCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("002031"));
      polygonCoversCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("002032"));
      polygonCoversCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("002033"));

      // Populate a polygon that has the cell within a hole created by an inner ring
      polygonCellInHoleOuterLinestring.push_back(a_indexer.CreateCell("00111"));
      polygonCellInHoleOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonCellInHoleOuterLinestring.push_back(a_indexer.CreateCell("00333"));

      polygonCellInHoleInnerRings.at(0).push_back(a_indexer.CreateCell("00100"));
      polygonCellInHoleInnerRings.at(0).push_back(a_indexer.CreateCell("00200"));
      polygonCellInHoleInnerRings.at(0).push_back(a_indexer.CreateCell("00300"));

      // Populate a polygon that has no inner rings and is disjoint from the cell
      polygonDisjointFromCellOuterLinestring.push_back(a_indexer.CreateCell("0021"));
      polygonDisjointFromCellOuterLinestring.push_back(a_indexer.CreateCell("0022"));
      polygonDisjointFromCellOuterLinestring.push_back(a_indexer.CreateCell("0023"));

      // Populate a polygon that has an inner ring and is disjoint from the cell
      polygonDisjointFromCellWithInnerOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonDisjointFromCellWithInnerOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonDisjointFromCellWithInnerOuterLinestring.push_back(a_indexer.CreateCell("00233"));

      polygonDisjointFromCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00201"));
      polygonDisjointFromCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00202"));
      polygonDisjointFromCellWithInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00203"));

      // Populate a polygon that has no inner rings and touches the vertex of a cell
      polygonCellVertexTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("0031"));
      polygonCellVertexTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("0001"));
      polygonCellVertexTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00333"));

      // Populate a polygon that has an inner ring and touches the vertex of a cell
      polygonCellVertexTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("0031"));
      polygonCellVertexTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("0001"));
      polygonCellVertexTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00333"));

      polygonCellVertexTouchWithInnerRingsInnerRings.at(0).push_back(
          a_indexer.CreateCell("003001"));
      polygonCellVertexTouchWithInnerRingsInnerRings.at(0).push_back(
          a_indexer.CreateCell("003002"));
      polygonCellVertexTouchWithInnerRingsInnerRings.at(0).push_back(
          a_indexer.CreateCell("003003"));

      // Populate linestrings for linestring/linestring comparison
      // Populate a linestring which is a sub-string of the linestring
      subLinestring.push_back(a_indexer.CreateCell("0001"));
      subLinestring.push_back(a_indexer.CreateCell("0002"));

      // Populate a linestring that covers the linestring
      coversLinestring.push_back(a_indexer.CreateCell("0022"));
      coversLinestring.push_back(a_indexer.CreateCell("0001"));
      coversLinestring.push_back(a_indexer.CreateCell("0002"));
      coversLinestring.push_back(a_indexer.CreateCell("0003"));
      coversLinestring.push_back(a_indexer.CreateCell("0033"));

      // Populate a linestring that intersects at the middle cell used to define the two linestrings
      middleCellIntersectLinestring.push_back(a_indexer.CreateCell("0000"));
      middleCellIntersectLinestring.push_back(a_indexer.CreateCell("0002"));
      middleCellIntersectLinestring.push_back(a_indexer.CreateCell("0012"));

      // Populate a linestring that intersects at a location that isn't the centre of a cell used to define
      // either linestring
      notCellIntersectLinestring.push_back(a_indexer.CreateCell("0000"));
      notCellIntersectLinestring.push_back(a_indexer.CreateCell("0010"));

      // Populate a linestring that touches the end of the linestring with an end
      endTouchLinestring.push_back(a_indexer.CreateCell("0022"));
      endTouchLinestring.push_back(a_indexer.CreateCell("0020"));
      endTouchLinestring.push_back(a_indexer.CreateCell("0001"));

      // Populate a linestring that touches the middle of the linestring with an end
      middleTouchLinestring.push_back(a_indexer.CreateCell("0022"));
      middleTouchLinestring.push_back(a_indexer.CreateCell("0002"));
      middleTouchLinestring.push_back(a_indexer.CreateCell("0011"));

      // Populate a linestring that touches the linestring at multiple points including
      // interior and boundary points
      multiTouchLinestring.push_back(a_indexer.CreateCell("0033"));
      multiTouchLinestring.push_back(a_indexer.CreateCell("0001"));
      multiTouchLinestring.push_back(a_indexer.CreateCell("0022"));
      multiTouchLinestring.push_back(a_indexer.CreateCell("0002"));
      multiTouchLinestring.push_back(a_indexer.CreateCell("0011"));

      // Populate a linestring that touches both boundary points of the linestring
      boundariesTouchLinestring.push_back(a_indexer.CreateCell("0001"));
      boundariesTouchLinestring.push_back(a_indexer.CreateCell("0022"));
      boundariesTouchLinestring.push_back(a_indexer.CreateCell("0011"));
      boundariesTouchLinestring.push_back(a_indexer.CreateCell("0003"));

      // Populate a linestring that partially intersects
      partialOverlapLinestring.push_back(a_indexer.CreateCell("0022"));
      partialOverlapLinestring.push_back(a_indexer.CreateCell("0001"));
      partialOverlapLinestring.push_back(a_indexer.CreateCell("0002"));

      // Populate a disjoint linestring
      disjointLinestring.push_back(a_indexer.CreateCell("0020"));
      disjointLinestring.push_back(a_indexer.CreateCell("0022"));
      disjointLinestring.push_back(a_indexer.CreateCell("0023"));

      // Populate outer and inner rings for linestring/polygon comparison

      // Populate a polygon that has no inner rings and the linestring runs along the outer ring boundary
      polygonWithNoInnerLinestringInBoundaryOuterLinestring.push_back(a_indexer.CreateCell("0001"));
      polygonWithNoInnerLinestringInBoundaryOuterLinestring.push_back(a_indexer.CreateCell("0002"));
      polygonWithNoInnerLinestringInBoundaryOuterLinestring.push_back(a_indexer.CreateCell("0003"));

      // Populate a polygon that has an inner ring and the linestring runs along its outer ring boundary
      polygonWithInnerLinestringInOuterBoundaryOuterLinestring.push_back(
          a_indexer.CreateCell("0001"));
      polygonWithInnerLinestringInOuterBoundaryOuterLinestring.push_back(
          a_indexer.CreateCell("0002"));
      polygonWithInnerLinestringInOuterBoundaryOuterLinestring.push_back(
          a_indexer.CreateCell("0003"));

      polygonWithInnerLinestringInOuterBoundaryInnerRings.at(0).push_back(
          a_indexer.CreateCell("00001"));
      polygonWithInnerLinestringInOuterBoundaryInnerRings.at(0).push_back(
          a_indexer.CreateCell("00002"));
      polygonWithInnerLinestringInOuterBoundaryInnerRings.at(0).push_back(
          a_indexer.CreateCell("00003"));

      // Populate a polygon that has an inner ring and the linestring runs along its inner ring boundary
      polygonWithInnerLinestringInInnerBoundaryOuterLinestring.push_back(
          a_indexer.CreateCell("0011"));
      polygonWithInnerLinestringInInnerBoundaryOuterLinestring.push_back(
          a_indexer.CreateCell("0022"));
      polygonWithInnerLinestringInInnerBoundaryOuterLinestring.push_back(
          a_indexer.CreateCell("0033"));

      polygonWithInnerLinestringInInnerBoundaryInnerRings.at(0).push_back(
          a_indexer.CreateCell("0001"));
      polygonWithInnerLinestringInInnerBoundaryInnerRings.at(0).push_back(
          a_indexer.CreateCell("0002"));
      polygonWithInnerLinestringInInnerBoundaryInnerRings.at(0).push_back(
          a_indexer.CreateCell("0003"));

      // Populate a polygon that has no inner rings and the linestring lies within its interior
      polygonNoInnerLinestringInInteriorOuterLinestring.push_back(a_indexer.CreateCell("0011"));
      polygonNoInnerLinestringInInteriorOuterLinestring.push_back(a_indexer.CreateCell("0022"));
      polygonNoInnerLinestringInInteriorOuterLinestring.push_back(a_indexer.CreateCell("0033"));

      // Populate a polygon that has an inner ring and the linestring lies within its interior
      polygonWithInnerLinestringInInteriorOuterLinestring.push_back(a_indexer.CreateCell("0011"));
      polygonWithInnerLinestringInInteriorOuterLinestring.push_back(a_indexer.CreateCell("0022"));
      polygonWithInnerLinestringInInteriorOuterLinestring.push_back(a_indexer.CreateCell("0033"));

      polygonWithInnerLinestringInInteriorInnerRings.at(0).push_back(a_indexer.CreateCell("00001"));
      polygonWithInnerLinestringInInteriorInnerRings.at(0).push_back(a_indexer.CreateCell("00002"));
      polygonWithInnerLinestringInInteriorInnerRings.at(0).push_back(a_indexer.CreateCell("00003"));

      // Populate a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
      polygonNoInnerLinestringOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("0001"));
      polygonNoInnerLinestringOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00322"));
      polygonNoInnerLinestringOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00333"));
      polygonNoInnerLinestringOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00311"));

      // Populate a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
      polygonWithInnerLinestringOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("0001"));
      polygonWithInnerLinestringOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00322"));
      polygonWithInnerLinestringOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00333"));
      polygonWithInnerLinestringOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00311"));

      polygonWithInnerLinestringInInteriorInnerRings.at(0).push_back(
          a_indexer.CreateCell("003001"));
      polygonWithInnerLinestringInInteriorInnerRings.at(0).push_back(
          a_indexer.CreateCell("003002"));
      polygonWithInnerLinestringInInteriorInnerRings.at(0).push_back(
          a_indexer.CreateCell("003003"));

      // Populate a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
      polygonWithInnerLinestringInnerTouchOuterLinestring.push_back(a_indexer.CreateCell("000111"));
      polygonWithInnerLinestringInnerTouchOuterLinestring.push_back(a_indexer.CreateCell("000222"));
      polygonWithInnerLinestringInnerTouchOuterLinestring.push_back(a_indexer.CreateCell("000333"));

      polygonWithInnerLinestringInnerTouchInnerRings.at(0).push_back(a_indexer.CreateCell("00011"));
      polygonWithInnerLinestringInnerTouchInnerRings.at(0).push_back(a_indexer.CreateCell("0003"));
      polygonWithInnerLinestringInnerTouchInnerRings.at(0).push_back(a_indexer.CreateCell("00022"));

      // Populate a polygon that has no inner rings and the linestring goes from the exterior and
      // partially runs along the outer boundary
      polygonNoInnerLinestringPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0001"));
      polygonNoInnerLinestringPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0002"));
      polygonNoInnerLinestringPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0022"));

      // Populate a polygon that has an inner ring and the linestring goes from the exterior and
      // partially runs along the outer boundary
      polygonWithInnerLinestringPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0001"));
      polygonWithInnerLinestringPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0002"));
      polygonWithInnerLinestringPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00211"));
      polygonWithInnerLinestringPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0022"));
      polygonWithInnerLinestringPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00233"));

      polygonWithInnerLinestringPartialOuterOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("002031"));
      polygonWithInnerLinestringPartialOuterOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("002032"));
      polygonWithInnerLinestringPartialOuterOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("002033"));

      // Populate a polygon that has an inner ring and the linestring goes from the exterior and
      // partially runs along the inner boundary
      polygonWithInnerLinestringPartialInnerOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("000111"));
      polygonWithInnerLinestringPartialInnerOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("000222"));
      polygonWithInnerLinestringPartialInnerOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("000333"));

      polygonWithInnerLinestringPartialInnerOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("00011"));
      polygonWithInnerLinestringPartialInnerOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("0003"));
      polygonWithInnerLinestringPartialInnerOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("0002"));

      // Populate a polygon that has no inner rings and the linestring goes from the interior and
      // partially runs along the outer boundary
      polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0001"));
      polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0002"));
      polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00111"));
      polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00333"));

      // Populate a polygon that has an inner ring and the linestring goes from the interior and
      // partially runs along the outer boundary
      polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0001"));
      polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("0002"));
      polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00111"));
      polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00333"));

      polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("003021"));
      polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("003022"));
      polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("003023"));

      // Populate a polygon that has an inner ring and the linestring goes from the interior and
      // partially runs along the inner ring boundary
      polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00111"));
      polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00222"));
      polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00333"));

      polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("0001"));
      polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("0002"));
      polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings.at(0).push_back(
          a_indexer.CreateCell("0020"));

      // Populate a polygon that has no inner rings and the linestring goes from the exterior to the interior,
      // crossing at a single boundary point
      polygonNoInnerLinestringOutToInOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00222"));
      polygonNoInnerLinestringOutToInOverlapOuterLinestring.push_back(a_indexer.CreateCell("0000"));
      polygonNoInnerLinestringOutToInOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00333"));
      polygonNoInnerLinestringOutToInOverlapOuterLinestring.push_back(
          a_indexer.CreateCell("00111"));

      // Populate a polygon that has an inner ring and the linestring goes from the exterior to the interior,
      // crossing at a single boundary point on the outer ring
      polygonWithInnerLinestringCrossOuterOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonWithInnerLinestringCrossOuterOuterLinestring.push_back(a_indexer.CreateCell("0000"));
      polygonWithInnerLinestringCrossOuterOuterLinestring.push_back(a_indexer.CreateCell("00333"));
      polygonWithInnerLinestringCrossOuterOuterLinestring.push_back(a_indexer.CreateCell("00111"));

      polygonWithInnerLinestringCrossOuterInnerRings.at(0).push_back(
          a_indexer.CreateCell("003001"));
      polygonWithInnerLinestringCrossOuterInnerRings.at(0).push_back(
          a_indexer.CreateCell("003002"));
      polygonWithInnerLinestringCrossOuterInnerRings.at(0).push_back(
          a_indexer.CreateCell("003003"));

      // Populate a polygon that has an inner ring and the linestring goes from the exterior to the interior,
      // crossing at a single boundary point on the inner ring
      polygonWithInnerLinestringCrossInnerOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonWithInnerLinestringCrossInnerOuterLinestring.push_back(a_indexer.CreateCell("00333"));
      polygonWithInnerLinestringCrossInnerOuterLinestring.push_back(a_indexer.CreateCell("00111"));

      polygonWithInnerLinestringCrossInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00031"));
      polygonWithInnerLinestringCrossInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00032"));
      polygonWithInnerLinestringCrossInnerInnerRings.at(0).push_back(a_indexer.CreateCell("00033"));

      // Populate a polygon that has no inner rings and is disjoint from the linestring
      polygonNoInnerDisjointFromLinestringOuterLinestring.push_back(a_indexer.CreateCell("0023"));
      polygonNoInnerDisjointFromLinestringOuterLinestring.push_back(a_indexer.CreateCell("0021"));
      polygonNoInnerDisjointFromLinestringOuterLinestring.push_back(a_indexer.CreateCell("0022"));

      // Populate a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
      // lie in the hole created by the inner ring
      polygonWithInnerDisjointFromLinestringOuterLinestring.push_back(a_indexer.CreateCell("0023"));
      polygonWithInnerDisjointFromLinestringOuterLinestring.push_back(a_indexer.CreateCell("0021"));
      polygonWithInnerDisjointFromLinestringOuterLinestring.push_back(a_indexer.CreateCell("0022"));

      polygonWithInnerDisjointFromLinestringInnerRings.at(0).push_back(
          a_indexer.CreateCell("00201"));
      polygonWithInnerDisjointFromLinestringInnerRings.at(0).push_back(
          a_indexer.CreateCell("00202"));
      polygonWithInnerDisjointFromLinestringInnerRings.at(0).push_back(
          a_indexer.CreateCell("00203"));

      // Populate a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
      // within the hole created by the inner ring
      polygonWithInnerDisjointFromLinestringInHoleOuterLinestring.push_back(
          a_indexer.CreateCell("001111"));
      polygonWithInnerDisjointFromLinestringInHoleOuterLinestring.push_back(
          a_indexer.CreateCell("002222"));
      polygonWithInnerDisjointFromLinestringInHoleOuterLinestring.push_back(
          a_indexer.CreateCell("003333"));

      polygonWithInnerDisjointFromLinestringInHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("00111"));
      polygonWithInnerDisjointFromLinestringInHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("00222"));
      polygonWithInnerDisjointFromLinestringInHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("00333"));

      // Populate the linestrings for polygon/linestring comparison
      // Populate the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
      noInnerRingsOuterBoundaryLinestring.push_back(a_indexer.CreateCell("00111"));
      noInnerRingsOuterBoundaryLinestring.push_back(a_indexer.CreateCell("00022"));
      noInnerRingsOuterBoundaryLinestring.push_back(a_indexer.CreateCell("00011"));

      // Populate the polygon that has inner rings, to a linestring that runs along the outer ring boundary
      withInnerRingsOuterBoundaryLinestring.push_back(a_indexer.CreateCell("00111"));
      withInnerRingsOuterBoundaryLinestring.push_back(a_indexer.CreateCell("00022"));
      withInnerRingsOuterBoundaryLinestring.push_back(a_indexer.CreateCell("00011"));

      // Populate the polygon that has inner rings, to a linestring that runs along the inner ring boundary
      withInnerRingsInnerBoundaryLinestring.push_back(a_indexer.CreateCell("00003"));
      withInnerRingsInnerBoundaryLinestring.push_back(a_indexer.CreateCell("00031"));
      withInnerRingsInnerBoundaryLinestring.push_back(a_indexer.CreateCell("00302"));

      // Populate the polygon that has no inner rings, to a linestring that lies within its interior
      noInnerRingsInteriorLinestring.push_back(a_indexer.CreateCell("00010"));
      noInnerRingsInteriorLinestring.push_back(a_indexer.CreateCell("00013"));
      noInnerRingsInteriorLinestring.push_back(a_indexer.CreateCell("00320"));

      // Populate the polygon that has inner rings, to a linestring that lies within its interior
      withInnerRingsInteriorLinestring.push_back(a_indexer.CreateCell("00010"));
      withInnerRingsInteriorLinestring.push_back(a_indexer.CreateCell("00013"));
      withInnerRingsInteriorLinestring.push_back(a_indexer.CreateCell("00320"));

      // Populate the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
      noInnerRingsTouchOuterLinestring.push_back(a_indexer.CreateCell("0022"));
      noInnerRingsTouchOuterLinestring.push_back(a_indexer.CreateCell("0021"));
      noInnerRingsTouchOuterLinestring.push_back(a_indexer.CreateCell("00022"));

      // Populate the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
      withInnerRingsTouchOuterLinestring.push_back(a_indexer.CreateCell("0022"));
      withInnerRingsTouchOuterLinestring.push_back(a_indexer.CreateCell("0021"));
      withInnerRingsTouchOuterLinestring.push_back(a_indexer.CreateCell("00022"));

      // Populate the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
      withInnerRingsTouchInnerLinestring.push_back(a_indexer.CreateCell("00001"));
      withInnerRingsTouchInnerLinestring.push_back(a_indexer.CreateCell("00032"));
      withInnerRingsTouchInnerLinestring.push_back(a_indexer.CreateCell("00132"));

      // Populate the polygon that has no inner rings, to a linestring that goes from the exterior and partially
      // runs along the outer ring
      noInnerRingsExteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("0022"));
      noInnerRingsExteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00022"));
      noInnerRingsExteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00011"));

      // Populate the polygon that has inner rings, to a linestring that goes from the exterior and partially
      // runs along the outer ring
      withInnerRingsExteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("0022"));
      withInnerRingsExteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00022"));
      withInnerRingsExteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00011"));

      // Populate the polygon that has inner rings, to a linestring that goes from the exterior and partially
      // runs along the inner ring
      withInnerRingsExteriorToInnerRingLinestring.push_back(a_indexer.CreateCell("00001"));
      withInnerRingsExteriorToInnerRingLinestring.push_back(a_indexer.CreateCell("0003"));
      withInnerRingsExteriorToInnerRingLinestring.push_back(a_indexer.CreateCell("0000"));

      // Populate the polygon that has no inner rings, to a linestring that goes from the interior and partially
      // runs along the outer ring
      noInnerRingsInteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00320"));
      noInnerRingsInteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00022"));
      noInnerRingsInteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00011"));

      // Populate the polygon that has inner rings, to a linestring that goes from the interior and partially
      // runs along the outer ring
      withInnerRingsInteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00320"));
      withInnerRingsInteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00022"));
      withInnerRingsInteriorToOuterRingLinestring.push_back(a_indexer.CreateCell("00011"));

      // Populate the polygon that has inner rings, to a linestring that goes from the interior and partially
      // runs along the inner ring
      withInnerRingsInteriorToInnerRingLinestring.push_back(a_indexer.CreateCell("00320"));
      withInnerRingsInteriorToInnerRingLinestring.push_back(a_indexer.CreateCell("00302"));
      withInnerRingsInteriorToInnerRingLinestring.push_back(a_indexer.CreateCell("00003"));

      // Populate the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
      // crossing at a single point on the outer ring
      noInnerRingsExteriorToInteriorViaOuterLinestring.push_back(a_indexer.CreateCell("00320"));
      noInnerRingsExteriorToInteriorViaOuterLinestring.push_back(a_indexer.CreateCell("00011"));
      noInnerRingsExteriorToInteriorViaOuterLinestring.push_back(a_indexer.CreateCell("0023"));

      // Populate the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
      // crossing at a single point on the outer ring
      withInnerRingsExteriorToInteriorViaOuterLinestring.push_back(a_indexer.CreateCell("00320"));
      withInnerRingsExteriorToInteriorViaOuterLinestring.push_back(a_indexer.CreateCell("00011"));
      withInnerRingsExteriorToInteriorViaOuterLinestring.push_back(a_indexer.CreateCell("0023"));

      // Populate the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
      // crossing at a single point on the inner ring
      withInnerRingsExteriorToInteriorViaInnerLinestring.push_back(a_indexer.CreateCell("00320"));
      withInnerRingsExteriorToInteriorViaInnerLinestring.push_back(a_indexer.CreateCell("0000"));
      withInnerRingsExteriorToInteriorViaInnerLinestring.push_back(a_indexer.CreateCell("00032"));

      // Populate the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
      noInnerRingsDisjointLinestring.push_back(a_indexer.CreateCell("0021"));
      noInnerRingsDisjointLinestring.push_back(a_indexer.CreateCell("0022"));
      noInnerRingsDisjointLinestring.push_back(a_indexer.CreateCell("0023"));

      // Populate the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
      withInnerRingsDisjointLinestring.push_back(a_indexer.CreateCell("0021"));
      withInnerRingsDisjointLinestring.push_back(a_indexer.CreateCell("0022"));
      withInnerRingsDisjointLinestring.push_back(a_indexer.CreateCell("0023"));

      // Populate the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
      withInnerRingsDisjointInHoleLinestring.push_back(a_indexer.CreateCell("000322"));
      withInnerRingsDisjointInHoleLinestring.push_back(a_indexer.CreateCell("001322"));
      withInnerRingsDisjointInHoleLinestring.push_back(a_indexer.CreateCell("001011"));

      // Populates outer and inner rings for polygon/polygon comparison

      // Populate the polygon with inner rings, to the same polygon with a ring removed
      oneRingInnerRings.at(0).push_back(a_indexer.CreateCell("0010"));
      oneRingInnerRings.at(0).push_back(a_indexer.CreateCell("0003"));
      oneRingInnerRings.at(0).push_back(a_indexer.CreateCell("0000"));
      oneRingInnerRings.at(0).push_back(a_indexer.CreateCell("00023"));
      oneRingInnerRings.at(0).push_back(a_indexer.CreateCell("00123"));

      // Populate the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
      // outer ring
      polygonNoInnerRingsOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonNoInnerRingsOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonNoInnerRingsOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00233"));
      polygonNoInnerRingsOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00022"));

      // Populate the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
      // outer ring
      polygonWithInnerRingsOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonWithInnerRingsOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonWithInnerRingsOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00233"));
      polygonWithInnerRingsOuterTouchOuterLinestring.push_back(a_indexer.CreateCell("00022"));

      polygonWithInnerRingsOuterTouchInnerRings.at(0).push_back(a_indexer.CreateCell("00201"));
      polygonWithInnerRingsOuterTouchInnerRings.at(0).push_back(a_indexer.CreateCell("00202"));
      polygonWithInnerRingsOuterTouchInnerRings.at(0).push_back(a_indexer.CreateCell("00203"));

      // Populate the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
      // outer ring
      polygonNoInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonNoInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonNoInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00233"));
      polygonNoInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00011"));
      polygonNoInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00022"));

      // Populate the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
      // outer ring
      polygonWithInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonWithInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonWithInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00233"));
      polygonWithInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00011"));
      polygonWithInnerRingsOuterEdgeOuterLinestring.push_back(a_indexer.CreateCell("00022"));

      polygonWithInnerRingsOuterEdgeInnerRings.at(0).push_back(a_indexer.CreateCell("00201"));
      polygonWithInnerRingsOuterEdgeInnerRings.at(0).push_back(a_indexer.CreateCell("00202"));
      polygonWithInnerRingsOuterEdgeInnerRings.at(0).push_back(a_indexer.CreateCell("00203"));

      // Populate the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
      polygonNoInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonNoInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonNoInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00233"));
      polygonNoInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00011"));
      polygonNoInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00010"));
      polygonNoInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00020"));
      polygonNoInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00022"));

      // Populate the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
      polygonWithInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonWithInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonWithInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00233"));
      polygonWithInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00011"));
      polygonWithInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00010"));
      polygonWithInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00020"));
      polygonWithInnerRingsInteriorOverlapOuterLinestring.push_back(a_indexer.CreateCell("00022"));

      polygonWithInnerRingsInteriorOverlapInnerRings.at(0).push_back(a_indexer.CreateCell("00201"));
      polygonWithInnerRingsInteriorOverlapInnerRings.at(0).push_back(a_indexer.CreateCell("00202"));
      polygonWithInnerRingsInteriorOverlapInnerRings.at(0).push_back(a_indexer.CreateCell("00203"));

      // Populate the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
      // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
      polygonWithInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00211"));
      polygonWithInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00222"));
      polygonWithInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00233"));
      polygonWithInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00011"));
      polygonWithInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00010"));
      polygonWithInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00020"));
      polygonWithInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00022"));

      polygonWithInnerRingsInnerRingTouchOuterInnerRings.at(0).push_back(
          a_indexer.CreateCell("00201"));
      polygonWithInnerRingsInnerRingTouchOuterInnerRings.at(0).push_back(
          a_indexer.CreateCell("00202"));
      polygonWithInnerRingsInnerRingTouchOuterInnerRings.at(0).push_back(
          a_indexer.CreateCell("00203"));
      polygonWithInnerRingsInnerRingTouchOuterInnerRings.at(0).push_back(
          a_indexer.CreateCell("00011"));

      // Populate the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
      // whose interior intersects the exterior of the former polygon, found within its inner ring
      polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring.push_back(
          a_indexer.CreateCell("00211"));
      polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring.push_back(
          a_indexer.CreateCell("00222"));
      polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring.push_back(
          a_indexer.CreateCell("00233"));
      polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring.push_back(
          a_indexer.CreateCell("00011"));
      polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring.push_back(
          a_indexer.CreateCell("000011"));
      polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring.push_back(
          a_indexer.CreateCell("00022"));

      polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings.at(0).push_back(
          a_indexer.CreateCell("00201"));
      polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings.at(0).push_back(
          a_indexer.CreateCell("00202"));
      polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings.at(0).push_back(
          a_indexer.CreateCell("00203"));

      // Populate the polygon that has inner rings, to a polygon with no inner ring,
      // whose interior intersects the exterior of the former polygon, found within its inner ring
      polygonNoInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00211"));
      polygonNoInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00222"));
      polygonNoInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00233"));
      polygonNoInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00011"));
      polygonNoInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("000011"));
      polygonNoInnerRingsInnerRingTouchOuterOuterLinestring.push_back(
          a_indexer.CreateCell("00022"));

      // Populate the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
      polygonWithInnerRingsInteriorOuterLinestring.push_back(a_indexer.CreateCell("000113"));
      polygonWithInnerRingsInteriorOuterLinestring.push_back(a_indexer.CreateCell("000103"));
      polygonWithInnerRingsInteriorOuterLinestring.push_back(a_indexer.CreateCell("003212"));

      polygonWithInnerRingsInteriorInnerRings.at(0).push_back(a_indexer.CreateCell("0001133"));
      polygonWithInnerRingsInteriorInnerRings.at(0).push_back(a_indexer.CreateCell("0001033"));
      polygonWithInnerRingsInteriorInnerRings.at(0).push_back(a_indexer.CreateCell("0032122"));

      // Populate the polygon that has inner rings, to an interior polygon with no inner rings
      polygonNoInnerRingsInteriorOuterLinestring.push_back(a_indexer.CreateCell("000113"));
      polygonNoInnerRingsInteriorOuterLinestring.push_back(a_indexer.CreateCell("000103"));
      polygonNoInnerRingsInteriorOuterLinestring.push_back(a_indexer.CreateCell("003212"));

      // Populate the polygon that has inner rings, to a covering polygon with an inner ring
      // outside the former polygon
      polygonWithInnerRingsCoversOuterLinestring.push_back(a_indexer.CreateCell("001111"));
      polygonWithInnerRingsCoversOuterLinestring.push_back(a_indexer.CreateCell("002222"));
      polygonWithInnerRingsCoversOuterLinestring.push_back(a_indexer.CreateCell("003333"));

      polygonWithInnerRingsCoversInnerRings.at(0).push_back(a_indexer.CreateCell("00201"));
      polygonWithInnerRingsCoversInnerRings.at(0).push_back(a_indexer.CreateCell("00202"));
      polygonWithInnerRingsCoversInnerRings.at(0).push_back(a_indexer.CreateCell("00203"));

      // Populate the polygon that has inner rings, to a covering polygon with an inner ring
      // in the interior of the former polygon
      polygonWithInnerRingsCoversInteriorHoleOuterLinestring.push_back(
          a_indexer.CreateCell("001111"));
      polygonWithInnerRingsCoversInteriorHoleOuterLinestring.push_back(
          a_indexer.CreateCell("002222"));
      polygonWithInnerRingsCoversInteriorHoleOuterLinestring.push_back(
          a_indexer.CreateCell("003333"));

      polygonWithInnerRingsCoversInteriorHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("000113"));
      polygonWithInnerRingsCoversInteriorHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("000103"));
      polygonWithInnerRingsCoversInteriorHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("003212"));

      // Populate the polygon that has inner rings, to a covering polygon with an inner ring
      // inside an inner ring of the former polygon
      polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring.push_back(
          a_indexer.CreateCell("001111"));
      polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring.push_back(
          a_indexer.CreateCell("002222"));
      polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring.push_back(
          a_indexer.CreateCell("003333"));

      polygonWithInnerRingsCoversHoleWithinHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("001011"));
      polygonWithInnerRingsCoversHoleWithinHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("001322"));
      polygonWithInnerRingsCoversHoleWithinHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("000011"));

      // Populate the polygon that has inner rings, to a covering polygon with no inner rings
      polygonNoInnerRingsCoversOuterLinestring.push_back(a_indexer.CreateCell("001111"));
      polygonNoInnerRingsCoversOuterLinestring.push_back(a_indexer.CreateCell("002222"));
      polygonNoInnerRingsCoversOuterLinestring.push_back(a_indexer.CreateCell("003333"));

      // Populate the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
      polygonWithInnerRingsDisjointOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonWithInnerRingsDisjointOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonWithInnerRingsDisjointOuterLinestring.push_back(a_indexer.CreateCell("00233"));

      polygonWithInnerRingsDisjointInnerRings.at(0).push_back(a_indexer.CreateCell("00201"));
      polygonWithInnerRingsDisjointInnerRings.at(0).push_back(a_indexer.CreateCell("00202"));
      polygonWithInnerRingsDisjointInnerRings.at(0).push_back(a_indexer.CreateCell("00203"));

      // Populate the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
      polygonNoInnerRingsDisjointOuterLinestring.push_back(a_indexer.CreateCell("00211"));
      polygonNoInnerRingsDisjointOuterLinestring.push_back(a_indexer.CreateCell("00222"));
      polygonNoInnerRingsDisjointOuterLinestring.push_back(a_indexer.CreateCell("00233"));

      // Populate the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
      // by an inner ring of the former polygon
      polygonWithInnerRingsDisjointInHoleOuterLinestring.push_back(a_indexer.CreateCell("00101"));
      polygonWithInnerRingsDisjointInHoleOuterLinestring.push_back(a_indexer.CreateCell("001322"));
      polygonWithInnerRingsDisjointInHoleOuterLinestring.push_back(a_indexer.CreateCell("00032"));
      polygonWithInnerRingsDisjointInHoleOuterLinestring.push_back(a_indexer.CreateCell("00001"));

      polygonWithInnerRingsDisjointInHoleInnerRings.at(0).push_back(a_indexer.CreateCell("001011"));
      polygonWithInnerRingsDisjointInHoleInnerRings.at(0).push_back(
          a_indexer.CreateCell("0013222"));
      polygonWithInnerRingsDisjointInHoleInnerRings.at(0).push_back(a_indexer.CreateCell("000322"));
      polygonWithInnerRingsDisjointInHoleInnerRings.at(0).push_back(a_indexer.CreateCell("000011"));

      // Populate the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
      // by an inner ring of the former polygon
      polygonNoInnerRingsDisjointInHoleOuterLinestring.push_back(a_indexer.CreateCell("00101"));
      polygonNoInnerRingsDisjointInHoleOuterLinestring.push_back(a_indexer.CreateCell("001322"));
      polygonNoInnerRingsDisjointInHoleOuterLinestring.push_back(a_indexer.CreateCell("00032"));
      polygonNoInnerRingsDisjointInHoleOuterLinestring.push_back(a_indexer.CreateCell("00001"));

    }

    // Initialises linestrings for comparison
    std::vector<std::unique_ptr<ICell> > linestring;
    std::vector<std::unique_ptr<ICell> > touchingCellVertexLinestring;

    // Initialises outer and inner rings for comparison
    std::vector<std::unique_ptr<ICell> > outerLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > innerRings = std::vector
        < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (2);
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > emptyInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (0);
    std::vector<std::unique_ptr<ICell> > touchingCellVertexOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > touchingCellVertexInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonCellVertexTouchNoInnerRingsOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonCellVertexTouchWithInnerRingsInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonCellVertexTouchWithInnerRingsOuterLinestring;

    // Initialises linestrings for cell/linestring comparison
    std::vector<std::unique_ptr<ICell> > linestringWithinCell;
    std::vector<std::unique_ptr<ICell> > linestringWithCellEnd;
    std::vector<std::unique_ptr<ICell> > linestringWithCellInMiddle;
    std::vector<std::unique_ptr<ICell> > linestringCrossesCell;
    std::vector<std::unique_ptr<ICell> > linestringDisjointFromCell;
    std::vector<std::unique_ptr<ICell> > linestringTouchesCellVertex;

    // Initialises outer and inner rings for cell/polygon comparison
    std::vector<std::unique_ptr<ICell> > polygonWithCellInBoundaryNoInnerOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithCellInBoundaryWithInnerOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithCellInBoundaryWithInnerInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithCellOnInnerRingOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithCellOnInnerInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithinCellNoInnerRingOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithinCellWithInnerRingOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithinCellWithInnerInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonCoversCellNoInnerRingOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonCoversCellWithInnerRingOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonCoversCellWithInnerInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonCellInHoleOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonCellInHoleInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonDisjointFromCellOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonDisjointFromCellWithInnerOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonDisjointFromCellWithInnerInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);

    // Initialises linestrings for linestring/linestring comparison
    std::vector<std::unique_ptr<ICell> > subLinestring;
    std::vector<std::unique_ptr<ICell> > coversLinestring;
    std::vector<std::unique_ptr<ICell> > middleCellIntersectLinestring;
    std::vector<std::unique_ptr<ICell> > notCellIntersectLinestring;
    std::vector<std::unique_ptr<ICell> > endTouchLinestring;
    std::vector<std::unique_ptr<ICell> > middleTouchLinestring;
    std::vector<std::unique_ptr<ICell> > multiTouchLinestring;
    std::vector<std::unique_ptr<ICell> > boundariesTouchLinestring;
    std::vector<std::unique_ptr<ICell> > partialOverlapLinestring;
    std::vector<std::unique_ptr<ICell> > disjointLinestring;

    // Initialises outer and inner rings for linestring/polygon comparison
    std::vector<std::unique_ptr<ICell> > polygonWithNoInnerLinestringInBoundaryOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringInOuterBoundaryOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringInOuterBoundaryInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringInInnerBoundaryOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringInInnerBoundaryInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerLinestringInInteriorOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringInInteriorOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringInInteriorInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerLinestringOuterTouchOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringOuterTouchOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringOuterTouchInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringInnerTouchOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringInnerTouchInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerLinestringPartialOuterOverlapOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringPartialOuterOverlapOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringPartialOuterOverlapInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringPartialInnerOverlapOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringPartialInnerOverlapInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerLinestringOutToInOverlapOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringCrossOuterOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringCrossOuterInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerLinestringCrossInnerOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerLinestringCrossInnerInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerDisjointFromLinestringOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerDisjointFromLinestringOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerDisjointFromLinestringInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerDisjointFromLinestringInHoleOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerDisjointFromLinestringInHoleInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);

    // Initialises linestrings for polygon/linestring comparison
    std::vector<std::unique_ptr<ICell> > noInnerRingsOuterBoundaryLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsOuterBoundaryLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsInnerBoundaryLinestring;
    std::vector<std::unique_ptr<ICell> > noInnerRingsInteriorLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsInteriorLinestring;
    std::vector<std::unique_ptr<ICell> > noInnerRingsTouchOuterLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsTouchOuterLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsTouchInnerLinestring;
    std::vector<std::unique_ptr<ICell> > noInnerRingsExteriorToOuterRingLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsExteriorToOuterRingLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsExteriorToInnerRingLinestring;
    std::vector<std::unique_ptr<ICell> > noInnerRingsInteriorToOuterRingLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsInteriorToOuterRingLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsInteriorToInnerRingLinestring;
    std::vector<std::unique_ptr<ICell> > noInnerRingsExteriorToInteriorViaOuterLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsExteriorToInteriorViaOuterLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsExteriorToInteriorViaInnerLinestring;
    std::vector<std::unique_ptr<ICell> > noInnerRingsDisjointLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsDisjointLinestring;
    std::vector<std::unique_ptr<ICell> > withInnerRingsDisjointInHoleLinestring;

    // Initialises outer and inner rings for polygon/polygon comparison
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > oneRingInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerRingsOuterTouchOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsOuterTouchOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsOuterTouchInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerRingsOuterEdgeOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsOuterEdgeOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsOuterEdgeInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerRingsInteriorOverlapOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsInteriorOverlapOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsInteriorOverlapInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsInnerRingTouchOuterOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsInnerRingTouchOuterInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerRingsInnerRingTouchOuterOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsInteriorOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsInteriorInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerRingsInteriorOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsCoversOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsCoversInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsCoversInteriorHoleOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsCoversInteriorHoleInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsCoversHoleWithinHoleInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerRingsCoversOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsDisjointOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsDisjointInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerRingsDisjointOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonWithInnerRingsDisjointInHoleOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonWithInnerRingsDisjointInHoleInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonNoInnerRingsDisjointInHoleOuterLinestring;
};

class TestDataHexagonal
{
  public:
    TestDataHexagonal(IGridIndexer & a_indexer)
    {
      // Populate a linestring that touches a cell edge, for comparing with
      touchingCellEdgeLinestring.push_back(a_indexer.CreateCell("00021,0"));
      touchingCellEdgeLinestring.push_back(a_indexer.CreateCell("00022,2"));

      // Populate an outer ring of a polygon for comparing with that touches the edge of a cell
      touchingCellEdgeOuterLinestring.push_back(a_indexer.CreateCell("00022,1"));
      touchingCellEdgeOuterLinestring.push_back(a_indexer.CreateCell("00021,2"));
      touchingCellEdgeOuterLinestring.push_back(a_indexer.CreateCell("00021,3"));
      touchingCellEdgeOuterLinestring.push_back(a_indexer.CreateCell("00022,4"));
      touchingCellEdgeOuterLinestring.push_back(a_indexer.CreateCell("00023,3"));
      touchingCellEdgeOuterLinestring.push_back(a_indexer.CreateCell("00024,2"));

      // Populate the inner ring of a polygon for comparing with that touches the vertex of a cell
      touchingCellEdgeInnerRings.at(0).push_back(a_indexer.CreateCell("00022,2"));
      touchingCellEdgeInnerRings.at(0).push_back(a_indexer.CreateCell("00022,3"));
      touchingCellEdgeInnerRings.at(0).push_back(a_indexer.CreateCell("00023,2"));

      // Populate a linestring that touches the edge of a cell
      linestringTouchesCellEdge.push_back(a_indexer.CreateCell("00022,1"));
      linestringTouchesCellEdge.push_back(a_indexer.CreateCell("00021,2"));

      // Populate a polygon that has no inner rings and touches the edge of a cell
      polygonCellEdgeTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00022,1"));
      polygonCellEdgeTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00021,2"));
      polygonCellEdgeTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00021,3"));
      polygonCellEdgeTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00022,4"));
      polygonCellEdgeTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00023,3"));
      polygonCellEdgeTouchNoInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00024,2"));

      // Populate a polygon that has an inner ring and touches the edge of a cell
      polygonCellEdgeTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00022,1"));
      polygonCellEdgeTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00021,2"));
      polygonCellEdgeTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00021,3"));
      polygonCellEdgeTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00022,4"));
      polygonCellEdgeTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00023,3"));
      polygonCellEdgeTouchWithInnerRingsOuterLinestring.push_back(a_indexer.CreateCell("00024,2"));

      polygonCellEdgeTouchWithInnerRingsInnerRings.at(0).push_back(a_indexer.CreateCell("00022,2"));
      polygonCellEdgeTouchWithInnerRingsInnerRings.at(0).push_back(a_indexer.CreateCell("00022,3"));
      polygonCellEdgeTouchWithInnerRingsInnerRings.at(0).push_back(a_indexer.CreateCell("00023,2"));
    }

    // Initialises linestrings for comparison
    std::vector<std::unique_ptr<ICell> > touchingCellEdgeLinestring;

    // Initialises linestrings for cell/linestring comparison
    std::vector<std::unique_ptr<ICell> > linestringTouchesCellEdge;

    // Initialises outer and inner rings for comparison
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > emptyInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (0);
    std::vector<std::unique_ptr<ICell> > touchingCellEdgeOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > touchingCellEdgeInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
    std::vector<std::unique_ptr<ICell> > polygonCellEdgeTouchNoInnerRingsOuterLinestring;
    std::vector<std::unique_ptr<ICell> > polygonCellEdgeTouchWithInnerRingsOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > polygonCellEdgeTouchWithInnerRingsInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (1);
};

class TestDataTriangleMultiFace
{
  public:
    TestDataTriangleMultiFace(IGridIndexer & a_indexer)
    {
      // Construct a multi-face linestring for comparing with
      multiFaceLinestring.push_back(a_indexer.CreateCell("0022"));
      multiFaceLinestring.push_back(a_indexer.CreateCell("0033"));
      multiFaceLinestring.push_back(a_indexer.CreateCell("0122"));
      multiFaceLinestring.push_back(a_indexer.CreateCell("0133"));

      // Construct a multi-face polygon for comparing with
      multiFaceOuterLinestring.push_back(a_indexer.CreateCell("00111"));
      multiFaceOuterLinestring.push_back(a_indexer.CreateCell("01111"));
      multiFaceOuterLinestring.push_back(a_indexer.CreateCell("01313"));
      multiFaceOuterLinestring.push_back(a_indexer.CreateCell("01212"));
      multiFaceOuterLinestring.push_back(a_indexer.CreateCell("00313"));
      multiFaceOuterLinestring.push_back(a_indexer.CreateCell("00212"));

      // Populate linestrings for linestring/linestring comparison

      // Populates a multi-face linestring that touches the end of the original multi-face linestring
      multiFaceLinestringTouchesLineEnd.push_back(a_indexer.CreateCell("0022"));
      multiFaceLinestringTouchesLineEnd.push_back(a_indexer.CreateCell("0002"));
      multiFaceLinestringTouchesLineEnd.push_back(a_indexer.CreateCell("0111"));

      // Populates a multi-face linestring that is disjoint from the original multi-face linestring
      multiFaceLinestringDisjointLine.push_back(a_indexer.CreateCell("0000"));
      multiFaceLinestringDisjointLine.push_back(a_indexer.CreateCell("0003"));
      multiFaceLinestringDisjointLine.push_back(a_indexer.CreateCell("0100"));

      // Populate a multi-face linestring that covers the original multi-face linestring
      multiFaceSuperstring.push_back(a_indexer.CreateCell("0022"));
      multiFaceSuperstring.push_back(a_indexer.CreateCell("0033"));
      multiFaceSuperstring.push_back(a_indexer.CreateCell("0122"));
      multiFaceSuperstring.push_back(a_indexer.CreateCell("0133"));
      multiFaceSuperstring.push_back(a_indexer.CreateCell("0131"));

      // Populate a multi-face linestring that is covered by the original multi-face linestring
      multiFaceSubstring.push_back(a_indexer.CreateCell("0033"));
      multiFaceSubstring.push_back(a_indexer.CreateCell("0122"));
      multiFaceSubstring.push_back(a_indexer.CreateCell("0133"));

      // Populate outer rings for linestring/polygon comparison

      // Populate multi-face outer rings that touches the end of the multi-face linestring
      multiFacePolygonTouchesLineOuterLinestring.push_back(a_indexer.CreateCell("0022"));
      multiFacePolygonTouchesLineOuterLinestring.push_back(a_indexer.CreateCell("0011"));
      multiFacePolygonTouchesLineOuterLinestring.push_back(a_indexer.CreateCell("0111"));
      multiFacePolygonTouchesLineOuterLinestring.push_back(a_indexer.CreateCell("0003"));

      // Compare to a multi-face polygon with no inner rings, that is disjoint from the multi-face linestring
      multiFacePolygonDisjointOuterLinestring.push_back(a_indexer.CreateCell("0011"));
      multiFacePolygonDisjointOuterLinestring.push_back(a_indexer.CreateCell("0111"));
      multiFacePolygonDisjointOuterLinestring.push_back(a_indexer.CreateCell("0100"));
      multiFacePolygonDisjointOuterLinestring.push_back(a_indexer.CreateCell("0000"));

      // Populate outer rings for polygon/polygon comparison

      // Populate multi-face outer rings that covers the multi-face polygon
      multiFaceCoveringPolygonOuterLinestring.push_back(a_indexer.CreateCell("00111"));
      multiFaceCoveringPolygonOuterLinestring.push_back(a_indexer.CreateCell("01111"));
      multiFaceCoveringPolygonOuterLinestring.push_back(a_indexer.CreateCell("01333"));
      multiFaceCoveringPolygonOuterLinestring.push_back(a_indexer.CreateCell("01222"));
      multiFaceCoveringPolygonOuterLinestring.push_back(a_indexer.CreateCell("00333"));
      multiFaceCoveringPolygonOuterLinestring.push_back(a_indexer.CreateCell("00222"));

      // Populate multi-face outer rings that covers the multi-face polygon
      multiFaceCoveredByPolygonOuterLinestring.push_back(a_indexer.CreateCell("0000"));
      multiFaceCoveredByPolygonOuterLinestring.push_back(a_indexer.CreateCell("0010"));
      multiFaceCoveredByPolygonOuterLinestring.push_back(a_indexer.CreateCell("0110"));
      multiFaceCoveredByPolygonOuterLinestring.push_back(a_indexer.CreateCell("0100"));
    }

    // Initialises multi-face linestring for comparison
    std::vector<std::unique_ptr<ICell> > multiFaceLinestring;

    // Initialise multi-face outer and inner rings for comparison
    std::vector<std::unique_ptr<ICell> > multiFaceOuterLinestring;
    std::vector<std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > emptyInnerRings =
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > (0);

    // Initialise multi-face linestrings for linestring/linestring comparison
    std::vector<std::unique_ptr<ICell> > multiFaceLinestringTouchesLineEnd;
    std::vector<std::unique_ptr<ICell> > multiFaceLinestringDisjointLine;
    std::vector<std::unique_ptr<ICell> > multiFaceSuperstring;
    std::vector<std::unique_ptr<ICell> > multiFaceSubstring;

    // Initialise multi-face outer and inner linestrings for linestring/polygon comparison
    std::vector<std::unique_ptr<ICell> > multiFacePolygonTouchesLineOuterLinestring;
    std::vector<std::unique_ptr<ICell> > multiFacePolygonDisjointOuterLinestring;

    // Initialise multi-face outer and inner linestring for polygon/polygon comparison
    std::vector<std::unique_ptr<ICell> > multiFaceCoveringPolygonOuterLinestring;
    std::vector<std::unique_ptr<ICell> > multiFaceCoveredByPolygonOuterLinestring;
};

UNIT_TEST(SpatialAnalysis, PointConstructor)
{
  HierarchicalGrid::Aperture4TriangleGrid grid;

  HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);
  Icosahedron icosahedron;
  Snyder projection(&icosahedron);

  SpatialAnalysis spatialAnalysis(indexer.CreateCell("00"), &indexer, &projection);
  EXPECT_EQ(GeometryType::POLYGON, spatialAnalysis.GetGeometryType());
}

UNIT_TEST(SpatialAnalysis, LinestringConstructor)
{
  HierarchicalGrid::Aperture4TriangleGrid grid;

  HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);
  Icosahedron icosahedron;
  Snyder projection(&icosahedron);

  std::vector < std::unique_ptr<ICell> > linestring;
  linestring.push_back(indexer.CreateCell("001"));
  linestring.push_back(indexer.CreateCell("002"));
  linestring.push_back(indexer.CreateCell("003"));
  SpatialAnalysis spatialAnalysis(linestring, &indexer, &projection);
  EXPECT_EQ(GeometryType::LINESTRING, spatialAnalysis.GetGeometryType());
}

UNIT_TEST(SpatialAnalysis, PolygonConstructor)
{
  HierarchicalGrid::Aperture4TriangleGrid grid;

  HierarchicalGridIndexer indexer(&grid, MAX_FACE_INDEX);
  Icosahedron icosahedron;
  Snyder projection(&icosahedron);

  std::vector < std::unique_ptr<ICell> > outerLinestring;
  outerLinestring.push_back(indexer.CreateCell("001"));
  outerLinestring.push_back(indexer.CreateCell("002"));
  outerLinestring.push_back(indexer.CreateCell("003"));

  std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> > > innerRings(2);

  innerRings.at(0).push_back(indexer.CreateCell("0001"));
  innerRings.at(0).push_back(indexer.CreateCell("0002"));
  innerRings.at(0).push_back(indexer.CreateCell("0003"));

  innerRings.at(1).push_back(indexer.CreateCell("00001"));
  innerRings.at(1).push_back(indexer.CreateCell("00002"));
  innerRings.at(1).push_back(indexer.CreateCell("00003"));

  SpatialAnalysis spatialAnalysis(outerLinestring, innerRings, &indexer, &projection);
  EXPECT_EQ(GeometryType::POLYGON, spatialAnalysis.GetGeometryType());
}

UNIT_TEST(SpatialAnalysis, AnalyseEquals)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the equals analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0000"), AnalysisType::EQUALS));

  // Compare with a child cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::EQUALS));

  // Compare with a parent cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("000"), AnalysisType::EQUALS));

  // Compare with an edge sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0001"), AnalysisType::EQUALS));

  // Compare with a vertex sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0030"), AnalysisType::EQUALS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0022"), AnalysisType::EQUALS));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::EQUALS));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::EQUALS));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::EQUALS));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_FALSE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::EQUALS));

  // Compare the cell with linestrings
  // A cell can never be equal to a linestring

  // Compare with a linestring that is contained within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::EQUALS));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::EQUALS));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::EQUALS));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::EQUALS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::EQUALS));

  // Compare with a linestring that touches the cell vertex
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::EQUALS));

  // Compare with a linestring that touches the cell edge
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::EQUALS));

  // Compare the cell with polygons
  // A cell can never be equal to a polygon, as despite the cell being treated as a polygon, they are defined very differently

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::EQUALS));

  // Compare the linestring with cells
  // A linestring can never be equal to a cell

  // Compare with a cell that contains the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::EQUALS));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::EQUALS));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::EQUALS));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::EQUALS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::EQUALS));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_FALSE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::EQUALS));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_FALSE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::EQUALS));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(testDataTriangle.linestring, AnalysisType::EQUALS));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::EQUALS));

  // Compare with a linestring that covers the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::EQUALS));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::EQUALS));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::EQUALS));

  // Compare with a linestring that touches the end of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::EQUALS));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::EQUALS));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::EQUALS));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::EQUALS));

  // Compare with a linestring that partially intersects
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::EQUALS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::EQUALS));

  // Compare the linestring with polygons
  // A linestring cannot be equal to a polygon

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::EQUALS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon with cells
  // A polygon can never be equal to a cell, as despite the cell being treated as a polygon, they are defined very differently

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::EQUALS));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::EQUALS));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::EQUALS));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(triangleIndexer.CreateCell("00"), AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::EQUALS));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::EQUALS));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::EQUALS));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::EQUALS));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::EQUALS));

  // Compare the polygon with linestrings
  // A polygon can never be equal to a linestring

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::EQUALS));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, to itself
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::EQUALS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::EQUALS));

  // Multi-face tests

  // Create a multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create a multi-face polygon
  SpatialAnalysis multiFacePolygonAnalysis(
      testDataTriangleMultiFace.multiFaceOuterLinestring,
      testDataTriangleMultiFace.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

// Compare the multi-face linestring to linestrings

  // Compare the multi-face linestring with itself
  EXPECT_TRUE(multiFaceLinestringAnalysis.Analyse(testDataTriangleMultiFace.multiFaceLinestring, AnalysisType::EQUALS));

  // Compare with a different multi-face linestring
  std::vector < std::unique_ptr<ICell> > anotherMultiFaceLinestring;
  anotherMultiFaceLinestring.push_back(triangleIndexer.CreateCell("00002"));
  anotherMultiFaceLinestring.push_back(triangleIndexer.CreateCell("00031"));
  anotherMultiFaceLinestring.push_back(triangleIndexer.CreateCell("01211"));

  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(anotherMultiFaceLinestring, AnalysisType::EQUALS));

  // Compare the multi-face polygon to polygons

  // Compare the multi-face polygon with itself
  EXPECT_TRUE(
      multiFacePolygonAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::EQUALS));

  // Compare with a different multi-face polygon
  std::vector < std::unique_ptr<ICell> > anotherMultiFacePolygon;
  anotherMultiFacePolygon.push_back(triangleIndexer.CreateCell("00001"));
  anotherMultiFacePolygon.push_back(triangleIndexer.CreateCell("00031"));
  anotherMultiFacePolygon.push_back(triangleIndexer.CreateCell("01211"));

  EXPECT_FALSE(
      multiFacePolygonAnalysis.Analyse(
          anotherMultiFacePolygon,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::EQUALS));
}

UNIT_TEST(SpatialAnalysis, AnalyseContains)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the contains analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::CONTAINS));

  // Compare with a child cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::CONTAINS));

  // Compare with a parent cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::CONTAINS));

  // Compare with an edge sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::CONTAINS));

  // Compare with a vertex sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0030"),
          AnalysisType::CONTAINS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::CONTAINS));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::CONTAINS));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::CONTAINS));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::CONTAINS));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_FALSE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::CONTAINS));

  // Compare the cell with linestrings

  // Compare with a linestring that is contained within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::CONTAINS));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::CONTAINS));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::CONTAINS));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::CONTAINS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::CONTAINS));

  // Compare with a linestring that touches the cell vertex
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::CONTAINS));

  // Compare with a linestring that touches the cell edge
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::CONTAINS));

  // Compare the cell with polygons

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::CONTAINS));

  // Compare the linestring with cells
  // Linestring never contains a cell as the cell is treated as a polygon

  // Compare with a cell that contains the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::CONTAINS));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::CONTAINS));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::CONTAINS));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::CONTAINS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::CONTAINS));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_FALSE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::CONTAINS));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_FALSE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::CONTAINS));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(testDataTriangle.linestring, AnalysisType::CONTAINS));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::CONTAINS));

  // Compare with a linestring that covers the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::CONTAINS));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::CONTAINS));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::CONTAINS));

  // Compare with a linestring that touches the end of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::CONTAINS));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::CONTAINS));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::CONTAINS));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::CONTAINS));

  // Compare with a linestring that partially intersects
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::CONTAINS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::CONTAINS));

  // Compare the linestring with polygons
  // A linestring can never contain a polygon

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::CONTAINS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon with cells

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::CONTAINS));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::CONTAINS));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::CONTAINS));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::CONTAINS));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::CONTAINS));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::CONTAINS));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::CONTAINS));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::CONTAINS));

  // Compare the polygon with linestrings

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  // The interior of the linestring is found between the points used to define the linestring, they need to
  // intersect with the interior of the polygon, to be contained within the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  // The interior of the linestring is found between the points used to define the linestring, they need to
  // intersect with the interior of the polygon, to be contained within the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  // The interior of the linestring is found between the points used to define the linestring, they need to
  // intersect with the interior of the polygon, to be contained within the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::CONTAINS));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, to itself
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::CONTAINS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CONTAINS));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Cells belong to a single face, so a cell can never contain multi-face linestring and polygons

  // Create a multi-face polygon
  SpatialAnalysis multiFacePolygonAnalysis(
      testDataTriangleMultiFace.multiFaceOuterLinestring,
      testDataTriangleMultiFace.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Compare the multi-face polygon to cells

  // Compare to a cell inside the polygon
  EXPECT_TRUE(
      multiFacePolygonAnalysis.Analyse(triangleIndexer.CreateCell("00001"), AnalysisType::CONTAINS));

  // Compare to a cell outside the polygon
  EXPECT_FALSE(
      multiFacePolygonAnalysis.Analyse(triangleIndexer.CreateCell("0133"), AnalysisType::CONTAINS));

  // Compare the multi-face polygon to linestrings

  // Compare with a multi-face linestring that lies within the multi-face polygon
  std::vector < std::unique_ptr<ICell> > linestringContainedInMultiFacePolygon;
  linestringContainedInMultiFacePolygon.push_back(triangleIndexer.CreateCell("00001"));
  linestringContainedInMultiFacePolygon.push_back(triangleIndexer.CreateCell("00031"));
  linestringContainedInMultiFacePolygon.push_back(triangleIndexer.CreateCell("01211"));

  EXPECT_TRUE(
      multiFacePolygonAnalysis.Analyse(
          linestringContainedInMultiFacePolygon,
          AnalysisType::CONTAINS));

  // Compare with a multi-face linestring that is disjoint from the multi-face polygon
  std::vector < std::unique_ptr<ICell> > linestringDisjointFromMultiFacePolygon;
  linestringDisjointFromMultiFacePolygon.push_back(triangleIndexer.CreateCell("0002"));
  linestringDisjointFromMultiFacePolygon.push_back(triangleIndexer.CreateCell("0033"));
  linestringDisjointFromMultiFacePolygon.push_back(triangleIndexer.CreateCell("0102"));

  EXPECT_FALSE(
      multiFacePolygonAnalysis.Analyse(
          linestringDisjointFromMultiFacePolygon,
          AnalysisType::CONTAINS));
}

UNIT_TEST(SpatialAnalysis, AnalyseWithin)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the within analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0000"), AnalysisType::WITHIN));

  // Compare with a child cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::WITHIN));

  // Compare with a parent cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("000"), AnalysisType::WITHIN));

  // Compare with an edge sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0001"), AnalysisType::WITHIN));

  // Compare with a vertex sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0030"), AnalysisType::WITHIN));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0022"), AnalysisType::WITHIN));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::WITHIN));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::WITHIN));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::WITHIN));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_FALSE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::WITHIN));

  // Compare the cell with linestrings
  // A cell is never within a linestring as the cell is treated as a polygon

  // Compare with a linestring that is contained within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::WITHIN));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::WITHIN));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::WITHIN));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::WITHIN));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::WITHIN));

  // Compare with a linestring that touches the cell vertex
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::WITHIN));

  // Compare with a linestring that touches the cell edge
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::WITHIN));

  // Compare the cell with polygons

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::WITHIN));

  // Compare the linestring with cells

  // Compare with a cell that contains the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::WITHIN));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::WITHIN));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::WITHIN));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::WITHIN));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::WITHIN));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_FALSE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::WITHIN));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_FALSE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::WITHIN));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(testDataTriangle.linestring, AnalysisType::WITHIN));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::WITHIN));

  // Compare with a linestring that covers the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::WITHIN));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::WITHIN));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::WITHIN));

  // Compare with a linestring that touches the end of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::WITHIN));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::WITHIN));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::WITHIN));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::WITHIN));

  // Compare with a linestring that partially intersects
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::WITHIN));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::WITHIN));

  // Compare the linestring with polygons

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  // The interior of the linestring is found between the points used to define the linestring, they need to
  // intersect with the interior of the polygon, to be within the polygon
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  // The interior of the linestring is found between the points used to define the linestring, they need to
  // intersect with the interior of the polygon, to be within the polygon
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  // The interior of the linestring is found between the points used to define the linestring, they need to
  // intersect with the interior of the polygon, to be within the polygon
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::WITHIN));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon with cells

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::WITHIN));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::WITHIN));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::WITHIN));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(triangleIndexer.CreateCell("00"), AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::WITHIN));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::WITHIN));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::WITHIN));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::WITHIN));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::WITHIN));

  // Compare the polygon with linestrings
  // A polygon can never be within a linestring

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::WITHIN));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, to itself
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::WITHIN));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::WITHIN));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Cells belong to a single face, so multi-face linestring and polygons can never be WITHIN cells

  // Create a multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Compare the linestring to polygons

  // Compare with a multi-face polygon, that contains the multi-face line
  std::vector < std::unique_ptr<ICell> > multiFacePolygonContainingLineOuterLinestring;
  multiFacePolygonContainingLineOuterLinestring.push_back(
      triangleIndexer.CreateCell("00111"));
  multiFacePolygonContainingLineOuterLinestring.push_back(
      triangleIndexer.CreateCell("01111"));
  multiFacePolygonContainingLineOuterLinestring.push_back(
      triangleIndexer.CreateCell("01333"));
  multiFacePolygonContainingLineOuterLinestring.push_back(
      triangleIndexer.CreateCell("01222"));
  multiFacePolygonContainingLineOuterLinestring.push_back(
      triangleIndexer.CreateCell("00333"));
  multiFacePolygonContainingLineOuterLinestring.push_back(
      triangleIndexer.CreateCell("00222"));

  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          multiFacePolygonContainingLineOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::WITHIN));

  // Compare with a multi-face polygon, that is disjoint from the multi-face line
  std::vector < std::unique_ptr<ICell> > multiFacePolygonDisjointOuterLinestring;
  multiFacePolygonDisjointOuterLinestring.push_back(triangleIndexer.CreateCell("00222"));
  multiFacePolygonDisjointOuterLinestring.push_back(triangleIndexer.CreateCell("00333"));
  multiFacePolygonDisjointOuterLinestring.push_back(triangleIndexer.CreateCell("01222"));
  multiFacePolygonDisjointOuterLinestring.push_back(triangleIndexer.CreateCell("01202"));
  multiFacePolygonDisjointOuterLinestring.push_back(triangleIndexer.CreateCell("00301"));
  multiFacePolygonDisjointOuterLinestring.push_back(triangleIndexer.CreateCell("00202"));

  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          multiFacePolygonDisjointOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::WITHIN));
}

UNIT_TEST(SpatialAnalysis, AnalyseTouches)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the touches analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::TOUCHES));

  // Compare with a child cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::TOUCHES));

  // Compare with a parent cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("000"), AnalysisType::TOUCHES));

  // Compare with an edge sharing cell
  // As a cell is treated as a polygon
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::TOUCHES));

  // Compare with a vertex sharing cell
  // As a cell is treated as a polygon
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0030"),
          AnalysisType::TOUCHES));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::TOUCHES));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::TOUCHES));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::TOUCHES));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::TOUCHES));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_FALSE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::TOUCHES));

  // Compare the cell with linestrings

  // Compare with a linestring that is contained within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::TOUCHES));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::TOUCHES));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::TOUCHES));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::TOUCHES));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::TOUCHES));

  // Compare with a linestring that touches the cell vertex
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::TOUCHES));

  // Compare with a linestring that touches the cell edge
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::TOUCHES));

  // Compare the cell with polygons

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::TOUCHES));

  // Compare the linestring with cells

  // Compare with a cell that contains the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::TOUCHES));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::TOUCHES));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::TOUCHES));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::TOUCHES));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::TOUCHES));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_TRUE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::TOUCHES));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_TRUE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::TOUCHES));

  // Compare the linestring with other linestrings
  // Two linestrings touch only if their boundaries and not their interiors intersect. The endpoints
  // of the linestring are defined as the boundaries and everything in between is the interior.

  // Compare with the same linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(testDataTriangle.linestring, AnalysisType::TOUCHES));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::TOUCHES));

  // Compare with a linestring that covers the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::TOUCHES));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::TOUCHES));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::TOUCHES));

  // Compare with a linestring that touches the end of the linestring with an end
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::TOUCHES));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::TOUCHES));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::TOUCHES));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::TOUCHES));

  // Compare with a linestring that partially intersects
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::TOUCHES));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::TOUCHES));

  // Compare the linestring with polygons

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::TOUCHES));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon with cells

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::TOUCHES));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::TOUCHES));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::TOUCHES));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::TOUCHES));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::TOUCHES));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::TOUCHES));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_TRUE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_TRUE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::TOUCHES));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_TRUE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_TRUE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::TOUCHES));

  // Compare the polygon with linestrings

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::TOUCHES));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, to itself
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::TOUCHES));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Create a multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Compare the linestring to other linestrings

  // Compare to a multi-face linestring that touches the end of the original multi-face linestring
  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceLinestringTouchesLineEnd,
          AnalysisType::TOUCHES));

  // Compare to a multi-face linestring that is disjoint
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceLinestringDisjointLine,
          AnalysisType::TOUCHES));

  // Compare the linestring to polygons

  // Compare to a multi-face polygon with no inner rings, that touches the end of the multi-face linestring
  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFacePolygonTouchesLineOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::TOUCHES));

  // Compare to a multi-face polygon with no inner rings, that is disjoint from the multi-face linestring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFacePolygonDisjointOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::TOUCHES));
}

UNIT_TEST(SpatialAnalysis, AnalyseDisjoint)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the disjoint analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::DISJOINT));

  // Compare with a child cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::DISJOINT));

  // Compare with a parent cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::DISJOINT));

  // Compare with an edge sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::DISJOINT));

  // Compare with a vertex sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0030"),
          AnalysisType::DISJOINT));

  // Compare with a disjoint cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::DISJOINT));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::DISJOINT));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::DISJOINT));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::DISJOINT));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_FALSE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::DISJOINT));

  // Compare the cell with linestrings

  // Compare with a linestring that is contained within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::DISJOINT));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::DISJOINT));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::DISJOINT));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::DISJOINT));

  // Compare with a disjoint linestring
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::DISJOINT));

  // Compare with a linestring that touches the cell vertex
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::DISJOINT));

  // Compare with a linestring that touches the cell edge
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::DISJOINT));

  // Compare the cell with polygons

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::DISJOINT));

  // Compare the linestring with cells

  // Compare with a cell that contains the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::DISJOINT));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::DISJOINT));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::DISJOINT));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::DISJOINT));

  // Compare with a disjoint cell
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::DISJOINT));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_FALSE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::DISJOINT));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_FALSE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::DISJOINT));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(testDataTriangle.linestring, AnalysisType::DISJOINT));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::DISJOINT));

  // Compare with a linestring that covers the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::DISJOINT));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::DISJOINT));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::DISJOINT));

  // Compare with a linestring that touches the end of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::DISJOINT));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::DISJOINT));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::DISJOINT));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::DISJOINT));

  // Compare with a linestring that partially intersects
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::DISJOINT));

  // Compare with a disjoint linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::DISJOINT));

  // Compare the linestring with polygons

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::DISJOINT));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon with cells

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::DISJOINT));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::DISJOINT));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::DISJOINT));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::DISJOINT));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::DISJOINT));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::DISJOINT));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::DISJOINT));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::DISJOINT));

  // Compare the polygon with linestrings

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::DISJOINT));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, to itself
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::DISJOINT));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Create multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Compare the linestring to other linestrings

  // Compare to a multi-face linestring that is disjoint
  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceLinestringDisjointLine,
          AnalysisType::DISJOINT));

  // Compare to a multi-face linestring that touches the end of the original multi-face linestring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceLinestringTouchesLineEnd,
          AnalysisType::DISJOINT));

  // Compare the linestring to polygons

  // Compare to a multi-face polygon with no inner rings, that is disjoint from the multi-face linestring
  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFacePolygonDisjointOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::DISJOINT));

  // Compare to a multi-face polygon with no inner rings, that touches the end of the multi-face linestring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFacePolygonTouchesLineOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::DISJOINT));
}

UNIT_TEST(SpatialAnalysis, AnalyseIntersects)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the intersects analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::INTERSECTS));

  // Compare with a child cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::INTERSECTS));

  // Compare with a parent cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::INTERSECTS));

  // Compare with an edge sharing cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::INTERSECTS));

  // Compare with a vertex sharing cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0030"),
          AnalysisType::INTERSECTS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::INTERSECTS));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::INTERSECTS));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::INTERSECTS));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::INTERSECTS));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_TRUE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::INTERSECTS));

  // Compare the cell with linestrings

  // Compare with a linestring that is contained within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::INTERSECTS));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::INTERSECTS));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::INTERSECTS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that touches the cell vertex
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that touches the cell edge
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::INTERSECTS));

  // Compare the cell with polygons

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::INTERSECTS));

  /// The cell is within the hole created by the inner ring so does not intersect it
  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the linestring with cells

  // Compare with a cell that contains the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::INTERSECTS));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::INTERSECTS));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::INTERSECTS));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::INTERSECTS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::INTERSECTS));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_TRUE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::INTERSECTS));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_TRUE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::INTERSECTS));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.linestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that covers the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that touches the end of the linestring with an end
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a linestring that partially intersects
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::INTERSECTS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::INTERSECTS));

  // Compare the linestring with polygons

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::INTERSECTS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon with cells
  // A polygon can never be equal to a cell, as despite the cell being treated as a polygon, they are defined very differently

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_TRUE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_TRUE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_TRUE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_TRUE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::INTERSECTS));

  // Compare the polygon with linestrings

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::INTERSECTS));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, to itself
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::INTERSECTS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Create a multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Compare the linestring to other linestrings

  // Compare to a multi-face linestring that touches the end of the original multi-face linestring
  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceLinestringTouchesLineEnd,
          AnalysisType::INTERSECTS));

  // Compare to a multi-face linestring that is disjoint
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceLinestringDisjointLine,
          AnalysisType::INTERSECTS));

  // Compare the linestring to polygons

  // Compare to a multi-face polygon with no inner rings, that touches the end of the multi-face linestring
  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFacePolygonTouchesLineOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::INTERSECTS));

  // Compare to a multi-face polygon with no inner rings, that is disjoint from the multi-face linestring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFacePolygonDisjointOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::INTERSECTS));
}

UNIT_TEST(SpatialAnalysis, AnalyseCovers)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the covers analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0000"), AnalysisType::COVERS));

  // Compare with a child cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::COVERS));

  // Compare with a parent cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("000"), AnalysisType::COVERS));

  // Compare with an edge sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0001"), AnalysisType::COVERS));

  // Compare with a vertex sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0030"), AnalysisType::COVERS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("0022"), AnalysisType::COVERS));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::COVERS));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::COVERS));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::COVERS));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_FALSE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::COVERS));

  // Compare the cell with linestrings

  // Compare with a linestring that is contained within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::COVERS));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::COVERS));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::COVERS));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::COVERS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::COVERS));

  // Compare with a linestring that touches the cell vertex
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::COVERS));

  // Compare with a linestring that touches the cell edge
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::COVERS));

  // Compare the cell with polygons

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::COVERS));

  // Compare the linestring with cells
  // A linestring can never cover a cell as the cell is treated as a polygon

  // Compare with a cell that contains the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::COVERS));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::COVERS));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::COVERS));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::COVERS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::COVERS));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_FALSE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::COVERS));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_FALSE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::COVERS));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(testDataTriangle.linestring, AnalysisType::COVERS));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::COVERS));

  // Compare with a linestring that covers the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::COVERS));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::COVERS));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::COVERS));

  // Compare with a linestring that touches the end of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::COVERS));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::COVERS));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::COVERS));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::COVERS));

  // Compare with a linestring that partially intersects
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::COVERS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::COVERS));

  // Compare the linestring with polygons
  // A linestring can never cover a polygon

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::COVERS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon with cells

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::COVERS));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::COVERS));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::COVERS));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::COVERS));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::COVERS));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(triangleIndexer.CreateCell("00"), AnalysisType::COVERS));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::COVERS));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::COVERS));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::COVERS));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::COVERS));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::COVERS));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::COVERS));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::COVERS));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::COVERS));

  // Compare the polygon with linestrings

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::COVERS));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::COVERS));

  // Compare the polygon with no inner rings, to itself
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::COVERS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERS));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Create a multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create a multi-face polygon
  SpatialAnalysis multiFacePolygonAnalysis(
      testDataTriangleMultiFace.multiFaceOuterLinestring, testDataTriangleMultiFace.emptyInnerRings, &triangleIndexer, &triangleProjection);

  // Compare the linestring to other linestrings

  // Compare to a multi-face linestring that is a substring
  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceSubstring,
          AnalysisType::COVERS));

  // Compare to a multi-face linestring that is a superstring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceSuperstring,
          AnalysisType::COVERS));

  // Compare the multi-face polygon with polygons

  // Compare to a multi-face polygon with no inner rings, that is covered by the multi-face polygon
  EXPECT_TRUE(multiFacePolygonAnalysis.Analyse(testDataTriangleMultiFace.multiFaceCoveredByPolygonOuterLinestring, testDataTriangleMultiFace.emptyInnerRings, AnalysisType::COVERS));

  // Compare to a multi-face polygon with no inner rings, that covers the multi-face polygon
  EXPECT_FALSE(
      multiFacePolygonAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceCoveringPolygonOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::COVERS));
}

UNIT_TEST(SpatialAnalysis, AnalyseCoveredBy)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the covered_by analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::COVERED_BY));

  // Compare with a child cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::COVERED_BY));

  // Compare with a parent cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::COVERED_BY));

  // Compare with an edge sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::COVERED_BY));

  // Compare with a vertex sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0030"),
          AnalysisType::COVERED_BY));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::COVERED_BY));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::COVERED_BY));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::COVERED_BY));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::COVERED_BY));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_FALSE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::COVERED_BY));

  // Compare the cell with linestrings
  // A cell can never be covered by a linestring as the cell is treated as a polygon

  // Compare with a linestring that is contained within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::COVERED_BY));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::COVERED_BY));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::COVERED_BY));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that touches the cell vertex
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that touches the cell edge
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::COVERED_BY));

  // Compare the cell with polygons

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  /// The cell is completely covered by the polygon
  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the linestring with cells

  // Compare with a cell that contains the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::COVERED_BY));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::COVERED_BY));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::COVERED_BY));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::COVERED_BY));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::COVERED_BY));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_FALSE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::COVERED_BY));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_FALSE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::COVERED_BY));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.linestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that covers the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that touches the end of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a linestring that partially intersects
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::COVERED_BY));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::COVERED_BY));

  // Compare the linestring with polygons

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::COVERED_BY));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon with cells

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::COVERED_BY));

  // Compare the polygon with linestrings
  // A polygon can never be covered by a linestring

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::COVERED_BY));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, to itself
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::COVERED_BY));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Create a multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create a multi-face polygon
  SpatialAnalysis multiFacePolygonAnalysis(
      testDataTriangleMultiFace.multiFaceOuterLinestring, testDataTriangleMultiFace.emptyInnerRings, &triangleIndexer, &triangleProjection);

  // Compare the linestring to other linestrings

  // Compare to a multi-face linestring that is a substring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceSubstring,
          AnalysisType::COVERED_BY));

  // Compare to a multi-face linestring that is a superstring
  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceSuperstring,
          AnalysisType::COVERED_BY));

  // Compare the multi-face polygon with polygons

  // Compare to a multi-face polygon with no inner rings, that is covered by the multi-face polygon
  EXPECT_FALSE(
      multiFacePolygonAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceCoveredByPolygonOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::COVERED_BY));

  // Compare to itself
  EXPECT_TRUE(
      multiFacePolygonAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::COVERED_BY));
}

UNIT_TEST(SpatialAnalysis, AnalyseCrosses)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the crosses analysis for each combination of geometries

  // Compare the cell with other cells
  // A cell can never cross a cell as they are treated as polygons

  // Compare with the same cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::CROSSES));

  // Compare with a child cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::CROSSES));

  // Compare with a parent cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(triangleIndexer.CreateCell("000"), AnalysisType::CROSSES));

  // Compare with an edge sharing cell
  // No interior points overlap
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::CROSSES));

  // Compare with a vertex sharing cell
  // No interior points overlap
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0030"),
          AnalysisType::CROSSES));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::CROSSES));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::CROSSES));

  // Compare the hexagonal cell, to the child cell it covers
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::CROSSES));

  // Compare the hexagonal cell with its single parent that covers it
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::CROSSES));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_FALSE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::CROSSES));

  // Compare the cell with linestrings

  // Compare with a linestring that is contained within the cell
  // All the interior points of the line, coincide with interior points of the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::CROSSES));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::CROSSES));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::CROSSES));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::CROSSES));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::CROSSES));

  // Compare with a linestring that touches the cell vertex
  // No interior points overlap
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::CROSSES));

  // Compare with a linestring that touches the cell edge
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::CROSSES));

  // Compare the cell with polygons
  // A cell can never cross a polygon, as the cell is treated as a polygon

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::CROSSES));

  // Compare the linestring with cells

  // Compare with a cell that contains the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::CROSSES));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::CROSSES));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::CROSSES));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::CROSSES));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::CROSSES));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  // No interior points overlap
  EXPECT_FALSE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::CROSSES));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  // No interior points overlap
  EXPECT_FALSE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::CROSSES));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(testDataTriangle.linestring, AnalysisType::CROSSES));

  // Compare with a linestring which is a sub-string of the linestring
  // The dimension of the overlap is the same as the contributing shapes
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::CROSSES));

  // Compare with a linestring that covers the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::CROSSES));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::CROSSES));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::CROSSES));

  // Compare with a linestring that touches the end of the linestring with an end
  // The overlap contains no interior points for either shape
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::CROSSES));

  // Compare with a linestring that touches the middle of the linestring with an end
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::CROSSES));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::CROSSES));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  // Only boundary points intersect
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::CROSSES));

  // Compare with a linestring that partially intersects
  // Overlap is the same dimension as the contributing shapes
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::CROSSES));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::CROSSES));

  // Compare the linestring with polygons

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  // No interior points overlap
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  // No interior points overlap
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  // No interior points overlap
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  // All the interior points of the line, overlap the with interior points of the polygon
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  // All the interior points of the line, overlap the with interior points of the polygon
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  // No interior points overlap
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  // No interior points overlap
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  // No interior points overlap
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  // No interior points overlap
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  // No interior points overlap
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  // No interior points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  // The exterior of the polygon does not overlap with interior points of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  // The exterior of the polygon does not overlap with interior points of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  // The exterior of the polygon does not overlap with interior points of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::CROSSES));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon with cells
  // A polygon can never cross a cell, as the cell is treated as a polygon

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::CROSSES));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::CROSSES));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::CROSSES));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::CROSSES));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::CROSSES));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::CROSSES));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::CROSSES));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::CROSSES));

  // Compare the polygon with linestrings

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  // No interior points overlap
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  // No interior points overlap
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  // No interior points overlap
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  // All interior points of the linestring, overlap with interior points of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::CROSSES));

  // Compare the polygon with other polygons
  // A polygon can never cross a polygon

  // Compare the polygon with inner rings, to itself
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, to itself
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::CROSSES));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::CROSSES));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Create a multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Compare the linestring to other linestrings

  // Compare to a multi-face linestring that crosses at a single cell used to define the middle of both linestrings
  std::vector<std::unique_ptr<ICell> > multiFaceCrossesLinestring;
  multiFaceCrossesLinestring.push_back(triangleIndexer.CreateCell("0021"));
  multiFaceCrossesLinestring.push_back(triangleIndexer.CreateCell("0033"));
  multiFaceCrossesLinestring.push_back(triangleIndexer.CreateCell("0121"));
  multiFaceCrossesLinestring.push_back(triangleIndexer.CreateCell("0111"));

  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          multiFaceCrossesLinestring,
          AnalysisType::CROSSES));

  // Compare to a multi-face linestring that is a substring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceSubstring,
          AnalysisType::CROSSES));

  // Compare to a multi-face linestring that is a superstring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceSuperstring,
          AnalysisType::CROSSES));

  // Compare the multi-face linestring with polygons

  // Compare to a multi-face polygon with no inner rings, that is covered by the multi-face polygon
  std::vector<std::unique_ptr<ICell> > multiFaceLineCrossesPolygonOuterLinestring;
  multiFaceLineCrossesPolygonOuterLinestring.push_back(triangleIndexer.CreateCell("00323"));
  multiFaceLineCrossesPolygonOuterLinestring.push_back(triangleIndexer.CreateCell("00333"));
  multiFaceLineCrossesPolygonOuterLinestring.push_back(triangleIndexer.CreateCell("0100"));
  multiFaceLineCrossesPolygonOuterLinestring.push_back(triangleIndexer.CreateCell("0102"));

  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          multiFaceLineCrossesPolygonOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::CROSSES));

  // Compare to a multi-face polygon with no inner rings, that covers the multi-face polygon
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFacePolygonDisjointOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::CROSSES));
}

UNIT_TEST(SpatialAnalysis, AnalyseOverlaps)
{
  HierarchicalGrid::Aperture4TriangleGrid triangularGrid;
  OffsetGrid::Aperture3HexagonGrid hexagonalGrid;

  HierarchicalGridIndexer triangleIndexer(&triangularGrid, MAX_FACE_INDEX);
  OffsetGridIndexer hexagonalIndexer(&hexagonalGrid, MAX_FACE_INDEX);

  Icosahedron icosahedron;
  Snyder triangleProjection(&icosahedron);
  Snyder hexagonalProjection(&icosahedron);

  TestDataTriangle testDataTriangle(triangleIndexer);
  TestDataHexagonal testDataHexagonal(hexagonalIndexer);
  TestDataTriangleMultiFace testDataTriangleMultiFace(triangleIndexer);

  // Create triangular cell
  SpatialAnalysis pointSpatialAnalysis(
      triangleIndexer.CreateCell("0000"),
      &triangleIndexer,
      &triangleProjection);

  // Create hexagonal cell with a single parent
  SpatialAnalysis singleParentHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00021,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create hexagonal cell with multiple parents
  SpatialAnalysis multipleParentsHexagonalPointSpatialAnalysis(
      hexagonalIndexer.CreateCell("00022,1"),
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create linestring
  SpatialAnalysis linestringSpatialAnalysis(
      testDataTriangle.linestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell vertex
  SpatialAnalysis touchingCellVertexLinestringSpatialAnalysis(
      testDataTriangle.touchingCellVertexLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create linestring that touches a cell edge
  SpatialAnalysis touchingCellEdgeLinestringSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeLinestring,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with inner rings
  SpatialAnalysis polygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.innerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings
  SpatialAnalysis noInnerRingsPolygonSpatialAnalysis(
      testDataTriangle.outerLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell vertex
  SpatialAnalysis polygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.touchingCellVertexInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with no inner rings that touches a cell vertex
  SpatialAnalysis noInnerRingsPolygonCellVertexTouchesSpatialAnalysis(
      testDataTriangle.touchingCellVertexOuterLinestring,
      testDataTriangle.emptyInnerRings,
      &triangleIndexer,
      &triangleProjection);

  // Create polygon with inner rings that touches a cell edge
  SpatialAnalysis polygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.touchingCellEdgeInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Create polygon with no inner rings that touches a cell edge
  SpatialAnalysis noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis(
      testDataHexagonal.touchingCellEdgeOuterLinestring,
      testDataHexagonal.emptyInnerRings,
      &hexagonalIndexer,
      &hexagonalProjection);

  // Test the overlaps analysis for each combination of geometries

  // Compare the cell with other cells

  // Compare with the same cell
  // The shapes are equal
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::OVERLAPS));

  // Compare with a child cell
  // The exterior of the cell does not intersect with the interior of the child cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00000"),
          AnalysisType::OVERLAPS));

  // Compare with a parent cell
  // The interior of the cell does not intersect with the exterios of the parent cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::OVERLAPS));

  // Compare with an edge sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::OVERLAPS));

  // Compare with a vertex sharing cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0030"),
          AnalysisType::OVERLAPS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::OVERLAPS));

  // Compare the hexagonal cell, to a partially intersecting child cell
  EXPECT_TRUE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,4"),
          AnalysisType::OVERLAPS));

  // Compare the hexagonal cell, to the child cell it covers
  // The exterior of the cell does not intersect the interior of the child cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00031,3"),
          AnalysisType::OVERLAPS));

  // Compare the hexagonal cell with its single parent that covers it
  // The interior of the cell does not intersect with the exterior of the parent cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::OVERLAPS));

  // Compare another hexagonal cell that has multiple parents, to one of its parent cells
  EXPECT_TRUE(
      multipleParentsHexagonalPointSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00010,1"),
          AnalysisType::OVERLAPS));

  // Compare the cell with linestrings
  // A linestring can never overlap a linestring

  // Compare with a linestring that is contained within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithinCell,
          AnalysisType::OVERLAPS));

  // Compare with a linestring constructed using the cell at an end
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellEnd,
          AnalysisType::OVERLAPS));

  // Compare with a linestring constructed using the cell in the middle
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringWithCellInMiddle,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that crosses the cell, but not at the cell centre
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringCrossesCell,
          AnalysisType::OVERLAPS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringDisjointFromCell,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that touches the cell vertex
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.linestringTouchesCellVertex,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that touches the cell edge
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.linestringTouchesCellEdge,
          AnalysisType::OVERLAPS));

  // Compare the cell with polygons

  // Compare with a polygon that has no inner rings and has the cell on its outer ring
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryNoInnerOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has inner rings and has the cell on its outer ring
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellInBoundaryWithInnerOuterLinestring,
          testDataTriangle.polygonWithCellInBoundaryWithInnerInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has the cell on its inner ring
  EXPECT_TRUE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithCellOnInnerRingOuterLinestring,
          testDataTriangle.polygonWithCellOnInnerInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and is within the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithinCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonWithinCellWithInnerInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellNoInnerRingOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and covers the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCoversCellWithInnerRingOuterLinestring,
          testDataTriangle.polygonCoversCellWithInnerInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has the cell within a hole created by an inner ring
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellInHoleOuterLinestring,
          testDataTriangle.polygonCellInHoleInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and is disjoint from the cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonDisjointFromCellWithInnerOuterLinestring,
          testDataTriangle.polygonDisjointFromCellWithInnerInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchNoInnerRingsOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has inner rings and touches the vertex of a cell
  EXPECT_FALSE(
      pointSpatialAnalysis.Analyse(
          testDataTriangle.polygonCellVertexTouchWithInnerRingsOuterLinestring,
          testDataTriangle.polygonCellVertexTouchWithInnerRingsInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchNoInnerRingsOuterLinestring,
          testDataHexagonal.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has inner rings and touches the edge of a cell
  EXPECT_FALSE(
      singleParentHexagonalPointSpatialAnalysis.Analyse(
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsOuterLinestring,
          testDataHexagonal.polygonCellEdgeTouchWithInnerRingsInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the linestring with cells
  // A linestring can never overlap a cell

  // Compare with a cell that contains the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000"),
          AnalysisType::OVERLAPS));

  // Compare with a cell used to construct an end of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0001"),
          AnalysisType::OVERLAPS));

  // Compare with a cell used to construct the middle of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0002"),
          AnalysisType::OVERLAPS));

  // Compare with a cell that the linestring crosses, but does not cross at the centre of the cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00001"),
          AnalysisType::OVERLAPS));

  // Compare with a disjoint cell
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0022"),
          AnalysisType::OVERLAPS));

  // Compare with a cell whose vertex is touched by the cell vertex touching linestring
  EXPECT_FALSE(
      touchingCellVertexLinestringSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::OVERLAPS));

  // Compare with a cell whose vertex is touched by the cell edge touching linestring
  EXPECT_FALSE(
      touchingCellEdgeLinestringSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::OVERLAPS));

  // Compare the linestring with other linestrings

  // Compare with the same linestring
  // The shapes are equal
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(testDataTriangle.linestring, AnalysisType::OVERLAPS));

  // Compare with a linestring which is a sub-string of the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.subLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that covers the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.coversLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that intersects at the middle cell used to define the two linestrings
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleCellIntersectLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that intersects at a location that isn't the centre of a cell used to define
  // either linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.notCellIntersectLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that touches the end of the linestring with an end
  // No interior points overlap and the dimension of intersection is incorrect
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.endTouchLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that touches the middle of the linestring with an end
  // No interior points overlap and the dimension of intersection is incorrect
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.middleTouchLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that touches the linestring at multiple points including
  // interior and boundary points
  // The dimension of intersection is incorrect
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.multiTouchLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that touches both boundary points of the linestring, with its boundary points
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.boundariesTouchLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a linestring that partially intersects
  EXPECT_TRUE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.partialOverlapLinestring,
          AnalysisType::OVERLAPS));

  // Compare with a disjoint linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.disjointLinestring,
          AnalysisType::OVERLAPS));

  // Compare the linestring with polygons
  // A linestring can never overlap a polygon

  // Compare with a polygon that has no inner rings and the linestring runs along the outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithNoInnerLinestringInBoundaryOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring runs along its outer ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInOuterBoundaryInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring runs along its inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInnerBoundaryInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring lies within its interior
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInInteriorInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringOuterTouchInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the end of the linestring touches a cell on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringInnerTouchOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringInnerTouchInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialOuterOverlapInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior and
  // partially runs along the inner boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringPartialInnerOverlapInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the outer boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialOuterOverlapInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring goes from the interior and
  // partially runs along the inner ring boundary
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringWithinPartialInnerOverlapInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerLinestringOutToInOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the outer ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossOuterOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossOuterInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and the linestring goes from the exterior to the interior,
  // crossing at a single boundary point on the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerLinestringCrossInnerOuterLinestring,
          testDataTriangle.polygonWithInnerLinestringCrossInnerInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has no inner rings and is disjoint from the linestring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, where the linestring does not
  // lie in the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInnerRings,
          AnalysisType::OVERLAPS));

  // Compare with a polygon that has an inner ring and is disjoint from the linestring, with the linestring lying
  // within the hole created by the inner ring
  EXPECT_FALSE(
      linestringSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerDisjointFromLinestringInHoleInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon with cells

  // Compare the polygon with inner rings, to a cell on the outer ring of the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, to a cell on the outer ring of the polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00111"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with inner rings, to a cell on the inner ring of the polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0000"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, to a cell in the interior of the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0032001"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, to a cell that contains the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("00"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with inner rings, to a cell found within the hole created by an inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("000322"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, to a cell that is disjoint from the polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0020"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with an inner ring, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, that touches the vertex of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellVertexTouchesSpatialAnalysis.Analyse(
          triangleIndexer.CreateCell("0010"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with an inner ring, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      polygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, that touches the edge of a cell, with the cell it touches
  EXPECT_FALSE(
      noInnerRingsPolygonCellEdgeTouchesSpatialAnalysis.Analyse(
          hexagonalIndexer.CreateCell("00021,1"),
          AnalysisType::OVERLAPS));

  // Compare the polygon with linestrings
  // A polygon can never overlap a linestring

  // Compare the polygon that has no inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsOuterBoundaryLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that runs along the outer ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsOuterBoundaryLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that runs along the inner ring boundary
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInnerBoundaryLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that lies within its interior
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsTouchOuterLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchOuterLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring whose end touches a cell on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsTouchInnerLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToOuterRingLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInnerRingLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToOuterRingLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that goes from the interior and partially
  // runs along the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsInteriorToInnerRingLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaOuterLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that goes from the exterior to the interior,
  // crossing at a single point on the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsExteriorToInteriorViaInnerLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.noInnerRingsDisjointLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a linestring that is disjoint from the polygon, within the inner ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.withInnerRingsDisjointInHoleLinestring,
          AnalysisType::OVERLAPS));

  // Compare the polygon with other polygons

  // Compare the polygon with inner rings, to itself
  // The shapes are equal
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, to itself
  // The shapes are equal
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon with inner rings, to the same polygon with a ring removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, to the same polygon with an inner ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.oneRingInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon with inner rings, to the same polygon with all inner rings removed
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon with no inner rings, to the same polygon with two inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.outerLinestring,
          testDataTriangle.innerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares a single point on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterTouchOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterTouchInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that shares an edge on the
  // outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsOuterEdgeOuterLinestring,
          testDataTriangle.polygonWithInnerRingsOuterEdgeInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a polygon with no inner rings, that intersects the interior
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring, that intersects the interior
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOverlapOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorOverlapInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring, that intersects the interior
  // and a point from the inner ring corresponds with a point on the outer ring of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInnerRingTouchOuterInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a polygon with an inner ring outside the former polygon,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorIntersectsInnerRingInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a polygon with no inner ring,
  // whose interior intersects the exterior of the former polygon, found within its inner ring
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInnerRingTouchOuterOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to an interior polygon with an inner ring covered by the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsInteriorOuterLinestring,
          testDataTriangle.polygonWithInnerRingsInteriorInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to an interior polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsInteriorOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // outside the former polygon
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_TRUE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a covering polygon with an inner ring
  // in the interior of the former polygon
  EXPECT_TRUE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversInteriorHoleInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a covering polygon with an inner ring
  // inside an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsCoversHoleWithinHoleInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a covering polygon with no inner rings
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsCoversOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has no inner rings, to a disjoint polygon with no inner rings, located outside the outer ring
  EXPECT_FALSE(
      noInnerRingsPolygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a disjoint polygon with inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonWithInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.polygonWithInnerRingsDisjointInHoleInnerRings,
          AnalysisType::OVERLAPS));

  // Compare the polygon that has inner rings, to a disjoint polygon with no inner rings, located in the hole created
  // by an inner ring of the former polygon
  EXPECT_FALSE(
      polygonSpatialAnalysis.Analyse(
          testDataTriangle.polygonNoInnerRingsDisjointInHoleOuterLinestring,
          testDataTriangle.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Multi-face tests

  // Cells belong to a single face, so no cell/cell tests need to be conducted

  // Create a multi-face linestring
  SpatialAnalysis multiFaceLinestringAnalysis(
      testDataTriangleMultiFace.multiFaceLinestring,
      &triangleIndexer,
      &triangleProjection);

  // Create a multi-face polygon
  SpatialAnalysis multiFacePolygonAnalysis(
      testDataTriangleMultiFace.multiFaceOuterLinestring, testDataTriangleMultiFace.emptyInnerRings, &triangleIndexer, &triangleProjection);

  // Compare the linestring to other linestrings

  // Compare to a multi-face linestring that is a substring
  EXPECT_FALSE(
      multiFaceLinestringAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceSubstring,
          AnalysisType::OVERLAPS));

  // Compare to a multi-face linestring that shares an edge
  std::vector<std::unique_ptr<ICell> > multiFaceLineEdgeShareLinestring;
  multiFaceLineEdgeShareLinestring.push_back(triangleIndexer.CreateCell("0021"));
  multiFaceLineEdgeShareLinestring.push_back(triangleIndexer.CreateCell("0033"));
  multiFaceLineEdgeShareLinestring.push_back(triangleIndexer.CreateCell("0122"));
  multiFaceLineEdgeShareLinestring.push_back(triangleIndexer.CreateCell("0121"));

  EXPECT_TRUE(
      multiFaceLinestringAnalysis.Analyse(
          multiFaceLineEdgeShareLinestring,
          AnalysisType::OVERLAPS));

  // Compare the multi-face polygon with polygons

  // Compare to a multi-face polygon with no inner rings, that is covered by the multi-face polygon
  EXPECT_FALSE(
      multiFacePolygonAnalysis.Analyse(
          testDataTriangleMultiFace.multiFaceCoveredByPolygonOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::OVERLAPS));

  // Compare to a multi-face polygon with no inner rings, that overlaps the multi-face polygon
  std::vector<std::unique_ptr<ICell> > multiFaceOverlapPolygonOuterLinestring;
  multiFaceOverlapPolygonOuterLinestring.push_back(triangleIndexer.CreateCell("0022"));
  multiFaceOverlapPolygonOuterLinestring.push_back(triangleIndexer.CreateCell("0002"));
  multiFaceOverlapPolygonOuterLinestring.push_back(triangleIndexer.CreateCell("0102"));
  multiFaceOverlapPolygonOuterLinestring.push_back(triangleIndexer.CreateCell("0122"));

  EXPECT_TRUE(
      multiFacePolygonAnalysis.Analyse(
          multiFaceOverlapPolygonOuterLinestring,
          testDataTriangleMultiFace.emptyInnerRings,
          AnalysisType::OVERLAPS));
}
