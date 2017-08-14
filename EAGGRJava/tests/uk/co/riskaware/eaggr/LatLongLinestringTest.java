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
import static org.junit.Assert.assertNotEquals;

import org.junit.Test;

import com.sun.jna.Pointer;

public class LatLongLinestringTest {
    @Test
    public void addPointsTest() {
        final LatLongLinestring linestring = new LatLongLinestring();

        assertEquals(0, linestring.getPoints().size());

        final LatLongPoint point1 = new LatLongPoint(1.234, 2.345, 3.456);
        final LatLongPoint point2 = new LatLongPoint(4.567, 5.678, 6.789);

        linestring.addPoint(point1);
        linestring.addPoint(point2);

        assertEquals(2, linestring.getPoints().size());

        assertEquals(point1, linestring.getPoints().get(0));
        assertEquals(point2, linestring.getPoints().get(1));
    }

    @Test
    public void toNativeTest() {
        final LatLongLinestring linestring = new LatLongLinestring();

        assertEquals(0, linestring.getPoints().size());

        final LatLongPoint point1 = new LatLongPoint(1.234, 2.345, 3.456);
        final LatLongPoint point2 = new LatLongPoint(4.567, 5.678, 6.789);

        linestring.addPoint(point1);
        linestring.addPoint(point2);

        final DggsNativeLatLongTypes.LatLongLinestring nativeLinestring = new DggsNativeLatLongTypes.LatLongLinestring();
        linestring.toNativeLatLongLinestring(nativeLinestring);

        assertEquals(2, nativeLinestring.numberOfPoints);
        assertNotEquals(Pointer.NULL, nativeLinestring.points);
    }
}
