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
 * Class to represent a DGGS polygon
 * 
 * @author i_bush
 *
 */
public class DggsPolygon {

    private final DggsLinestring outerRing;
    private final ArrayList<DggsLinestring> innerRings = new ArrayList<DggsLinestring>();

    /**
     * Package internal constructor - not required to be constructed by client code
     * 
     * @param polygon
     *            The native polygon object
     */
    DggsPolygon(final DggsNativeDggsTypes.DggsPolygon polygon) {
        outerRing = new DggsLinestring(polygon.outerRing);

        final DggsNativeDggsTypes.DggsLinestring firstInnerRing = new DggsNativeDggsTypes.DggsLinestring(
                polygon.innerRings);

        if (polygon.numberOfInnerRings > 0) {
            final DggsNativeDggsTypes.DggsLinestring[] innerRingArray = (DggsNativeDggsTypes.DggsLinestring[]) firstInnerRing
                    .toArray(polygon.numberOfInnerRings);

            for (final DggsNativeDggsTypes.DggsLinestring innerRing : innerRingArray) {
                innerRings.add(new DggsLinestring(innerRing));
            }
        }
    }

    /**
     * Package internal constructor - not required to be constructed by client code
     * 
     * @param outerLinestring
     *            A DGGS linestring used for the outer ring
     * @param innerLinestrings
     *            An array of DGGS linestrings used for the inner rings
     */
    public DggsPolygon(final DggsLinestring outerLinestring, final ArrayList<DggsLinestring> innerLinestrings) {
        this.outerRing = outerLinestring;

        this.innerRings.addAll(innerLinestrings);
    }

    /**
     * @return The outer ring of the polygon
     */
    public DggsLinestring getOuterRing() {
        return outerRing;
    }

    /**
     * @return A list of the inner rings of the polygon
     */
    public List<DggsLinestring> getInnerRings() {
        return innerRings;
    }
}
