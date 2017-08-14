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

import java.util.Arrays;
import java.util.List;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.Union;

/**
 * Class containing all of the native lat/Long JNA structures.
 * 
 * @author i_bush
 *
 */
final class DggsNativeLatLongTypes {

    /**
     * Private constructor
     */
    private DggsNativeLatLongTypes() {
    }

    /**
     * Represents the DGGS_LatLongShape structure of the DGGS C API
     * 
     * @author i_bush
     *
     */
    public static class LatLongShape extends Structure {

        /**
         * The lat/long shape type. This corresponds to the LatLongShapeType enum
         */
        public int latLongShapeType;

        /**
         * The lat/long shape data. The type of data in this union can be determined from the latLongShapeType field
         */
        public LatLongShapeData latLongShapeData;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("latLongShapeType", "latLongShapeData");
        }

        /**
         * Creates an array of LatLongShape objects in contiguous memory
         * 
         * @param arraySize
         *            The required size of the array
         * @return The newly created array
         */
        public static LatLongShape[] createlatLongShapeArray(final int arraySize) {
            return (LatLongShape[]) new LatLongShape().toArray(arraySize);
        }
    }

    /**
     * Represents the DGGS_LatLongShapeData union of the DGGS C API
     * 
     * Only one of the fields will be set with data depending on the type of shape represented
     * 
     * @author i_bush
     *
     */
    public static class LatLongShapeData extends Union {

        /**
         * The lat/long point data
         */
        public LatLongPoint point;

        /**
         * The lat/long linestring data
         */
        public LatLongLinestring linestring;

        /**
         * The lat/long polygon data
         */
        public LatLongPolygon polygon;
    }

    /**
     * Represents the DGGS_LatLongPoint structure of the DGGS C API
     * 
     * @author i_bush
     *
     */
    public static class LatLongPoint extends Structure {

        /**
         * The latitude of the point in WGS84 coordinates
         */
        public double latitude;

        /**
         * The longitude of the point in WGS84 coordinates
         */
        public double longitude;

        /**
         * The accuracy of the point in metres squared
         */
        public double accuracy;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("latitude", "longitude", "accuracy");
        }

        /**
         * Creates an array of LatLongPoint objects in contiguous memory
         * 
         * @param arraySize
         *            The required size of the array
         * @return The newly created array
         */
        public static LatLongPoint[] createlatLongPointArray(final int arraySize) {
            return (LatLongPoint[]) new LatLongPoint().toArray(arraySize);
        }

        /**
         * Creates a pointer to an array of LatLongPoint objects
         * 
         * @param points
         *            The points to add to the pointer
         * @return The pointer to the array
         */
        public static Pointer createPointerToLatLongPointArray(final LatLongPoint[] points) {
            final Pointer pointsPointer = points[0].getPointer();

            for (int point = 0; point < points.length; ++point) {
                points[point].write();
            }

            return pointsPointer;
        }
    }

    /**
     * Represents the DGGS_LatLongLinestring structure of the DGGS C API
     * 
     * @author i_bush
     *
     */
    public static class LatLongLinestring extends Structure {

        /**
         * Pointer to an array of LatLongPoint objects that make up this linestring
         */
        public Pointer points;

        /**
         * The number of points making up this linestring
         */
        public short numberOfPoints;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("points", "numberOfPoints");
        }

        /**
         * Creates an array of LatLongLinestring objects in contiguous memory
         * 
         * @param arraySize
         *            The required size of the array
         * @return The newly created array
         */
        public static LatLongLinestring[] createlatLongPointArray(final int arraySize) {
            return (LatLongLinestring[]) new LatLongLinestring().toArray(arraySize);
        }

        /**
         * Creates a pointer to an array of LatLongLinestring objects
         * 
         * @param linestrings
         *            The linestrings to add to the pointer
         * @return The pointer to the array
         */
        public static Pointer createPointerToLatLongLinestringArray(final LatLongLinestring[] linestrings) {
            final Pointer pointsPointer = linestrings[0].getPointer();

            for (int point = 0; point < linestrings.length; ++point) {
                linestrings[point].write();
            }

            return pointsPointer;
        }
    }

    /**
     * Represents the DGGS_LatLongPolygon structure of the DGGS C API
     * 
     * @author i_bush
     *
     */
    public static class LatLongPolygon extends Structure {

        /**
         * Linestring that represents the outer ring of the polygon
         */
        public LatLongLinestring outerRing;

        /**
         * Pointer to an array of LatLongLinestring objects that represent the inner rings of the polygon
         */
        public Pointer innerRings;

        /**
         * The number of inner rings of the polygon
         */
        public short numberOfInnerRings;

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("outerRing", "innerRings", "numberOfInnerRings");
        }
    }
}
