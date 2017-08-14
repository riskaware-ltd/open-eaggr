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

import org.junit.Test;

public class LatLongPointTest {

    @Test
    public void constructorTest() {
        final double latitude = 1.234;
        final double longitude = 2.345;
        final double accuracy = 3.456;

        final double tolerance = 1E-10;

        final LatLongPoint point = new LatLongPoint(latitude, longitude, accuracy);

        assertEquals(latitude, point.getLatitude(), tolerance);
        assertEquals(longitude, point.getLongitude(), tolerance);
        assertEquals(accuracy, point.getAccuracy(), tolerance);
    }

    @Test
    public void toNativeTest() {
        final double latitude = 1.234;
        final double longitude = 2.345;
        final double accuracy = 3.456;

        final double tolerance = 1E-10;

        final LatLongPoint point = new LatLongPoint(latitude, longitude, accuracy);

        final DggsNativeLatLongTypes.LatLongPoint nativePoint = new DggsNativeLatLongTypes.LatLongPoint();
        point.toNativeLatLongPoint(nativePoint);

        assertEquals(latitude, nativePoint.latitude, tolerance);
        assertEquals(longitude, nativePoint.longitude, tolerance);
        assertEquals(accuracy, nativePoint.accuracy, tolerance);
    }
}
