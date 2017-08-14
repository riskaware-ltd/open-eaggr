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

import java.nio.ByteBuffer;

import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

import uk.co.riskaware.eaggr.DggsNativeDggsTypes.DggsPolygon;
import uk.co.riskaware.eaggr.DggsNativeDggsTypes.DggsShapeData;
import uk.co.riskaware.eaggr.enums.AnalysisType;
import uk.co.riskaware.eaggr.enums.DggsModel;
import uk.co.riskaware.eaggr.enums.DggsShapeType;
import uk.co.riskaware.eaggr.enums.ReturnCode;
import uk.co.riskaware.eaggr.enums.ShapeStringFormat;

/**
 * Java API for the EAGGR library
 * 
 * @author i_bush
 *
 */
public class Eaggr {

    private Pointer dggsHandle = Pointer.NULL;

    /**
     * Constructor
     * 
     * @param dggsModel
     *            The type of DGGS model to create
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to create the DGGS model
     */
    public Eaggr(final DggsModel dggsModel) throws EaggrException, EaggrLibraryException {
        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;

        final ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(dggsModel.ordinal(), pointers));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, "Failed to create DGGS instance.");
        }

        dggsHandle = pointers[0];
    }

    @Override
    protected void finalize() throws EaggrException, EaggrLibraryException {

        if (dggsHandle != Pointer.NULL) {
            final Pointer[] pointers = new Pointer[1];
            pointers[0] = dggsHandle;

            final ReturnCode returnCode = ReturnCode
                    .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CloseDggsHandle(pointers));

            if (returnCode != ReturnCode.DGGS_SUCCESS) {
                throw new EaggrLibraryException(returnCode, "Failed to close DGGS instance.");
            }
        }
    }

    /**
     * Gets the version of the DGGS library
     * 
     * @return The DGGS version
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to access the DGGS version
     */
    public String getVersion() throws EaggrException, EaggrLibraryException {

        final byte[] versionMemory = new byte[EaggrLibrary.VERSION_STRING_LENGTH];
        final ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetVersion(versionMemory));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        return new String(versionMemory).trim();
    }

    /**
     * Converts a single lat/long point to DGGS cell
     * 
     * @param point
     *            Lat/long point to convert
     * @return DGGS cell
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to convert the point
     */
    public DggsCell convertPointToDggsCell(final LatLongPoint point) throws EaggrException, EaggrLibraryException {

        final int noOfPoints = 1;

        final DggsNativeLatLongTypes.LatLongPoint[] nativeLatLongPoints = DggsNativeLatLongTypes.LatLongPoint
                .createlatLongPointArray(noOfPoints);

        point.toNativeLatLongPoint(nativeLatLongPoints[0]);

        final Pointer dggsCellPointer = new Memory(noOfPoints * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE
                .EAGGR_ConvertPointsToDggsCells(dggsHandle, nativeLatLongPoints, noOfPoints, dggsCellPointer));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        final String cellId = dggsCellPointer.getString(0);

        return new DggsCell(cellId);
    }

    /**
     * Converts lat/long shapes to DGGS shapes
     * 
     * @param shapes
     *            Array of lat/long shapes to convert
     * @return Array of DGGS shapes (one for each of the lat/long shapes supplied)
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to convert the shapes
     */
    public DggsShape[] convertShapesToDggsShapes(final LatLongShape[] shapes)
            throws EaggrException, EaggrLibraryException {

        if (shapes.length == 0) {
            return new DggsShape[0];
        }

        final DggsNativeLatLongTypes.LatLongShape[] nativeLatLongShapes = DggsNativeLatLongTypes.LatLongShape
                .createlatLongShapeArray(shapes.length);

        for (int shape = 0; shape < shapes.length; ++shape) {
            shapes[shape].toNativeLatLongShape(nativeLatLongShapes[shape]);
        }

        final PointerByReference dggsShapesPointer = new PointerByReference();

        ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapesToDggsShapes(
                dggsHandle, nativeLatLongShapes, nativeLatLongShapes.length, dggsShapesPointer));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        final DggsNativeDggsTypes.DggsShape firstShape = new DggsNativeDggsTypes.DggsShape(
                dggsShapesPointer.getValue());

        final DggsNativeDggsTypes.DggsShape[] nativeDggsShapes = (DggsNativeDggsTypes.DggsShape[]) firstShape
                .toArray(shapes.length);

        final DggsShape[] dggsShapes = new DggsShape[nativeDggsShapes.length];

        for (int shape = 0; shape < dggsShapes.length; ++shape) {
            dggsShapes[shape] = new DggsShape(nativeDggsShapes[shape]);
        }

        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_DeallocateDggsShapes(dggsHandle, dggsShapesPointer, dggsShapes.length));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        return dggsShapes;
    }

    /**
     * Converts a shape string to DGGS shapes
     *
     * @param shapeString
     *            The string containing the lat/long geometry data
     * @param format
     *            The format of the shape string
     * @param accuracy
     *            The accuracy that should be associated with the locations in the shape string
     * @return Array of DGGS shapes (one for each of the shapes in the supplied string)
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to convert the shape string
     */
    public DggsShape[] convertShapeStringToDggsShapes(final String shapeString, final ShapeStringFormat format,
            final double accuracy) throws EaggrException, EaggrLibraryException {

        final PointerByReference dggsShapesPointer = new PointerByReference();
        final IntByReference numberOfShapes = new IntByReference();

        ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapeStringToDggsShapes(
                dggsHandle, shapeString, format.ordinal(), accuracy, dggsShapesPointer, numberOfShapes));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        final DggsNativeDggsTypes.DggsShape firstShape = new DggsNativeDggsTypes.DggsShape(
                dggsShapesPointer.getValue());

        final DggsNativeDggsTypes.DggsShape[] nativeDggsShapes = (DggsNativeDggsTypes.DggsShape[]) firstShape
                .toArray(numberOfShapes.getValue());

        final DggsShape[] dggsShapes = new DggsShape[nativeDggsShapes.length];

        for (int shape = 0; shape < dggsShapes.length; ++shape) {
            dggsShapes[shape] = new DggsShape(nativeDggsShapes[shape]);
        }

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_DeallocateDggsShapes(dggsHandle,
                dggsShapesPointer, numberOfShapes.getValue()));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        return dggsShapes;
    }

    /**
     * Converts a single DGGS cell to a lat/long point
     *
     * @param cell
     *            DGGS cell to convert
     * @return Lat/long point
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to convert the DGGS cell
     */
    public LatLongPoint convertDggsCellToPoint(final DggsCell cell) throws EaggrException, EaggrLibraryException {

        final DggsCell[] cells = { cell };

        final LatLongPoint[] latLongPoints = convertDggsCellsToPoints(cells);
        return latLongPoints[0];
    }

    /**
     * Converts DGGS cells to lat/long points
     *
     * @param cells
     *            Array of DGGS cells
     * @return Array of lat/long points (one for each DGGS cell supplied)
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to convert the DGGS cells
     */
    public LatLongPoint[] convertDggsCellsToPoints(final DggsCell[] cells)
            throws EaggrException, EaggrLibraryException {

        if (cells.length == 0) {
            return new LatLongPoint[0];
        }

        final DggsNativeDggsTypes.DggsShape[] dggsCells = DggsNativeDggsTypes.DggsShape
                .createDggsShapeArray(cells.length);

        for (int cell = 0; cell < dggsCells.length; ++cell) {
            dggsCells[cell].shapeType = DggsShapeType.CELL.ordinal();
            dggsCells[cell].shapeData = new DggsShapeData();
            dggsCells[cell].shapeData.setType(byte[].class);
            dggsCells[cell].shapeData.setCellId(cells[cell].getCellId());
        }

        final int cellStringSize = dggsCells[0].shapeData.cell.length;

        final Pointer cellIdsPointer = new Memory(cellStringSize * cells.length);

        int offset = 0;
        for (int cell = 0; cell < dggsCells.length; ++cell) {
            cellIdsPointer.write(offset, dggsCells[cell].shapeData.cell, 0, cellStringSize);
            offset += cellStringSize;
        }

        final DggsNativeLatLongTypes.LatLongPoint[] nativeLatLongPoints = DggsNativeLatLongTypes.LatLongPoint
                .createlatLongPointArray(cells.length);

        final ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE
                .EAGGR_ConvertDggsCellsToPoints(dggsHandle, cellIdsPointer, dggsCells.length, nativeLatLongPoints[0]));

        final LatLongPoint[] latLongPoints = new LatLongPoint[nativeLatLongPoints.length];

        for (int point = 0; point < latLongPoints.length; ++point) {
            latLongPoints[point] = new LatLongPoint(nativeLatLongPoints[point]);
        }

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        return latLongPoints;
    }

    /**
     * Converts DGGS cells to a shape string
     *
     * @param cells
     *            Array of DGGS cells
     * @param format
     *            The required format for the output shape string
     * @return The shape string
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to convert DGGS cells
     */
    public String convertDggsCellsToShapeString(final DggsCell[] cells, final ShapeStringFormat format)
            throws EaggrException, EaggrLibraryException {

        if (cells.length == 0) {
            throw new EaggrException("No DGGS cells were provided.");
        }

        final DggsNativeDggsTypes.DggsShape[] dggsCells = DggsNativeDggsTypes.DggsShape
                .createDggsShapeArray(cells.length);

        for (int cell = 0; cell < dggsCells.length; ++cell) {
            dggsCells[cell].shapeType = DggsShapeType.CELL.ordinal();
            dggsCells[cell].shapeData = new DggsShapeData();
            dggsCells[cell].shapeData.setType(byte[].class);
            dggsCells[cell].shapeData.setCellId(cells[cell].getCellId());
        }

        final int cellStringSize = dggsCells[0].shapeData.cell.length;

        final Pointer cellIdsPointer = new Memory(cellStringSize * cells.length);

        int offset = 0;
        for (int cell = 0; cell < dggsCells.length; ++cell) {
            cellIdsPointer.write(offset, dggsCells[cell].shapeData.cell, 0, cellStringSize);
            offset += cellStringSize;
        }

        // Pass the shape string back as a pointer so it can be freed later
        PointerByReference latLongShapeString = new PointerByReference();

        ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToShapeString(
                dggsHandle, cellIdsPointer, cells.length, format.ordinal(), latLongShapeString));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        // Copy string to allow native memory to be cleaned up
        String returnString = new String(latLongShapeString.getValue().getString(0));

        // Free native memory
        returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_DeallocateString(dggsHandle, latLongShapeString));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        return returnString;
    }

    /**
     * Outputs the shape of the DGGS cell as a string
     * 
     * @param cell
     *            The cell to get the shape for
     * @param format
     *            The required format for the output shape string
     * @return The shape string
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to get cell shape
     */
    public String convertDggsCellOutlineToShapeString(final DggsCell cell, final ShapeStringFormat format)
            throws EaggrException, EaggrLibraryException {

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);
        bb.put(cell.getCellId().getBytes(), 0, cell.getCellId().length());

        // Pass the shape string back as a pointer so it can be freed later
        final PointerByReference latLongShapeString = new PointerByReference();

        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellOutlineToShapeString(dggsHandle, cellIdBytes,
                        format.ordinal(), latLongShapeString));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        // Copy string to allow native memory to be cleaned up
        String returnString = new String(latLongShapeString.getValue().getString(0));

        // Free native memory
        returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_DeallocateString(dggsHandle, latLongShapeString));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        return returnString;
    }

    /**
     * Gets the parent DGGS cells for a DGGS cell
     *
     * @param cell
     *            The cell to get the parent cells for
     * @return The parent cells
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to determine the parent cells
     */
    public DggsCell[] getCellParents(final DggsCell cell) throws EaggrException, EaggrLibraryException {

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);

        String cellId = cell.getCellId();
        bb.put(cellId.getBytes(), 0, cellId.length());

        final Pointer parentCellsPointer = new Memory(
                EaggrLibrary.MAX_NUMBER_OF_PARENTS * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        final IntByReference numberOfParents = new IntByReference();

        final ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE
                .EAGGR_GetDggsCellParents(dggsHandle, cellIdBytes, parentCellsPointer, numberOfParents));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        final DggsCell[] parentCellIds = new DggsCell[numberOfParents.getValue()];

        for (int parent = 0; parent < parentCellIds.length; ++parent) {
            parentCellIds[parent] = new DggsCell(
                    new String(parentCellsPointer.getByteArray(parent * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH,
                            DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim());
        }

        return parentCellIds;
    }

    /**
     * Gets the child DGGS cells for a DGGS cell
     *
     * @param cell
     *            The cell to get the child cells for
     * @return The child cells
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to determine the child cells
     */
    public DggsCell[] getCellChildren(final DggsCell cell) throws EaggrException, EaggrLibraryException {

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);

        String cellId = cell.getCellId();
        bb.put(cellId.getBytes(), 0, cellId.length());

        final Pointer childCellsPointer = new Memory(
                EaggrLibrary.MAX_NUMBER_OF_CHILDREN * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        final IntByReference numberOfChildren = new IntByReference();

        final ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE
                .EAGGR_GetDggsCellChildren(dggsHandle, cellIdBytes, childCellsPointer, numberOfChildren));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        final DggsCell[] childCellIds = new DggsCell[numberOfChildren.getValue()];

        for (int child = 0; child < childCellIds.length; ++child) {
            childCellIds[child] = new DggsCell(
                    new String(childCellsPointer.getByteArray(child * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH,
                            DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim());
        }

        return childCellIds;
    }

    /**
     * Gets the sibling DGGS cells for a DGGS cell
     *
     * @param cell
     *            The cell to get the sibling cells for
     * @return The sibling cells
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to determine the sibling cells
     */
    public DggsCell[] getCellSiblings(final DggsCell cell) throws EaggrException, EaggrLibraryException {

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);

        String cellId = cell.getCellId();
        bb.put(cellId.getBytes(), 0, cellId.length());

        final Pointer siblingCellsPointer = new Memory(
                EaggrLibrary.MAX_NUMBER_OF_SIBLINGS * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        final IntByReference numberOfSiblings = new IntByReference();

        final ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE
                .EAGGR_GetDggsCellSiblings(dggsHandle, cellIdBytes, siblingCellsPointer, numberOfSiblings));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        final DggsCell[] siblingCellIds = new DggsCell[numberOfSiblings.getValue()];

        for (int sibling = 0; sibling < siblingCellIds.length; ++sibling) {
            siblingCellIds[sibling] = new DggsCell(
                    new String(siblingCellsPointer.getByteArray(sibling * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH,
                            DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim());
        }

        return siblingCellIds;
    }

    /**
     * Finds the smallest DGGS cell that includes the supplied cells
     * 
     * @param cells
     *            The cells to process
     * @return The cell that bounds the supplied cells
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             If the supplied cells do not have a common parent cell
     */
    public DggsCell getBoundingCell(DggsCell[] cells) throws EaggrException, EaggrLibraryException {

        final Pointer cellIdsPointer = new Memory(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cells.length);

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cells.length];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);

        for (int cellIndex = 0; cellIndex < cells.length; ++cellIndex) {
            bb.position(cellIndex * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

            String cellId = cells[cellIndex].getCellId();
            bb.put(cellId.getBytes(), 0, cellId.length());
        }

        cellIdsPointer.write(0, cellIdBytes, 0, cellIdBytes.length);

        final Pointer boundingCellPointer = new Memory(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetBoundingDggsCell(dggsHandle,
                cellIdsPointer, cells.length, boundingCellPointer));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        return new DggsCell(
                new String(boundingCellPointer.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim());
    }

    /**
     * Outputs the supplied DGGS cells to a KML file
     *
     * @param filename
     *            The name of the KML file to be generated
     * @param cells
     *            The DGGS cells to output
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Failed to generate KML file
     */
    public void createKmlFile(final String filename, final DggsCell[] cells)
            throws EaggrException, EaggrLibraryException {

        final Pointer cellIdsPointer = new Memory(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cells.length);

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cells.length];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);

        for (int cellIndex = 0; cellIndex < cells.length; ++cellIndex) {
            bb.position(cellIndex * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

            String cellId = cells[cellIndex].getCellId();
            bb.put(cellId.getBytes(), 0, cellId.length());
        }

        cellIdsPointer.write(0, cellIdBytes, 0, cellIdBytes.length);

        final ReturnCode returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_CreateDggsKmlFile(dggsHandle, filename, cellIdsPointer, cells.length));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }
    }

    private String getErrorMessage() throws EaggrException, EaggrLibraryException {

        final IntByReference messageLength = new IntByReference();

        // Pass the error message back as a pointer so it can be freed later
        final PointerByReference errorMessage = new PointerByReference();

        ReturnCode returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_GetLastErrorMessage(dggsHandle, errorMessage, messageLength));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, "Failed to get error message.");
        }

        // Copy string to allow native memory to be cleaned up
        String errorString = new String(errorMessage.getValue().getString(0));

        // Free native memory
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_DeallocateString(dggsHandle, errorMessage));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, "Failed to deallocate string memory.");
        }

        return errorString;
    }

    /**
     * Outputs the result of a spatial analysis of two DGGS shapes
     *
     * @param baseShape
     *            The DGGS shape used as a base shape in the spatial analysis
     * @param comparisonShape
     *            The DGGS shape to be compared with the base shape in the spatial analysis
     * @param analysisType
     *            The spatial analysis type
     * @return A boolean representing the result of the spatial analysis
     * @throws EaggrException
     *             Unsupported library return code
     * @throws EaggrLibraryException
     *             Unable to perform spatial analysis
     */
    public boolean compareShapes(final DggsShape baseShape, final DggsShape comparisonShape,
            final AnalysisType analysisType) throws EaggrException, EaggrLibraryException {

        final DggsNativeDggsTypes.DggsShape nativeBaseShape = convertDggsShapeToNativeShape(baseShape);
        final DggsNativeDggsTypes.DggsShape nativeComparisonShape = convertDggsShapeToNativeShape(comparisonShape);
        final IntByReference spatialAnalysisResultInt = new IntByReference();

        ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CompareShapes(dggsHandle,
                analysisType.ordinal(), nativeBaseShape, nativeComparisonShape, spatialAnalysisResultInt));

        if (returnCode != ReturnCode.DGGS_SUCCESS) {
            throw new EaggrLibraryException(returnCode, getErrorMessage());
        }

        return (spatialAnalysisResultInt.getValue() != 0);
    }

    private DggsNativeDggsTypes.DggsShape convertDggsShapeToNativeShape(final DggsShape shape)
            throws EaggrException, EaggrLibraryException {

        final DggsNativeDggsTypes.DggsShape nativeShape = new DggsNativeDggsTypes.DggsShape();

        nativeShape.shapeType = shape.getShapeType().ordinal();
        nativeShape.shapeLocation = shape.getShapeLocation().ordinal();

        switch (shape.getShapeType()) {
            case CELL:
                nativeShape.shapeData.setType(byte[].class);
                nativeShape.shapeData.setCellId(shape.getCell().getCellId());
                break;
            case LINESTRING:
                nativeShape.shapeData.setType(DggsNativeDggsTypes.DggsLinestring.class);
                nativeShape.shapeData.linestring.numberOfcells = (short) shape.getLinestring().getLinestringCellIds()
                        .size();
                String[] cellIds = new String[nativeShape.shapeData.linestring.numberOfcells];

                for (int cellIndex = 0; cellIndex < nativeShape.shapeData.linestring.numberOfcells; cellIndex++) {
                    cellIds[cellIndex] = shape.getLinestring().getLinestringCellIds().get(cellIndex).getCellId();
                }

                final Pointer cellIdsPointer = new Memory(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cellIds.length);
                final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cellIds.length];

                final ByteBuffer linestringBuffer = ByteBuffer.wrap(cellIdBytes);
                for (int cellIndex = 0; cellIndex < nativeShape.shapeData.linestring.numberOfcells; cellIndex++) {
                    linestringBuffer.put(cellIds[cellIndex].getBytes(), 0, cellIds[cellIndex].length());

                    if (cellIndex < nativeShape.shapeData.linestring.numberOfcells - 1) {

                        linestringBuffer.position((cellIndex + 1) * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
                    }
                }

                cellIdsPointer.write(0, cellIdBytes, 0, cellIdBytes.length);
                nativeShape.shapeData.linestring.cells = cellIdsPointer;
                break;
            case POLYGON:
                nativeShape.shapeData.setType(DggsNativeDggsTypes.DggsPolygon.class);
                short numberOfInnerRings = (short) shape.getPolygon().getInnerRings().size();
                nativeShape.shapeData.polygon.numberOfInnerRings = numberOfInnerRings;
                int numberOfOuterRingCells = shape.getPolygon().getOuterRing().getLinestringCellIds().size();
                nativeShape.shapeData.polygon.outerRing.numberOfcells = (short) numberOfOuterRingCells;
                String[] outerCellIds = new String[numberOfOuterRingCells];

                // Outer ring
                for (int cellIndex = 0; cellIndex < numberOfOuterRingCells; cellIndex++) {
                    outerCellIds[cellIndex] = shape.getPolygon().getOuterRing().getLinestringCellIds().get(cellIndex)
                            .getCellId();
                }

                final Pointer outerCellIdsPointer = new Memory(
                        DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * outerCellIds.length);
                final byte[] outerCellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH
                        * outerCellIds.length];

                final ByteBuffer outerLinestringBuffer = ByteBuffer.wrap(outerCellIdBytes);

                for (int cellIndex = 0; cellIndex < numberOfOuterRingCells; cellIndex++) {
                    outerLinestringBuffer.put(outerCellIds[cellIndex].getBytes(), 0, outerCellIds[cellIndex].length());

                    if (cellIndex < numberOfOuterRingCells - 1) {
                        outerLinestringBuffer.position((cellIndex + 1) * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
                    }
                }

                outerCellIdsPointer.write(0, outerCellIdBytes, 0, outerCellIdBytes.length);
                nativeShape.shapeData.polygon.outerRing.cells = outerCellIdsPointer;

                if (numberOfInnerRings > 0) {
                    // Inner rings
                    DggsNativeDggsTypes.DggsLinestring[] innerRingsLinestringArray = DggsNativeDggsTypes.DggsLinestring
                            .createDggsLinestringArray(numberOfInnerRings);

                    for (int ringIndex = 0; ringIndex < numberOfInnerRings; ringIndex++) {
                        int numberOfInnerRingCells = shape.getPolygon().getInnerRings().get(ringIndex)
                                .getLinestringCellIds().size();

                        String[] innerCellIds = new String[numberOfInnerRingCells];

                        for (int cellIndex = 0; cellIndex < numberOfInnerRingCells; cellIndex++) {
                            innerCellIds[cellIndex] = shape.getPolygon().getInnerRings().get(ringIndex)
                                    .getLinestringCellIds().get(cellIndex).getCellId();
                        }

                        Pointer innerCellIdsPointer = new Memory(
                                DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * innerCellIds.length);
                        byte[] innerCellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH
                                * innerCellIds.length];
                        ByteBuffer innerLinestringBuffer = ByteBuffer.wrap(innerCellIdBytes);

                        for (int cellIndex = 0; cellIndex < numberOfInnerRingCells; cellIndex++) {
                            innerLinestringBuffer.put(innerCellIds[cellIndex].getBytes(), 0,
                                    innerCellIds[cellIndex].length());

                            if (cellIndex < numberOfInnerRingCells - 1) {
                                innerLinestringBuffer
                                        .position((cellIndex + 1) * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
                            }
                        }

                        innerCellIdsPointer.write(0, innerCellIdBytes, 0, innerCellIdBytes.length);
                        innerRingsLinestringArray[ringIndex].numberOfcells = (short) numberOfInnerRingCells;
                        innerRingsLinestringArray[ringIndex].cells = innerCellIdsPointer;
                    }
                    nativeShape.shapeData.polygon.innerRings = DggsPolygon
                            .createPointerToDggsLinestringArray(innerRingsLinestringArray);
                }
                break;
            default:
                throw new EaggrException("Unrecognised shape type " + shape.getShapeType() + ".");
        }

        return nativeShape;
    }
}
