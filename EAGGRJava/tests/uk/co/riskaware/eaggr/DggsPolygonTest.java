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

import java.util.ArrayList;

import org.junit.Test;

import com.sun.jna.Memory;

public class DggsPolygonTest {
    @Test
    public void constructorTest() {
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

        final DggsPolygon polygon = new DggsPolygon(nativePolygon);

        assertEquals(3, polygon.getOuterRing().getLinestringCellIds().size());
        assertEquals(cell1, polygon.getOuterRing().getLinestringCellIds().get(0).getCellId());
        assertEquals(cell2, polygon.getOuterRing().getLinestringCellIds().get(1).getCellId());
        assertEquals(cell3, polygon.getOuterRing().getLinestringCellIds().get(2).getCellId());

        assertEquals(0, polygon.getInnerRings().size());
    }

    @Test
    public void dggsLinestringsConstructorTest() {

        // Outer linestring
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

        assertEquals(3, dggsPolygon.getOuterRing().getLinestringCellIds().size());
        assertEquals("0001", dggsPolygon.getOuterRing().getLinestringCellIds().get(0).getCellId());
        assertEquals("0002", dggsPolygon.getOuterRing().getLinestringCellIds().get(1).getCellId());
        assertEquals("0003", dggsPolygon.getOuterRing().getLinestringCellIds().get(2).getCellId());

        assertEquals(1, dggsPolygon.getInnerRings().size());

        assertEquals(3, dggsPolygon.getInnerRings().get(0).getLinestringCellIds().size());
        assertEquals("00001", dggsPolygon.getInnerRings().get(0).getLinestringCellIds().get(0).getCellId());
        assertEquals("00032", dggsPolygon.getInnerRings().get(0).getLinestringCellIds().get(1).getCellId());
        assertEquals("00101", dggsPolygon.getInnerRings().get(0).getLinestringCellIds().get(2).getCellId());
    }
}
