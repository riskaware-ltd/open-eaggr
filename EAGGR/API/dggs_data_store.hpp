//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: C API
//
//------------------------------------------------------
/// @file dggs_data_store.hpp
/// 
/// Stores the DGGS internal data for each DGGS instance
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

#include <mutex>
#include <map>

#include "eaggr_api.h"

#include "Src/Model/DGGS.hpp"
#include "Src/Model/ICell.hpp"
#include "Src/Model/IGrid/IHierarchicalGrid/Aperture4TriangleGrid.hpp"
#include "Src/Model/IGrid/IOffsetGrid/Aperture3HexagonGrid.hpp"
#include "Src/Model/IGridIndexer/HierarchicalGridIndexer.hpp"
#include "Src/Model/IGridIndexer/OffsetGridIndexer.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/Model/IProjection/Snyder.hpp"
#include "Src/CoordinateConversion/CoordinateConverter.hpp"

namespace EAGGR
{
  namespace API
  {
    /// Structure for keeping track of the objects used by the DGGS model.
    typedef struct
    {
        Model::PolyhedralGlobe::IPolyhedralGlobe * m_pGlobe;
        Model::Projection::IProjection * m_pProjection;
        Model::Grid::IGrid * m_pGrid;
        Model::GridIndexer::IGridIndexer * m_pIndexer;
        CoordinateConversion::CoordinateConverter * m_pConverter;
        std::string m_lastErrorMesssage;
    } DggsData;

    class DggsDataStore
    {
      public:
        void SetErrorMessage(const DGGS_Handle a_handle, const std::string& a_errorMessage);
        void SetDggsData(const DGGS_Handle a_handle, const DggsData& a_data);

        std::string& GetLastErrorMessage(const DGGS_Handle a_handle);
        DggsData& GetDggsData(const DGGS_Handle a_handle);

        void RemoveDggsData(const DGGS_Handle a_handle);
      private:
        std::map<DGGS_Handle, DggsData> m_dggsData;
        std::mutex m_mtx;
    };
  }
}
