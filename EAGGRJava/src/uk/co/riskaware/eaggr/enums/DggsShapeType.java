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

package uk.co.riskaware.eaggr.enums;

import uk.co.riskaware.eaggr.EaggrException;

/**
 * The types of DGGS shape supported by the DGGS library
 * 
 * @author i_bush
 *
 */
public enum DggsShapeType {
    CELL, LINESTRING, POLYGON;

    // Store the values array so it doesn't need to be generated every time
    private static DggsShapeType[] codes = DggsShapeType.values();

    /**
     * Converts the integer representing the shape type in native code to an enum value
     * 
     * @param nativeCode
     *            The value representing the shape type in native code
     * @return The corresponding DggsShapeType value
     * @throws EaggrException
     *             If there is no DggsShapeType that matches the supplied value
     */
    public static DggsShapeType fromNativeCode(final int nativeCode) throws EaggrException {
        if (nativeCode >= codes.length) {
            throw new EaggrException("Invalid native DGGS shape type  " + nativeCode + ".");
        }

        return codes[nativeCode];
    }
}
