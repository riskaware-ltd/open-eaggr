//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRPostgresTestHarness
//
// subsystem: Postgres Tests
//
//------------------------------------------------------
/// @file ErrorsTest.cpp
/// 
/// Tests for error messages returned by the Postgres Extension
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
#include "utils/errcodes.h"

POSTGRES_TEST(PostgresError, InvalidDggsModel)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('POINT(2.345 1.234)', 3.879, 'INVALID')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  Unsupported DGGS model. Please choose 'ISEA4T' or 'ISEA3H'.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, InvalidWKT)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('POINT_INVALID(2.345 1.234)', 3.879, 'ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  EAGGR Exception: Unsupported WKT: POINT_INVALID(2.345 1.234). WKT string is not valid.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, InvalidAccuracy)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ToCells('POINT(2.345 1.234)', -3.879, 'ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  EAGGR Exception: Accuracy cannot be negative.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, InvalidBoundingCells)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_GetBoundingCell('{07231,0723402,07230130}', 'ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  EAGGR Exception: Cell index, '4', exceeds maximum (maximum = 3)\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, InvalidCellForGeometry)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_CellGeometry('07123123123123123123123123123123123123123123123', 'ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  EAGGR Exception: Cell ID exceeds maximum length (by 3 characters) - reduce required accuracy\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, InvalidCellToPoint)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_CellToPoint('07231131111113100331009', 'ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  EAGGR Exception: Cell index, '9', exceeds maximum (maximum = 3)\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, BlankShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  Invalid shape identifier separator provided. The shape identifier is a character supplied at the start of the shape string and is separated from the cells of the shape string by a '~'.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, OnlyShapeIdentifierShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','~','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  Invalid shape identifier separator provided. The shape identifier is a character supplied at the start of the shape string and is separated from the cells of the shape string by a '~'.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, NoShapeIdentifierShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','~0001','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  Invalid shape identifier separator provided. The shape identifier is a character supplied at the start of the shape string and is separated from the cells of the shape string by a '~'.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, NoShapeIdentifierSeparatorShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','0001','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  Invalid shape identifier separator provided. The shape identifier is a character supplied at the start of the shape string and is separated from the cells of the shape string by a '~'.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, StringLengthOneShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','1','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  Invalid shape identifier separator provided. The shape identifier is a character supplied at the start of the shape string and is separated from the cells of the shape string by a '~'.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, InvalidShapeIdentifierShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','4~0001','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  Invalid shape identifier provided. The shape identifier is provided at the start of a shape and is separated from the cells of the shape by a '~'. Valid shape identifiers are '1', '2' or '3', representing a cell, linestring or polygon respectively.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, NoShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','1~','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  No cells were provided for the shape, after the '~' character in the shape string.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, ManyShapesShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','1~0000/0001','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  Too many shapes supplied in one or more shape string. EAGGR_ShapeComparison only compares two shapes, each specified in separate shape strings.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, LinestringIdentifierCellShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','2~0000','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  The shape identifier and the shape do not match. The shape identifier indicates a linestring, but the shape supplied is not a linestring.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, CellIdentifierLinestringShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','1~0000;0002;0003','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  The shape identifier and the shape do not match. The shape identifier indicates a cell, but the shape supplied is not a cell.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, CellIdentifierPolygonShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','1~0000;0002;0003','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  The shape identifier and the shape do not match. The shape identifier indicates a cell, but the shape supplied is not a cell.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}

POSTGRES_TEST(PostgresError, LinestringIdentifierPolygonShapeString)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','2~0001;0002;0003:00001;00002;00003','1~0000','ISEA4T')");

    EXPECT_EQ(PGRES_FATAL_ERROR, PQresultStatus(result)) << PQerrorMessage(connection);
    EXPECT_STREQ("ERROR:  The shape identifier and the shape do not match. The shape identifier indicates a linestring, but the shape supplied is not a linestring.\n", PQerrorMessage(connection));

    PQclear(result);
  }

  PQfinish(connection);
}
