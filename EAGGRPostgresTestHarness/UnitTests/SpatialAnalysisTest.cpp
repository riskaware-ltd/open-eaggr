//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRPostgresTestHarness
//
// subsystem: Postgres Tests
//
//------------------------------------------------------
/// @file SpatialAnalysisTest.cpp
/// 
/// Tests for the Postgres Extension, for the EAGGR_ShapeComparison method
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

static const char* const K_PG_RESULT_TRUE = "t";
static const char* const K_PG_RESULT_FALSE = "f";

// EQUALS tests

// Compare with the same cell
POSTGRES_TEST(PostgresShapeComparison, EqualsTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','1~0000','1~0000','ISEA4T'), "
        "EAGGR_ShapeComparison('EQUALS','1~0000','1~0011','ISEA4T'), "
        "EAGGR_ShapeComparison('EQUALS','2~0001;0002;0003','2~0001;0002;0003','ISEA4T'), "
        "EAGGR_ShapeComparison('EQUALS','2~0001;0002;0003','2~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('EQUALS','3~00111;0002;0001;00333:00312;00302;00321','3~00111;0002;0001;00333:00312;00302;00321','ISEA4T'), "
        "EAGGR_ShapeComparison('EQUALS','3~00111;0002;0001;00333:00312;00302;00321','3~0021;0022;0023','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with the same cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with the same linestring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with the same polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// CONTAINS tests
POSTGRES_TEST(PostgresShapeComparison, ContainsTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('CONTAINS','1~0000','1~00000','ISEA4T'), "
        "EAGGR_ShapeComparison('CONTAINS','1~0000','1~0011','ISEA4T'), "
        "EAGGR_ShapeComparison('CONTAINS','2~0001;0002;0003','2~0001;0002','ISEA4T'), "
        "EAGGR_ShapeComparison('CONTAINS','2~0001;0002;0003','2~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('CONTAINS','3~00111;0002;0001;00333:00312;00302;00321','3~00101;00132;00001','ISEA4T'), "
        "EAGGR_ShapeComparison('CONTAINS','3~00111;0002;0001;00333:00312;00302;00321','3~0021;0022;0023','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with a child cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a substring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with an interior polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// WITHIN tests

// Compare with a parent cell
POSTGRES_TEST(PostgresShapeComparison, WithinTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('WITHIN','1~0000','1~000','ISEA4T'), "
        "EAGGR_ShapeComparison('WITHIN','1~0000','1~0011','ISEA4T'), "
        "EAGGR_ShapeComparison('WITHIN','2~0001;0002;0003','2~0033;0001;0002;0003;0011','ISEA4T'), "
        "EAGGR_ShapeComparison('WITHIN','2~0001;0002;0003','2~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('WITHIN','3~00111;0002;0001;00333:00312;00302;00321','3~00111;00222;00333','ISEA4T'), "
        "EAGGR_ShapeComparison('WITHIN','3~00111;0002;0001;00333:00312;00302;00321','3~0021;0022;0023','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with a parent cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a superstring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a polygon contained with the base polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// TOUCHES tests

POSTGRES_TEST(PostgresShapeComparison, TouchesTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('TOUCHES','1~0000','1~0001','ISEA4T'), "
        "EAGGR_ShapeComparison('TOUCHES','1~0000','1~0011','ISEA4T'), "
        "EAGGR_ShapeComparison('TOUCHES','2~0001;0002;0003','2~0033;0032;0001','ISEA4T'), "
        "EAGGR_ShapeComparison('TOUCHES','2~0001;0002;0003','2~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('TOUCHES','3~00111;0002;0001;00333:00312;00302;00321','3~0002;0001;0022','ISEA4T'), "
        "EAGGR_ShapeComparison('TOUCHES','3~00111;0002;0001;00333:00312;00302;00321','3~0021;0022;0023','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with an edge sharing cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a linestring whose end touches the end of the base linestring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with an outer ring edge sharing polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// DISJOINT tests

POSTGRES_TEST(PostgresShapeComparison, DisjointTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('DISJOINT','1~0000','1~0011','ISEA4T'), "
        "EAGGR_ShapeComparison('DISJOINT','1~0000','1~0001','ISEA4T'), "
        "EAGGR_ShapeComparison('DISJOINT','2~0001;0002;0003','2~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('DISJOINT','2~0001;0002;0003','2~0033;0032;0001','ISEA4T'), "
        "EAGGR_ShapeComparison('DISJOINT','3~00111;0002;0001;00333:00312;00302;00321','3~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('DISJOINT','3~00111;0002;0001;00333:00312;00302;00321','3~0002;0001;0022','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with a disjoint cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with an edge sharing cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a linestring whose end touches the end of the base linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with an outer ring edge sharing polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// INTERSECTS tests

POSTGRES_TEST(PostgresShapeComparison, IntersectsTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('INTERSECTS','1~0000','1~0001','ISEA4T'), "
        "EAGGR_ShapeComparison('INTERSECTS','1~0000','1~0011','ISEA4T'), "
        "EAGGR_ShapeComparison('INTERSECTS','2~0001;0002;0003','2~0033;0032;0001','ISEA4T'), "
        "EAGGR_ShapeComparison('INTERSECTS','2~0001;0002;0003','2~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('INTERSECTS','3~00111;0002;0001;00333:00312;00302;00321','3~0002;0001;0022','ISEA4T'), "
        "EAGGR_ShapeComparison('INTERSECTS','3~00111;0002;0001;00333:00312;00302;00321','3~0021;0022;0023','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with an edge sharing cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a linestring whose end touches the end of the base linestring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with an outer ring edge sharing polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// COVERS tests

POSTGRES_TEST(PostgresShapeComparison, CoversTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('COVERS','1~0000','1~00000','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERS','1~0000','1~0011','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERS','2~0001;0002;0003','2~0001;0002','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERS','2~0001;0002;0003','2~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERS','3~00111;0002;0001;00333:00312;00302;00321','3~00101;00132;00001','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERS','3~00111;0002;0001;00333:00312;00302;00321','3~0021;0022;0023','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with a child cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a substring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a polygon that lies in the interior of the base polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// COVERED_BY tests

POSTGRES_TEST(PostgresShapeComparison, CoveredByTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('COVERED_BY','1~0000','1~000','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERED_BY','1~0000','1~0011','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERED_BY','2~0001;0002;0003','2~0033;0001;0002;0003;0011','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERED_BY','2~0001;0002;0003','2~0021;0022;0023','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERED_BY','3~00111;0002;0001;00333:00312;00302;00321','3~00111;00222;00333','ISEA4T'), "
        "EAGGR_ShapeComparison('COVERED_BY','3~00111;0002;0001;00333:00312;00302;00321','3~0021;0022;0023','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with a parent cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a superstring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a polygon contained within the base polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// CROSSES tests

POSTGRES_TEST(PostgresShapeComparison, CrossesTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('CROSSES','1~00021,1','1~00031,4','ISEA3H'), "
        "EAGGR_ShapeComparison('CROSSES','1~00021,1','1~00023,1','ISEA3H'), "
        "EAGGR_ShapeComparison('CROSSES','2~00021,2;00023,1;00023,3','2~00022,3;00023,1;00024,0','ISEA3H'), "
        "EAGGR_ShapeComparison('CROSSES','2~00021,2;00023,1;00023,3','2~00020,2;00023,0;00024,1;00024,4','ISEA3H'), "
        "EAGGR_ShapeComparison('CROSSES','3~00020,2;00024,0;00024,4;00023,4;00020,3:00021,2;00023,1;00023,3','3~00020,2;00023,0;00024,1;00024,4;00023,4;00020,3','ISEA3H'), "
        "EAGGR_ShapeComparison('CROSSES','3~00020,2;00024,0;00024,4;00023,4;00020,3:00021,2;00023,1;00023,3','3~00020,0;00020,1;00021,0','ISEA3H')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with an overlapping child cell
      // Two cells can never cross as cells are treated as polygons
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a linestring that crosses the base linestring at a single point
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with an overlapping polygon
      // Two polygons can never cross
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

// OVERLAPS tests

POSTGRES_TEST(PostgresShapeComparison, OverlapsTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('OVERLAPS','1~00021,1','1~00031,4','ISEA3H'), "
        "EAGGR_ShapeComparison('OVERLAPS','1~00021,1','1~00023,1','ISEA3H'), "
        "EAGGR_ShapeComparison('OVERLAPS','2~00021,2;00023,1;00023,3','2~00021,4;00021,2;00023,1','ISEA3H'), "
        "EAGGR_ShapeComparison('OVERLAPS','2~00021,2;00023,1;00023,3','2~00020,0;00020,1;00021,0','ISEA3H'), "
        "EAGGR_ShapeComparison('OVERLAPS','3~00020,2;00024,0;00024,4;00023,4;00020,3:00021,2;00023,1;00023,3','3~00020,2;00023,0;00024,1;00024,4;00023,4;00020,3','ISEA3H'), "
        "EAGGR_ShapeComparison('OVERLAPS','3~00020,2;00024,0;00024,4;00023,4;00020,3:00021,2;00023,1;00023,3','3~00020,0;00020,1;00021,0','ISEA3H')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with an overlapping child cell
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint cell
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with a linestring that partially overlaps the base linestring
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint linestring
      comparisonResult = PQgetvalue(result, 0, 3);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with an overlapping polygon
      comparisonResult = PQgetvalue(result, 0, 4);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with a disjoint polygon
      comparisonResult = PQgetvalue(result, 0, 5);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}

POSTGRES_TEST(PostgresShapeComparison, ManyInnerRingsTest)
{
  PGconn* connection = PQconnectdb(CONNECTION_INFO);
  EXPECT_EQ(CONNECTION_OK, PQstatus(connection)) << PQerrorMessage(connection);

  if (PQstatus(connection) == CONNECTION_OK)
  {
    PGresult* result = PQexec(connection, "select EAGGR_ShapeComparison('EQUALS','3~00111;0002;0001;00333:00312;00302;00321:00132;00101;00023','3~00111;0002;0001;00333:00312;00302;00321:00132;00101;00023','ISEA4T'), "
        "EAGGR_ShapeComparison('EQUALS','3~00111;0002;0001;00333:00312;00302;00321:00132;00101;00023','3~00111;0002;0001;00333:00312;00302;00321','ISEA4T'), "
        "EAGGR_ShapeComparison('EQUALS','3~00111;0002;0001;00333:00312;00302;00321:00132;00101;00023','3~00111;0002;0001;00333','ISEA4T')");
    EXPECT_EQ(PGRES_TUPLES_OK, PQresultStatus(result)) << PQerrorMessage(connection);

    if (PQresultStatus(result) == PGRES_TUPLES_OK)
    {
      // Compare with the same polygon
      char * comparisonResult = PQgetvalue(result, 0, 0);
      EXPECT_STREQ(K_PG_RESULT_TRUE,comparisonResult);

      // Compare with the same polygon, but with one less inner ring
      comparisonResult = PQgetvalue(result, 0, 1);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      // Compare with the same polygon, but with all inner rings removed
      comparisonResult = PQgetvalue(result, 0, 2);
      EXPECT_STREQ(K_PG_RESULT_FALSE,comparisonResult);

      PQclear(result);
    }
  }

  PQfinish (connection);
}
