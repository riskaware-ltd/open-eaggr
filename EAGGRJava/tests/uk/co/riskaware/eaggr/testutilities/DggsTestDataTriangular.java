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
 * ISEA4T DGGS model
 * 
 * @author s_jenkins
 *
 */

import java.util.ArrayList;

import uk.co.riskaware.eaggr.DggsCell;
import uk.co.riskaware.eaggr.DggsLinestring;
import uk.co.riskaware.eaggr.DggsPolygon;
import uk.co.riskaware.eaggr.DggsShape;

public final class DggsTestDataTriangular {

    // Define the base shapes for spatial analysis testing on a triangular grid

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

    // Define variables for an empty ring for a polygon
    public static final ArrayList<DggsLinestring> EMPTY_INNER_RING_LINESTRING_ARRAY;

    // Define the comparison shapes for spatial analysis testing on a triangular grid

    // Define comparison DGGS shapes from cells
    public static final DggsCell CHILD_DGGS_CELL;
    public static final DggsShape CHILD_DGGS_SHAPE_CELL;
    public static final DggsCell DISJOINT_DGGS_CELL;
    public static final DggsShape DISJOINT_DGGS_SHAPE_CELL;
    public static final DggsCell PARENT_DGGS_CELL;
    public static final DggsShape PARENT_DGGS_SHAPE_CELL;
    public static final DggsCell EDGE_SHARING_DGGS_CELL;
    public static final DggsShape EDGE_SHARING_DGGS_SHAPE_CELL;

    // Define comparison DGGS shapes from linestrings
    public static final ArrayList<DggsCell> SUBSTRING_LINESTRING_CELL_IDS;
    public static final DggsCell SUBSTRING_LINESTRING_DGGS_CELL_1;
    public static final DggsCell SUBSTRING_LINESTRING_DGGS_CELL_2;
    public static final DggsLinestring SUBSTRING_DGGS_LINESTRING;
    public static final DggsShape SUBSTRING_DGGS_SHAPE_LINESTRING;

    public static final ArrayList<DggsCell> DISJOINT_LINESTRING_CELL_IDS;
    public static final DggsCell DISJOINT_LINESTRING_DGGS_CELL_1;
    public static final DggsCell DISJOINT_LINESTRING_DGGS_CELL_2;
    public static final DggsCell DISJOINT_LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring DISJOINT_DGGS_LINESTRING;
    public static final DggsShape DISJOINT_DGGS_SHAPE_LINESTRING;

    public static final ArrayList<DggsCell> SUPERSTRING_LINESTRING_CELL_IDS;
    public static final DggsCell SUPERSTRING_LINESTRING_DGGS_CELL_1;
    public static final DggsCell SUPERSTRING_LINESTRING_DGGS_CELL_2;
    public static final DggsCell SUPERSTRING_LINESTRING_DGGS_CELL_3;
    public static final DggsCell SUPERSTRING_LINESTRING_DGGS_CELL_4;
    public static final DggsCell SUPERSTRING_LINESTRING_DGGS_CELL_5;
    public static final DggsLinestring SUPERSTRING_DGGS_LINESTRING;
    public static final DggsShape SUPERSTRING_DGGS_SHAPE_LINESTRING;

    public static final ArrayList<DggsCell> ENDS_TOUCHING_LINESTRING_CELL_IDS;
    public static final DggsCell ENDS_TOUCHING_LINESTRING_DGGS_CELL_1;
    public static final DggsCell ENDS_TOUCHING_LINESTRING_DGGS_CELL_2;
    public static final DggsCell ENDS_TOUCHING_LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring ENDS_TOUCHING_DGGS_LINESTRING;
    public static final DggsShape ENDS_TOUCHING_DGGS_SHAPE_LINESTRING;

    // Define comparison DGGS shapes from polygons
    public static final ArrayList<DggsCell> DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS;
    public static final DggsCell DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_1;
    public static final DggsCell DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_2;
    public static final DggsCell DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring DISJOINT_POLYGON_OUTER_RING_LINESTRING;
    public static final DggsPolygon DISJOINT_DGGS_POLYGON;
    public static final DggsShape DISJOINT_DGGS_SHAPE_POLYGON;

    public static final ArrayList<DggsCell> INTERIOR_POLYGON_OUTER_LINESTRING_CELL_IDS;
    public static final DggsCell INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_1;
    public static final DggsCell INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_2;
    public static final DggsCell INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring INTERIOR_POLYGON_OUTER_RING_LINESTRING;
    public static final DggsPolygon INTERIOR_DGGS_POLYGON;
    public static final DggsShape INTERIOR_DGGS_SHAPE_POLYGON;

    public static final ArrayList<DggsCell> CONTAINS_POLYGON_OUTER_LINESTRING_CELL_IDS;
    public static final DggsCell CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_1;
    public static final DggsCell CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_2;
    public static final DggsCell CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring CONTAINS_POLYGON_OUTER_RING_LINESTRING;
    public static final DggsPolygon CONTAINS_DGGS_POLYGON;
    public static final DggsShape CONTAINS_DGGS_SHAPE_POLYGON;

    public static final ArrayList<DggsCell> EDGE_SHARING_OUTER_LINESTRING_CELL_IDS;
    public static final DggsCell EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_1;
    public static final DggsCell EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_2;
    public static final DggsCell EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_3;
    public static final DggsLinestring EDGE_SHARING_OUTER_RING_LINESTRING;
    public static final DggsPolygon EDGE_SHARING_DGGS_POLYGON;
    public static final DggsShape EDGE_SHARING_DGGS_SHAPE_POLYGON;

    static {

        // Initialise the base shapes for testing spatial analysis on a triangular grid

        // Initialise a DGGS shape from a cell
        DGGS_CELL = new DggsCell("0000");
        DGGS_SHAPE_CELL = new DggsShape(DGGS_CELL);
        // Initialise a DGGS shape from a linestring
        LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        LINESTRING_DGGS_CELL_1 = new DggsCell("0001");
        LINESTRING_DGGS_CELL_2 = new DggsCell("0002");
        LINESTRING_DGGS_CELL_3 = new DggsCell("0003");
        LINESTRING_CELL_IDS.add(LINESTRING_DGGS_CELL_1);
        LINESTRING_CELL_IDS.add(LINESTRING_DGGS_CELL_2);
        LINESTRING_CELL_IDS.add(LINESTRING_DGGS_CELL_3);
        DGGS_LINESTRING = new DggsLinestring(LINESTRING_CELL_IDS);
        DGGS_SHAPE_LINESTRING = new DggsShape(DGGS_LINESTRING);

        // Initialise an outer ring for a polygon
        OUTER_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        OUTER_LINESTRING_CELL_1 = new DggsCell("00111");
        OUTER_LINESTRING_CELL_2 = new DggsCell("0002");
        OUTER_LINESTRING_CELL_3 = new DggsCell("0001");
        OUTER_LINESTRING_CELL_4 = new DggsCell("00333");
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_1);
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_2);
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_3);
        OUTER_LINESTRING_CELL_IDS.add(OUTER_LINESTRING_CELL_4);
        OUTER_RING_LINESTRING = new DggsLinestring(OUTER_LINESTRING_CELL_IDS);

        // Initialise an inner ring for a polygon
        INNER_RING_LINESTRING_ARRAY = new ArrayList<DggsLinestring>();
        INNER_RING_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        INNER_RING_LINESTRING_DGGS_CELL_1 = new DggsCell("00312");
        INNER_RING_LINESTRING_DGGS_CELL_2 = new DggsCell("00302");
        INNER_RING_LINESTRING_DGGS_CELL_3 = new DggsCell("00321");
        INNER_RING_LINESTRING_CELL_IDS.add(INNER_RING_LINESTRING_DGGS_CELL_1);
        INNER_RING_LINESTRING_CELL_IDS.add(INNER_RING_LINESTRING_DGGS_CELL_2);
        INNER_RING_LINESTRING_CELL_IDS.add(INNER_RING_LINESTRING_DGGS_CELL_3);
        INNER_RING_LINESTRING = new DggsLinestring(INNER_RING_LINESTRING_CELL_IDS);
        INNER_RING_LINESTRING_ARRAY.add(INNER_RING_LINESTRING);

        // Initialise a DGGS shape from a polygon with an inner ring
        DGGS_POLYGON = new DggsPolygon(OUTER_RING_LINESTRING, INNER_RING_LINESTRING_ARRAY);
        DGGS_SHAPE_POLYGON = new DggsShape(DGGS_POLYGON);

        // Initialises empty inner rings
        EMPTY_INNER_RING_LINESTRING_ARRAY = new ArrayList<DggsLinestring>();

        // Initialise the comparison shapes for testing spatial analysis on a triangular grid

        // Initialise DGGS shapes from cells

        // Initialise a DGGS shape from a cell that is a child cell of the base cell
        CHILD_DGGS_CELL = new DggsCell("00000");
        CHILD_DGGS_SHAPE_CELL = new DggsShape(CHILD_DGGS_CELL);

        // Initialise a DGGS shape from a cell that is disjoint from the base cell
        DISJOINT_DGGS_CELL = new DggsCell("0011");
        DISJOINT_DGGS_SHAPE_CELL = new DggsShape(DISJOINT_DGGS_CELL);

        // Initialise a DGGS shape from a cell that is a parent cell of the base cell
        PARENT_DGGS_CELL = new DggsCell("000");
        PARENT_DGGS_SHAPE_CELL = new DggsShape(PARENT_DGGS_CELL);

        // Initialise a DGGS shape from a cell that shares an edge with the base cell
        EDGE_SHARING_DGGS_CELL = new DggsCell("0001");
        EDGE_SHARING_DGGS_SHAPE_CELL = new DggsShape(EDGE_SHARING_DGGS_CELL);

        // Initialise DGGS shapes from linestrings

        // Initialise a DGGS shape from a linestring that is a substring of the base linestring
        SUBSTRING_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        SUBSTRING_LINESTRING_DGGS_CELL_1 = new DggsCell("0001");
        SUBSTRING_LINESTRING_DGGS_CELL_2 = new DggsCell("0002");
        SUBSTRING_LINESTRING_CELL_IDS.add(SUBSTRING_LINESTRING_DGGS_CELL_1);
        SUBSTRING_LINESTRING_CELL_IDS.add(SUBSTRING_LINESTRING_DGGS_CELL_2);
        SUBSTRING_DGGS_LINESTRING = new DggsLinestring(SUBSTRING_LINESTRING_CELL_IDS);
        SUBSTRING_DGGS_SHAPE_LINESTRING = new DggsShape(SUBSTRING_DGGS_LINESTRING);

        // Initialise a DGGS shape from a linestring that is disjoint from the base linestring
        DISJOINT_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        DISJOINT_LINESTRING_DGGS_CELL_1 = new DggsCell("0021");
        DISJOINT_LINESTRING_DGGS_CELL_2 = new DggsCell("0022");
        DISJOINT_LINESTRING_DGGS_CELL_3 = new DggsCell("0023");
        DISJOINT_LINESTRING_CELL_IDS.add(DISJOINT_LINESTRING_DGGS_CELL_1);
        DISJOINT_LINESTRING_CELL_IDS.add(DISJOINT_LINESTRING_DGGS_CELL_2);
        DISJOINT_LINESTRING_CELL_IDS.add(DISJOINT_LINESTRING_DGGS_CELL_3);
        DISJOINT_DGGS_LINESTRING = new DggsLinestring(DISJOINT_LINESTRING_CELL_IDS);
        DISJOINT_DGGS_SHAPE_LINESTRING = new DggsShape(DISJOINT_DGGS_LINESTRING);

        // Initialise a DGGS shape from a linestring that is a superstring of the base linestring
        SUPERSTRING_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        SUPERSTRING_LINESTRING_DGGS_CELL_1 = new DggsCell("0033");
        SUPERSTRING_LINESTRING_DGGS_CELL_2 = new DggsCell("0001");
        SUPERSTRING_LINESTRING_DGGS_CELL_3 = new DggsCell("0002");
        SUPERSTRING_LINESTRING_DGGS_CELL_4 = new DggsCell("0003");
        SUPERSTRING_LINESTRING_DGGS_CELL_5 = new DggsCell("0011");
        SUPERSTRING_LINESTRING_CELL_IDS.add(SUPERSTRING_LINESTRING_DGGS_CELL_1);
        SUPERSTRING_LINESTRING_CELL_IDS.add(SUPERSTRING_LINESTRING_DGGS_CELL_2);
        SUPERSTRING_LINESTRING_CELL_IDS.add(SUPERSTRING_LINESTRING_DGGS_CELL_3);
        SUPERSTRING_LINESTRING_CELL_IDS.add(SUPERSTRING_LINESTRING_DGGS_CELL_4);
        SUPERSTRING_LINESTRING_CELL_IDS.add(SUPERSTRING_LINESTRING_DGGS_CELL_5);
        SUPERSTRING_DGGS_LINESTRING = new DggsLinestring(SUPERSTRING_LINESTRING_CELL_IDS);
        SUPERSTRING_DGGS_SHAPE_LINESTRING = new DggsShape(SUPERSTRING_DGGS_LINESTRING);

        // Initialise a DGGS shape from a linestring whose end touches the end of the base linestring
        ENDS_TOUCHING_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        ENDS_TOUCHING_LINESTRING_DGGS_CELL_1 = new DggsCell("0033");
        ENDS_TOUCHING_LINESTRING_DGGS_CELL_2 = new DggsCell("0032");
        ENDS_TOUCHING_LINESTRING_DGGS_CELL_3 = new DggsCell("0001");
        ENDS_TOUCHING_LINESTRING_CELL_IDS.add(ENDS_TOUCHING_LINESTRING_DGGS_CELL_1);
        ENDS_TOUCHING_LINESTRING_CELL_IDS.add(ENDS_TOUCHING_LINESTRING_DGGS_CELL_2);
        ENDS_TOUCHING_LINESTRING_CELL_IDS.add(ENDS_TOUCHING_LINESTRING_DGGS_CELL_3);
        ENDS_TOUCHING_DGGS_LINESTRING = new DggsLinestring(ENDS_TOUCHING_LINESTRING_CELL_IDS);
        ENDS_TOUCHING_DGGS_SHAPE_LINESTRING = new DggsShape(ENDS_TOUCHING_DGGS_LINESTRING);

        // Initialise DGGS shapes from polygons

        // Initialise a DGGS shape from a polygon that is disjoint from the base polygon and has no inner rings
        DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_1 = new DggsCell("0021");
        DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_2 = new DggsCell("0022");
        DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_3 = new DggsCell("0023");
        DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS.add(DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_1);
        DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS.add(DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_2);
        DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS.add(DISJOINT_POLYGON_OUTER_LINESTRING_DGGS_CELL_3);
        DISJOINT_POLYGON_OUTER_RING_LINESTRING = new DggsLinestring(DISJOINT_POLYGON_OUTER_LINESTRING_CELL_IDS);

        DISJOINT_DGGS_POLYGON = new DggsPolygon(DISJOINT_POLYGON_OUTER_RING_LINESTRING,
                EMPTY_INNER_RING_LINESTRING_ARRAY);
        DISJOINT_DGGS_SHAPE_POLYGON = new DggsShape(DISJOINT_DGGS_POLYGON);

        // Initialise a DGGS shape from a polygon with no inner rings, that lies in the interior of the
        // base polygon
        INTERIOR_POLYGON_OUTER_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_1 = new DggsCell("00101");
        INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_2 = new DggsCell("00132");
        INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_3 = new DggsCell("00001");
        INTERIOR_POLYGON_OUTER_LINESTRING_CELL_IDS.add(INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_1);
        INTERIOR_POLYGON_OUTER_LINESTRING_CELL_IDS.add(INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_2);
        INTERIOR_POLYGON_OUTER_LINESTRING_CELL_IDS.add(INTERIOR_POLYGON_OUTER_LINESTRING_DGGS_CELL_3);
        INTERIOR_POLYGON_OUTER_RING_LINESTRING = new DggsLinestring(INTERIOR_POLYGON_OUTER_LINESTRING_CELL_IDS);
        INTERIOR_DGGS_POLYGON = new DggsPolygon(INTERIOR_POLYGON_OUTER_RING_LINESTRING,
                EMPTY_INNER_RING_LINESTRING_ARRAY);
        INTERIOR_DGGS_SHAPE_POLYGON = new DggsShape(INTERIOR_DGGS_POLYGON);

        // Initialise a DGGS shape from a polygon with no inner rings, that contains the base polygon
        CONTAINS_POLYGON_OUTER_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_1 = new DggsCell("00111");
        CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_2 = new DggsCell("00222");
        CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_3 = new DggsCell("00333");
        CONTAINS_POLYGON_OUTER_LINESTRING_CELL_IDS.add(CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_1);
        CONTAINS_POLYGON_OUTER_LINESTRING_CELL_IDS.add(CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_2);
        CONTAINS_POLYGON_OUTER_LINESTRING_CELL_IDS.add(CONTAINS_POLYGON_OUTER_LINESTRING_DGGS_CELL_3);
        CONTAINS_POLYGON_OUTER_RING_LINESTRING = new DggsLinestring(CONTAINS_POLYGON_OUTER_LINESTRING_CELL_IDS);

        CONTAINS_DGGS_POLYGON = new DggsPolygon(CONTAINS_POLYGON_OUTER_RING_LINESTRING,
                EMPTY_INNER_RING_LINESTRING_ARRAY);
        CONTAINS_DGGS_SHAPE_POLYGON = new DggsShape(CONTAINS_DGGS_POLYGON);

        // Initialise a DGGS shape from a polygon with no inner rings, that shares an edge with the base polygon
        EDGE_SHARING_OUTER_LINESTRING_CELL_IDS = new ArrayList<DggsCell>();
        EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_1 = new DggsCell("0002");
        EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_2 = new DggsCell("0001");
        EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_3 = new DggsCell("0022");
        EDGE_SHARING_OUTER_LINESTRING_CELL_IDS.add(EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_1);
        EDGE_SHARING_OUTER_LINESTRING_CELL_IDS.add(EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_2);
        EDGE_SHARING_OUTER_LINESTRING_CELL_IDS.add(EDGE_SHARING_OUTER_LINESTRING_DGGS_CELL_3);
        EDGE_SHARING_OUTER_RING_LINESTRING = new DggsLinestring(EDGE_SHARING_OUTER_LINESTRING_CELL_IDS);

        EDGE_SHARING_DGGS_POLYGON = new DggsPolygon(EDGE_SHARING_OUTER_RING_LINESTRING,
                EMPTY_INNER_RING_LINESTRING_ARRAY);
        EDGE_SHARING_DGGS_SHAPE_POLYGON = new DggsShape(EDGE_SHARING_DGGS_POLYGON);
    }
}
