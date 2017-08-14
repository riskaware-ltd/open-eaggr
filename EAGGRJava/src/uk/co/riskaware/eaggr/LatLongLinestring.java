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
 * Class to represent a Lat/Long linestring
 * 
 * @author i_bush
 *
 */
public class LatLongLinestring {
    private final ArrayList<LatLongPoint> points = new ArrayList<LatLongPoint>();

    /**
     * @return A list of the points making up the linestring
     */
    public List<LatLongPoint> getPoints() {
        return points;
    }

    /**
     * Adds a point to the linestring
     * 
     * @param point
     *            The point to add to the linestring
     */
    public void addPoint(final LatLongPoint point) {
        points.add(point);
    }

    /**
     * Package internal conversion to a native linestring
     * 
     * @param nativeLinestring
     *            The native linestring object to be populated with this linestring's data
     */
    void toNativeLatLongLinestring(final DggsNativeLatLongTypes.LatLongLinestring nativeLinestring) {
        nativeLinestring.numberOfPoints = (short) points.size();

        final DggsNativeLatLongTypes.LatLongPoint[] linestringPoints = DggsNativeLatLongTypes.LatLongPoint
                .createlatLongPointArray(points.size());

        // Convert each of the points in the linestring
        for (int point = 0; point < points.size(); ++point) {
            points.get(point).toNativeLatLongPoint(linestringPoints[point]);
        }

        nativeLinestring.points = DggsNativeLatLongTypes.LatLongPoint
                .createPointerToLatLongPointArray(linestringPoints);
    }
}
