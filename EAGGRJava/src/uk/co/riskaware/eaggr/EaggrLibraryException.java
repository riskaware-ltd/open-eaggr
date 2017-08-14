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

import uk.co.riskaware.eaggr.enums.ReturnCode;

/**
 * Exceptions thrown by the EAGGR Java API
 * 
 * @author i_bush
 *
 */
public class EaggrLibraryException extends Exception {

    private static final long serialVersionUID = 1L;

    private ReturnCode returnCode;

    /**
     * Constructor
     * 
     * @param returnCode
     *            The return code of the function throwing the exception
     * @param message
     *            The error message
     */
    public EaggrLibraryException(final ReturnCode returnCode, final String message) {
        super(message);
    }

    /**
     * @return The return code that caused the exception
     */
    public ReturnCode getReturnCode() {
        return returnCode;
    }
}
