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

import java.util.ArrayList;
import java.util.List;

/**
 * Class to represent a DGGS linestring
 * 
 * @author i_bush
 *
 */
public class DggsLinestring {

    private final ArrayList<DggsCell> linestringCellIds = new ArrayList<DggsCell>();

    /**
     * Package internal constructor - not required to be constructed by client code
     * 
     * @param linestring
     *            The native linestring object
     */
    DggsLinestring(final DggsNativeDggsTypes.DggsLinestring linestring) {

        // Loop over each cell in the linestring and extract the cell id
        for (int cell = 0; cell < linestring.numberOfcells; ++cell) {
            linestringCellIds.add(new DggsCell(
                    new String(linestring.cells.getByteArray(cell * DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH,
                            DggsNativeDggsTypes.MAX_CELL_STRING_LENGTH)).trim()));
        }
    }

    /**
     * Package internal constructor - not required to be constructed by client code
     * 
     * @param dggsLinestring
     *            An array of DGGS cells used to populate the linestring
     */
    public DggsLinestring(final ArrayList<DggsCell> dggsLinestring) {

        // Loop over each cell in the linestring and extract the cell id
        for (int cell = 0; cell < dggsLinestring.size(); ++cell) {
            this.linestringCellIds.add(dggsLinestring.get(cell));
        }
    }

    /**
     * @return A list of the cells making up the linestring
     */
    public List<DggsCell> getLinestringCellIds() {
        return linestringCellIds;
    }
}
