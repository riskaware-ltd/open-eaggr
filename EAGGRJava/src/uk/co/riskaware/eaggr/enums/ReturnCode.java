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
 * Return values of the DGGS library functions
 * 
 * @author i_bush
 *
 */
public enum ReturnCode {
    DGGS_SUCCESS, DGGS_NOT_IMPLEMENTED, DGGS_INVALID_HANDLE, DGGS_INVALID_PARAM, DGGS_NULL_POINTER, DGGS_MODEL_ERROR, DGGS_CELL_LENGTH_TOO_LONG, DGGS_MEMORY_ALLOCATION_FAILURE, DGGS_UNKNOWN_ERROR;

    // Store the values array so it doesn't need to be generated every time
    private static ReturnCode[] codes = ReturnCode.values();

    /**
     * Converts the integer returned by the native functions to a return code value
     * 
     * @param nativeCode
     *            The value returned by the native function
     * @return The corresponding ReturnCode value
     * @throws EaggrException
     *             If there is no ReturnCode that matches the supplied value
     */
    public static ReturnCode fromNativeCode(final int nativeCode) throws EaggrException {
        if (nativeCode >= codes.length) {
            throw new EaggrException("Invalid native return code " + nativeCode + ".");
        }

        return codes[nativeCode];
    }
}
