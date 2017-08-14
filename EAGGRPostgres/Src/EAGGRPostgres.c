//------------------------------------------------------
// Copyright (c) Riskaware 2016
//------------------------------------------------------
//
// system: EAGGRPostgreSQL
//
// subsystem: EAGGR PostgreSQL Extension
//
//------------------------------------------------------
/// @file EAGGRPostgres.c
/// 
/// Wraps the EAGGR C API functions to for use in PostgreSQL
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

#include "postgres.h"
#include "fmgr.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#include "access/htup.h"
#include "utils/builtins.h"
#include "utils/errcodes.h"
#include "utils/lsyscache.h"

#include "eaggr_api.h"

static DGGS_Handle g_dggsISEA4T;
static DGGS_Handle g_dggsISEA3H;

static const char K_SHAPE_IDENTIFIER_SEPARATOR = '~';
static const char K_CELL_SEPARATOR = ';';
static const char K_LINESTRING_SEPARATOR = ':';
static const char K_SHAPE_SEPARATOR = '/';
static const char K_TERMINATING_CHAR = '\0';

static const char K_CELL_IDENTIFIER = '1';
static const char K_LINESTRING_IDENTIFIER = '2';
static const char K_POLYGON_IDENTIFIER = '3';

unsigned int GetCellLength(const DGGS_Cell a_cell)
{
  return strlen(a_cell);
}

unsigned int GetLinestringLength(const DGGS_Linestring a_linestring)
{
  unsigned int stringLength = 0;

  for (int cellIndex = 0; cellIndex < a_linestring.m_noOfCells; ++cellIndex)
  {
    // Adds one to accommodate separator character
    stringLength += (GetCellLength(a_linestring.m_cells[cellIndex]) + 1);
  }

  return stringLength;
}

unsigned int GetPolygonLength(const DGGS_Polygon a_polygon)
{
  unsigned int stringLength = 0;

  // Process the outer ring
  stringLength += GetLinestringLength(a_polygon.m_outerRing);

  // Process the inner rings
  for (int ringIndex = 0; ringIndex < a_polygon.m_noOfInnerRings; ++ringIndex)
  {
    stringLength += GetLinestringLength(a_polygon.m_innerRings[ringIndex]);
  }

  return stringLength;
}

void CopyShapeIdentifier(char* a_destination, const DGGS_ShapeType shapeType)
{
  switch (shapeType)
  {
    case DGGS_CELL:
    {
      a_destination[0] = K_CELL_IDENTIFIER;
      break;
    }
    case DGGS_LINESTRING:
    {
      a_destination[0] = K_LINESTRING_IDENTIFIER;
      break;
    }
    case DGGS_POLYGON:
    {
      a_destination[0] = K_POLYGON_IDENTIFIER;
      break;
    }
  }

  a_destination[1] = K_SHAPE_IDENTIFIER_SEPARATOR;
}

unsigned int CopyCell(char* a_destination, const DGGS_Cell a_cell)
{
  unsigned int cellLength = GetCellLength(a_cell);
  strncpy(a_destination, a_cell, cellLength);
  strncpy(&a_destination[cellLength], &K_CELL_SEPARATOR, 1);
  return (cellLength + 1);
}

unsigned int CopyLinestring(char* a_destination, const DGGS_Linestring a_linestring)
{
  unsigned int charsCopied = 0;

  for (int cellIndex = 0; cellIndex < a_linestring.m_noOfCells; ++cellIndex)
  {
    charsCopied += CopyCell(&a_destination[charsCopied], a_linestring.m_cells[cellIndex]);
  }

  // Overwrite the last cell separator with a linestring separator
  strncpy(&a_destination[charsCopied - 1], &K_LINESTRING_SEPARATOR, 1);

  return charsCopied;
}

unsigned int CopyPolygon(char* a_destination, const DGGS_Polygon a_polygon)
{
  unsigned int charsCopied = 0;

  // Copy the outer ring
  charsCopied += CopyLinestring(&a_destination[charsCopied], a_polygon.m_outerRing);

  // Copy the inner rings
  for (int ringIndex = 0; ringIndex < a_polygon.m_noOfInnerRings; ++ringIndex)
  {
    charsCopied += CopyLinestring(&a_destination[charsCopied], a_polygon.m_innerRings[ringIndex]);
  }

  return charsCopied;
}

// AssignCell is different from CopyCell, as it copies all the characters corresponding to the cell id
void AssignCell(DGGS_Cell destination, const DGGS_Cell source)
{
  strncpy(destination, source, EAGGR_MAX_CELL_STRING_LENGTH);
}

DGGS_Handle GetDggsHandle(char* a_dggsModelString)
{
  DGGS_Handle g_dggs;

  if (strcmp(a_dggsModelString, "ISEA4T") == 0)
  {
    if (g_dggsISEA4T == NULL)
    {
      EAGGR_OpenDggsHandle(DGGS_ISEA4T, &g_dggsISEA4T);
    }
    g_dggs = g_dggsISEA4T;
  }
  else if (strcmp(a_dggsModelString, "ISEA3H") == 0)
  {
    if (g_dggsISEA3H == NULL)
    {
      EAGGR_OpenDggsHandle(DGGS_ISEA3H, &g_dggsISEA3H);
    }
    g_dggs = g_dggsISEA3H;
  }
  else
  {
    ereport(
        ERROR,
        (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg(
            "Unsupported DGGS model. Please choose 'ISEA4T' or 'ISEA3H'.")));
  }
  return g_dggs;
}

int CountTokens(char* stringToTokenise, char* delimiters)
{
  // Creates a copy of stringToTokenise as strtok will modify stringToTokenise
  int stringToTokeniseLength = strlen(stringToTokenise);
  char stringToTokeniseCopy[stringToTokeniseLength + 1];
  strlcpy(stringToTokeniseCopy, stringToTokenise, stringToTokeniseLength + 1);

  int tokenCount = 0;
  char* stringToken = strtok(stringToTokeniseCopy, delimiters);
  while (stringToken != NULL)
  {
    ++tokenCount;
    stringToken = strtok(NULL, delimiters);
  }
  return tokenCount;
}

void ExtractShapeTypeAndCells(char* shapeString, char** shapeStringCells, char** shapeTypeChar)
{

  // Counts the number of characters in shapeString before the shape identifier separator
  int shapeTypeDelimiterPosition = strcspn(shapeString, &K_SHAPE_IDENTIFIER_SEPARATOR);

  // strcspn will return 1 if shapeString is of length 1, so the length of shapeString is also tested
  // A valid shape string should be more than one character
  if ((shapeTypeDelimiterPosition == 1) && (strlen(shapeString) > 1))
  {
    *shapeTypeChar = strtok(shapeString, &K_SHAPE_IDENTIFIER_SEPARATOR);

    // Retrieves the shape string after the shape identification character
    *shapeStringCells = strtok(NULL, &K_SHAPE_IDENTIFIER_SEPARATOR);

    if (*shapeStringCells != NULL)
    {
      if ((strncmp(*shapeTypeChar, &K_CELL_IDENTIFIER, 1) != 0)
          && (strncmp(*shapeTypeChar, &K_LINESTRING_IDENTIFIER, 1) != 0)
          && (strncmp(*shapeTypeChar, &K_POLYGON_IDENTIFIER, 1) != 0))
      {
        ereport(
            ERROR,
            (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg(
                "Invalid shape identifier provided. The shape identifier is provided at the start of a shape and is separated from the cells of the shape by a '~'. Valid shape identifiers are '1', '2' or '3', representing a cell, linestring or polygon respectively.")));
      }
    }
    else
    {
      ereport(
          ERROR,
          (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg(
              "No cells were provided for the shape, after the '~' character in the shape string.")));
    }
  }
  else
  {
    ereport(
        ERROR,
        (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg(
            "Invalid shape identifier separator provided. The shape identifier is a character supplied at the start of the shape string and is separated from the cells of the shape string by a '~'.")));
  }
}

void ExtractDggsShapeFromString(char* shapeString, DGGS_Shape * shape)
{
  char shapeDelimiters[2] =
  { K_SHAPE_SEPARATOR, K_TERMINATING_CHAR };

  char linestringDelimiters[2] =
  { K_LINESTRING_SEPARATOR, K_TERMINATING_CHAR };

  char cellDelimiters[2] =
  { K_CELL_SEPARATOR, K_TERMINATING_CHAR };

  char* shapeTypeChar;
  char* shapeStringCells;

  ExtractShapeTypeAndCells(shapeString, &shapeStringCells, &shapeTypeChar);

  // Counts the number of shapes
  unsigned int shapeCount = CountTokens(shapeStringCells, shapeDelimiters);

  if (shapeCount != 1)
  {
    ereport(
        ERROR,
        (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg(
            "Too many shapes supplied in one or more shape string. EAGGR_ShapeComparison only compares two shapes, each specified in separate shape strings.")));
  }

  // Counts the number of linestrings
  unsigned char linestringCount = CountTokens(shapeStringCells, linestringDelimiters);

  // Identifies the tokenising of the shape into linestrings
  char* shapeParsingIdentifier;
  char* linestring = strtok_r(shapeStringCells, linestringDelimiters, &shapeParsingIdentifier);

  // Identifies the tokenising of the linestring into cells
  char* linestringParsingIdentifier;

  // Counts the number of cells in the first linestring
  unsigned int linestringCellCount = CountTokens(linestring, cellDelimiters);

  switch (*shapeTypeChar)
  {
    case (int) K_CELL_IDENTIFIER:
    {
      if ((linestringCount == 1) && (linestringCellCount == 1))
      {
        shape->m_type = DGGS_CELL;
        shape->m_location = DGGS_NO_LOCATION;
        AssignCell(shape->m_data.m_cell, linestring);
      }
      else
      {
        ereport(
            ERROR,
            (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg(
                "The shape identifier and the shape do not match. The shape identifier indicates a cell, but the shape supplied is not a cell.")));
      }
      break;
    }
    case (int) K_LINESTRING_IDENTIFIER:
    {
      if ((linestringCount == 1) && (linestringCellCount > 1))
      {
        shape->m_type = DGGS_LINESTRING;
        shape->m_location = DGGS_NO_LOCATION;
        shape->m_data.m_linestring.m_noOfCells = linestringCellCount;

        // Allocates memory
        shape->m_data.m_linestring.m_cells = (DGGS_Cell*) palloc(
            linestringCellCount * sizeof(DGGS_Cell));

        int cellIndex = 0;
        char* cellId = strtok_r(linestring, cellDelimiters, &linestringParsingIdentifier);
        while (cellId != NULL)
        {
          AssignCell(shape->m_data.m_linestring.m_cells[cellIndex], cellId);
          ++cellIndex;
          cellId = strtok_r(NULL, cellDelimiters, &linestringParsingIdentifier);
        }
      }
      else
      {
        ereport(
            ERROR,
            (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg(
                "The shape identifier and the shape do not match. The shape identifier indicates a linestring, but the shape supplied is not a linestring.")));
      }
      break;
    }
    case (int) K_POLYGON_IDENTIFIER:
    {
      shape->m_type = DGGS_POLYGON;
      shape->m_location = DGGS_NO_LOCATION;
      shape->m_data.m_polygon.m_outerRing.m_noOfCells = linestringCellCount;
      shape->m_data.m_polygon.m_noOfInnerRings = linestringCount - 1;

      // Extract outer linestring

      // Allocates memory
      shape->m_data.m_polygon.m_outerRing.m_cells = (DGGS_Cell*) palloc(
          linestringCellCount * sizeof(DGGS_Cell));

      char* cellId = strtok_r(linestring, cellDelimiters, &linestringParsingIdentifier);

      int cellIndex = 0;
      while (cellId != NULL)
      {
        AssignCell(shape->m_data.m_polygon.m_outerRing.m_cells[cellIndex], cellId);
        ++cellIndex;
        cellId = strtok_r(NULL, cellDelimiters, &linestringParsingIdentifier);
      }

      // Extract inner rings

      linestring = strtok_r(NULL, linestringDelimiters, &shapeParsingIdentifier);

      // Allocates memory for inner rings
      if (linestringCount > 1)
      {
        shape->m_data.m_polygon.m_innerRings = (DGGS_Linestring*) palloc(
            (linestringCount - 1) * sizeof(DGGS_Linestring));
      }

      int linestringIndex = 0;
      while (linestring != NULL)
      {
        unsigned int innerRingCellCount = CountTokens(linestring, cellDelimiters);
        shape->m_data.m_polygon.m_innerRings[linestringIndex].m_noOfCells = innerRingCellCount;

        // Allocates memory
        shape->m_data.m_polygon.m_innerRings[linestringIndex].m_cells = (DGGS_Cell*) palloc(
            innerRingCellCount * sizeof(DGGS_Cell));

        cellId = strtok_r(linestring, cellDelimiters, &linestringParsingIdentifier);

        int cellIndex = 0;
        while (cellId != NULL)
        {
          AssignCell(
              shape->m_data.m_polygon.m_innerRings[linestringIndex].m_cells[cellIndex],
              cellId);
          ++cellIndex;
          cellId = strtok_r(NULL, cellDelimiters, &linestringParsingIdentifier);
        }

        ++linestringIndex;
        linestring = strtok_r(NULL, linestringDelimiters, &shapeParsingIdentifier);
      }
      break;
    }
  }

}

void GetDggsErrorMessage(DGGS_Handle a_dggs)
{
  char* p_dggsErrorMessage;
  unsigned short messageLength;
  EAGGR_GetLastErrorMessage(a_dggs, &p_dggsErrorMessage, &messageLength);
  char dggsErrorMessage[messageLength + 1];
  strlcpy(dggsErrorMessage, p_dggsErrorMessage, messageLength + 1);
  EAGGR_DeallocateString(a_dggs, &p_dggsErrorMessage);
  ereport(ERROR, (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg(dggsErrorMessage)));
}

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1 (EAGGR_Version);
Datum EAGGR_Version(PG_FUNCTION_ARGS)
{
  char* versionBuffer = palloc(EAGGR_VERSION_STRING_LENGTH * sizeof(char));
  EAGGR_GetVersion(versionBuffer);

  PG_RETURN_TEXT_P(cstring_to_text(versionBuffer));
}

PG_FUNCTION_INFO_V1 (EAGGR_ToCells);
Datum EAGGR_ToCells(PG_FUNCTION_ARGS)
{
  text* shapeText = PG_GETARG_TEXT_P(0);
  char* wktString = text_to_cstring(shapeText);

  double accuracy = PG_GETARG_FLOAT8(1);

  text* dggsModelText = PG_GETARG_TEXT_P(2);
  char* dggsModelString = text_to_cstring(dggsModelText);

  DGGS_Handle g_dggs = GetDggsHandle(dggsModelString);

  DGGS_Shape* shapes;
  unsigned short noOfShapes = 0;
  int noOfShapeIdentifierCharacters = 2;

  DGGS_ReturnCode returnCode = EAGGR_ConvertShapeStringToDggsShapes(
      g_dggs,
      wktString,
      DGGS_WKT_FORMAT,
      accuracy,
      &shapes,
      &noOfShapes);

  if (returnCode != DGGS_SUCCESS)
  {
    GetDggsErrorMessage(g_dggs);
  }

  // Calculate the length of the resulting string
  unsigned int stringLength = 0;

  for (unsigned short shapeIndex = 0; shapeIndex < noOfShapes; ++shapeIndex)
  {
    switch (shapes[shapeIndex].m_type)
    {
      case DGGS_CELL:
        stringLength += (GetCellLength(shapes[shapeIndex].m_data.m_cell) + 1);
        break;
      case DGGS_LINESTRING:
        stringLength += GetLinestringLength(shapes[shapeIndex].m_data.m_linestring);
        break;
      case DGGS_POLYGON:
        stringLength += GetPolygonLength(shapes[shapeIndex].m_data.m_polygon);
        break;
    }
  }

  // Adds on the characters for the shape identifier
  stringLength += noOfShapeIdentifierCharacters * noOfShapes;

  char result[stringLength];

  unsigned int resultIndex = 0;
  for (unsigned short shapeIndex = 0; shapeIndex < noOfShapes; ++shapeIndex)
  {
    switch (shapes[shapeIndex].m_type)
    {
      case DGGS_CELL:
        CopyShapeIdentifier(&result[resultIndex], DGGS_CELL);
        resultIndex += noOfShapeIdentifierCharacters;
        resultIndex += CopyCell(&result[resultIndex], shapes[shapeIndex].m_data.m_cell);
        break;
      case DGGS_LINESTRING:
        CopyShapeIdentifier(&result[resultIndex], DGGS_LINESTRING);
        resultIndex += noOfShapeIdentifierCharacters;
        resultIndex += CopyLinestring(&result[resultIndex], shapes[shapeIndex].m_data.m_linestring);
        break;
      case DGGS_POLYGON:
        CopyShapeIdentifier(&result[resultIndex], DGGS_POLYGON);
        resultIndex += noOfShapeIdentifierCharacters;
        resultIndex += CopyPolygon(&result[resultIndex], shapes[shapeIndex].m_data.m_polygon);
        break;
    }

    // Overwrite the last separator with a shape separator
    strncpy(&result[resultIndex - 1], &K_SHAPE_SEPARATOR, 1);
  }

  // Overwrite the last shape separator with a null terminator
  strncpy(&result[stringLength - 1], &K_TERMINATING_CHAR, 1);

  // Deallocate memory
  EAGGR_DeallocateDggsShapes(g_dggs, &shapes, noOfShapes);

  PG_RETURN_TEXT_P(cstring_to_text(result));
}

PG_FUNCTION_INFO_V1 (EAGGR_ToCellArray);
Datum EAGGR_ToCellArray(PG_FUNCTION_ARGS)
{
  text* shapeText = PG_GETARG_TEXT_P(0);
  char* shapeString = text_to_cstring(shapeText);

  char delimiters[4];
  delimiters[0] = K_CELL_SEPARATOR;
  delimiters[1] = K_LINESTRING_SEPARATOR;
  delimiters[2] = K_SHAPE_SEPARATOR;
  delimiters[3] = K_TERMINATING_CHAR;

  // Count the number of cells
  unsigned int cellCount = CountTokens(shapeString, delimiters);

  char* shapeParsingIdentifier;
  char* cell;

  // Loop again to extract the cells
  // Get the shape text again - it was modified by strtok in CountTokens
  shapeString = text_to_cstring(shapeText);
  Datum* cellsDatum = (Datum*) palloc(cellCount * sizeof(Datum));
  char* cellString = strtok_r(shapeString, delimiters, &shapeParsingIdentifier);

  unsigned int cellIndex = 0;
  while (cellString != NULL)
  {
    // If the shape identifier is found in cellString and the cellString is more than one character,
    // then extract the cell after it
    if ((strcspn(cellString, &K_SHAPE_IDENTIFIER_SEPARATOR) == 1) && (strlen(cellString) > 1))
    {
      char* linestringParsingIdentifier;
      cell = strtok_r(cellString, &K_SHAPE_IDENTIFIER_SEPARATOR, &linestringParsingIdentifier);
      cell = strtok_r(NULL, &K_SHAPE_IDENTIFIER_SEPARATOR, &linestringParsingIdentifier);
    }
    else
    {
      cell = cellString;
    }

    cellsDatum[cellIndex] = PointerGetDatum(cstring_to_text(cell));
    ++cellIndex;
    cellString = strtok_r(NULL, delimiters, &shapeParsingIdentifier);
  }

  int16 typlen;
  bool typbyval;
  char typalign;
  get_typlenbyvalalign(TEXTOID, &typlen, &typbyval, &typalign);
  ArrayType* cellArray = construct_array(
      cellsDatum,
      cellCount,
      TEXTOID,
      typlen,
      typbyval,
      typalign);

  PG_RETURN_ARRAYTYPE_P(cellArray);
}

PG_FUNCTION_INFO_V1 (EAGGR_GetBoundingCell);
Datum EAGGR_GetBoundingCell(PG_FUNCTION_ARGS)
{
  ArrayType *array = PG_GETARG_ARRAYTYPE_P(0);

  text* dggsModelText = PG_GETARG_TEXT_P(1);
  char* dggsModelString = text_to_cstring(dggsModelText);

  DGGS_Handle g_dggs = GetDggsHandle(dggsModelString);

  Oid arrayElementType;
  int16 arrayElementTypeWidth;
  bool arrayElementTypeByValue;
  char arrayElementTypeAlignmentCode;
  Datum *arrayContent;
  bool *arrayNullFlags;
  int noOfCells = 0;

  // Determine the array element types.
  arrayElementType = ARR_ELEMTYPE(array);
  get_typlenbyvalalign(
      arrayElementType,
      &arrayElementTypeWidth,
      &arrayElementTypeByValue,
      &arrayElementTypeAlignmentCode);

  // Extract the array contents (as Datum objects).
  deconstruct_array(
      array,
      arrayElementType,
      arrayElementTypeWidth,
      arrayElementTypeByValue,
      arrayElementTypeAlignmentCode,
      &arrayContent,
      &arrayNullFlags,
      &noOfCells);

  DGGS_Cell p_cells[noOfCells];

  // Extract the DGGS cells from the array
  for (unsigned int cellIndex = 0; cellIndex < noOfCells; cellIndex++)
  {
    char* cell = DatumGetCString(
        DirectFunctionCall1(textout, PointerGetDatum(arrayContent[cellIndex])));
    AssignCell(p_cells[cellIndex], cell);
  }

  DGGS_Cell boundingCell;
  DGGS_ReturnCode returnCode = EAGGR_GetBoundingDggsCell(g_dggs, p_cells, noOfCells, &boundingCell);

  if (returnCode != DGGS_SUCCESS)
  {
    GetDggsErrorMessage(g_dggs);
  }

  PG_RETURN_TEXT_P(cstring_to_text(boundingCell));
}

PG_FUNCTION_INFO_V1 (EAGGR_CellGeometry);
Datum EAGGR_CellGeometry(PG_FUNCTION_ARGS)
{
  text* cellText = PG_GETARG_TEXT_P(0);
  char* cellString = text_to_cstring(cellText);

  text* dggsModelText = PG_GETARG_TEXT_P(1);
  char* dggsModelString = text_to_cstring(dggsModelText);

  DGGS_Handle g_dggs = GetDggsHandle(dggsModelString);

  DGGS_Cell cell;
  AssignCell(cell, cellString);

  char* shapeString;

  DGGS_ReturnCode returnCode = EAGGR_ConvertDggsCellOutlineToShapeString(
      g_dggs,
      cell,
      DGGS_WKT_FORMAT,
      &shapeString);

  if (returnCode != DGGS_SUCCESS)
  {
    GetDggsErrorMessage(g_dggs);
  }

  int shapeStringLength = strlen(shapeString);
  char returnedShapeString[shapeStringLength + 1];
  strlcpy(returnedShapeString, shapeString, shapeStringLength + 1);

  EAGGR_DeallocateString(g_dggs, &shapeString);

  PG_RETURN_TEXT_P(cstring_to_text(returnedShapeString));
}

PG_FUNCTION_INFO_V1 (EAGGR_CellToPoint);
Datum EAGGR_CellToPoint(PG_FUNCTION_ARGS)
{
  text* cellText = PG_GETARG_TEXT_P(0);
  char* cellString = text_to_cstring(cellText);

  text* dggsModelText = PG_GETARG_TEXT_P(1);
  char* dggsModelString = text_to_cstring(dggsModelText);

  DGGS_Handle g_dggs = GetDggsHandle(dggsModelString);

  DGGS_Cell cell;

  AssignCell(cell, cellString);

  char* shapeString;
  DGGS_ReturnCode returnCode = EAGGR_ConvertDggsCellsToShapeString(
      g_dggs,
      &cell,
      1,
      DGGS_WKT_FORMAT,
      &shapeString);

  if (returnCode != DGGS_SUCCESS)
  {
    GetDggsErrorMessage(g_dggs);
  }

  int shapeStringLength = strlen(shapeString);
  char returnedShapeString[shapeStringLength + 1];
  strlcpy(returnedShapeString, shapeString, shapeStringLength + 1);

  EAGGR_DeallocateString(g_dggs, &shapeString);

  PG_RETURN_TEXT_P(cstring_to_text(returnedShapeString));
}

PG_FUNCTION_INFO_V1 (EAGGR_ShapeComparison);
Datum EAGGR_ShapeComparison(PG_FUNCTION_ARGS)
{
  text* analysisTypeText = PG_GETARG_TEXT_P(0);
  char* analysisTypeString = text_to_cstring(analysisTypeText);

  DGGS_AnalysisType analysisType;

  if (strcmp(analysisTypeString, "EQUALS") == 0)
  {
    analysisType = DGGS_EQUALS;
  }
  else if (strcmp(analysisTypeString, "CONTAINS") == 0)
  {
    analysisType = DGGS_CONTAINS;
  }
  else if (strcmp(analysisTypeString, "WITHIN") == 0)
  {
    analysisType = DGGS_WITHIN;
  }
  else if (strcmp(analysisTypeString, "TOUCHES") == 0)
  {
    analysisType = DGGS_TOUCHES;
  }
  else if (strcmp(analysisTypeString, "DISJOINT") == 0)
  {
    analysisType = DGGS_DISJOINT;
  }
  else if (strcmp(analysisTypeString, "INTERSECTS") == 0)
  {
    analysisType = DGGS_INTERSECTS;
  }
  else if (strcmp(analysisTypeString, "COVERS") == 0)
  {
    analysisType = DGGS_COVERS;
  }
  else if (strcmp(analysisTypeString, "COVERED_BY") == 0)
  {
    analysisType = DGGS_COVERED_BY;
  }
  else if (strcmp(analysisTypeString, "CROSSES") == 0)
  {
    analysisType = DGGS_CROSSES;
  }
  else if (strcmp(analysisTypeString, "OVERLAPS") == 0)
  {
    analysisType = DGGS_OVERLAPS;
  }
  else
  {
    ereport(
        ERROR,
        (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION), errmsg("Unsupported analysis type.")));
  }

  text* baseShapeText = PG_GETARG_TEXT_P(1);
  char* baseShapeString = text_to_cstring(baseShapeText);

  text* comparisonShapeText = PG_GETARG_TEXT_P(2);
  char* comparisonShapeString = text_to_cstring(comparisonShapeText);

  text* dggsModelText = PG_GETARG_TEXT_P(3);
  char* dggsModelString = text_to_cstring(dggsModelText);

  DGGS_Handle g_dggs = GetDggsHandle(dggsModelString);

  DGGS_Shape baseShape;
  DGGS_Shape comparisonShape;
  ExtractDggsShapeFromString(baseShapeString, &baseShape);
  ExtractDggsShapeFromString(comparisonShapeString, &comparisonShape);

  bool comparisonResult = false;
  DGGS_ReturnCode returnCode = EAGGR_CompareShapes(
      g_dggs,
      analysisType,
      &baseShape,
      &comparisonShape,
      &comparisonResult);

  if (returnCode != DGGS_SUCCESS)
  {
    GetDggsErrorMessage(g_dggs);
  }

  PG_RETURN_BOOL(comparisonResult);
}
