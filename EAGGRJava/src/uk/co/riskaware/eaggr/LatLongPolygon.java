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
 * Class to represent a Lat/Long polygon
 * 
 * @author i_bush
 *
 */
public class LatLongPolygon {
    private final LatLongLinestring outerRing;
    private final ArrayList<LatLongLinestring> innerRings = new ArrayList<LatLongLinestring>();

    /**
     * Constructor
     * 
     * @param outerRing
     *            The linestring representing the outer ring of the polygon
     */
    public LatLongPolygon(final LatLongLinestring outerRing) {
        this.outerRing = outerRing;
    }

    /**
     * Adds an inner ring to the polygon
     * 
     * @param innerRing
     *            The linestring representing the inner ring to add
     */
    public void addInnerRing(final LatLongLinestring innerRing) {
        innerRings.add(innerRing);
    }

    /**
     * @return The outer ring of the polygon
     */
    public LatLongLinestring getOuterRing() {
        return outerRing;
    }

    /**
     * @return A list of the inner rings of the polygon
     */
    public List<LatLongLinestring> getInnerRings() {
        return innerRings;
    }

    /**
     * Package internal conversion to a native polygon
     * 
     * @param nativePolygon
     *            The native polygon object that will be populated with this polygon's data
     */
    void toNativeLatLongPolygon(final DggsNativeLatLongTypes.LatLongPolygon nativePolygon) {
        outerRing.toNativeLatLongLinestring(nativePolygon.outerRing);

        nativePolygon.numberOfInnerRings = (short) innerRings.size();

        if (innerRings.size() > 0) {
            final DggsNativeLatLongTypes.LatLongLinestring[] nativeInnerRings = DggsNativeLatLongTypes.LatLongLinestring
                    .createlatLongPointArray(innerRings.size());

            for (int innerRing = 0; innerRing < innerRings.size(); ++innerRing) {
                innerRings.get(innerRing).toNativeLatLongLinestring(nativeInnerRings[innerRing]);
            }

            nativePolygon.innerRings = DggsNativeLatLongTypes.LatLongLinestring
                    .createPointerToLatLongLinestringArray(nativeInnerRings);
        }
    }
}
