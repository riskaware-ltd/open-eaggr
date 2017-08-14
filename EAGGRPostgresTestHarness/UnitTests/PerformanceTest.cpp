//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRPostgresTestHarness
//
// subsystem: Postgres Tests
//
//------------------------------------------------------
/// @file PerformanceTest.cpp
/// 
/// Outputs the time taken for DGGS cell processing compared to Geohash
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

#include "TestUtilities/TestTimer.hpp"

// This test is disabled from running in the full test suite as for some reason
// it causes all subsequent tests to fail to connect to the Postgres server
// Change the test definition line below to include the test and output performance statistics
DISABLED_POSTGRES_TEST(Performance, ComparisonAgainstGeoHash)
//POSTGIS_TEST(Performance, ComparisonAgainstGeoHash)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PQexec(connection, "drop table dggs_cells");

    PGresult* result = PQexec(connection, "create table dggs_cells(wkt text, geom geometry, dggs_cell_from_wkt_isea4t text, dggs_cell_from_geom_isea4t text, dggs_cell_from_wkt_isea3h text, dggs_cell_from_geom_isea3h text, geohash_from_wkt text, geohash_from_geom text)");
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    // Insert the WKT data
    for (int i = 0; i < 10000; ++i)
    {
      result = PQexec(connection, "insert into dggs_cells(wkt) values ('POINT(2.345 1.234)')");
      EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

      result = PQexec(connection, "insert into dggs_cells(wkt) values ('LINESTRING(2.345 1.234, 4.567 3.456)')");
      EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

      result = PQexec(connection, "insert into dggs_cells(wkt) values ('POLYGON((2.345 1.234, 4.567 3.456, 6.789 5.678, 2.345 1.234))')");
      EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

      result = PQexec(connection, "insert into dggs_cells(wkt) values ('GEOMETRYCOLLECTION(POINT(2.345 1.234),LINESTRING(2.345 1.234, 4.567 3.456),POLYGON((2.345 1.234, 4.567 3.456, 6.789 5.678, 2.345 1.234)))')");
      EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);
    }

    // Update the table to have a geometry generated from the WKT
    result = PQexec(connection, "update dggs_cells set geom = ST_GeomFromText(wkt)");
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    // Update the table to include the dggs cell and geohashes
    TestTimer wktToGeohashTimer;
    wktToGeohashTimer.Start();
    result = PQexec(connection, "update dggs_cells set geohash_from_wkt = ST_GeoHash(ST_GeomFromText(wkt))");
    wktToGeohashTimer.Pause();
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    TestTimer geomToGeohashTimer;
    geomToGeohashTimer.Start();
    result = PQexec(connection, "update dggs_cells set geohash_from_geom = ST_GeoHash(geom)");
    geomToGeohashTimer.Pause();
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    TestTimer wktToDggsCellTimerISEA4T;
    wktToDggsCellTimerISEA4T.Start();
    result = PQexec(connection, "update dggs_cells set dggs_cell_from_wkt_isea4t = EAGGR_ToCells(wkt, 3.879, 'ISEA4T')");
    wktToDggsCellTimerISEA4T.Pause();
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    TestTimer wktToDggsCellTimerISEA3H;
    wktToDggsCellTimerISEA3H.Start();
    result = PQexec(connection, "update dggs_cells set dggs_cell_from_wkt_isea3h = EAGGR_ToCells(wkt, 3.879, 'ISEA3H')");
    wktToDggsCellTimerISEA3H.Pause();
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    TestTimer geomToDggsCellTimerISEA4T;
    geomToDggsCellTimerISEA4T.Start();
    result = PQexec(connection, "update dggs_cells set dggs_cell_from_geom_isea4t = EAGGR_ToCells(ST_AsText(geom), 3.879, 'ISEA4T')");
    geomToDggsCellTimerISEA4T.Pause();
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    TestTimer geomToDggsCellTimerISEA3H;
    geomToDggsCellTimerISEA3H.Start();
    result = PQexec(connection, "update dggs_cells set dggs_cell_from_geom_isea3h = EAGGR_ToCells(ST_AsText(geom), 3.879, 'ISEA3H')");
    geomToDggsCellTimerISEA3H.Pause();
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    std::stringstream keyStream1;
    keyStream1 << "Geohash from WKT";
    std::stringstream valueStream1;
    valueStream1 << wktToGeohashTimer.GetTimeInMilliseconds();
    ::testing::Test::RecordProperty(keyStream1.str(), valueStream1.str());

    std::stringstream keyStream2;
    keyStream2 << "Geohash from Geometry";
    std::stringstream valueStream2;
    valueStream2 << geomToGeohashTimer.GetTimeInMilliseconds();
    ::testing::Test::RecordProperty(keyStream2.str(), valueStream2.str());

    std::stringstream keyStream3;
    keyStream3 << "DGGS Cell from WKT ISEA4T";
    std::stringstream valueStream3;
    valueStream3 << wktToDggsCellTimerISEA4T.GetTimeInMilliseconds();
    ::testing::Test::RecordProperty(keyStream3.str(), valueStream3.str());

    std::stringstream keyStream4;
    keyStream4 << "DGGS Cell from Geometry ISEA4T";
    std::stringstream valueStream4;
    valueStream4 << geomToDggsCellTimerISEA4T.GetTimeInMilliseconds();
    ::testing::Test::RecordProperty(keyStream4.str(), valueStream4.str());

    std::stringstream keyStream5;
    keyStream5 << "DGGS Cell from WKT ISEA3H";
    std::stringstream valueStream5;
    valueStream5 << wktToDggsCellTimerISEA3H.GetTimeInMilliseconds();
    ::testing::Test::RecordProperty(keyStream5.str(), valueStream5.str());

    std::stringstream keyStream6;
    keyStream6 << "DGGS Cell from Geometry ISEA3H";
    std::stringstream valueStream6;
    valueStream6 << geomToDggsCellTimerISEA3H.GetTimeInMilliseconds();
    ::testing::Test::RecordProperty(keyStream6.str(), valueStream6.str());

    PQclear(result);
  }

  PQfinish(connection);
}
