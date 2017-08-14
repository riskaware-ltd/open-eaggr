//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: C API
//
//------------------------------------------------------
/// @file dggs_data_store.cpp
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

#include "dggs_data_store.hpp"

namespace EAGGR
{
  namespace API
  {
    void DggsDataStore::SetErrorMessage(
        const DGGS_Handle a_handle,
        const std::string& a_errorMessage)
    {
      m_mtx.lock();
      m_dggsData[a_handle].m_lastErrorMesssage = a_errorMessage;
      m_mtx.unlock();
    }

    void DggsDataStore::SetDggsData(const DGGS_Handle a_handle, const DggsData& a_data)
    {
      m_mtx.lock();
      m_dggsData[a_handle] = a_data;
      m_mtx.unlock();
    }

    std::string& DggsDataStore::GetLastErrorMessage(const DGGS_Handle a_handle)
    {
      m_mtx.lock();
      std::string& errorMessage = m_dggsData[a_handle].m_lastErrorMesssage;
      m_mtx.unlock();

      return errorMessage;
    }

    DggsData& DggsDataStore::GetDggsData(const DGGS_Handle a_handle)
    {
      m_mtx.lock();
      DggsData& data = m_dggsData[a_handle];
      m_mtx.unlock();
      return data;
    }

    void DggsDataStore::RemoveDggsData(const DGGS_Handle a_handle)
    {
      m_mtx.lock();
      m_dggsData.erase(a_handle);
      m_mtx.unlock();
    }
  }
}
