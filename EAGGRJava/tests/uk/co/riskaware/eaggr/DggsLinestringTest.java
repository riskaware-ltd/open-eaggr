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

public class DggsLinestringTest {

    @Test
    public void constructionTest() {
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

        final DggsLinestring linestring = new DggsLinestring(nativeLineString);

        assertEquals(3, linestring.getLinestringCellIds().size());

        assertEquals(cell1, linestring.getLinestringCellIds().get(0).getCellId());
        assertEquals(cell2, linestring.getLinestringCellIds().get(1).getCellId());
        assertEquals(cell3, linestring.getLinestringCellIds().get(2).getCellId());
    }

    @Test
    public void dggsCellConstructorTest() {
        final ArrayList<DggsCell> linestringCellIds = new ArrayList<DggsCell>();

        final DggsCell linestringDggsCell1 = new DggsCell("0001");
        final DggsCell linestringDggsCell2 = new DggsCell("0002");
        final DggsCell linestringDggsCell3 = new DggsCell("0003");
        linestringCellIds.add(linestringDggsCell1);
        linestringCellIds.add(linestringDggsCell2);
        linestringCellIds.add(linestringDggsCell3);

        final DggsLinestring dggsLinestring = new DggsLinestring(linestringCellIds);

        assertEquals(3, dggsLinestring.getLinestringCellIds().size());
        assertEquals("0001", dggsLinestring.getLinestringCellIds().get(0).getCellId());
        assertEquals("0002", dggsLinestring.getLinestringCellIds().get(1).getCellId());
        assertEquals("0003", dggsLinestring.getLinestringCellIds().get(2).getCellId());

    }
}
