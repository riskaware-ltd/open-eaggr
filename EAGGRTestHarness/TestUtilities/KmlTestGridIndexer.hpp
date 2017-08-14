//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Test Utilities
//
//------------------------------------------------------
/// @file KmlTestGridIndexer.hpp
/// 
/// Implements the EAGGR::TestUtilities::KmlTestGridIndexer interface for use in KML testing
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

#include <list>
#include <map>

#include "Src/Model/IGridIndexer.hpp"

namespace EAGGR
{
  namespace TestUtilities
  {
    /// Implementation of IGridIndexer that provides functionality for KML testing
    class KmlTestGridIndexer: public Model::GridIndexer::IGridIndexer
    {
      public:
        KmlTestGridIndexer();

        /// Adds a cell to the indexer
        /// @param a_cellId The id of the cell
        /// @param a_centre The object to be returned by GetFaceCoordinate()
        /// @param a_vertices The object to be returned by GetCellVertices()
        void AddCell(
            const Model::Cell::DggsCellId a_cellId,
            const Model::FaceCoordinate a_centre,
            const std::list<Model::FaceCoordinate> a_vertices);

        void IncrementIndex();

        virtual std::unique_ptr<Model::Cell::ICell> GetCell(
            const Model::FaceCoordinate a_faceCoordinate) const;

        virtual Model::FaceCoordinate GetFaceCoordinate(const Model::Cell::ICell & a_cell) const;

        virtual std::unique_ptr<Model::Cell::ICell> CreateCell(
            const Model::Cell::DggsCellId & a_cellId) const;

        virtual void GetParents(
            const Model::Cell::ICell& a_cell,
            std::vector<std::unique_ptr<Model::Cell::ICell> >& a_parentCells) const;

        virtual void GetChildren(
            const Model::Cell::ICell& a_cell,
            std::vector<std::unique_ptr<Model::Cell::ICell> >& a_childCells) const;

        virtual void GetCellVertices(
            const Model::Cell::ICell & a_cell,
            std::list<Model::FaceCoordinate>& a_cellVertices) const;

      private:
        std::map<Model::Cell::DggsCellId, Model::FaceCoordinate> m_centres;
        std::map<Model::Cell::DggsCellId, std::list<Model::FaceCoordinate> > m_vertices;
        int m_index;
    };
  }
}
