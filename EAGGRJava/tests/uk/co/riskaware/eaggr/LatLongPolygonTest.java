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

public class LatLongPolygonTest {

    @Test
    public void constructorTest() {
        final LatLongLinestring linestring = new LatLongLinestring();

        final LatLongPoint point1 = new LatLongPoint(1.234, 2.345, 3.456);
        final LatLongPoint point2 = new LatLongPoint(4.567, 5.678, 6.789);

        linestring.addPoint(point1);
        linestring.addPoint(point2);

        final LatLongPolygon polygon = new LatLongPolygon(linestring);

        assertEquals(linestring, polygon.getOuterRing());
        assertEquals(0, polygon.getInnerRings().size());
    }

    @Test
    public void addInnerRingTest() {
        final LatLongLinestring linestring = new LatLongLinestring();

        final LatLongPoint point1 = new LatLongPoint(1.234, 2.345, 3.456);
        final LatLongPoint point2 = new LatLongPoint(4.567, 5.678, 6.789);

        linestring.addPoint(point1);
        linestring.addPoint(point2);

        final LatLongPolygon polygon = new LatLongPolygon(linestring);

        final LatLongLinestring innerLinestring = new LatLongLinestring();

        final LatLongPoint point3 = new LatLongPoint(4.321, 5.432, 6.543);
        final LatLongPoint point4 = new LatLongPoint(7.654, 8.765, 9.876);

        innerLinestring.addPoint(point3);
        innerLinestring.addPoint(point4);

        polygon.addInnerRing(innerLinestring);

        assertEquals(linestring, polygon.getOuterRing());
        assertEquals(1, polygon.getInnerRings().size());
        assertEquals(innerLinestring, polygon.getInnerRings().get(0));
    }

    @Test
    public void toNativeTest() {
        final LatLongLinestring linestring = new LatLongLinestring();

        final LatLongPoint point1 = new LatLongPoint(1.234, 2.345, 3.456);
        final LatLongPoint point2 = new LatLongPoint(4.567, 5.678, 6.789);

        linestring.addPoint(point1);
        linestring.addPoint(point2);

        final LatLongPolygon polygon = new LatLongPolygon(linestring);

        final LatLongLinestring innerLinestring = new LatLongLinestring();

        final LatLongPoint point3 = new LatLongPoint(4.321, 5.432, 6.543);
        final LatLongPoint point4 = new LatLongPoint(7.654, 8.765, 9.876);

        innerLinestring.addPoint(point3);
        innerLinestring.addPoint(point4);

        polygon.addInnerRing(innerLinestring);

        final DggsNativeLatLongTypes.LatLongPolygon nativePolygon = new DggsNativeLatLongTypes.LatLongPolygon();
        polygon.toNativeLatLongPolygon(nativePolygon);

        assertEquals(2, nativePolygon.outerRing.numberOfPoints);
        assertNotEquals(Pointer.NULL, nativePolygon.outerRing.points);
        assertNotEquals(Pointer.NULL, nativePolygon.innerRings);
    }
}
