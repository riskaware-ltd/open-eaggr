//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRPostgresTestHarness
//
// subsystem: Postgres Tests
//
//------------------------------------------------------
/// @file ISEA4TTest.cpp
///
/// Tests for the Postgres Extension using the ISEA4T implementation
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

POSTGRES_TEST(PostGISISEA4T, PointToCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('POINT(2.345 1.234)', 3.879, 'ISEA4T')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("1~07231131111113100331001", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA4T, LinestringToCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('LINESTRING(2.345 1.234, 4.567 3.456)', 3.879, 'ISEA4T')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("2~07231131111113100331001;07012000001303022011321", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA4T, PolygonToCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('POLYGON((2.345 1.234, 4.567 3.456), (6.789 5.678, 8.901 7.890))', 3.879, 'ISEA4T')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("3~07231131111113100331001;07012000001303022011321:07010120000123111312330;07010133330110231202021", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA4T, MultiShapeToCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('GEOMETRYCOLLECTION(POINT(2.345 1.234),LINESTRING(2.345 1.234, 4.567 3.456),POLYGON((2.345 1.234, 4.567 3.456), (6.789 5.678), (8.901 7.890)))', 3.879, 'ISEA4T')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("1~07231131111113100331001/2~07231131111113100331001;07012000001303022011321/3~07231131111113100331001;07012000001303022011321:07010120000123111312330:07010133330110231202021", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA4T, BoundingCell)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_GetBoundingCell('{07231,0723102,07230130}', 'ISEA4T')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("0723", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA4T, ToCellArray)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCellArray(EAGGR_ToCells('GEOMETRYCOLLECTION(POINT(2.345 1.234),LINESTRING(2.345 1.234, 4.567 3.456),POLYGON((2.345 1.234, 4.567 3.456), (6.789 5.678), (8.901 7.890)))', 3.879, 'ISEA4T'))");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("{07231131111113100331001,07231131111113100331001,07012000001303022011321,07231131111113100331001,07012000001303022011321,07010120000123111312330,07010133330110231202021}", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA4T, CellGeometry)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_CellGeometry('07231131111113100331032', 'ISEA4T')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cells = PQgetvalue(result, 0, 0);

      unsigned int numberOfValues = 0;
      char* cell;
      char* values[6];
      // Delimited on space, comma and brackets
      char delimiters[] =
      { '(', ' ', ',', ')'};
      // Ignore the first 2 tokens - 'POLYGON' and '('
      cell = strtok(cells, delimiters);
      cell = strtok(NULL, delimiters);

      while (cell != NULL)
      {
        if (numberOfValues >= 6)
        {
          FAIL() << "Exceeded expected number of values.";
        }

        values[numberOfValues] = cell;
        ++numberOfValues;
        cell = strtok(NULL, delimiters);
      }

      EXPECT_EQ(6U, numberOfValues);
      EXPECT_NEAR(2.345022, atof(values[0]), 1E-6);
      EXPECT_NEAR(1.234003, atof(values[1]), 1E-6);
      EXPECT_NEAR(2.344987, atof(values[2]), 1E-6);
      EXPECT_NEAR(1.234008, atof(values[3]), 1E-6);
      EXPECT_NEAR(2.345004, atof(values[4]), 1E-6);
      EXPECT_NEAR(1.234033, atof(values[5]), 1E-6);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA4T, CellToPoint)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_CellToPoint('07231131111113100331001', 'ISEA4T')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* point = PQgetvalue(result, 0, 0);

      unsigned int numberOfValues = 0;
      char* coordinate;
      char* values[2];
      // Delimited on space, comma and brackets
      char delimiters[] =
      { '(', ' ', ',', ')'};
      // Ignore the first 2 tokens - 'POLYGON' and '('
      coordinate = strtok(point, delimiters);
      coordinate = strtok(NULL, delimiters);

      while (coordinate != NULL)
      {
        if (numberOfValues >= 2)
        {
          FAIL() << "Exceeded expected number of values.";
        }

        values[numberOfValues] = coordinate;
        ++numberOfValues;
        coordinate = strtok(NULL, delimiters);
      }

      // Number of values includes the terminating characters in addition to the point
      EXPECT_EQ(2U, numberOfValues);
      EXPECT_NEAR(2.345004, atof(values[0]), 1E-6);
      EXPECT_NEAR(1.233997, atof(values[1]), 1E-6);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

// This test is to ensure that cell data can be stored in a database table as well as in select statements.
// It is only in the ISEA4T test since both ISEA4T and ISEA3H work in the same way.
POSTGRES_TEST(PostGISISEA4T, AddCellsToTable)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PQexec(connection, "drop table dggs_cells");

    PGresult* result = PQexec(connection, "create table dggs_cells(wkt text, dggs_cell text)");
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    // Insert the WKT data
    result = PQexec(connection, "insert into dggs_cells(wkt) values ('POINT(2.345 1.234)')");
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    result = PQexec(connection, "insert into dggs_cells(wkt) values ('LINESTRING(2.345 1.234, 4.567 3.456)')");
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    result = PQexec(connection, "insert into dggs_cells(wkt) values ('POLYGON((2.345 1.234, 4.567 3.456), (6.789 5.678), (8.901 7.890))')");
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    result = PQexec(connection, "insert into dggs_cells(wkt) values ('GEOMETRYCOLLECTION(POINT(2.345 1.234),LINESTRING(2.345 1.234, 4.567 3.456),POLYGON((2.345 1.234, 4.567 3.456), (6.789 5.678), (8.901 7.890)))')");
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    // Update the table to include the dggs cell
    result = PQexec(connection, "update dggs_cells set dggs_cell = EAGGR_ToCells(wkt, 3.879, 'ISEA4T')");
    EXPECT_EQ(PGRES_COMMAND_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    result = PQexec(connection, "select wkt, dggs_cell from dggs_cells order by wkt");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      //Geometry collection
      char* cellsString = PQgetvalue(result, 0, 1);
      EXPECT_STREQ("1~07231131111113100331001/2~07231131111113100331001;07012000001303022011321/3~07231131111113100331001;07012000001303022011321:07010120000123111312330:07010133330110231202021", cellsString);

      //Linestring
      cellsString = PQgetvalue(result, 1, 1);
      EXPECT_STREQ("2~07231131111113100331001;07012000001303022011321", cellsString);

      //Point
      cellsString = PQgetvalue(result, 2, 1);
      EXPECT_STREQ("1~07231131111113100331001", cellsString);

      //Polygon
      cellsString = PQgetvalue(result, 3, 1);
      EXPECT_STREQ("3~07231131111113100331001;07012000001303022011321:07010120000123111312330:07010133330110231202021", cellsString);

      PQclear(result);
    }
  }

  PQfinish(connection);
}
