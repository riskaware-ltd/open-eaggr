//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRPostgresTestHarness
//
// subsystem: Postgres Tests
//
//------------------------------------------------------
/// @file Version.cpp
/// 
/// Tests the Postgres Extension version
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

#include "libpq-fe.h"

POSTGRES_TEST(Postgres, VersionTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_Version()");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* version = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("v2.0", version);

      PQclear(result);
    }
  }

  PQfinish(connection);
}
