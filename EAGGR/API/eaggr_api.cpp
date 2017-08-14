//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: C API
//
//------------------------------------------------------
/// @file eaggr_api.cpp
///
/// C API interface for the EAGGR library.
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "eaggr_api.h"

#include "API/eaggr_api_funcs.hpp"
#include "API/eaggr_api_exceptions.hpp"
#include "API/dggs_data_store.hpp"
#include "Src/ImportExport/GeoJsonImporter.hpp"
#include "Src/ImportExport/WktImporter.hpp"
#include "Src/ImportExport/IShapeExporter.hpp"
#include "Src/ImportExport/GeoJsonExporter.hpp"
#include "Src/ImportExport/WktExporter.hpp"
#include "Src/ImportExport/KmlExporter.hpp"
#include "Src/SpatialAnalysis/SpatialAnalysis.hpp"

using namespace EAGGR;
using namespace EAGGR::API;
using namespace EAGGR::SpatialAnalysis;

/// Macro for storing the message for the last error to occur.
#define SET_ERROR_MESSAGE(a_handle, a_message) \
  g_dggsDataStore.SetErrorMessage(a_handle, a_message);

/// Macro for checking the handle to the DGGS model. Function will return a
/// DGGS_INVALID_HANDLE code if handle is null.
#define CHECK_DGGS_HANDLE(a_handle) \
  if (a_handle == NULL) \
  { \
    SET_ERROR_MESSAGE(a_handle, "Handle to DGGS model is null.") \
    return (DGGS_INVALID_HANDLE); \
  }

/// Macro for checking a pointer argument to a function. Function will return
/// a DGGS_NULL_POINTER code if pointer is null.
#define CHECK_POINTER(a_handle, a_pointer, a_pointerName) \
  if (a_pointer == NULL) \
  { \
    if (a_handle != NULL) \
    { \
      std::stringstream stream; \
      stream << "Pointer argument '" << a_pointerName << "' is null"; \
      SET_ERROR_MESSAGE(a_handle, stream.str()) \
    } \
    return (DGGS_NULL_POINTER); \
  }

/// Macro to prevent any exceptions being thrown by the DLL.
#define CATCH_ALL(a_handle) \
  catch (EAGGR::EAGGRException & exception) \
  { \
    SET_ERROR_MESSAGE(a_handle, exception.what()) \
    return (DGGS_MODEL_ERROR); \
  } \
  catch (...) \
  { \
    SET_ERROR_MESSAGE(a_handle, "Unknown Error") \
    return (DGGS_UNKNOWN_ERROR); \
  }

/// Global map for storing data so it can be destroyed when the DGGS handle is closed.
DggsDataStore g_dggsDataStore;

DGGS_ReturnCode EAGGR_GetVersion(char * a_versionString)
{
  CHECK_POINTER(NULL, a_versionString, "a_versionString");

  try
  {
    static_cast<void>(strncpy(a_versionString, "v2.0", EAGGR_VERSION_STRING_LENGTH));
  }
  // Ensure no exceptions are thrown from the function
  catch (...)
  {
    return (DGGS_UNKNOWN_ERROR);
  }

  return (DGGS_SUCCESS);
}

DGGS_ReturnCode EAGGR_GetLastErrorMessage(
    DGGS_Handle a_handle,
    char ** a_pErrorMessage,
    unsigned short * a_pMessageLength)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_pErrorMessage, "a_pErrorMessage");
  CHECK_POINTER(a_handle, a_pMessageLength, "a_pMessageLength");

  try
  {
    std::string & message = g_dggsDataStore.GetLastErrorMessage(a_handle);

    *a_pMessageLength = message.size() * sizeof(char);
    if (*a_pMessageLength > 0U)
    {
      // Allow for string's terminating character
      *a_pMessageLength += sizeof(TERMINATING_CHAR);

      *a_pErrorMessage = static_cast<char *>(malloc(*a_pMessageLength));
      static_cast<void>(strncpy(*a_pErrorMessage, message.c_str(), *a_pMessageLength));
    }
    else
    {
      *a_pErrorMessage = NULL;
    }
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_OpenDggsHandle(const DGGS_Model a_dggsModel, DGGS_Handle * a_pHandle)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_POINTER(NULL, a_pHandle, "a_pHandle");

  try
  {
    DggsData data;

    // Create the necessary object for the DGGS model
    data.m_pGlobe = new Model::PolyhedralGlobe::Icosahedron;
    data.m_pProjection = new Model::Projection::Snyder(data.m_pGlobe);
    data.m_pConverter = new CoordinateConversion::CoordinateConverter;

    switch (a_dggsModel)
    {
      case DGGS_ISEA4T:
      {
        Model::Grid::HierarchicalGrid::Aperture4TriangleGrid * triangleGrid =
            new Model::Grid::HierarchicalGrid::Aperture4TriangleGrid;
        data.m_pGrid = triangleGrid;
        data.m_pIndexer = new Model::GridIndexer::HierarchicalGridIndexer(
            triangleGrid,
            data.m_pGlobe->GetNoOfFaces() - 1U);
        break;
      }
      case DGGS_ISEA3H:
      {
        Model::Grid::OffsetGrid::Aperture3HexagonGrid * hexagonGrid =
            new Model::Grid::OffsetGrid::Aperture3HexagonGrid;
        data.m_pGrid = hexagonGrid;
        data.m_pIndexer = new Model::GridIndexer::OffsetGridIndexer(
            hexagonGrid,
            data.m_pGlobe->GetNoOfFaces() - 1U);
        break;
      }
      default:
      {
        SET_ERROR_MESSAGE(*a_pHandle, "Requested DGGS model is not supported.")
        return (DGGS_INVALID_PARAM);
      }
    }

    // Create the DGGS model
    *a_pHandle = static_cast<DGGS_Handle>(new Model::DGGS(data.m_pProjection, data.m_pIndexer));

    // Initialise the last error message to a blank string
    data.m_lastErrorMesssage = "";

    // Store DGGS data so we can destroy it when the handle is closed
    g_dggsDataStore.SetDggsData(*a_pHandle, data);
  }
  catch (std::bad_alloc &)
  {
    return (DGGS_MEMORY_ALLOCATION_FAILURE);
  }
  CATCH_ALL(*a_pHandle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_CloseDggsHandle(DGGS_Handle * a_pHandle)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  // Check both the pointer and the handle it points to are not null
  CHECK_POINTER(NULL, a_pHandle, "a_pHandle");
  CHECK_DGGS_HANDLE(*a_pHandle);

  try
  {
    // Destroy all the objects used by the DGGS model
    DggsData dggsData = g_dggsDataStore.GetDggsData(*a_pHandle);
    delete dggsData.m_pGlobe;
    delete dggsData.m_pProjection;
    delete dggsData.m_pGrid;
    delete dggsData.m_pIndexer;
    delete dggsData.m_pConverter;

    g_dggsDataStore.RemoveDggsData(*a_pHandle);

    // Destroy the DGGS model itself
    delete (Model::DGGS *) *a_pHandle;

    // Set the handle to null so it cannot be used anymore
    *a_pHandle = NULL;
  }
  CATCH_ALL(*a_pHandle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_ConvertPointsToDggsCells(
    const DGGS_Handle a_handle,
    const DGGS_LatLongPoint * a_points,
    const unsigned short a_noOfPoints,
    DGGS_Cell * a_pDggsCells)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_points, "a_points");
  CHECK_POINTER(a_handle, a_pDggsCells, "a_dggsCells");

  try
  {
    // Iterate through the array of points
    for (unsigned short pointIndex = 0U; pointIndex < a_noOfPoints; pointIndex++)
    {
      // Move data into a Wgs84AccuracyPoint object
      const LatLong::Wgs84AccuracyPoint wgs84Point(
          a_points[pointIndex].m_latitude,
          a_points[pointIndex].m_longitude,
          a_points[pointIndex].m_accuracy);

      // Convert the point and add it to the DGGS cells
      DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

      GetDggsCellFromWgs84AccuracyPoint(
          a_handle,
          dggsData.m_pConverter,
          &wgs84Point,
          &(a_pDggsCells[pointIndex]));
    }
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_ConvertShapesToDggsShapes(
    const DGGS_Handle a_handle,
    const DGGS_LatLongShape * a_shapes,
    const unsigned short a_noOfShapes,
    DGGS_Shape ** a_pDggsShapes)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_shapes, "a_shapes");
  CHECK_POINTER(a_handle, a_pDggsShapes, "a_pDggsShapes");

  try
  {
    // Initialise the pointer so a new allocation is made on the first iteration
    *a_pDggsShapes = NULL;

    // Iterate through the array of shapes
    for (unsigned short shapeIndex = 0U; shapeIndex < a_noOfShapes; shapeIndex++)
    {
      DGGS_LatLongShape shape = a_shapes[shapeIndex];

      // Call the correct method for the shape type
      switch (shape.m_type)
      {
        case DGGS_LAT_LONG_POINT:
        {
          // Move data into a Wgs84AccuracyPoint object
          const LatLong::Wgs84AccuracyPoint wgs84Point(
              shape.m_data.m_point.m_latitude,
              shape.m_data.m_point.m_longitude,
              shape.m_data.m_point.m_accuracy);

          // Convert the point and add it to the DGGS shapes
          DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

          ConvertWgs84PointAndAddToDggsShapes(
              a_handle,
              dggsData.m_pConverter,
              &wgs84Point,
              shapeIndex,
              a_pDggsShapes);

          break;
        }
        case DGGS_LAT_LONG_LINESTRING:
        {
          // Move data into a Wgs84Linestring object
          LatLong::Wgs84Linestring wgs84Linestring;
          for (unsigned short pointIndex = 0U; pointIndex < shape.m_data.m_linestring.m_noOfPoints;
              pointIndex++)
          {
            const DGGS_LatLongPoint & latLongPoint = shape.m_data.m_linestring.m_points[pointIndex];
            wgs84Linestring.AddAccuracyPoint(
                latLongPoint.m_latitude,
                latLongPoint.m_longitude,
                latLongPoint.m_accuracy);
          }

          // Convert the linestring and add it to the DGGS shapes
          DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

          ConvertWgs84LinestringAndAddToDggsShapes(
              a_handle,
              dggsData.m_pConverter,
              &wgs84Linestring,
              shapeIndex,
              a_pDggsShapes);

          break;
        }
        case DGGS_LAT_LONG_POLYGON:
        {
          // Move data into a Wgs84Polygon object
          LatLong::Wgs84Polygon wgs84Polygon;

          for (unsigned short pointIndex = 0U;
              pointIndex < shape.m_data.m_polygon.m_outerRing.m_noOfPoints; pointIndex++)
          {
            const DGGS_LatLongPoint & latLongPoint =
                shape.m_data.m_polygon.m_outerRing.m_points[pointIndex];
            wgs84Polygon.AddAccuracyPointToOuterRing(
                latLongPoint.m_latitude,
                latLongPoint.m_longitude,
                latLongPoint.m_accuracy);
          }

          for (unsigned short ringIndex = 0U; ringIndex < shape.m_data.m_polygon.m_noOfInnerRings;
              ringIndex++)
          {
            wgs84Polygon.CreateInnerRing();

            for (unsigned short pointIndex = 0U;
                pointIndex < shape.m_data.m_polygon.m_innerRings[ringIndex].m_noOfPoints;
                pointIndex++)
            {
              const DGGS_LatLongPoint & latLongPoint =
                  shape.m_data.m_polygon.m_innerRings[ringIndex].m_points[pointIndex];
              wgs84Polygon.AddAccuracyPointToInnerRing(
                  ringIndex,
                  latLongPoint.m_latitude,
                  latLongPoint.m_longitude,
                  latLongPoint.m_accuracy);
            }
          }

          // Convert the polygon and add it to the DGGS shapes
          DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

          ConvertWgs84PolygonAndAddToDggsShapes(
              a_handle,
              dggsData.m_pConverter,
              &wgs84Polygon,
              shapeIndex,
              a_pDggsShapes);

          break;
        }
        default:
        {
          std::stringstream stream;
          stream << "Type of shape " << (shapeIndex + 1U) << " in the input array is not supported";
          SET_ERROR_MESSAGE(a_handle, stream.str());
          return (DGGS_INVALID_PARAM);
        }
      }
    }
  }
  catch (MemoryAllocationException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_MEMORY_ALLOCATION_FAILURE);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_ConvertShapeStringToDggsShapes(
    const DGGS_Handle a_handle,
    const DGGS_ShapeString a_string,
    const DGGS_ShapeStringFormat a_format,
    const double a_accuracy,
    DGGS_Shape ** a_pDggsShapes,
    unsigned short * a_pNoOfShapes)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_string, "a_string");
  CHECK_POINTER(a_handle, a_pDggsShapes, "a_pDggsShapes");
  CHECK_POINTER(a_handle, a_pNoOfShapes, "a_pNoOfShapes");

  try
  {
    ImportExport::AbstractShapeImporter * pShapeImporter = NULL;

    try
    {
      // Get the correct shape importer for the string format
      switch (a_format)
      {
        case (DGGS_WKT_FORMAT):
        {
          ImportExport::WktImporter * pWktImporter = new ImportExport::WktImporter(
              a_string,
              a_accuracy);
          pShapeImporter = pWktImporter;
          break;
        }
        case (DGGS_GEO_JSON_FORMAT):
        {
          ImportExport::GeoJsonImporter * pGeoJsonImporter = new ImportExport::GeoJsonImporter(
              a_string,
              a_accuracy);
          pShapeImporter = pGeoJsonImporter;
          break;
        }
        default:
        {
          SET_ERROR_MESSAGE(a_handle, "Unrecognised shape string format.");
          return (DGGS_INVALID_PARAM);
        }
      }

      // Initialise the pointer so a new allocation is created on the first iteration
      *a_pDggsShapes = NULL;

      // Count the number of shapes as we add them to the output array
      *a_pNoOfShapes = 0U;

      // Iterate through the array of shapes
      while (pShapeImporter->HasNext())
      {
        LatLong::LatLongShape shape = pShapeImporter->GetNextShape();

        // Call the correct method for the shape type
        switch (shape.GetShapeType())
        {
          case LatLong::WGS84_ACCURACY_POINT:
          {
            // Convert the point and add it to the DGGS shapes
            const LatLong::Wgs84AccuracyPoint * pWgs84Point =
                static_cast<const LatLong::Wgs84AccuracyPoint *>(shape.GetShapeData());
            DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

            ConvertWgs84PointAndAddToDggsShapes(
                a_handle,
                dggsData.m_pConverter,
                pWgs84Point,
                *a_pNoOfShapes,
                a_pDggsShapes);
            break;
          }
          case LatLong::WGS84_LINESTRING:
          {
            // Convert the linestring and add it to the DGGS shapes
            const LatLong::Wgs84Linestring * pWgs84Linestring =
                static_cast<const LatLong::Wgs84Linestring *>(shape.GetShapeData());
            DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

            ConvertWgs84LinestringAndAddToDggsShapes(
                a_handle,
                dggsData.m_pConverter,
                pWgs84Linestring,
                *a_pNoOfShapes,
                a_pDggsShapes);
            break;
          }
          case LatLong::WGS84_POLYGON:
          {
            // Convert the polygon and add it to the DGGS shapes
            const LatLong::Wgs84Polygon * pWgs84Polygon =
                static_cast<const LatLong::Wgs84Polygon *>(shape.GetShapeData());
            DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

            ConvertWgs84PolygonAndAddToDggsShapes(
                a_handle,
                dggsData.m_pConverter,
                pWgs84Polygon,
                *a_pNoOfShapes,
                a_pDggsShapes);
            break;
          }
          default:
          {
            std::stringstream stream;
            stream << "Type of shape " << (*a_pNoOfShapes) + 1U << " in string is not supported";
            SET_ERROR_MESSAGE(a_handle, stream.str());
            return (DGGS_INVALID_PARAM);
          }
        }

        (*a_pNoOfShapes)++;
      }

      // Delete the importer
      delete pShapeImporter;
    }
    catch (...)
    {
      if (pShapeImporter != NULL)
      {
        // Clean up memory allocated to the importer
        delete pShapeImporter;
      }

      throw;
    }
  }
  catch (MemoryAllocationException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_MEMORY_ALLOCATION_FAILURE);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_DeallocateDggsShapes(
    const DGGS_Handle a_handle,
    DGGS_Shape ** a_pDggsShapes,
    const unsigned short a_noOfShapes)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_pDggsShapes, "a_pDggsShapes");

  // Iterate through the array freeing up the memory for each shape
  for (unsigned short shapeIndex = 0U; shapeIndex < a_noOfShapes; shapeIndex++)
  {
    switch ((*a_pDggsShapes)[shapeIndex].m_type)
    {
      case DGGS_CELL:
      {
        // Do nothing - no addition memory is allocated for cells
        break;
      }
      case DGGS_LINESTRING:
      {
        // Free memory allocated for cells in the linestring
        free(static_cast<void *>((*a_pDggsShapes)[shapeIndex].m_data.m_linestring.m_cells));

        break;
      }
      case DGGS_POLYGON:
      {
        // Free memory allocated for cells in the outer ring
        free(
            static_cast<void *>((*a_pDggsShapes)[shapeIndex].m_data.m_polygon.m_outerRing.m_cells));

        // Free memory allocated for cells in the inner rings
        const unsigned short noOfInnerRings =
            (*a_pDggsShapes)[shapeIndex].m_data.m_polygon.m_noOfInnerRings;
        for (unsigned short ringIndex = 0U; ringIndex < noOfInnerRings; ringIndex++)
        {
          free(
              static_cast<void *>((*a_pDggsShapes)[shapeIndex].m_data.m_polygon.m_innerRings[ringIndex].m_cells));
        }

        // Free memory allocated for the inner rings themselves
        free(static_cast<void *>((*a_pDggsShapes)[shapeIndex].m_data.m_polygon.m_innerRings));

        break;
      }
      default:
      {
        SET_ERROR_MESSAGE(a_handle, "Unrecognised shape type in DGGS shape array.");
        return (DGGS_INVALID_PARAM);
      }
    }
  }

  // Free up memory used for the array
  free(static_cast<void *>(*a_pDggsShapes));
  a_pDggsShapes = NULL;

  return (returnCode);
}

DGGS_ReturnCode EAGGR_DeallocateString(const DGGS_Handle a_handle, char ** a_pDggsString)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_pDggsString, "a_pDggsString");

  // Free up memory used for the string
  if (*a_pDggsString != NULL)
  {
    free(static_cast<void *>(*a_pDggsString));
    *a_pDggsString = NULL;
  }

  return (returnCode);
}

DGGS_ReturnCode EAGGR_ConvertDggsCellsToPoints(
    const DGGS_Handle a_handle,
    const DGGS_Cell * a_cells,
    const unsigned short a_noOfCells,
    DGGS_LatLongPoint * a_points)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_cells, "a_cells");
  CHECK_POINTER(a_handle, a_points, "a_points");

  try
  {
    // Iterate through the array of DGGS cells
    for (unsigned short cellIndex = 0U; cellIndex < a_noOfCells; cellIndex++)
    {
      // Check cell ID does not exceed the maximum length
      CheckCellIdLength(a_cells[cellIndex]);

      // Create an ICell object expected by the DGGS class
      DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

      std::unique_ptr < Model::Cell::ICell > cell = dggsData.m_pIndexer->CreateCell(
          a_cells[cellIndex]);

      // Convert DGGS cell to a spherical lat/long point
      LatLong::SphericalAccuracyPoint sphericalPoint =
          static_cast<Model::DGGS *>(a_handle)->ConvertCellToLatLongPoint(*cell);

      // Convert the spherical coordinates to WGS84
      const LatLong::Wgs84AccuracyPoint wgs84Point = dggsData.m_pConverter->ConvertSphereToWGS84(
          sphericalPoint);

      // Move data into the output LatLongPoint structure
      a_points[cellIndex].m_latitude = wgs84Point.GetLatitude();
      a_points[cellIndex].m_longitude = wgs84Point.GetLongitude();
      a_points[cellIndex].m_accuracy = wgs84Point.GetAccuracy();
    }
  }
  catch (MaxCellIdLengthException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_ConvertDggsCellsToShapeString(
    const DGGS_Handle a_handle,
    const DGGS_Cell * a_cells,
    const unsigned short a_noOfCells,
    const DGGS_ShapeStringFormat a_format,
    DGGS_ShapeString * a_pString)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_cells, "a_cells");
  CHECK_POINTER(a_handle, a_pString, "a_pString");

  try
  {
    ImportExport::IShapeExporter * pShapeExporter = NULL;
    std::string shapeString = "";

    try
    {
      // Get the correct shape exporter for the string format
      switch (a_format)
      {
        case (DGGS_WKT_FORMAT):
        {
          ImportExport::WktExporter * pWktExporter = new ImportExport::WktExporter();
          pShapeExporter = pWktExporter;
          break;
        }
        case (DGGS_GEO_JSON_FORMAT):
        {
          ImportExport::GeoJsonExporter * pGeoJsonExporter = new ImportExport::GeoJsonExporter();
          pShapeExporter = pGeoJsonExporter;
          break;
        }
        default:
        {
          SET_ERROR_MESSAGE(a_handle, "Unrecognised shape string format.");
          return (DGGS_INVALID_PARAM);
        }
      }

      // Create an array of shape objects to pass into the exporter
      std::vector < LatLong::LatLongShape > shapes;
      // Shape array does not allocate memory for the data so we also need another array for this
      std::vector < LatLong::Wgs84AccuracyPoint > points;

      // Iterate through the array of DGGS cells
      for (unsigned short cellIndex = 0U; cellIndex < a_noOfCells; cellIndex++)
      {
        // Check cell ID does not exceed the maximum length
        CheckCellIdLength(a_cells[cellIndex]);

        // Create an ICell object expected by the DGGS class
        DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

        std::unique_ptr < Model::Cell::ICell > cell = dggsData.m_pIndexer->CreateCell(
            a_cells[cellIndex]);

        // Convert DGGS cell to a spherical lat/long point
        LatLong::SphericalAccuracyPoint sphericalPoint =
            static_cast<Model::DGGS *>(a_handle)->ConvertCellToLatLongPoint(*cell);

        // Convert the spherical coordinates to WGS84 and store the point
        points.push_back(dggsData.m_pConverter->ConvertSphereToWGS84(sphericalPoint));
      }

      // Add lat/long points to the shape vector.
      // Note: This must be done after the points vector has been populated because elements
      // already in the vector can change location when new elements are added.
      for (unsigned short cellIndex = 0U; cellIndex < a_noOfCells; cellIndex++)
      {
        shapes.push_back(
            LatLong::LatLongShape(LatLong::WGS84_ACCURACY_POINT, &(points[cellIndex])));
      }

      // Export the points as a string and delete the exporter
      shapeString = pShapeExporter->ExportShapes(shapes);
      delete pShapeExporter;
    }
    catch (...)
    {
      if (pShapeExporter != NULL)
      {
        // Clean up memory allocated to the exporter
        delete pShapeExporter;
      }

      throw;
    }

    // Allocate memory for the output string
    *a_pString = (DGGS_ShapeString) malloc(shapeString.length() + sizeof(TERMINATING_CHAR));
    if (*a_pString == NULL)
    {
      throw MemoryAllocationException("Failed to allocate memory for the shape string");
    }

    // Copy data to the output string
    static_cast<void>(strcpy(*a_pString, shapeString.c_str()));
  }
  catch (MaxCellIdLengthException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  catch (MemoryAllocationException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_MEMORY_ALLOCATION_FAILURE);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_ConvertDggsCellOutlineToShapeString(
    const DGGS_Handle a_handle,
    const DGGS_Cell a_cell,
    const DGGS_ShapeStringFormat a_format,
    DGGS_ShapeString * a_pString)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_pString, "a_pString");

  try
  {
    ImportExport::IShapeExporter * pShapeExporter = NULL;
    std::string shapeString = "";

    try
    {
      // Get the correct shape exporter for the string format
      switch (a_format)
      {
        case (DGGS_WKT_FORMAT):
        {
          ImportExport::WktExporter * pWktExporter = new ImportExport::WktExporter();
          pShapeExporter = pWktExporter;
          break;
        }
        case (DGGS_GEO_JSON_FORMAT):
        {
          ImportExport::GeoJsonExporter * pGeoJsonExporter = new ImportExport::GeoJsonExporter();
          pShapeExporter = pGeoJsonExporter;
          break;
        }
        default:
        {
          SET_ERROR_MESSAGE(a_handle, "Unrecognised shape string format.");
          return (DGGS_INVALID_PARAM);
        }
      }

      // Create an array of shape objects to pass into the exporter
      std::vector < LatLong::LatLongShape > shapes;
      LatLong::Wgs84Polygon polygon;

      // Check cell ID does not exceed the maximum length
      CheckCellIdLength(a_cell);

      // Create an ICell object expected by the DGGS class
      DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

      std::unique_ptr < Model::Cell::ICell > cell = dggsData.m_pIndexer->CreateCell(a_cell);

      // Get vertices in spherical coordinates
      std::vector < LatLong::SphericalAccuracyPoint > sphericalPoints;
      static_cast<Model::DGGS *>(a_handle)->GetCellVertices(*cell, sphericalPoints);

      // Convert the spherical coordinates to WGS84 and add to the polygon
      for (std::vector<LatLong::SphericalAccuracyPoint>::const_iterator iter =
          sphericalPoints.begin(); iter != sphericalPoints.end(); ++iter)
      {
        LatLong::Wgs84AccuracyPoint wgsAccuracyPoint = dggsData.m_pConverter->ConvertSphereToWGS84(
            *iter);
        polygon.AddAccuracyPointToOuterRing(
            wgsAccuracyPoint.GetLatitude(),
            wgsAccuracyPoint.GetLongitude(),
            wgsAccuracyPoint.GetAccuracy());
      }

      shapes.push_back(LatLong::LatLongShape(LatLong::WGS84_POLYGON, &polygon));

      // Export the points as a string and delete the exporter
      shapeString = pShapeExporter->ExportShapes(shapes);
      delete pShapeExporter;
    }
    catch (...)
    {
      if (pShapeExporter != NULL)
      {
        // Clean up memory allocated to the exporter
        delete pShapeExporter;
      }

      throw;
    }

    // Allocate memory for the output string
    *a_pString = (DGGS_ShapeString) malloc(shapeString.length() + sizeof(TERMINATING_CHAR));
    if (*a_pString == NULL)
    {
      throw MemoryAllocationException("Failed to allocate memory for the shape string");
    }

    // Copy data to the output string
    static_cast<void>(strcpy(*a_pString, shapeString.c_str()));
  }
  catch (MaxCellIdLengthException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  catch (MemoryAllocationException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_MEMORY_ALLOCATION_FAILURE);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_GetDggsCellParents(
    const DGGS_Handle a_handle,
    const DGGS_Cell a_cell,
    DGGS_Cell * a_parentCells,
    unsigned short * a_pNoOfParents)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_parentCells, "a_parentCells");
  CHECK_POINTER(a_handle, a_pNoOfParents, "a_pNoOfParents");

  try
  {
    // Check cell ID length does not exceed the maximum length
    CheckCellIdLength(a_cell);

    // Get the parent(s) of the cell using its ID
    std::vector < std::unique_ptr<Model::Cell::ICell> > parents;
    std::unique_ptr < Model::Cell::ICell > cell = static_cast<Model::DGGS *>(a_handle)->CreateCell(
        a_cell);
    static_cast<Model::DGGS *>(a_handle)->GetParents(*cell, parents);

    // Set the number of parent cells
    *a_pNoOfParents = parents.size();

    // Copy the parent cell IDs into the output array
    for (std::vector<Model::Cell::DggsCellId>::size_type parentIndex = 0;
        parentIndex != parents.size(); parentIndex++)
    {
      // Check cell ID of the parents does not exceed the maximum length
      CheckCellIdLength(parents[parentIndex]->GetCellId().c_str());

      // Copy data to the output string
      static_cast<void>(strncpy(
          a_parentCells[parentIndex],
          parents[parentIndex]->GetCellId().c_str(),
          EAGGR_MAX_CELL_STRING_LENGTH));
    }
  }
  catch (MaxCellIdLengthException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_GetDggsCellChildren(
    const DGGS_Handle a_handle,
    const DGGS_Cell a_cell,
    DGGS_Cell * a_childCells,
    unsigned short * a_pNoOfChildren)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_childCells, "a_childCells");
  CHECK_POINTER(a_handle, a_pNoOfChildren, "a_pNoOfChildren");

  try
  {
    // Check cell ID length does not exceed the maximum length
    CheckCellIdLength(a_cell);

    // Get the children of the cell using its ID
    std::vector < std::unique_ptr<Model::Cell::ICell> > children;
    std::unique_ptr < Model::Cell::ICell > cell = static_cast<Model::DGGS *>(a_handle)->CreateCell(
        a_cell);
    static_cast<Model::DGGS *>(a_handle)->GetChildren(*cell, children);

    // Set the number of child cells
    *a_pNoOfChildren = children.size();

    // Copy the child cell IDs into the output array
    for (std::vector<Model::Cell::DggsCellId>::size_type childIndex = 0;
        childIndex != children.size(); childIndex++)
    {
      // Check cell ID of the children does not exceed the maximum length
      CheckCellIdLength(children[childIndex]->GetCellId().c_str());

      // Copy data to the output string
      static_cast<void>(strncpy(
          a_childCells[childIndex],
          children[childIndex]->GetCellId().c_str(),
          EAGGR_MAX_CELL_STRING_LENGTH));
    }
  }
  catch (MaxCellIdLengthException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_GetDggsCellSiblings(
    const DGGS_Handle a_handle,
    const DGGS_Cell a_cell,
    DGGS_Cell * a_siblingCells,
    unsigned short * a_pNoOfSiblings)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_siblingCells, "a_siblingCells");
  CHECK_POINTER(a_handle, a_pNoOfSiblings, "a_pNoOfSiblings");

  try
  {
    // Check cell ID length does not exceed the maximum length
    CheckCellIdLength(a_cell);

    // Get the sibling(s) of the cell using its ID
    std::vector < std::unique_ptr<Model::Cell::ICell> > siblings;
    std::unique_ptr < Model::Cell::ICell > cell = static_cast<Model::DGGS *>(a_handle)->CreateCell(
        a_cell);
    static_cast<Model::DGGS *>(a_handle)->GetSiblings(*cell, siblings);

    // Set the number of sibling cells
    *a_pNoOfSiblings = siblings.size();

    // Copy the sibling cell IDs into the output array
    for (std::vector<Model::Cell::DggsCellId>::size_type siblingIndex = 0;
        siblingIndex != siblings.size(); siblingIndex++)
    {
      // Check cell ID of the siblings does not exceed the maximum length
      CheckCellIdLength(siblings[siblingIndex]->GetCellId().c_str());

      // Copy data to the output string
      static_cast<void>(strncpy(
          a_siblingCells[siblingIndex],
          siblings[siblingIndex]->GetCellId().c_str(),
          EAGGR_MAX_CELL_STRING_LENGTH));
    }
  }
  catch (MaxCellIdLengthException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_GetBoundingDggsCell(
    const DGGS_Handle a_handle,
    const DGGS_Cell * a_cells,
    const unsigned short a_noOfCells,
    DGGS_Cell * a_pBoundingCell)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_cells, "a_cells");
  CHECK_POINTER(a_handle, a_pBoundingCell, "a_pBoundingCell");

  try
  {
    std::vector < std::unique_ptr<Model::Cell::ICell> > cells;

    unsigned short minimumResolution = USHRT_MAX;

    Model::DGGS* handle = static_cast<Model::DGGS *>(a_handle);

    // Create the cells from the supplied cell ids and record the lowest resolution value
    for (unsigned short cellIndex = 0U; cellIndex < a_noOfCells; ++cellIndex)
    {
      cells.push_back(handle->CreateCell(a_cells[cellIndex]));

      minimumResolution = std::min(minimumResolution, cells[cellIndex]->GetResolution());
    }

    // Loop through all of the cells and bring them to the minimum resolution level
    for (unsigned short cellIndex = 0U; cellIndex < a_noOfCells; ++cellIndex)
    {
      DGGS_Shape originalCellShape;
      originalCellShape.m_type = DGGS_ShapeType::DGGS_CELL;
      strncpy(originalCellShape.m_data.m_cell, a_cells[cellIndex], EAGGR_MAX_CELL_STRING_LENGTH);

      while (cells[cellIndex]->GetResolution() > minimumResolution)
      {
        std::vector < std::unique_ptr<Model::Cell::ICell> > parentCells;

        handle->GetParents(*(cells[cellIndex]), parentCells);

        unsigned int bestParentIndex = GetBestCellParentIndex(
            handle,
            parentCells,
            originalCellShape);

        cells[cellIndex] = std::move(parentCells[bestParentIndex]);
      }
    }

    // Now loop over each cell and parent each one until the cells are identical
    while (AreCellsDifferent (cells))
    {
      for (unsigned short cellIndex = 0U; cellIndex < a_noOfCells; ++cellIndex)
      {
        DGGS_Shape originalCellShape;
        originalCellShape.m_type = DGGS_ShapeType::DGGS_CELL;
        strncpy(originalCellShape.m_data.m_cell, a_cells[cellIndex], EAGGR_MAX_CELL_STRING_LENGTH);

        std::vector < std::unique_ptr<Model::Cell::ICell> > parentCells;

        handle->GetParents(*(cells[cellIndex]), parentCells);

        unsigned int bestParentIndex = GetBestCellParentIndex(
            handle,
            parentCells,
            originalCellShape);

        cells[cellIndex] = std::move(parentCells[bestParentIndex]);
      }

      if (cells[0]->GetResolution() == 0U)
      {
        break;
      }
    }

    // If we still don't have a single parent then these cells are on different faces
    if (AreCellsDifferent (cells))
    {
      SET_ERROR_MESSAGE(a_handle, "Supplied cells do not have a common parent.");
      returnCode = DGGS_INVALID_PARAM;
    }
    else
    {
      // Copy data to the output string
      static_cast<void>(strncpy(
          *a_pBoundingCell,
          cells[0]->GetCellId().c_str(),
          EAGGR_MAX_CELL_STRING_LENGTH));
    }
  }
  catch (MaxCellIdLengthException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_CreateDggsKmlFile(
    const DGGS_Handle a_handle,
    const char * const a_filename,
    const DGGS_Cell * a_cells,
    const unsigned short a_noOfCells)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_filename, "a_filename");
  CHECK_POINTER(a_handle, a_cells, "a_cells");

  try
  {
    // Create the exporter to produce the KML file
    DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

    ImportExport::KmlExporter exporter(dggsData.m_pProjection, dggsData.m_pIndexer);

    // Add the DGGS cells to the KML exporter
    for (unsigned short cellIndex = 0U; cellIndex < a_noOfCells; cellIndex++)
    {
      // Check cell ID length does not exceed the maximum length
      CheckCellIdLength(a_cells[cellIndex]);

      // Create an ICell object expected by the exporter
      std::unique_ptr < Model::Cell::ICell > cell = dggsData.m_pIndexer->CreateCell(
          a_cells[cellIndex]);

      // Include the cell in the KML file
      exporter.AddCell(*cell);
    }

    // Generate the KML file
    exporter.ExportToFile(a_filename);
  }
  catch (MaxCellIdLengthException & exception)
  {
    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

DGGS_ReturnCode EAGGR_CompareShapes(
    const DGGS_Handle a_handle,
    const DGGS_AnalysisType a_spatialAnalysisType,
    const DGGS_Shape * a_baseShape,
    const DGGS_Shape * a_comparisonShape,
    bool * a_shapeComparisonResult)
{
  DGGS_ReturnCode returnCode = DGGS_SUCCESS;

  CHECK_DGGS_HANDLE(a_handle);
  CHECK_POINTER(a_handle, a_shapeComparisonResult, "a_shapeComparisonResult");

  try
  {
    // Convert the point and add it to the DGGS cells
    DggsData dggsData = g_dggsDataStore.GetDggsData(a_handle);

    std::unique_ptr < EAGGR::SpatialAnalysis::SpatialAnalysis > baseShapeAnalysis;

    DGGS_ShapeType baseShapeType = a_baseShape->m_type;
    switch (baseShapeType)
    {
      case DGGS_CELL:
      {
        //Check cell ID length does not exceed the maximum length
        CheckCellIdLength(a_baseShape->m_data.m_cell);

        std::unique_ptr < EAGGR::Model::Cell::ICell > baseCell =
            static_cast<Model::DGGS *>(a_handle)->CreateCell(a_baseShape->m_data.m_cell);

        baseShapeAnalysis = std::unique_ptr < EAGGR::SpatialAnalysis::SpatialAnalysis
            > (new EAGGR::SpatialAnalysis::SpatialAnalysis(
                std::move(baseCell),
                dggsData.m_pIndexer,
                dggsData.m_pProjection));
        break;
      }

      case DGGS_LINESTRING:
      {
        std::vector < std::unique_ptr<EAGGR::Model::Cell::ICell> > baseLinestring;

        ConvertLinestringToVector(a_handle, a_baseShape->m_data.m_linestring, baseLinestring);

        baseShapeAnalysis = std::unique_ptr < EAGGR::SpatialAnalysis::SpatialAnalysis
            > (new EAGGR::SpatialAnalysis::SpatialAnalysis(
                std::move(baseLinestring),
                dggsData.m_pIndexer,
                dggsData.m_pProjection));
        break;
      }

      case DGGS_POLYGON:
      {
        std::vector < std::unique_ptr<EAGGR::Model::Cell::ICell> > basePolygonOuterRing;
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> >
            > basePolygonInnerRings;

        ConvertPolygonToVectors(
            a_handle,
            a_baseShape->m_data.m_polygon,
            basePolygonOuterRing,
            basePolygonInnerRings);

        baseShapeAnalysis = std::unique_ptr < EAGGR::SpatialAnalysis::SpatialAnalysis
            > (new EAGGR::SpatialAnalysis::SpatialAnalysis(
                std::move(basePolygonOuterRing),
                std::move(basePolygonInnerRings),
                dggsData.m_pIndexer,
                dggsData.m_pProjection));
        break;
      }

      default:
        throw EAGGRException("Unsupported base shape type.");
    }

    DGGS_ShapeType comparisonShapeType = a_comparisonShape->m_type;
    AnalysisType analysisType = ConvertAnalysisType(a_spatialAnalysisType);

    switch (comparisonShapeType)
    {
      case DGGS_CELL:
      {
        //Check cell ID length does not exceed the maximum length
        CheckCellIdLength(a_comparisonShape->m_data.m_cell);

        std::unique_ptr < EAGGR::Model::Cell::ICell > comparisonCell =
            static_cast<Model::DGGS *>(a_handle)->CreateCell(a_comparisonShape->m_data.m_cell);

        *a_shapeComparisonResult = baseShapeAnalysis->Analyse(
            std::move(comparisonCell),
            analysisType);
        break;
      }

      case DGGS_LINESTRING:
      {
        std::vector < std::unique_ptr<EAGGR::Model::Cell::ICell> > comparisonLinestring;

        ConvertLinestringToVector(
            a_handle,
            a_comparisonShape->m_data.m_linestring,
            comparisonLinestring);

        *a_shapeComparisonResult = baseShapeAnalysis->Analyse(
            std::move(comparisonLinestring),
            analysisType);
        break;
      }

      case DGGS_POLYGON:
      {
        std::vector < std::unique_ptr<EAGGR::Model::Cell::ICell> > comparisonPolygonOuterRing;
        std::vector < std::vector<std::unique_ptr<EAGGR::Model::Cell::ICell> >
            > comparisonPolygonInnerRings;

        ConvertPolygonToVectors(
            a_handle,
            a_comparisonShape->m_data.m_polygon,
            comparisonPolygonOuterRing,
            comparisonPolygonInnerRings);

        *a_shapeComparisonResult = baseShapeAnalysis->Analyse(
            std::move(comparisonPolygonOuterRing),
            std::move(comparisonPolygonInnerRings),
            analysisType);
        break;
      }

      default:
        throw EAGGRException("Unsupported comparison shape type.");
    }
  }
  catch (MaxCellIdLengthException & exception)
  {

    SET_ERROR_MESSAGE(a_handle, exception.what());
    return (DGGS_CELL_LENGTH_TOO_LONG);
  }
  CATCH_ALL(a_handle)

  return (returnCode);
}

