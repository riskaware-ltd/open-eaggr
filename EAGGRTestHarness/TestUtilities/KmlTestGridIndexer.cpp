//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Test Utilities
//
//------------------------------------------------------
/// @file KmlTestGridIndexer.cpp
/// 
/// Implements the EAGGR::TestUtilities::KMLTestGridIndexer interface for use in KML testing
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

#include "KmlTestGridIndexer.hpp"

using namespace EAGGR::Model;

namespace EAGGR
{
  namespace TestUtilities
  {
    KmlTestGridIndexer::KmlTestGridIndexer()
        : m_index(0)
    {
    }

    void KmlTestGridIndexer::AddCell(
        const Cell::DggsCellId a_cellId,
        const EAGGR::Model::FaceCoordinate a_centre,
        const std::list<EAGGR::Model::FaceCoordinate> a_vertices)
    {
      m_centres.insert(std::pair<Cell::DggsCellId, FaceCoordinate>(a_cellId, a_centre));
      m_vertices.insert(
          std::pair<Cell::DggsCellId, std::list<FaceCoordinate> >(a_cellId, a_vertices));
    }

    void KmlTestGridIndexer::IncrementIndex()
    {
      ++m_index;
    }

    std::unique_ptr<Cell::ICell> KmlTestGridIndexer::GetCell(
        const FaceCoordinate a_faceCoordinate) const
    {
      // Not used by KML export
      return std::unique_ptr<Cell::ICell>();
    }

    FaceCoordinate KmlTestGridIndexer::GetFaceCoordinate(const Cell::ICell & a_cell) const
    {
      return m_centres.find(a_cell.GetCellId())->second;
    }

    std::unique_ptr<Cell::ICell> KmlTestGridIndexer::CreateCell(
        const Cell::DggsCellId & a_cellId) const
    {
      // Not used by KML export
      return std::unique_ptr<Cell::ICell>();
    }

    void KmlTestGridIndexer::GetParents(
        const Cell::ICell& a_cellId,
        std::vector<std::unique_ptr<Cell::ICell> >& a_parentCellIds) const
    {
      // Not used by KML export
    }

    void KmlTestGridIndexer::GetChildren(
        const Cell::ICell& a_cellId,
        std::vector<std::unique_ptr<Cell::ICell> >& a_childCellIds) const
    {
      // Not used by KML export
    }

    void KmlTestGridIndexer::GetCellVertices(
        const Cell::ICell & a_cell,
        std::list<FaceCoordinate>& a_cellVertices) const
    {
      std::list < FaceCoordinate > vertices = m_vertices.find(a_cell.GetCellId())->second;

      for (std::list<FaceCoordinate>::const_iterator iter = vertices.begin();
          iter != vertices.end(); ++iter)
      {
        a_cellVertices.push_back(*iter);
      }
    }
  }
}
