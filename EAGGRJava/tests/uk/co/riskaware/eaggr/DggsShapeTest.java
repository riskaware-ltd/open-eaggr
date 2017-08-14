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
import static org.junit.Assert.assertNull;

import java.util.ArrayList;

import org.junit.Test;

import com.sun.jna.Memory;

import uk.co.riskaware.eaggr.enums.DggsShapeLocation;
import uk.co.riskaware.eaggr.enums.DggsShapeType;

public class DggsShapeTest {

    @Test
    public void cellConstructorTest() throws EaggrException {
        final String cellId = "CellId";

        final DggsNativeDggsTypes.DggsShape nativeShape = new DggsNativeDggsTypes.DggsShape();
        nativeShape.shapeType = DggsShapeType.CELL.ordinal();
        nativeShape.shapeLocation = DggsShapeLocation.NO_LOCATION.ordinal();
        nativeShape.shapeData.cell = cellId.getBytes();

        final DggsShape shape = new DggsShape(nativeShape);

        assertEquals(DggsShapeType.CELL, shape.getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, shape.getShapeLocation());
        assertEquals(cellId, shape.getCell().getCellId());
        assertNull(shape.getLinestring());
        assertNull(shape.getPolygon());
    }

    @Test
    public void linestringConstructorTest() throws EaggrException {
        final DggsNativeDggsTypes.DggsLinestring nativeLineString = new DggsNativeDggsTypes.DggsLinestring();
        nativeLineString.numberOfcells = 3;

        final String cell1 = "Cell1";
        final String cell2 = "Cell2";
        final String cell3 = "Cell3";

        final Memory cells = new Memory(3 * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        cells.clear();
        cells.write(0, cell1.getBytes(), 0, cell1.length());
        cells.write(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, cell2.getBytes(), 0, cell2.length());
        cells.write(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * 2, cell3.getBytes(), 0, cell3.length());

        nativeLineString.cells = cells;

        final DggsNativeDggsTypes.DggsShape nativeShape = new DggsNativeDggsTypes.DggsShape();
        nativeShape.shapeType = DggsShapeType.LINESTRING.ordinal();
        nativeShape.shapeLocation = DggsShapeLocation.NO_LOCATION.ordinal();
        nativeShape.shapeData.linestring = nativeLineString;

        final DggsShape shape = new DggsShape(nativeShape);

        assertEquals(DggsShapeType.LINESTRING, shape.getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, shape.getShapeLocation());

        assertEquals(3, shape.getLinestring().getLinestringCellIds().size());
        assertEquals(cell1, shape.getLinestring().getLinestringCellIds().get(0).getCellId());
        assertEquals(cell2, shape.getLinestring().getLinestringCellIds().get(1).getCellId());
        assertEquals(cell3, shape.getLinestring().getLinestringCellIds().get(2).getCellId());

        assertNull(shape.getCell());
        assertNull(shape.getPolygon());
    }

    @Test
    public void polygonConstructorTest() throws EaggrException {
        final DggsNativeDggsTypes.DggsPolygon nativePolygon = new DggsNativeDggsTypes.DggsPolygon();

        final DggsNativeDggsTypes.DggsLinestring outerRingLineString = new DggsNativeDggsTypes.DggsLinestring();
        outerRingLineString.numberOfcells = 3;

        final String cell1 = "Cell1";
        final String cell2 = "Cell2";
        final String cell3 = "Cell3";

        final Memory outerCells = new Memory(3 * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH);
        outerCells.clear();
        outerCells.write(0, cell1.getBytes(), 0, cell1.length());
        outerCells.write(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH, cell2.getBytes(), 0, cell2.length());
        outerCells.write(DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH * 2, cell3.getBytes(), 0, cell3.length());

        outerRingLineString.cells = outerCells;
        nativePolygon.outerRing = outerRingLineString;

        final DggsNativeDggsTypes.DggsShape nativeShape = new DggsNativeDggsTypes.DggsShape();
        nativeShape.shapeType = DggsShapeType.POLYGON.ordinal();
        nativeShape.shapeLocation = DggsShapeLocation.NO_LOCATION.ordinal();
        nativeShape.shapeData.polygon = nativePolygon;

        final DggsShape shape = new DggsShape(nativeShape);

        assertEquals(DggsShapeType.POLYGON, shape.getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, shape.getShapeLocation());

        assertEquals(3, shape.getPolygon().getOuterRing().getLinestringCellIds().size());
        assertEquals(cell1, shape.getPolygon().getOuterRing().getLinestringCellIds().get(0).getCellId());
        assertEquals(cell2, shape.getPolygon().getOuterRing().getLinestringCellIds().get(1).getCellId());
        assertEquals(cell3, shape.getPolygon().getOuterRing().getLinestringCellIds().get(2).getCellId());

        assertEquals(0, shape.getPolygon().getInnerRings().size());

        assertNull(shape.getCell());
        assertNull(shape.getLinestring());
    }

    @Test
    public void dggsCellConstructorTest() throws EaggrException {

        final DggsCell dggsCell = new DggsCell("0000");
        final DggsShape shapeFromCell = new DggsShape(dggsCell);

        assertEquals(DggsShapeType.CELL, shapeFromCell.getShapeType());
        assertEquals(DggsShapeLocation.ONE_FACE, shapeFromCell.getShapeLocation());
        assertEquals("0000", shapeFromCell.getCell().getCellId());

        assertNull(shapeFromCell.getLinestring());
        assertNull(shapeFromCell.getPolygon());
    }

    @Test
    public void dggsLinestringConstructorTest() throws EaggrException {

        final ArrayList<DggsCell> linestringCellIds = new ArrayList<DggsCell>();
        final DggsCell linestringDggsCell1 = new DggsCell("0001");
        final DggsCell linestringDggsCell2 = new DggsCell("0002");
        final DggsCell linestringDggsCell3 = new DggsCell("0003");
        linestringCellIds.add(linestringDggsCell1);
        linestringCellIds.add(linestringDggsCell2);
        linestringCellIds.add(linestringDggsCell3);
        final DggsLinestring dggsLinestring = new DggsLinestring(linestringCellIds);
        final DggsShape shapeFromLineString = new DggsShape(dggsLinestring);

        assertEquals(DggsShapeType.LINESTRING, shapeFromLineString.getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, shapeFromLineString.getShapeLocation());
        assertEquals("0001", shapeFromLineString.getLinestring().getLinestringCellIds().get(0).getCellId());
        assertEquals("0002", shapeFromLineString.getLinestring().getLinestringCellIds().get(1).getCellId());
        assertEquals("0003", shapeFromLineString.getLinestring().getLinestringCellIds().get(2).getCellId());

        assertNull(shapeFromLineString.getCell());
        assertNull(shapeFromLineString.getPolygon());
    }

    @Test
    public void dggsPolygonConstructorTest() throws EaggrException {

        // Construct outer ring
        final ArrayList<DggsCell> outerLinestringCellIds = new ArrayList<DggsCell>();
        final DggsCell outerLinestringDggsCell1 = new DggsCell("0001");
        final DggsCell outerLinestringDggsCell2 = new DggsCell("0002");
        final DggsCell outerLinestringDggsCell3 = new DggsCell("0003");
        outerLinestringCellIds.add(outerLinestringDggsCell1);
        outerLinestringCellIds.add(outerLinestringDggsCell2);
        outerLinestringCellIds.add(outerLinestringDggsCell3);
        final DggsLinestring outerRingLinestring = new DggsLinestring(outerLinestringCellIds);

        // Inner rings
        final ArrayList<DggsLinestring> innerRingLinestringArray = new ArrayList<DggsLinestring>();
        final ArrayList<DggsCell> innerRingLinestringCellIds = new ArrayList<DggsCell>();
        final DggsCell innerRingLinestringDggsCell1 = new DggsCell("00001");
        final DggsCell innerRingLinestringDggsCell2 = new DggsCell("00032");
        final DggsCell innerRingLinestringDggsCell3 = new DggsCell("00101");
        innerRingLinestringCellIds.add(innerRingLinestringDggsCell1);
        innerRingLinestringCellIds.add(innerRingLinestringDggsCell2);
        innerRingLinestringCellIds.add(innerRingLinestringDggsCell3);
        final DggsLinestring innerRingLinestring = new DggsLinestring(innerRingLinestringCellIds);
        innerRingLinestringArray.add(innerRingLinestring);

        final DggsPolygon dggsPolygon = new DggsPolygon(outerRingLinestring, innerRingLinestringArray);
        final DggsShape shapeFromPolygon = new DggsShape(dggsPolygon);

        assertEquals(DggsShapeType.POLYGON, shapeFromPolygon.getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, shapeFromPolygon.getShapeLocation());
        assertEquals(3, shapeFromPolygon.getPolygon().getOuterRing().getLinestringCellIds().size());
        assertEquals("0001", shapeFromPolygon.getPolygon().getOuterRing().getLinestringCellIds().get(0).getCellId());
        assertEquals("0002", shapeFromPolygon.getPolygon().getOuterRing().getLinestringCellIds().get(1).getCellId());
        assertEquals("0003", shapeFromPolygon.getPolygon().getOuterRing().getLinestringCellIds().get(2).getCellId());

        assertEquals(1, shapeFromPolygon.getPolygon().getInnerRings().size());
        assertEquals(3, shapeFromPolygon.getPolygon().getInnerRings().get(0).getLinestringCellIds().size());
        assertEquals("00001",
                shapeFromPolygon.getPolygon().getInnerRings().get(0).getLinestringCellIds().get(0).getCellId());
        assertEquals("00032",
                shapeFromPolygon.getPolygon().getInnerRings().get(0).getLinestringCellIds().get(1).getCellId());
        assertEquals("00101",
                shapeFromPolygon.getPolygon().getInnerRings().get(0).getLinestringCellIds().get(2).getCellId());
    }
}
