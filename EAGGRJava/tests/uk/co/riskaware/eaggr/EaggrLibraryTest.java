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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.junit.Test;

import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

import uk.co.riskaware.eaggr.enums.AnalysisType;
import uk.co.riskaware.eaggr.enums.DggsModel;
import uk.co.riskaware.eaggr.enums.DggsShapeLocation;
import uk.co.riskaware.eaggr.enums.DggsShapeType;
import uk.co.riskaware.eaggr.enums.LatLongShapeType;
import uk.co.riskaware.eaggr.enums.ReturnCode;
import uk.co.riskaware.eaggr.enums.ShapeStringFormat;
import uk.co.riskaware.eaggr.testutilities.TestUtilities;

public class EaggrLibraryTest {
    @Test
    public void dggsGetVersionTest() throws EaggrException {

        final byte[] versionMemory = new byte[EaggrLibrary.VERSION_STRING_LENGTH];
        final ReturnCode returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetVersion(versionMemory));

        final String versionString = new String(versionMemory).trim();

        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals("v2.0", versionString);
    }

    @Test
    public void dggsOpenAndCloseTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];

        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertNotEquals(Pointer.NULL, pointers[0]);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CloseDggsHandle(pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(Pointer.NULL, pointers[0]);

        returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA3H.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertNotEquals(Pointer.NULL, pointers[0]);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CloseDggsHandle(pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(Pointer.NULL, pointers[0]);

        // Test null pointer passed to open and close
        returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CloseDggsHandle(null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
    }

    @Test
    public void dggsConvertShapesToDggsShapesTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final DggsNativeLatLongTypes.LatLongShape[] shapes = DggsNativeLatLongTypes.LatLongShape
                .createlatLongShapeArray(4);

        shapes[0].latLongShapeType = LatLongShapeType.POINT.ordinal();
        shapes[0].latLongShapeData = new DggsNativeLatLongTypes.LatLongShapeData();
        shapes[0].latLongShapeData.setType(DggsNativeLatLongTypes.LatLongPoint.class);
        shapes[0].latLongShapeData.point = new DggsNativeLatLongTypes.LatLongPoint();
        shapes[0].latLongShapeData.point.latitude = 1.234;
        shapes[0].latLongShapeData.point.longitude = 2.345;
        shapes[0].latLongShapeData.point.accuracy = 3.879;

        shapes[1].latLongShapeType = LatLongShapeType.POINT.ordinal();
        shapes[1].latLongShapeData = new DggsNativeLatLongTypes.LatLongShapeData();
        shapes[1].latLongShapeData.setType(DggsNativeLatLongTypes.LatLongPoint.class);
        shapes[1].latLongShapeData.point = new DggsNativeLatLongTypes.LatLongPoint();
        shapes[1].latLongShapeData.point.latitude = 3.456;
        shapes[1].latLongShapeData.point.longitude = 4.567;
        shapes[1].latLongShapeData.point.accuracy = 3.879;

        shapes[2].latLongShapeType = LatLongShapeType.LINESTRING.ordinal();
        shapes[2].latLongShapeData = new DggsNativeLatLongTypes.LatLongShapeData();
        shapes[2].latLongShapeData.setType(DggsNativeLatLongTypes.LatLongLinestring.class);
        shapes[2].latLongShapeData.linestring = new DggsNativeLatLongTypes.LatLongLinestring();

        final DggsNativeLatLongTypes.LatLongPoint[] points = DggsNativeLatLongTypes.LatLongPoint
                .createlatLongPointArray(2);
        points[0].latitude = 1.234;
        points[0].longitude = 2.345;
        points[0].accuracy = 3.879;
        points[1].latitude = 3.456;
        points[1].longitude = 4.567;
        points[1].accuracy = 3.879;

        shapes[2].latLongShapeData.linestring.points = DggsNativeLatLongTypes.LatLongPoint
                .createPointerToLatLongPointArray(points);
        shapes[2].latLongShapeData.linestring.numberOfPoints = 2;

        shapes[3].latLongShapeType = LatLongShapeType.POLYGON.ordinal();
        shapes[3].latLongShapeData = new DggsNativeLatLongTypes.LatLongShapeData();
        shapes[3].latLongShapeData.setType(DggsNativeLatLongTypes.LatLongPolygon.class);
        shapes[3].latLongShapeData.polygon = new DggsNativeLatLongTypes.LatLongPolygon();

        final DggsNativeLatLongTypes.LatLongLinestring outerRing = new DggsNativeLatLongTypes.LatLongLinestring();
        outerRing.numberOfPoints = 2;
        outerRing.points = DggsNativeLatLongTypes.LatLongPoint.createPointerToLatLongPointArray(points);
        shapes[3].latLongShapeData.polygon.outerRing = outerRing;

        shapes[3].latLongShapeData.polygon.numberOfInnerRings = 2;

        final DggsNativeLatLongTypes.LatLongLinestring[] innerRings = DggsNativeLatLongTypes.LatLongLinestring
                .createlatLongPointArray(2);
        innerRings[0].numberOfPoints = 2;
        innerRings[0].points = DggsNativeLatLongTypes.LatLongPoint.createPointerToLatLongPointArray(points);
        innerRings[1].numberOfPoints = 2;
        innerRings[1].points = DggsNativeLatLongTypes.LatLongPoint.createPointerToLatLongPointArray(points);

        shapes[3].latLongShapeData.polygon.innerRings = DggsNativeLatLongTypes.LatLongLinestring
                .createPointerToLatLongLinestringArray(innerRings);

        final PointerByReference dggsShapesPointer = new PointerByReference();

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapesToDggsShapes(dggsHandle, shapes,
                shapes.length, dggsShapesPointer));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final DggsNativeDggsTypes.DggsShape firstShape = new DggsNativeDggsTypes.DggsShape(
                dggsShapesPointer.getValue());

        final DggsNativeDggsTypes.DggsShape[] dggsShapes = (DggsNativeDggsTypes.DggsShape[]) firstShape
                .toArray(shapes.length);

        assertEquals(4, dggsShapes.length);

        assertEquals(DggsShapeType.CELL.ordinal(), dggsShapes[0].shapeType);
        assertEquals(DggsShapeLocation.ONE_FACE.ordinal(), dggsShapes[0].shapeLocation);
        String cellId = new String(dggsShapes[0].shapeData.cell);
        cellId = cellId.trim();
        assertEquals("07231131111113100331001", cellId);

        assertEquals(DggsShapeType.CELL.ordinal(), dggsShapes[1].shapeType);
        assertEquals(DggsShapeLocation.ONE_FACE.ordinal(), dggsShapes[1].shapeLocation);
        cellId = new String(dggsShapes[1].shapeData.cell);
        cellId = cellId.trim();
        assertEquals("07012000001303022011321", cellId);

        assertEquals(DggsShapeType.LINESTRING.ordinal(), dggsShapes[2].shapeType);
        assertEquals(DggsShapeLocation.NO_LOCATION.ordinal(), dggsShapes[2].shapeLocation);
        assertEquals(2, dggsShapes[2].shapeData.linestring.numberOfcells);

        final String linestringCell1 = new String(
                dggsShapes[2].shapeData.linestring.cells.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();
        final String linestringCell2 = new String(dggsShapes[2].shapeData.linestring.cells
                .getByteArray(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();

        assertEquals("07231131111113100331001", linestringCell1);
        assertEquals("07012000001303022011321", linestringCell2);

        assertEquals(DggsShapeType.POLYGON.ordinal(), dggsShapes[3].shapeType);
        assertEquals(DggsShapeLocation.NO_LOCATION.ordinal(), dggsShapes[3].shapeLocation);
        assertEquals(2, dggsShapes[3].shapeData.polygon.outerRing.numberOfcells);

        final String outerRingLinestringCell1 = new String(dggsShapes[3].shapeData.polygon.outerRing.cells
                .getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim();
        final String outerRingLinestringCell2 = new String(dggsShapes[3].shapeData.polygon.outerRing.cells
                .getByteArray(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();

        assertEquals("07231131111113100331001", outerRingLinestringCell1);
        assertEquals("07012000001303022011321", outerRingLinestringCell2);

        assertEquals(2, dggsShapes[3].shapeData.polygon.numberOfInnerRings);

        final DggsNativeDggsTypes.DggsLinestring firstInnerRing = new DggsNativeDggsTypes.DggsLinestring(
                dggsShapes[3].shapeData.polygon.innerRings);

        final DggsNativeDggsTypes.DggsLinestring[] innerRingArray = (DggsNativeDggsTypes.DggsLinestring[]) firstInnerRing
                .toArray(dggsShapes[3].shapeData.polygon.numberOfInnerRings);

        final String innerRing1LinestringCell1 = new String(
                innerRingArray[0].cells.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim();
        final String innerRing1LinestringCell2 = new String(innerRingArray[0].cells
                .getByteArray(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();

        assertEquals("07231131111113100331001", innerRing1LinestringCell1);
        assertEquals("07012000001303022011321", innerRing1LinestringCell2);

        final String innerRing2LinestringCell1 = new String(
                innerRingArray[0].cells.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim();
        final String innerRing2LinestringCell2 = new String(innerRingArray[0].cells
                .getByteArray(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();

        assertEquals("07231131111113100331001", innerRing2LinestringCell1);
        assertEquals("07012000001303022011321", innerRing2LinestringCell2);

        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_DeallocateDggsShapes(dggsHandle, dggsShapesPointer, dggsShapes.length));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CloseDggsHandle(pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        // Test passing null
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_ConvertShapesToDggsShapes(null, shapes, shapes.length, dggsShapesPointer));
        assertEquals(ReturnCode.DGGS_INVALID_HANDLE, returnCode);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapesToDggsShapes(dggsHandle, null,
                shapes.length, dggsShapesPointer));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);

        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_ConvertShapesToDggsShapes(dggsHandle, shapes, shapes.length, null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);

        // Test invalid point data
        final DggsNativeLatLongTypes.LatLongShape[] invalidShapes = DggsNativeLatLongTypes.LatLongShape
                .createlatLongShapeArray(1);
        invalidShapes[0].latLongShapeType = LatLongShapeType.POINT.ordinal();
        invalidShapes[0].latLongShapeData = new DggsNativeLatLongTypes.LatLongShapeData();
        invalidShapes[0].latLongShapeData.setType(DggsNativeLatLongTypes.LatLongPoint.class);
        invalidShapes[0].latLongShapeData.point = new DggsNativeLatLongTypes.LatLongPoint();
        invalidShapes[0].latLongShapeData.point.latitude = 91.0;
        invalidShapes[0].latLongShapeData.point.longitude = 2.345;
        invalidShapes[0].latLongShapeData.point.accuracy = 3.879;

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapesToDggsShapes(dggsHandle,
                invalidShapes, invalidShapes.length, dggsShapesPointer));
        assertEquals(ReturnCode.DGGS_MODEL_ERROR, returnCode);
    }

    @Test
    public void dggsConvertShapeStringToDggsShapesTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        String wktText = "GEOMETRYCOLLECTION(MULTIPOINT(2.345 1.234, 4.567 3.456),LINESTRING(2.345 1.234, 4.567 3.456),POLYGON((2.345 1.234, 4.567 3.456), (2.345 1.234, 4.567 3.456), (2.345 1.234, 4.567 3.456)))";

        final PointerByReference dggsShapesPointer = new PointerByReference();
        final IntByReference numberOfShapes = new IntByReference();

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapeStringToDggsShapes(dggsHandle,
                wktText, ShapeStringFormat.WKT.ordinal(), 3.879, dggsShapesPointer, numberOfShapes));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(4, numberOfShapes.getValue());

        final DggsNativeDggsTypes.DggsShape firstShape = new DggsNativeDggsTypes.DggsShape(
                dggsShapesPointer.getValue());

        final DggsNativeDggsTypes.DggsShape[] dggsShapes = (DggsNativeDggsTypes.DggsShape[]) firstShape
                .toArray(numberOfShapes.getValue());

        assertEquals(DggsShapeType.CELL.ordinal(), dggsShapes[0].shapeType);
        assertEquals(DggsShapeLocation.ONE_FACE.ordinal(), dggsShapes[0].shapeLocation);
        String cellId = new String(dggsShapes[0].shapeData.cell);
        cellId = cellId.trim();
        assertEquals("07231131111113100331001", cellId);

        assertEquals(DggsShapeType.CELL.ordinal(), dggsShapes[1].shapeType);
        assertEquals(DggsShapeLocation.ONE_FACE.ordinal(), dggsShapes[1].shapeLocation);
        cellId = new String(dggsShapes[1].shapeData.cell);
        cellId = cellId.trim();
        assertEquals("07012000001303022011321", cellId);

        assertEquals(DggsShapeType.LINESTRING.ordinal(), dggsShapes[2].shapeType);
        assertEquals(DggsShapeLocation.NO_LOCATION.ordinal(), dggsShapes[2].shapeLocation);
        assertEquals(2, dggsShapes[2].shapeData.linestring.numberOfcells);

        final String linestringCell1 = new String(
                dggsShapes[2].shapeData.linestring.cells.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();
        final String linestringCell2 = new String(dggsShapes[2].shapeData.linestring.cells
                .getByteArray(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();

        assertEquals("07231131111113100331001", linestringCell1);
        assertEquals("07012000001303022011321", linestringCell2);

        assertEquals(DggsShapeType.POLYGON.ordinal(), dggsShapes[3].shapeType);
        assertEquals(DggsShapeLocation.NO_LOCATION.ordinal(), dggsShapes[3].shapeLocation);
        assertEquals(2, dggsShapes[3].shapeData.polygon.outerRing.numberOfcells);

        final String outerRingLinestringCell1 = new String(dggsShapes[3].shapeData.polygon.outerRing.cells
                .getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim();
        final String outerRingLinestringCell2 = new String(dggsShapes[3].shapeData.polygon.outerRing.cells
                .getByteArray(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();

        assertEquals("07231131111113100331001", outerRingLinestringCell1);
        assertEquals("07012000001303022011321", outerRingLinestringCell2);

        assertEquals(2, dggsShapes[3].shapeData.polygon.numberOfInnerRings);

        final DggsNativeDggsTypes.DggsLinestring firstInnerRing = new DggsNativeDggsTypes.DggsLinestring(
                dggsShapes[3].shapeData.polygon.innerRings);

        final DggsNativeDggsTypes.DggsLinestring[] innerRingArray = (DggsNativeDggsTypes.DggsLinestring[]) firstInnerRing
                .toArray(dggsShapes[3].shapeData.polygon.numberOfInnerRings);

        final String innerRing1LinestringCell1 = new String(
                innerRingArray[0].cells.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim();
        final String innerRing1LinestringCell2 = new String(innerRingArray[0].cells
                .getByteArray(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();

        assertEquals("07231131111113100331001", innerRing1LinestringCell1);
        assertEquals("07012000001303022011321", innerRing1LinestringCell2);

        final String innerRing2LinestringCell1 = new String(
                innerRingArray[0].cells.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim();
        final String innerRing2LinestringCell2 = new String(innerRingArray[0].cells
                .getByteArray(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                        .trim();

        assertEquals("07231131111113100331001", innerRing2LinestringCell1);
        assertEquals("07012000001303022011321", innerRing2LinestringCell2);

        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_DeallocateDggsShapes(dggsHandle, dggsShapesPointer, dggsShapes.length));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CloseDggsHandle(pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        // Test passing null
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapeStringToDggsShapes(null, wktText,
                ShapeStringFormat.WKT.ordinal(), 3.879, dggsShapesPointer, numberOfShapes));
        assertEquals(ReturnCode.DGGS_INVALID_HANDLE, returnCode);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapeStringToDggsShapes(dggsHandle,
                null, ShapeStringFormat.WKT.ordinal(), 3.879, dggsShapesPointer, numberOfShapes));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapeStringToDggsShapes(dggsHandle,
                wktText, ShapeStringFormat.WKT.ordinal(), 3.879, null, numberOfShapes));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapeStringToDggsShapes(dggsHandle,
                wktText, ShapeStringFormat.WKT.ordinal(), 3.879, dggsShapesPointer, null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);

        // Test invalid point data
        wktText = "GEOMETRYCOLLECTION(MULTIPOINT(2.345 91.0, 4.567 3.456))";

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertShapeStringToDggsShapes(dggsHandle,
                wktText, ShapeStringFormat.WKT.ordinal(), 3.879, dggsShapesPointer, numberOfShapes));
        assertEquals(ReturnCode.DGGS_MODEL_ERROR, returnCode);
    }

    @Test
    public void dggsConvertDggsCellsToPointsTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final DggsNativeDggsTypes.DggsShape[] dggsShapes = DggsNativeDggsTypes.DggsShape.createDggsShapeArray(2);

        final String[] cellIds = new String[] { "07231131111113100331001", "07012000001303022011321" };

        dggsShapes[0].shapeType = DggsShapeType.CELL.ordinal();
        dggsShapes[0].shapeData = new DggsNativeDggsTypes.DggsShapeData();
        dggsShapes[0].shapeData.setType(byte[].class);
        dggsShapes[0].shapeData.setCellId(cellIds[0]);

        dggsShapes[1].shapeType = DggsShapeType.CELL.ordinal();
        dggsShapes[1].shapeData = new DggsNativeDggsTypes.DggsShapeData();
        dggsShapes[1].shapeData.setType(byte[].class);
        dggsShapes[1].shapeData.setCellId(cellIds[1]);

        final DggsNativeLatLongTypes.LatLongPoint[] latLongPoints = DggsNativeLatLongTypes.LatLongPoint
                .createlatLongPointArray(cellIds.length);

        final int cellStringSize = dggsShapes[0].shapeData.cell.length;

        final Pointer cellIdsPointer = new Memory(cellStringSize * cellIds.length);
        cellIdsPointer.write(0, dggsShapes[0].shapeData.cell, 0, cellStringSize);
        cellIdsPointer.write(cellStringSize, dggsShapes[1].shapeData.cell, 0, cellStringSize);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToPoints(dggsHandle,
                cellIdsPointer, cellIds.length, latLongPoints[0]));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        assertEquals(cellIds.length, latLongPoints.length);

        final double tolerance = 1E-3;

        assertEquals(1.234, latLongPoints[0].latitude, tolerance);
        assertEquals(2.345, latLongPoints[0].longitude, tolerance);
        assertEquals(5.804, latLongPoints[0].accuracy, tolerance);

        assertEquals(3.456, latLongPoints[1].latitude, tolerance);
        assertEquals(4.567, latLongPoints[1].longitude, tolerance);
        assertEquals(5.804, latLongPoints[1].accuracy, tolerance);

        // Test for error cases
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToPoints(null,
                cellIdsPointer, cellIds.length, latLongPoints[0]));
        assertEquals(ReturnCode.DGGS_INVALID_HANDLE, returnCode);
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToPoints(dggsHandle, null,
                cellIds.length, latLongPoints[0]));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToPoints(dggsHandle, cellIdsPointer, cellIds.length, null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);

        final Pointer invalidCellIdsPointer = new Memory(cellStringSize);
        invalidCellIdsPointer.clear(cellStringSize);
        invalidCellIdsPointer.write(0, new String("XX").getBytes(), 0, 2);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToPoints(dggsHandle,
                invalidCellIdsPointer, 1, latLongPoints[0]));
        assertEquals(ReturnCode.DGGS_MODEL_ERROR, returnCode);
    }

    @Test
    public void dggsConvertDggsCellsToShapeStringTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final DggsNativeDggsTypes.DggsShape[] dggsShapes = DggsNativeDggsTypes.DggsShape.createDggsShapeArray(2);

        final String[] cellIds = new String[] { "07231131111113100331001", "07012000001303022011321" };

        dggsShapes[0].shapeType = DggsShapeType.CELL.ordinal();
        dggsShapes[0].shapeData = new DggsNativeDggsTypes.DggsShapeData();
        dggsShapes[0].shapeData.setType(byte[].class);
        dggsShapes[0].shapeData.setCellId(cellIds[0]);

        dggsShapes[1].shapeType = DggsShapeType.CELL.ordinal();
        dggsShapes[1].shapeData = new DggsNativeDggsTypes.DggsShapeData();
        dggsShapes[1].shapeData.setType(byte[].class);
        dggsShapes[1].shapeData.setCellId(cellIds[1]);

        PointerByReference latLongShapeString = new PointerByReference();

        final int cellStringSize = dggsShapes[0].shapeData.cell.length;

        final Pointer cellIdsPointer = new Memory(cellStringSize * cellIds.length);
        cellIdsPointer.write(0, dggsShapes[0].shapeData.cell, 0, cellStringSize);
        cellIdsPointer.write(cellStringSize, dggsShapes[1].shapeData.cell, 0, cellStringSize);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToShapeString(dggsHandle,
                cellIdsPointer, cellIds.length, ShapeStringFormat.WKT.ordinal(), latLongShapeString));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        // Values in the string may vary due to numerical precision - extract them using a regex and compare them as
        // doubles
        final Pattern pattern = Pattern
                .compile("MULTIPOINT \\((\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*),(\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*)\\)");

        final Matcher matcher = pattern.matcher(latLongShapeString.getValue().getString(0));

        final double[] coordinates = new double[4];

        assertTrue(matcher.matches());
        coordinates[0] = Double.parseDouble(matcher.group(1));
        coordinates[1] = Double.parseDouble(matcher.group(2));
        coordinates[2] = Double.parseDouble(matcher.group(3));
        coordinates[3] = Double.parseDouble(matcher.group(4));

        final double tolerance = 1E-3;

        assertEquals(2.345, coordinates[0], tolerance);
        assertEquals(1.234, coordinates[1], tolerance);
        assertEquals(4.567, coordinates[2], tolerance);
        assertEquals(3.456, coordinates[3], tolerance);

        returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_DeallocateString(dggsHandle, latLongShapeString));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        // Test for error cases
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToShapeString(null,
                cellIdsPointer, cellIds.length, ShapeStringFormat.WKT.ordinal(), latLongShapeString));
        assertEquals(ReturnCode.DGGS_INVALID_HANDLE, returnCode);
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToShapeString(dggsHandle,
                null, cellIds.length, ShapeStringFormat.WKT.ordinal(), latLongShapeString));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellsToShapeString(dggsHandle,
                cellIdsPointer, cellIds.length, ShapeStringFormat.WKT.ordinal(), null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
    }

    @Test
    public void dggsGetCellParentsTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final String cellId = "07012212222221011101013";

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);
        bb.put(cellId.getBytes(), 0, cellId.length());

        final Pointer parentCellsPointer = new Memory(
                EaggrLibrary.MAX_NUMBER_OF_PARENTS * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        final IntByReference numberOfParents = new IntByReference();

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetDggsCellParents(dggsHandle, cellIdBytes,
                parentCellsPointer, numberOfParents));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        assertEquals(1, numberOfParents.getValue());
        final String parentCellId = new String(
                parentCellsPointer.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim();
        assertEquals("0701221222222101110101", parentCellId);

        // Test for error cases
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_GetDggsCellParents(null, cellIdBytes, parentCellsPointer, numberOfParents));
        assertEquals(ReturnCode.DGGS_INVALID_HANDLE, returnCode);
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_GetDggsCellParents(dggsHandle, cellIdBytes, null, numberOfParents));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_GetDggsCellParents(dggsHandle, cellIdBytes, parentCellsPointer, null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
    }

    @Test
    public void dggsGetCellChildrenTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final String cellId = "07012212222221011101013";

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);
        bb.put(cellId.getBytes(), 0, cellId.length());

        final Pointer childCellsPointer = new Memory(
                EaggrLibrary.MAX_NUMBER_OF_CHILDREN * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        final IntByReference numberOfChildren = new IntByReference();

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetDggsCellChildren(dggsHandle, cellIdBytes,
                childCellsPointer, numberOfChildren));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        assertEquals(4, numberOfChildren.getValue());

        final String[] childCellIds = new String[numberOfChildren.getValue()];

        for (int child = 0; child < numberOfChildren.getValue(); ++child) {
            childCellIds[child] = new String(childCellsPointer.getByteArray(
                    child * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                            .trim();
        }

        assertEquals("070122122222210111010130", childCellIds[0]);
        assertEquals("070122122222210111010131", childCellIds[1]);
        assertEquals("070122122222210111010132", childCellIds[2]);
        assertEquals("070122122222210111010133", childCellIds[3]);

        // Test for error cases
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetDggsCellChildren(null, cellIdBytes,
                childCellsPointer, numberOfChildren));
        assertEquals(ReturnCode.DGGS_INVALID_HANDLE, returnCode);
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_GetDggsCellChildren(dggsHandle, cellIdBytes, null, numberOfChildren));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_GetDggsCellChildren(dggsHandle, cellIdBytes, childCellsPointer, null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
    }

    @Test
    public void dggsGetCellSiblingsTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final String cellId = "070122122222210111010130";

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);
        bb.put(cellId.getBytes(), 0, cellId.length());

        final Pointer siblingCellsPointer = new Memory(
                EaggrLibrary.MAX_NUMBER_OF_SIBLINGS * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        final IntByReference numberOfSiblings = new IntByReference();

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetDggsCellSiblings(dggsHandle, cellIdBytes,
                siblingCellsPointer, numberOfSiblings));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        assertEquals(3, numberOfSiblings.getValue());

        final String[] siblingCellIds = new String[numberOfSiblings.getValue()];

        for (int sibling = 0; sibling < numberOfSiblings.getValue(); ++sibling) {
            siblingCellIds[sibling] = new String(siblingCellsPointer.getByteArray(
                    sibling * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH))
                            .trim();
        }

        assertEquals("070122122222210111010131", siblingCellIds[0]);
        assertEquals("070122122222210111010132", siblingCellIds[1]);
        assertEquals("070122122222210111010133", siblingCellIds[2]);

        // Test for error cases
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetDggsCellSiblings(null, cellIdBytes,
                siblingCellsPointer, numberOfSiblings));
        assertEquals(ReturnCode.DGGS_INVALID_HANDLE, returnCode);
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_GetDggsCellSiblings(dggsHandle, cellIdBytes, null, numberOfSiblings));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
        returnCode = ReturnCode.fromNativeCode(
                EaggrLibrary.INSTANCE.EAGGR_GetDggsCellSiblings(dggsHandle, cellIdBytes, siblingCellsPointer, null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
    }

    @Test
    public void dggsGetBoundingCellTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final String[] cellIds = new String[] { "07001", "07002", "07003" };

        final Pointer cellIdsPointer = new Memory(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cellIds.length);

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cellIds.length];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);
        bb.put(cellIds[0].getBytes(), 0, cellIds[0].length());
        bb.position(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        bb.put(cellIds[1].getBytes(), 0, cellIds[1].length());
        bb.position(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * 2);
        bb.put(cellIds[2].getBytes(), 0, cellIds[2].length());

        cellIdsPointer.write(0, cellIdBytes, 0, cellIdBytes.length);

        final Pointer boundingCellPointer = new Memory(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_GetBoundingDggsCell(dggsHandle,
                cellIdsPointer, cellIds.length, boundingCellPointer));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        String boundingCell = new String(
                boundingCellPointer.getByteArray(0, DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim();

        assertEquals("0700", boundingCell);
    }

    @Test
    public void dggsCreateKmlFileTest() throws EaggrException, IOException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final String[] cellIds = new String[] { "07231131111113100331001", "07012000001303022011321" };

        final Pointer cellIdsPointer = new Memory(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cellIds.length);

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cellIds.length];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);
        bb.put(cellIds[0].getBytes(), 0, cellIds[0].length());
        bb.position(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        bb.put(cellIds[1].getBytes(), 0, cellIds[1].length());

        cellIdsPointer.write(0, cellIdBytes, 0, cellIdBytes.length);

        final String outputKmlFileName = "../EAGGRJava/testData/EaggrLibraryTestOutput.kml";

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CreateDggsKmlFile(dggsHandle,
                outputKmlFileName, cellIdsPointer, cellIds.length));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final String expectedKmlFileName = "../EAGGRJava/testData/ExpectedEaggrLibraryTestOutput.kml";

        // Ensure the files are identical
        final String actualOutput = TestUtilities
                .removeEndOfLineCharacters(new String(Files.readAllBytes(Paths.get(outputKmlFileName))));
        final String expectedOutput = TestUtilities
                .removeEndOfLineCharacters(new String(Files.readAllBytes(Paths.get(expectedKmlFileName))));

        assertEquals(expectedOutput, actualOutput);

        final File outputKmlFile = new File(outputKmlFileName);
        outputKmlFile.delete();
    }

    @Test
    public void dggsGetCellShapeStringTest() throws EaggrException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final String cellId = "07231131111113100331001";

        final PointerByReference latLongShapeString = new PointerByReference();

        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH];
        final ByteBuffer bb = ByteBuffer.wrap(cellIdBytes);
        bb.put(cellId.getBytes(), 0, cellId.length());

        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellOutlineToShapeString(
                dggsHandle, cellIdBytes, ShapeStringFormat.WKT.ordinal(), latLongShapeString));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        // Values in the string may vary due to numerical precision - extract them using a regex and compare them as
        // doubles
        final Pattern pattern = Pattern.compile(
                "POLYGON \\(\\((\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*),(\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*),(\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*)\\)\\)");

        final Matcher matcher = pattern.matcher(latLongShapeString.getValue().getString(0));

        final double[] coordinates = new double[6];

        assertTrue(matcher.matches());
        coordinates[0] = Double.parseDouble(matcher.group(1));
        coordinates[1] = Double.parseDouble(matcher.group(2));
        coordinates[2] = Double.parseDouble(matcher.group(3));
        coordinates[3] = Double.parseDouble(matcher.group(4));
        coordinates[4] = Double.parseDouble(matcher.group(5));
        coordinates[5] = Double.parseDouble(matcher.group(6));

        final double tolerance = 1E-3;

        assertEquals(2.3449866, coordinates[0], tolerance);
        assertEquals(1.2340080, coordinates[1], tolerance);
        assertEquals(2.3450042, coordinates[2], tolerance);
        assertEquals(1.2339790, coordinates[3], tolerance);
        assertEquals(2.3450218, coordinates[4], tolerance);
        assertEquals(1.2340036, coordinates[5], tolerance);

        returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_DeallocateString(dggsHandle, latLongShapeString));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        // Test for error cases
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellOutlineToShapeString(null,
                cellId.getBytes(), ShapeStringFormat.WKT.ordinal(), latLongShapeString));
        assertEquals(ReturnCode.DGGS_INVALID_HANDLE, returnCode);
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_ConvertDggsCellOutlineToShapeString(
                dggsHandle, cellId.getBytes(), ShapeStringFormat.WKT.ordinal(), null));
        assertEquals(ReturnCode.DGGS_NULL_POINTER, returnCode);
    }

    @Test
    public void dggsCompareShapesEqualsTest() throws EaggrException, IOException {

        final Pointer[] pointers = new Pointer[1];
        pointers[0] = Pointer.NULL;
        ReturnCode returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_OpenDggsHandle(DggsModel.ISEA4T.ordinal(), pointers));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);

        final Pointer dggsHandle = pointers[0];

        final IntByReference spatialAnalysisResultInt = new IntByReference();

        // Construct a cell
        final String cellId = "0000";
        final DggsNativeDggsTypes.DggsShape nativeShapeCell = new DggsNativeDggsTypes.DggsShape();
        nativeShapeCell.shapeType = DggsShapeType.CELL.ordinal();
        nativeShapeCell.shapeLocation = DggsShapeLocation.ONE_FACE.ordinal();
        nativeShapeCell.shapeData.setType(byte[].class);
        nativeShapeCell.shapeData.setCellId(cellId);

        // Construct a linestring
        final DggsNativeDggsTypes.DggsShape nativeShapeLineString = new DggsNativeDggsTypes.DggsShape();
        final String[] cellIds = new String[] { "0001", "0002", "0003" };
        nativeShapeLineString.shapeType = DggsShapeType.LINESTRING.ordinal();
        nativeShapeLineString.shapeLocation = DggsShapeLocation.ONE_FACE.ordinal();
        nativeShapeLineString.shapeData.setType(DggsNativeDggsTypes.DggsLinestring.class);
        nativeShapeLineString.shapeData.linestring.numberOfcells = (short) cellIds.length;

        final Pointer cellIdsPointer = new Memory(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cellIds.length);
        final byte[] cellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * cellIds.length];

        final ByteBuffer linestringBuffer = ByteBuffer.wrap(cellIdBytes);
        linestringBuffer.put(cellIds[0].getBytes(), 0, cellIds[0].length());
        linestringBuffer.position(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        linestringBuffer.put(cellIds[1].getBytes(), 0, cellIds[1].length());
        linestringBuffer.position(2 * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        linestringBuffer.put(cellIds[2].getBytes(), 0, cellIds[2].length());

        cellIdsPointer.write(0, cellIdBytes, 0, cellIdBytes.length);
        nativeShapeLineString.shapeData.linestring.cells = cellIdsPointer;

        // Construct a polygon
        final DggsNativeDggsTypes.DggsShape nativeShapePolygon = new DggsNativeDggsTypes.DggsShape();
        final String[] outerCellIds = new String[] { "0001", "0002", "0003" };
        nativeShapePolygon.shapeType = DggsShapeType.POLYGON.ordinal();
        nativeShapePolygon.shapeLocation = DggsShapeLocation.ONE_FACE.ordinal();
        nativeShapePolygon.shapeData.setType(DggsNativeDggsTypes.DggsPolygon.class);
        nativeShapePolygon.shapeData.polygon.outerRing.numberOfcells = (short) outerCellIds.length;
        nativeShapePolygon.shapeData.polygon.numberOfInnerRings = 0;

        final Pointer outerCellIdsPointer = new Memory(
                DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * outerCellIds.length);
        final byte[] outerCellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * outerCellIds.length];

        final ByteBuffer outerLinestringBuffer = ByteBuffer.wrap(outerCellIdBytes);
        outerLinestringBuffer.put(outerCellIds[0].getBytes(), 0, outerCellIds[0].length());
        outerLinestringBuffer.position(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        outerLinestringBuffer.put(outerCellIds[1].getBytes(), 0, outerCellIds[1].length());
        outerLinestringBuffer.position(2 * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        outerLinestringBuffer.put(outerCellIds[2].getBytes(), 0, outerCellIds[2].length());

        outerCellIdsPointer.write(0, outerCellIdBytes, 0, outerCellIdBytes.length);
        nativeShapePolygon.shapeData.polygon.outerRing.cells = outerCellIdsPointer;

        // Test the cell

        // Compare the cell with itself
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CompareShapes(dggsHandle,
                AnalysisType.EQUALS.ordinal(), nativeShapeCell, nativeShapeCell, spatialAnalysisResultInt));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(1, spatialAnalysisResultInt.getValue());

        // Construct a disjoint cell
        final String anotherCellId = "0001";
        final DggsNativeDggsTypes.DggsShape anotherNativeShapeCell = new DggsNativeDggsTypes.DggsShape();
        anotherNativeShapeCell.shapeType = DggsShapeType.CELL.ordinal();
        anotherNativeShapeCell.shapeLocation = DggsShapeLocation.ONE_FACE.ordinal();
        anotherNativeShapeCell.shapeData.setType(byte[].class);
        anotherNativeShapeCell.shapeData.setCellId(anotherCellId);

        // Compare the cell with a disjoint cell
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CompareShapes(dggsHandle,
                AnalysisType.EQUALS.ordinal(), nativeShapeCell, anotherNativeShapeCell, spatialAnalysisResultInt));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(0, spatialAnalysisResultInt.getValue());

        // Test the linestring

        // Compare the linestring with itself
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CompareShapes(dggsHandle,
                AnalysisType.EQUALS.ordinal(), nativeShapeLineString, nativeShapeLineString, spatialAnalysisResultInt));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(1, spatialAnalysisResultInt.getValue());

        // Construct another linestring
        final DggsNativeDggsTypes.DggsShape anotherNativeShapeLineString = new DggsNativeDggsTypes.DggsShape();
        final String[] anotherCellIds = new String[] { "0000", "0002", "0003" };
        anotherNativeShapeLineString.shapeType = DggsShapeType.LINESTRING.ordinal();
        anotherNativeShapeLineString.shapeLocation = DggsShapeLocation.ONE_FACE.ordinal();
        anotherNativeShapeLineString.shapeData.setType(DggsNativeDggsTypes.DggsLinestring.class);
        anotherNativeShapeLineString.shapeData.linestring.numberOfcells = (short) anotherCellIds.length;

        final Pointer anotherCellIdsPointer = new Memory(
                DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * anotherCellIds.length);
        final byte[] anotherCellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * anotherCellIds.length];

        final ByteBuffer anotherLinestringBuffer = ByteBuffer.wrap(anotherCellIdBytes);
        anotherLinestringBuffer.put(anotherCellIds[0].getBytes(), 0, anotherCellIds[0].length());
        anotherLinestringBuffer.position(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        anotherLinestringBuffer.put(anotherCellIds[1].getBytes(), 0, anotherCellIds[1].length());
        anotherLinestringBuffer.position(2 * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        anotherLinestringBuffer.put(anotherCellIds[2].getBytes(), 0, anotherCellIds[2].length());

        anotherCellIdsPointer.write(0, anotherCellIdBytes, 0, anotherCellIdBytes.length);
        anotherNativeShapeLineString.shapeData.linestring.cells = anotherCellIdsPointer;

        // Compare the linestring with a different linestring
        returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CompareShapes(dggsHandle, AnalysisType.EQUALS.ordinal(),
                        nativeShapeLineString, anotherNativeShapeLineString, spatialAnalysisResultInt));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(0, spatialAnalysisResultInt.getValue());

        // Test the polygon

        // Compare the polygon with itself
        returnCode = ReturnCode.fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CompareShapes(dggsHandle,
                AnalysisType.EQUALS.ordinal(), nativeShapePolygon, nativeShapePolygon, spatialAnalysisResultInt));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(1, spatialAnalysisResultInt.getValue());

        // Construct a different polygon
        final DggsNativeDggsTypes.DggsShape anotherNativeShapePolygon = new DggsNativeDggsTypes.DggsShape();
        final String[] anotherOuterCellIds = new String[] { "0000", "0002", "0003" };
        anotherNativeShapePolygon.shapeType = DggsShapeType.POLYGON.ordinal();
        anotherNativeShapePolygon.shapeLocation = DggsShapeLocation.ONE_FACE.ordinal();
        anotherNativeShapePolygon.shapeData.setType(DggsNativeDggsTypes.DggsPolygon.class);
        anotherNativeShapePolygon.shapeData.polygon.outerRing.numberOfcells = (short) anotherOuterCellIds.length;
        anotherNativeShapePolygon.shapeData.polygon.numberOfInnerRings = 0;

        final Pointer anotherOuterCellIdsPointer = new Memory(
                DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * anotherOuterCellIds.length);
        final byte[] anotherOuterCellIdBytes = new byte[DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH
                * anotherOuterCellIds.length];

        final ByteBuffer anotherOuterLinestringBuffer = ByteBuffer.wrap(anotherOuterCellIdBytes);
        anotherOuterLinestringBuffer.put(anotherOuterCellIds[0].getBytes(), 0, anotherOuterCellIds[0].length());
        anotherOuterLinestringBuffer.position(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        anotherOuterLinestringBuffer.put(anotherOuterCellIds[1].getBytes(), 0, anotherOuterCellIds[1].length());
        anotherOuterLinestringBuffer.position(2 * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        anotherOuterLinestringBuffer.put(anotherOuterCellIds[2].getBytes(), 0, anotherOuterCellIds[2].length());

        anotherOuterCellIdsPointer.write(0, anotherOuterCellIdBytes, 0, anotherOuterCellIdBytes.length);
        anotherNativeShapePolygon.shapeData.polygon.outerRing.cells = anotherOuterCellIdsPointer;

        // Compare the polygon with itself
        returnCode = ReturnCode
                .fromNativeCode(EaggrLibrary.INSTANCE.EAGGR_CompareShapes(dggsHandle, AnalysisType.EQUALS.ordinal(),
                        nativeShapePolygon, anotherNativeShapePolygon, spatialAnalysisResultInt));
        assertEquals(ReturnCode.DGGS_SUCCESS, returnCode);
        assertEquals(0, spatialAnalysisResultInt.getValue());
    }

}
