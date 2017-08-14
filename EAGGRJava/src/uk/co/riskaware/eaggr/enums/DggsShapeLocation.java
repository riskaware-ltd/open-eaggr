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
 * The types of shape location returned by the DGGS library
 * 
 * Currently point data will have one of ONE_FACE, TWO_FACES or MANY_FACES. Linestrings and polygons will always use
 * NO_LOCATION
 * 
 * @author i_bush
 *
 */
public enum DggsShapeLocation {
    ONE_FACE, TWO_FACES, MANY_FACES, NO_LOCATION;

    // Store the values array so it doesn't need to be generated every time
    private static DggsShapeLocation[] codes = DggsShapeLocation.values();

    /**
     * Converts the integer representing the shape location in native code to an enum value
     * 
     * @param nativeCode
     *            The value representing the shape location in native code
     * @return The corresponding DggsShapeLocation value
     * @throws EaggrException
     *             If there is no DggsShapeLocation that matches the supplied value
     */
    public static DggsShapeLocation fromNativeCode(final int nativeCode) throws EaggrException {
        if (nativeCode >= codes.length) {
            throw new EaggrException("Invalid native DGGS location type  " + nativeCode + ".");
        }

        return codes[nativeCode];
    }
}
