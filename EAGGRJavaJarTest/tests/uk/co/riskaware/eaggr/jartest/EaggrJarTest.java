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

package uk.co.riskaware.eaggr.jartest;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import uk.co.riskaware.eaggr.Eaggr;
import uk.co.riskaware.eaggr.EaggrException;
import uk.co.riskaware.eaggr.EaggrLibraryException;
import uk.co.riskaware.eaggr.enums.DggsModel;

public class EaggrJarTest {

    @Test
    public void getVersionTest() throws EaggrException, EaggrLibraryException {
        Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        String version = dggs.getVersion();

        assertEquals("v2.0", version);
    }
}
