/*
 * Copyright (c) Riskaware 2015
 *
 * This file is part of OpenEAGGR.
 * 
 * OpenEAGGR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OpenEAGGR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * A copy of the GNU Lesser General Public License is available in COPYING.LESSER
 * or can be found at <http://www.gnu.org/licenses/>.
 */

package uk.co.riskaware.eaggr;

import java.util.HashMap;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;
import com.sun.jna.win32.StdCallLibrary;

/**
 * JNA wrapper for the EAGGR C API
 * 
 * C API wrapper functions require int datatypes rather than short for short C datatypes. This is contrary to the
 * structures where C short datatypes are represented by Java shorts. Not sure currently why this should be the case but
 * it works.
 * 
 * @author i_bush
 *
 */
public interface EaggrLibrary extends Library {

    /**
     * The maximum number of child cells that can be returned by the DGGS library
     */
    public static final int VERSION_STRING_LENGTH = 5;

    /**
     * The maximum number of parent cells that can be returned by the DGGS library
     */
    public static final int MAX_NUMBER_OF_PARENTS = 3;

    /**
     * The maximum number of child cells that can be returned by the DGGS library
     */
    public static final int MAX_NUMBER_OF_CHILDREN = 7;

    /**
     * The maximum number of sibling cells that can be returned by the DGGS library
     */
    public static final int MAX_NUMBER_OF_SIBLINGS = 15;

    /**
     * The instance of the EAGGR library to call functions on
     */
    EaggrLibrary INSTANCE = (EaggrLibrary) Native.loadLibrary("eaggr", EaggrLibrary.class,
            new HashMap<Object, Object>() {
                private static final long serialVersionUID = 1L;

                {
                    // Need to use the StdCallLibrary function mapper to decode the functions exported by the DLL
                    put(OPTION_FUNCTION_MAPPER, StdCallLibrary.FUNCTION_MAPPER);
                }
            });

    /**
     * Gets the version of the EAGGR library
     * 
     * @param version
     *            a byte array that will be populated with the version string. The array should be large enough to hold
     *            the returned string
     * @return the return code from the library function
     */
    int EAGGR_GetVersion(byte[] version);

    /**
     * Opens an instance of the DGGS model
     * 
     * @param dggsModel
     *            the type of model to open
     * @param dggsHandlePointer
     *            pointer to the location to store the handle of a DGGS instance. After the call to this method the
     *            pointer will be set to the handle of the opened DGGS model object
     * @return the return code from the library function
     */
    int EAGGR_OpenDggsHandle(int dggsModel, Pointer[] dggsHandlePointer);

    /**
     * Closes an instance of the DGGS model, cleaning up the native memory
     * 
     * @param dggsHandlePointer
     *            pointer to the handle of a DGGS instance to be closed. After the call to this method the pointer will
     *            be set to null
     * @return the return code from the library function
     */
    int EAGGR_CloseDggsHandle(Pointer[] dggsHandlePointer);

    /**
     * Gets the most recent error message for the DGGS model
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param errorMessage
     *            pointer to a string to be populated with the error message. Free using EAGGR_DeallocateDggsString
     * @param messageLength
     *            pointer to a int that will be populated with the length of the error message
     * 
     * @return the return code from the library function
     */
    int EAGGR_GetLastErrorMessage(Pointer dggsHandle, PointerByReference errorMessage, IntByReference messageLength);

    /**
     * Converts lat/long points to DGGS cells
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param points
     *            array of lat/long point objects to be converted
     * @param numberOfPoints
     *            the number of lat/long point objects in the array
     * @param dggsCellPointer
     *            block of memory assigned to hold the converted cell ids. This needs to be large enough to hold a cell
     *            for each of the input points. After the method call the memory will be populated with an array of cell
     *            id strings each taking up a number of bytes equal to the maximum size of a cell id string
     * @return the return code from the library function
     */
    int EAGGR_ConvertPointsToDggsCells(Pointer dggsHandle, DggsNativeLatLongTypes.LatLongPoint[] points,
            int numberOfPoints, Pointer dggsCellPointer);

    /**
     * Converts lat/long shapes to DGGS shapes
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param shapes
     *            array of lat/long shape objects to be converted
     * @param numberOfShapes
     *            the number of lat/long shape objects in the array
     * @param dggsShapes
     *            pointer to the memory that will be populated with the DGGS shape data. This memory will be populated
     *            with an array of DggsShape objects equal in size to the input lat/long shape array. This object should
     *            be cleaned up by calling EAGGR_DeallocateDggsShapes
     * @return the return code from the library function
     */
    int EAGGR_ConvertShapesToDggsShapes(Pointer dggsHandle, DggsNativeLatLongTypes.LatLongShape[] shapes,
            int numberOfShapes, PointerByReference dggsShapes);

    /**
     * Converts a shape string to DGGS shapes
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param shapeString
     *            the string representing the shape objects
     * @param format
     *            the format of the string
     * @param accuracy
     *            the accuracy that should be associated with the locations in the shape string
     * @param dggsShapes
     *            pointer to the memory that will be populated with the DGGS shape data. This object should be cleaned
     *            up by calling EAGGR_DeallocateDggsShapes
     * @param numberOfShapes
     *            pointer to an int that will be populated with the number of DggsShape objects in the array pointed to
     *            by the dggsShapes argument
     * @return the return code from the library function
     */
    int EAGGR_ConvertShapeStringToDggsShapes(Pointer dggsHandle, String shapeString, int format, double accuracy,
            PointerByReference dggsShapes, IntByReference numberOfShapes);

    /**
     * Frees the native memory used by the DGGS shape object
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param dggsShapes
     *            pointer to the DGGS shape array to be cleaned up
     * @param numberOfShapes
     *            the number of DGGS shapes in the array
     * @return the return code from the library function
     */
    int EAGGR_DeallocateDggsShapes(Pointer dggsHandle, PointerByReference dggsShapes, int numberOfShapes);

    /**
     * Frees the native memory used by a string created by the library
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param dggsString
     *            pointer to the DGGS shape string to deallocate
     * @return the return code from the library function
     */
    int EAGGR_DeallocateString(Pointer dggsHandle, PointerByReference dggsString);

    /**
     * Converts DGGS cells to lat/long points
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param dggsCells
     *            pointer to an array of byte arrays representing the cell ids. Each cell id entry should take the
     *            maximum length of the cell id string (padded by zeros)
     * @param numberOfCells
     *            the number of DGGS cells to be converted
     * @param latLongPoints
     *            the first item in an array of contiguous memory assigned to contain the lat/long point data. The size
     *            of the array should equal the number of DGGS cells being converted
     * @return the return code from the library function
     */
    int EAGGR_ConvertDggsCellsToPoints(Pointer dggsHandle, Pointer dggsCells, int numberOfCells,
            DggsNativeLatLongTypes.LatLongPoint latLongPoints);

    /**
     * Converts DGGS cells to a shape string
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param dggsCells
     *            pointer to an array of byte arrays representing the cell ids. Each cell id entry should take the
     *            maximum length of the cell id string (padded by zeros)
     * @param numberOfCells
     *            the number of DGGS cells to be converted
     * @param format
     *            the required format for the output shape string
     * @param latLongShapeString
     *            pointer to a string to be populated with the shape string. Free using EAGGR_DeallocateDggsString.
     * @return the return code from the library function
     */
    int EAGGR_ConvertDggsCellsToShapeString(Pointer dggsHandle, Pointer dggsCells, int numberOfCells, int format,
            PointerByReference latLongShapeString);

    /**
     * Outputs the shape of the DGGS cell as a string
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param cellId
     *            the DGGS cell id string encoded as a byte array
     * @param format
     *            the required format for the output shape string
     * @param latLongShapeString
     *            pointer to a string to be populated with the shape string. Free using EAGGR_DeallocateDggsString.
     * @return the return code from the library function
     */
    int EAGGR_ConvertDggsCellOutlineToShapeString(Pointer dggsHandle, byte[] cellId, int format,
            PointerByReference latLongShapeString);

    /**
     * Gets the parent DGGS cells for a DGGS cell
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param cellId
     *            the DGGS cell id string encoded as a byte array
     * @param parentCells
     *            block of memory assigned to hold the parent cell ids. This needs to be large enough to hold the
     *            maximum number of parent cells that could be returned. After the method call the memory will be
     *            populated with an array of cell id strings each taking up a number of bytes equal to the maximum size
     *            of a cell id string
     * @param numberOfParents
     *            the number of parent cells returned
     * @return the return code from the library function
     */
    int EAGGR_GetDggsCellParents(Pointer dggsHandle, byte[] cellId, Pointer parentCells,
            IntByReference numberOfParents);

    /**
     * Gets the child DGGS cells for a DGGS cell
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param cellId
     *            the DGGS cell id string encoded as a byte array
     * @param childCells
     *            block of memory assigned to hold the child cell ids. This needs to be large enough to hold the maximum
     *            number of child cells that could be returned. After the method call the memory will be populated with
     *            an array of cell id strings each taking up a number of bytes equal to the maximum size of a cell id
     *            string
     * @param numberOfChildren
     *            the number of child cells returned
     * @return the return code from the library function
     */
    int EAGGR_GetDggsCellChildren(Pointer dggsHandle, byte[] cellId, Pointer childCells,
            IntByReference numberOfChildren);

    /**
     * Gets the sibling DGGS cells for a DGGS cell
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param cellId
     *            the DGGS cell id string encoded as a byte array
     * @param siblingCells
     *            block of memory assigned to hold the sibling cell ids. This needs to be large enough to hold the
     *            maximum number of sibling cells that could be returned. After the method call the memory will be
     *            populated with an array of cell id strings each taking up a number of bytes equal to the maximum size
     *            of a cell id string
     * @param numberOfSiblings
     *            the number of sibling cells returned
     * @return the return code from the library function
     */
    int EAGGR_GetDggsCellSiblings(Pointer dggsHandle, byte[] cellId, Pointer siblingCells,
            IntByReference numberOfSiblings);

    /**
     * Gets the bounding cell for a set of DGGS cells
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param dggsCells
     *            pointer to an array of byte arrays representing the cell ids. Each cell id entry should take the
     *            maximum length of the cell id string (padded by zeros)
     * @param numberOfCells
     *            the number of DGGS cells to be processed
     * @param boundingCell
     *            pointer to an byte array that will be populated with the bounding cell id
     * @return the return code from the library function
     */
    int EAGGR_GetBoundingDggsCell(Pointer dggsHandle, Pointer dggsCells, int numberOfCells, Pointer boundingCell);

    /**
     * Outputs the supplied DGGS cells to a KML file
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param fileName
     *            the name of the KML file to be generated
     * @param cells
     *            pointer to an array of byte arrays representing the cell ids. Each cell id entry should take the
     *            maximum length of the cell id string (padded by zeros)
     * @param numberOfCells
     *            the number of cells to be output to the KML file
     * @return the return code from the library function
     */
    int EAGGR_CreateDggsKmlFile(Pointer dggsHandle, String fileName, Pointer cells, int numberOfCells);

    /**
     * Outputs the result of a spatial analysis of two DggsShape's
     * 
     * @param dggsHandle
     *            the handle to the DGGS model
     * @param analysisType
     *            the integer denoting the spatial analysis to be performed. This should match the native
     *            DGGS_AnalysisType enum
     * @param baseShape
     *            the base DggsShape for the spatial analysis
     * @param comparisonShape
     *            the comparison DggsShape for the spatial analysis
     * @param spatialAnalysisResult
     *            pointer to an integer representing the outcome of the spatial analysis
     * @return the return code from the library function
     */
    int EAGGR_CompareShapes(Pointer dggsHandle, int analysisType, DggsNativeDggsTypes.DggsShape baseShape,
            DggsNativeDggsTypes.DggsShape comparisonShape, IntByReference spatialAnalysisResult);
}
