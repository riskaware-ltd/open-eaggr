/*
 * Copyright (c) Riskaware 2016
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
 * Class containing the shapes required for unit testing the compareShapes method on the DGGS API, for the 
 * ISEA3H DGGS model
 * 
 * @author s_jenkins
 *
 */

import java.util.ArrayList;

import uk.co.riskaware.eaggr.DggsCell;
import uk.co.riskaware.eaggr.DggsLinestring;
import uk.co.riskaware.eaggr.DggsPolygon;
import uk.co.riskaware.eaggr.DggsShape;

public final class DggsTestDataHexagonal {

    // Define the base shapes for spatial analysis testing on a hexagonal grid

    // Define variables for creating a DGGS shape from a cell
    public static final DggsCell DGGS_CELL;
    public static final DggsShape DGGS_SHAPE_CELL;

    // Define variables for creating a DGGS shape from a linestring
    public static final ArrayList<DggsCell> LINESTRING_CELL_IDS;
    public static final DggsCell LINESTRING_DGGS_CELL_1;
    public static final DggsCell LINESTRING_DGGS_CELL_2;
    public static final DggsCell LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring DGGS_LINESTRING;
    public static final DggsShape DGGS_SHAPE_LINESTRING;

    // Define variables for creating an outer ring for a polygon
    public static final ArrayList<DggsCell> OUTER_LINESTRING_CELL_IDS;
    public static final DggsCell OUTER_LINESTRING_CELL_1;
    public static final DggsCell OUTER_LINESTRING_CELL_2;
    public static final DggsCell OUTER_LINESTRING_CELL_3;
    public static final DggsCell OUTER_LINESTRING_CELL_4;
    public static final DggsCell OUTER_LINESTRING_CELL_5;
    public static final DggsLinestring OUTER_RING_LINESTRING;

    // Define variables for creating a non-empty inner ring for a polygon
    public static final ArrayList<DggsLinestring> INNER_RING_LINESTRING_ARRAY;
    public static final ArrayList<DggsCell> INNER_RING_LINESTRING_CELL_IDS;
    public static final DggsCell INNER_RING_LINESTRING_DGGS_CELL_1;
    public static final DggsCell INNER_RING_LINESTRING_DGGS_CELL_2;
    public static final DggsCell INNER_RING_LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring INNER_RING_LINESTRING;

    // Define variables for creating a DGGS shape from a polygon
    public static final DggsPolygon DGGS_POLYGON;
    public static final DggsShape DGGS_SHAPE_POLYGON;

    // Define variables for creating non-empty inner rings for a polygon
    public static final ArrayList<DggsLinestring> EMPTY_INNER_RING_LINESTRING_ARRAY;

    // Define the comparison shapes for spatial analysis testing on a hexagonal grid

    // Define comparison DGGS shapes from cells
    public static final DggsCell OVERLAPPING_CHILD_DGGS_CELL;
    public static final DggsShape OVERLAPPING_CHILD_DGGS_SHAPE_CELL;
    public static final DggsCell DISJOINT_DGGS_CELL;
    public static final DggsShape DISJOINT_DGGS_SHAPE_CELL;

    // Define comparison DGGS shapes from linestrings
    public static final ArrayList<DggsCell> DISJOINT_LINESTRING_CELL_IDS;
    public static final DggsCell DISJOINT_LINESTRING_DGGS_CELL_1;
    public static final DggsCell DISJOINT_LINESTRING_DGGS_CELL_2;
    public static final DggsCell DISJOINT_LINESTRING_DGGS_CELL_3;
    public static final DggsCell DISJOINT_LINESTRING_DGGS_CELL_4;
    public static final DggsLinestring DISJOINT_DGGS_LINESTRING;
    public static final DggsShape DISJOINT_DGGS_SHAPE_LINESTRING;

    // Define comparison DGGS shapes from polygons
    public static final ArrayList<DggsCell> DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS;
    public static final DggsCell DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_1;
    public static final DggsCell DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_2;
    public static final DggsCell DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring DISJOINT_POLYGON_OUTER_RING_LINESTRING;
    public static final DggsPolygon DISJOINT_DGGS_POLYGON;
    public static final DggsShape DISJOINT_DGGS_SHAPE_POLYGON;

    public static final ArrayList<DggsCell> OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS;
    public static final DggsCell OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_1;
    public static final DggsCell OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_2;
    public static final DggsCell OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_3;
    public static final DggsCell OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_4;
    public static final DggsCell OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_5;
    public static final DggsCell OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_6;
    public static final DggsLinestring OVERLAPPING_POLYGON_OUTER_RING_LINESTRING;
    public static final DggsPolygon OVERLAPPING_DGGS_POLYGON;
    public static final DggsShape OVERLAPPING_DGGS_SHAPE_POLYGON;

    static {

        // Initialise a cell, linestring and polygon for spatial analysis testing on a hexagonal grid

        // Initialise a DGGS shape from a cell
        DGGS_CELL = new DggsCell("00021,1");
        DGGS_SHAPE_CELL = new DggsShape(DGGS_CELL);

        // Initialise a DGGS shape from a linestring
        LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        LINESTRING_DGGS_CELL_1 = new DggsCell("00021,2");
        LINESTRING_DGGS_CELL_2 = new DggsCell("00023,1");
        LINESTRING_DGGS_CELL_3 = new DggsCell("00023,3");
        LINESTRING_CELL_IDS.add(LINESTRING_DGGS_CELL_1);
        LINESTRING_CELL_IDS.add(LINESTRING_DGGS_CELL_2);
        LINESTRING_CELL_IDS.add(LINESTRING_DGGS_CELL_3);
        DGGS_LINESTRING = new DggsLinestring(LINESTRING_CELL_IDS);
        DGGS_SHAPE_LINESTRING = new DggsShape(DGGS_LINESTRING);

        // Initialise an outer ring for a polygon
        OUTER_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        OUTER_LINESTRING_CELL_1 = new DggsCell("00020,2");
        OUTER_LINESTRING_CELL_2 = new DggsCell("00024,0");
        OUTER_LINESTRING_CELL_3 = new DggsCell("00024,4");
        OUTER_LINESTRING_CELL_4 = new DggsCell("00023,4");
        OUTER_LINESTRING_CELL_5 = new DggsCell("00020,3");
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_1);
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_2);
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_3);
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_4);
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_5);
        OUTER_RING_LINESTRING = new DggsLinestring(OUTER_LINESTRING_CELL_IDS);

        // Initialise a non-empty inner ring for a polygon
        INNER_RING_LINESTRING_ARRAY = new ArrayList<DggsLinestring>();
        INNER_RING_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        INNER_RING_LINESTRING_DGGS_CELL_1 = new DggsCell("00021,2");
        INNER_RING_LINESTRING_DGGS_CELL_2 = new DggsCell("00023,1");
        INNER_RING_LINESTRING_DGGS_CELL_3 = new DggsCell("00023,3");
        INNER_RING_LINESTRING_CELL_IDS.add(INNER_RING_LINESTRING_DGGS_CELL_1);
        INNER_RING_LINESTRING_CELL_IDS.add(INNER_RING_LINESTRING_DGGS_CELL_2);
        INNER_RING_LINESTRING_CELL_IDS.add(INNER_RING_LINESTRING_DGGS_CELL_3);
        INNER_RING_LINESTRING = new DggsLinestring(INNER_RING_LINESTRING_CELL_IDS);
        INNER_RING_LINESTRING_ARRAY.add(INNER_RING_LINESTRING);

        // Initialise a DGGS shape from a polygon with an inner ring
        DGGS_POLYGON = new DggsPolygon(OUTER_RING_LINESTRING, INNER_RING_LINESTRING_ARRAY);
        DGGS_SHAPE_POLYGON = new DggsShape(DGGS_POLYGON);

        // Initialise empty inner rings
        EMPTY_INNER_RING_LINESTRING_ARRAY = new ArrayList<DggsLinestring>();

        // Initialise the comparison shapes for testing spatial analysis on a hexagonal grid

        // Initialise DGGS shapes from cells

        // Construct a DGGS shape from a cell, that is an overlapping child cell when compared to the base cell
        OVERLAPPING_CHILD_DGGS_CELL = new DggsCell("00031,4");
        OVERLAPPING_CHILD_DGGS_SHAPE_CELL = new DggsShape(OVERLAPPING_CHILD_DGGS_CELL);

        // Construct a DGGS shape from a cell, that is disjoint from the base cell
        DISJOINT_DGGS_CELL = new DggsCell("00023,1");
        DISJOINT_DGGS_SHAPE_CELL = new DggsShape(DISJOINT_DGGS_CELL);

        // Initialise DGGS shapes from linestrings

        // Initialise a DGGS shape from a linestring that is disjoint from the base linestring
        DISJOINT_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        DISJOINT_LINESTRING_DGGS_CELL_1 = new DggsCell("00020,2");
        DISJOINT_LINESTRING_DGGS_CELL_2 = new DggsCell("00023,0");
        DISJOINT_LINESTRING_DGGS_CELL_3 = new DggsCell("00024,1");
        DISJOINT_LINESTRING_DGGS_CELL_4 = new DggsCell("00024,4");
        DISJOINT_LINESTRING_CELL_IDS.add(DISJOINT_LINESTRING_DGGS_CELL_1);
        DISJOINT_LINESTRING_CELL_IDS.add(DISJOINT_LINESTRING_DGGS_CELL_2);
        DISJOINT_LINESTRING_CELL_IDS.add(DISJOINT_LINESTRING_DGGS_CELL_3);
        DISJOINT_LINESTRING_CELL_IDS.add(DISJOINT_LINESTRING_DGGS_CELL_4);
        DISJOINT_DGGS_LINESTRING = new DggsLinestring(DISJOINT_LINESTRING_CELL_IDS);
        DISJOINT_DGGS_SHAPE_LINESTRING = new DggsShape(DISJOINT_DGGS_LINESTRING);

        // Initialise a DGGS shape from a polygon with no inner rings, that is disjoint from the base polygon
        DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_1 = new DggsCell("00020,0");
        DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_2 = new DggsCell("00020,1");
        DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_3 = new DggsCell("00021,0");
        DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS.add(DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_1);
        DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS.add(DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_2);
        DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS.add(DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_3);
        DISJOINT_POLYGON_OUTER_RING_LINESTRING = new DggsLinestring(DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS);
        DISJOINT_DGGS_POLYGON = new DggsPolygon(DISJOINT_POLYGON_OUTER_RING_LINESTRING,
                EMPTY_INNER_RING_LINESTRING_ARRAY);
        DISJOINT_DGGS_SHAPE_POLYGON = new DggsShape(DISJOINT_DGGS_POLYGON);

        // Initialise a DGGS shape from a polygon with no inner rings, that overlaps the base polygon
        OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_1 = new DggsCell("00020,2");
        OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_2 = new DggsCell("00023,0");
        OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_3 = new DggsCell("00024,1");
        OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_4 = new DggsCell("00024,4");
        OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_5 = new DggsCell("00023,4");
        OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_6 = new DggsCell("00020,3");
        OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS.add(OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_1);
        OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS.add(OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_2);
        OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS.add(OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_3);
        OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS.add(OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_4);
        OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS.add(OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_5);
        OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS.add(OVERLAPPING_POLYGON_OUTER_LINESTRING_DGGS_CELL_6);
        OVERLAPPING_POLYGON_OUTER_RING_LINESTRING = new DggsLinestring(OVERLAPPING_POLYGON_OUTER_LINESTRING_CELL_IDS);

        OVERLAPPING_DGGS_POLYGON = new DggsPolygon(OVERLAPPING_POLYGON_OUTER_RING_LINESTRING,
                EMPTY_INNER_RING_LINESTRING_ARRAY);
        OVERLAPPING_DGGS_SHAPE_POLYGON = new DggsShape(OVERLAPPING_DGGS_POLYGON);
    }
}
