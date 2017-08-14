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
 * Class containing all of the native DGGS JNA structures.
 * 
 * @author i_bush
 *
 */
final class DggsNativeDggsTypes {

    /**
     * The maximum length of a DGGS cell. DGGS cell objects are represented by a char array of this length on the C API
     */
    public static final int MAX_CELL_STRING_LENGTH = 43;

    /**
     * Private constructor
     */
    private DggsNativeDggsTypes() {
    }

    /**
     * Represents the DGGS_Shape structure of the DGGS C API
     * 
     * @author i_bush
     *
     */
    public static class DggsShape extends Structure {

        /**
         * Constructor
         * 
         * To create an array of DggsShape objects use the createDggsShapeArray method
         */
        public DggsShape() {
            super();
        }

        /**
         * Constructor using a pointer object
         * 
         * @param p
         *            The pointer object containing the DggsShape
         */
        public DggsShape(final Pointer p) {
            super(p);
            read();
        }

        /**
         * The DGGS shape type. This corresponds to a value in the DggsShapeType enum
         */
        public int shapeType;

        /**
         * The DGGS shape data. The type of data in this union can be determined from the shapeType field
         */
        public DggsShapeData shapeData;

        /**
         * The DGGS shape location. This corresponds to a value in the DggsShapeLocation enum
         */
        public int shapeLocation;

        @Override
        public void read() {
            super.read();

            switch (shapeType) {
                case 0:
                    shapeData.setType(byte[].class);
                    break;
                case 1:
                    shapeData.setType(DggsLinestring.class);
                    break;
                case 2:
                    shapeData.setType(DggsPolygon.class);
                    break;
            }

            shapeData.read();
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("shapeType", "shapeData", "shapeLocation");
        }

        /**
         * Creates an array of DggsShape objects in contiguous memory
         * 
         * @param arraySize
         *            The required size of the array
         * @return The newly created array
         */
        public static DggsShape[] createDggsShapeArray(final int arraySize) {
            return (DggsShape[]) new DggsShape().toArray(arraySize);
        }
    }

    /**
     * Represents the DGGS_ShapeData union of the DGGS C API
     * 
     * Only one of the fields will be set with data depending on the type of shape represented
     * 
     * @author i_bush
     *
     */
    public static class DggsShapeData extends Union {

        /**
         * The DGGS cell id string encoded as a byte array
         */
        public byte[] cell = new byte[MAX_CELL_STRING_LENGTH];

        /**
         * The DGGS linestring data
         */
        public DggsLinestring linestring;

        /**
         * The DGGS polygon data
         */
        public DggsPolygon polygon;

        /**
         * Sets the DGGS cell Id byte array from a string
         * 
         * @param cellIdString
         *            The DGGS cell Id string
         */
        public void setCellId(final String cellIdString) {
            cell = new byte[MAX_CELL_STRING_LENGTH];

            final byte[] stringInBytes = cellIdString.getBytes();

            for (int i = 0; i < stringInBytes.length; ++i) {
                cell[i] = stringInBytes[i];
            }
        }
    }

    /**
     * Represents the DGGS_Linestring structure of the DGGS C API
     * 
     * @author i_bush
     *
     */
    public static class DggsLinestring extends Structure {

        /**
         * Pointer to the byte array that contain the cells id that make up the linestring
         */
        public Pointer cells;

        /**
         * The number of cells in the linestring
         */
        public short numberOfcells;

        /**
         * Constructor
         */
        public DggsLinestring() {
            super();
        }

        /**
         * Constructor using a pointer object
         * 
         * @param p
         *            The pointer object containing the DggsLinestring
         */
        public DggsLinestring(final Pointer p) {
            super(p);
            read();
        }

        /**
         * Creates an array of DggsLinestring objects in contiguous memory
         * 
         * @param arraySize
         *            The required size of the array
         * @return The newly created array
         */
        public static DggsLinestring[] createDggsLinestringArray(final int arraySize) {
            return (DggsLinestring[]) new DggsLinestring().toArray(arraySize);
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cells", "numberOfcells");
        }
    }

    /**
     * Represents the DGGS_Polygon structure of the DGGS C API
     * 
     * @author i_bush
     *
     */
    public static class DggsPolygon extends Structure {

        /**
         * Linestring that represents the outer ring of the polygon
         */
        public DggsLinestring outerRing;

        /**
         * Pointer to an array of DggsLinestring objects that represent the inner rings of the polygon
         */
        public Pointer innerRings;

        /**
         * The number of inner rings of the polygon
         */
        public short numberOfInnerRings;

        /**
         * Creates a pointer to an array of DggsLinestring objects
         * 
         * @param linestrings
         *            The linestrings to add to the pointer
         * @return The pointer to the array
         */
        public static Pointer createPointerToDggsLinestringArray(final DggsLinestring[] linestrings) {
            final Pointer linestringsPointer = linestrings[0].getPointer();

            for (int index = 0; index < linestrings.length; ++index) {
                linestrings[index].write();
            }

            return linestringsPointer;
        }

        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("outerRing", "innerRings", "numberOfInnerRings");
        }
    }
}
