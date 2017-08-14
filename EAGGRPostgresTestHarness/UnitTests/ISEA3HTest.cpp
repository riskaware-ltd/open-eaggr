//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRPostgresTestHarness
//
// subsystem: Postgres Tests
//
//------------------------------------------------------
/// @file ISEA3HTest.cpp
///
/// Tests for the Postgres Extension using the ISEA3H implementation
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

POSTGRES_TEST(PostGISISEA3H, PointToCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('POINT(2.345 1.234)', 3.879, 'ISEA3H')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("1~0728-549628,-522499", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA3H, LinestringToCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('LINESTRING(2.345 1.234, 4.567 3.456)', 3.879, 'ISEA3H')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("2~0728-549628,-522499;0728-592991,-296234", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA3H, PolygonToCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('POLYGON((2.345 1.234, 4.567 3.456), (6.789 5.678, 8.901 7.890))', 3.879, 'ISEA3H')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("3~0728-549628,-522499;0728-592991,-296234:0728-645076,-75283;0728-696172,139932", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA3H, MultiShapeToCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('GEOMETRYCOLLECTION(POINT(2.345 1.234),LINESTRING(2.345 1.234, 4.567 3.456),POLYGON((2.345 1.234, 4.567 3.456), (6.789 5.678), (8.901 7.890)))', 3.879, 'ISEA3H')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("1~0728-549628,-522499/2~0728-549628,-522499;0728-592991,-296234/3~0728-549628,-522499;0728-592991,-296234:0728-645076,-75283:0728-696172,139932", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA3H, BoundingCell)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    // Use \ to escape the commas that are part of the strings
    PGresult* result = PQexec(connection, "select EAGGR_GetBoundingCell('{\"07040,0\", \"07040,1\", \"07040,-1\"}', 'ISEA3H')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("07020,0", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA3H, ToCellArray)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCellArray(EAGGR_ToCells('GEOMETRYCOLLECTION(POINT(2.345 1.234),LINESTRING(2.345 1.234, 4.567 3.456),POLYGON((2.345 1.234, 4.567 3.456), (6.789 5.678), (8.901 7.890)))', 3.879, 'ISEA3H'))");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cell = PQgetvalue(result, 0, 0);

      EXPECT_STREQ("{\"0728-549628,-522499\",\"0728-549628,-522499\",\"0728-592991,-296234\",\"0728-549628,-522499\",\"0728-592991,-296234\",\"0728-645076,-75283\",\"0728-696172,139932\"}", cell);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA3H, CellGeometry)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_CellGeometry('0728-549628,-522499', 'ISEA3H')");

    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      char* cells = PQgetvalue(result, 0, 0);

      unsigned int numberOfValues = 0;
      char* cell;
      char* values[12];
      // Delimited on space, comma and brackets
      char delimiters[] =
      { '(', ' ', ',', ')'};
      // Ignore the first 2 tokens - 'POLYGON' and '('
      cell = strtok(cells, delimiters);
      cell = strtok(NULL, delimiters);

      while (cell != NULL)
      {
        if (numberOfValues >= 12)
        {
          FAIL() << "Exceeded expected number of values.";
        }

        values[numberOfValues] = cell;
        ++numberOfValues;
        cell = strtok(NULL, delimiters);
      }

      EXPECT_EQ(12U, numberOfValues);
      EXPECT_NEAR(2.344997, atof(values[0]), 1E-6);
      EXPECT_NEAR(1.234003, atof(values[1]), 1E-6);
      EXPECT_NEAR(2.344990, atof(values[2]), 1E-6);
      EXPECT_NEAR(1.234000, atof(values[3]), 1E-6);
      EXPECT_NEAR(2.344990, atof(values[4]), 1E-6);
      EXPECT_NEAR(1.233992, atof(values[5]), 1E-6);
      EXPECT_NEAR(2.344998, atof(values[6]), 1E-6);
      EXPECT_NEAR(1.233987, atof(values[7]), 1E-6);
      EXPECT_NEAR(2.345006, atof(values[8]), 1E-6);
      EXPECT_NEAR(1.233990, atof(values[9]), 1E-6);
      EXPECT_NEAR(2.345006, atof(values[10]), 1E-6);
      EXPECT_NEAR(1.233998, atof(values[11]), 1E-6);

      PQclear(result);
    }
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostGISISEA3H, CellToPoint)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_CellToPoint('0728-549628,-522499', 'ISEA3H')");

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
      // Ignore the first 2 tokens - 'POINT' and '('
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
      EXPECT_NEAR(2.344998, atof(values[0]), 1E-6);
      EXPECT_NEAR(1.233995, atof(values[1]), 1E-6);

      PQclear(result);
    }
  }

  PQfinish(connection);
}
