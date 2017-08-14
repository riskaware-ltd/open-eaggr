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

package uk.co.riskaware.eaggr.testutilities;

/**
 * Class containing utility methods used in unit tests
 * 
 * @author i_bush
 *
 */
public final class TestUtilities {

    /**
     * Private constructor
     */
    private TestUtilities() {
    }

    /**
     * Removes end of line characters from a supplied string
     * 
     * @param str
     *            the string to strip end of line characters from
     * @return the updated string
     */
    public static String removeEndOfLineCharacters(String str) {
        return str.replace("\r", "").replace("\n", "");
    }
}
