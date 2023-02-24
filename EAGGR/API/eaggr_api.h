/*
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
 /// All latitude and longitude coordinates should use
 /// the WGS84 coordinate system.
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
 */
#ifndef EAGGR_API_H
#define EAGGR_API_H

/* Enumerated Types */

/**
 * Used to specify the model type to use for the DGGS.
 */
typedef enum
{
  DGGS_ISEA4T, DGGS_ISEA3H
} DGGS_Model;

/**
 * Type returned by every function in the API. Indicates if the function
 * completed successfully, or the reason the function failed.
 * @note If a function fails, GetLastErrorMessage() can be called obtain
 *       a string containing a human-readable description of the error.
 */
typedef enum
{
  DGGS_SUCCESS,
  DGGS_NOT_IMPLEMENTED,
  DGGS_INVALID_HANDLE,
  DGGS_INVALID_PARAM,
  DGGS_NULL_POINTER,
  DGGS_MODEL_ERROR,
  DGGS_CELL_LENGTH_TOO_LONG,
  DGGS_MEMORY_ALLOCATION_FAILURE,
  DGGS_UNKNOWN_ERROR
} DGGS_ReturnCode;

/**
 * Different lat / long shape types supported by the library.
 */
typedef enum
{
  DGGS_LAT_LONG_POINT, DGGS_LAT_LONG_LINESTRING, DGGS_LAT_LONG_POLYGON
} DGGS_LatLongShapeType;

/**
 * Different DGGS shape types.
 */
typedef enum
{
  DGGS_CELL, /** Cell representing a point. */
  DGGS_LINESTRING, /** Cells representing points that define a line. */
  DGGS_POLYGON /** Cells contained inside a polygon. */
} DGGS_ShapeType;

/**
 * Different DGGS shape location types.
 */
typedef enum
{
  DGGS_ONE_FACE, /** Shape sits entirely on one face. */
  DGGS_TWO_FACES, /** Shape spans two faces. */
  DGGS_MANY_FACES, /** Shape spans more than two faces. */
  DGGS_NO_LOCATION /** Shape location only applies to cells - this value is used for linestrings or polygons. */
} DGGS_ShapeLocation;

/**
 * Different shape string formats supported by the library.
 */
typedef enum
{
  DGGS_WKT_FORMAT, DGGS_GEO_JSON_FORMAT
} DGGS_ShapeStringFormat;

/**
 * Different shape analysis types supported by the library.
 */
typedef enum
{
  DGGS_CONTAINS,
  DGGS_COVERED_BY,
  DGGS_COVERS,
  DGGS_CROSSES,
  DGGS_DISJOINT,
  DGGS_EQUALS,
  DGGS_INTERSECTS,
  DGGS_OVERLAPS,
  DGGS_TOUCHES,
  DGGS_WITHIN
} DGGS_AnalysisType;

/* Type definitions for converting lat / long shapes from a string. */

/**
 * String containing shape information in one of the supported formats.
 */
typedef char * DGGS_ShapeString;

/* Type definitions used by all the function in the API */

/**
 * Handle to the DGGS model.
 */
typedef void * DGGS_Handle;

/* Type definitions for storing shapes as lat / long points */

/**
 * Lat / long coordinate with an accuracy in metres squared.
 */
typedef struct
{
    double m_latitude;
    double m_longitude;
    double m_accuracy;
} DGGS_LatLongPoint;

/**
 * Series of lat / long coordinates that define a line.
 */
typedef struct
{
    DGGS_LatLongPoint * m_points;
    unsigned short m_noOfPoints;
} DGGS_LatLongLinestring;

/**
 * Two lines (made up of lat / long coordinates) that define a polygon.
 */
typedef struct
{
    DGGS_LatLongLinestring m_outerRing;
    DGGS_LatLongLinestring * m_innerRings;
    unsigned short m_noOfInnerRings;
} DGGS_LatLongPolygon;

/**
 * Shape defined by lat / long coordinates.
 */
typedef union
{
    DGGS_LatLongPoint m_point;
    DGGS_LatLongLinestring m_linestring;
    DGGS_LatLongPolygon m_polygon;
} DGGS_LatLongShapeData;

/**
 * Structure containing a shape and its type.
 */
typedef struct
{
    DGGS_LatLongShapeType m_type;
    DGGS_LatLongShapeData m_data;
} DGGS_LatLongShape;

/* Constants and type definitions for defining a DGGS cell */

/**
 * Maximum length of a string which defines a DGGS cell. Should be set based
 * on the highest resolution the DGGS supports.
 */
#define EAGGR_MAX_CELL_STRING_LENGTH 43U

/**
 * String defining a DGGS cell.
 */
typedef char DGGS_Cell[EAGGR_MAX_CELL_STRING_LENGTH];

/* Type definitions for outputting shapes as DGGS cells */

/**
 * Cells representing points that define a line.
 */
typedef struct
{
    DGGS_Cell * m_cells;
    unsigned short m_noOfCells;
} DGGS_Linestring;

/**
 * Two lines represented by cells that define a polygon.
 */
typedef struct
{
    DGGS_Linestring m_outerRing;
    DGGS_Linestring * m_innerRings;
    unsigned short m_noOfInnerRings;
} DGGS_Polygon;

/**
 * Shape defined by DGGS cells.
 */
typedef union
{
    DGGS_Cell m_cell;
    DGGS_Linestring m_linestring;
    DGGS_Polygon m_polygon;
} DGGS_ShapeData;

/**
 * Structure containing a DGGS shape and its type.
 */
typedef struct
{
    DGGS_ShapeType m_type;
    DGGS_ShapeData m_data;
    DGGS_ShapeLocation m_location;
} DGGS_Shape;

/* Constants for the number of parents and children of a DGGS cell */

/**
 * Maximum number of parents a DGGS cell can have.
 */
static const unsigned short EAGGR_MAX_PARENT_CELLS = 3U;

/**
 * Maximum number of children a DGGS cell can have.
 */
static const unsigned short EAGGR_MAX_CHILD_CELLS = 7U;

/**
 * Maximum number of siblings a DGGS cell can have.
 */
static const unsigned short EAGGR_MAX_SIBLING_CELLS = 15U;

/* Constants for version information */

/**
 * Number of characters in this DLL's version string (including the null
 * terminating character).
 */
static const unsigned short EAGGR_VERSION_STRING_LENGTH = 5U;

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
    // Windows
    #ifdef EAGGR_BUILD_STATIC
        #define EXPORT
    #elif defined(EAGGR_BUILD_DLL)
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#else
    // Linux
    #ifdef EAGGR_BUILD_STATIC
        #define EXPORT
    #else
        #define EXPORT __attribute__((visibility("default")))
    #endif
#endif

  /**
   * Outputs the version of the DLL as a string.
   * @note Because the function does not take a DGGS handle, in the event of an
   *       error, the last error message will be not be stored and therefore
   *       cannot be retrieved with DGGS_GetLastErrorMessage().
   */
  EXPORT DGGS_ReturnCode EAGGR_GetVersion(char * a_versionString);

  /**
   * Outputs a message for the last error to occur.
   * @note If no errors have occurred the message pointer will be set to NULL.
   */
  EXPORT DGGS_ReturnCode EAGGR_GetLastErrorMessage(DGGS_Handle a_handle, /**<IN - Handle for the DGGS model. */
  char ** a_pErrorMessage, /**<OUT - Error message string. Memory allocated to this pointer must be freed by the caller. */
  unsigned short * a_pMessageLength /**<OUT - Length of the output string. */);

  /* Functions for creating and destroying the handle to the DGGS model */

  /**
   * Creates the DGGS model and outputs its handle.
   */
  EXPORT DGGS_ReturnCode EAGGR_OpenDggsHandle(const DGGS_Model a_dggsModel, /**<IN - Specifies the type of DGGS model to use. */
  DGGS_Handle * a_pHandle /**<OUT - Pointer to the handle for the DGGS model */);

  /**
   * Destroys the DGGS model and set the handle to null.
   */
  EXPORT DGGS_ReturnCode EAGGR_CloseDggsHandle(
      DGGS_Handle * a_pHandle /**<IN - Pointer to the handle for the DGGS model */);

  /* Functions for converting from Lat / Long shapes */

  /**
   * Converts an array of points in lat / long coordinates into an array of DGGS cells.
   */
  EXPORT DGGS_ReturnCode EAGGR_ConvertPointsToDggsCells(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model. */
  const DGGS_LatLongPoint * a_points, /**<IN - Array of lat / long points. */
  const unsigned short a_noOfPoints, /**<IN - Number of points in the input array (and cells in the output array). */
  DGGS_Cell * a_pDggsCells /**<IN - Array of DGGS cells. */
  );

  /**
   * Converts an array of shapes in lat / long coordinates into an array of
   * shapes defined by DGGS cells.
   */
  EXPORT DGGS_ReturnCode EAGGR_ConvertShapesToDggsShapes(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model. */
  const DGGS_LatLongShape * a_shapes, /**<IN - Array of shapes defined by lat / long coordinates. */
  const unsigned short a_noOfShapes, /**<IN - Number of shapes in the input array (and output array). */
  DGGS_Shape ** a_pDggsShapes /**<OUT - Pointer to an array of DGGS shapes, with the same number of elements as the input array. */
  );

  /**
   * Converts a shape string into an array of shapes defined by DGGS cells.
   */
  EXPORT DGGS_ReturnCode EAGGR_ConvertShapeStringToDggsShapes(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model. */
  const DGGS_ShapeString a_string, /**<IN - String containing shape information in one of the supported formats. */
  const DGGS_ShapeStringFormat a_format, /**<IN - Format used for the string, e.g. WKT. */
  const double a_accuracy, /**<IN - Defines an area of accuracy (in meters squared) to use for every point in the string. */
  DGGS_Shape ** a_pDggsShapes, /**<OUT - Pointer to an array containing the input shapes defined by DGGS cells. */
  unsigned short * a_pNoOfShapes /**<OUT - Number of shapes found in the input string (and the length of the output array). */
  );

  /**
   * Deallocates the memory used by an array of DGGS shapes.
   */
  EXPORT DGGS_ReturnCode EAGGR_DeallocateDggsShapes(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model. */
  DGGS_Shape ** a_pDggsShapes, /**<IN - Array containing the DGGS shapes. */
  const unsigned short a_noOfShapes /**<IN - Number of shapes in the array. */
  );

  /**
   * Deallocates the memory used by a string. Use to free memory used by strings that are allocated and
   * returned by functions on the API.
   */
  EXPORT DGGS_ReturnCode EAGGR_DeallocateString(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model. */
  char ** a_pDggsString /**<IN - The string to deallocate. */
  );

  /* Functions for converting DGGS shapes */

  /**
   * Converts an array of DGGS cells into an array of points in lat / long
   * coordinates.
   */
  EXPORT DGGS_ReturnCode EAGGR_ConvertDggsCellsToPoints(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const DGGS_Cell * a_cells, /**<IN - Array of DGGS cells. */
  const unsigned short a_noOfCells, /**<IN - Number of cells in the input array (and points in the output array). */
  DGGS_LatLongPoint * a_points /**<OUT - Array of lat / long points. */
  );

  /**
   * Converts an array of DGGS cells into a string of lat / long points.
   */
  EXPORT DGGS_ReturnCode EAGGR_ConvertDggsCellsToShapeString(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const DGGS_Cell * a_cells, /**<IN - Array of DGGS cells. */
  const unsigned short a_noOfCells, /**<IN - Number of cells in the input array (and points defined in the output string). */
  const DGGS_ShapeStringFormat a_format, /**<IN - Format to use for the output string. */
  DGGS_ShapeString * a_pString /**<OUT - String defining the DGGS cells in lat / long coordinates. Memory needs to be freed by client. */
  );

  /**
   * Converts a cell area to a shape string containing a lat / long polygon.
   */
  EXPORT DGGS_ReturnCode EAGGR_ConvertDggsCellOutlineToShapeString(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const DGGS_Cell a_cell, /**<IN - DGGS cell to get the shape string for. */
  const DGGS_ShapeStringFormat a_format, /**<IN - Format to use for the output string. */
  DGGS_ShapeString * a_pString /**<OUT - String defining the DGGS cells in lat / long coordinates. Memory needs to be freed by client. */
  );

  /* Functions for handling DGGS cells */

  /**
   * Outputs the parent of the specified cell. The parent cell is defined as the
   * cell in the resolution above that is closest to the location of the
   * specified cell.
   */
  EXPORT DGGS_ReturnCode EAGGR_GetDggsCellParents(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const DGGS_Cell a_cell, /**<IN - DGGS cell to find the parents of. */
  DGGS_Cell * a_parentCells, /**<OUT - Parent cells. */
  unsigned short * a_pNoOfParents /**<OUT - Number of parent cells (depends on the grid system being used). */
  );

  /**
   * Outputs the children of the specified cell. The child cells are defined as
   * those cells in the resolution below that share area with the specified cell.
   */
  EXPORT DGGS_ReturnCode EAGGR_GetDggsCellChildren(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const DGGS_Cell a_cell, /**<IN - DGGS cell to find the children of. */
  DGGS_Cell * a_childCells, /**<OUT - Child cells. */
  unsigned short * a_pNoOfChildren /**<OUT - Number of children cells (depends on the grid system being used). */
  );

  /**
   * Outputs the siblings of the specified cell. The sibling cells are defined as
   * those cells in the same resolution that share area with the cells in the
   * resolution above that are closest to the location of the specified cell.
   */
  EXPORT DGGS_ReturnCode EAGGR_GetDggsCellSiblings(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const DGGS_Cell a_cell, /**<IN - DGGS cell to find the siblings of. */
  DGGS_Cell * a_siblingsCells, /**<OUT - Sibling cells. */
  unsigned short * a_pNoOfSiblings /**<OUT - Number of sibling cells (depends on the grid system being used). */
  );

  /**
   * Outputs the highest resolution cell that contains all the given cells.
   */
  EXPORT DGGS_ReturnCode EAGGR_GetBoundingDggsCell(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const DGGS_Cell * a_cells, /**<IN - Array of DGGS cells to find the bounding cell of. */
  const unsigned short a_noOfCells, /**<IN - Number of cells in the input array. */
  DGGS_Cell * a_pBoundingCell /**<OUT - Cell containing all of the cells in the input array. */
  );

  /**
   * Creates a KML file for displaying the given cells on mapping applications,
   * such as Google Earth.
   */
  EXPORT DGGS_ReturnCode EAGGR_CreateDggsKmlFile(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const char * const a_filename, /**<IN - Filename of the KML file to be created. */
  const DGGS_Cell * a_cells, /**<IN - Array of DGGS cells to include in the KML file. */
  const unsigned short a_noOfCells /**<IN - Number of cells in the input array. */
  );

  /**
   * Outputs the results from comparing two shapes
   */
  EXPORT DGGS_ReturnCode EAGGR_CompareShapes(const DGGS_Handle a_handle, /**<IN - Handle for the DGGS model */
  const DGGS_AnalysisType a_spatialAnalysisType, /**<IN - The type of shape comparison to be performed. */
  const DGGS_Shape * a_baseShape, /**<IN - Base shape for the comparison. */
  const DGGS_Shape * a_comparionShape, /**<IN - Shape to be compared with the base shape. */
  bool * a_shapeComparisonResult /**<OUT - flag indicating the result of the shape comparison. */
  );

#ifdef __cplusplus
}
#endif

#endif /* EAGGR_API_H */
