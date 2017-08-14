"""
Copyright (c) Riskaware 2015

This file is part of OpenEAGGR.

OpenEAGGR is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenEAGGR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

A copy of the GNU Lesser General Public License is available in COPYING.LESSER
or can be found at <http://www.gnu.org/licenses/>.
"""

import os
import re
import unittest
from eaggr.eaggr import Eaggr
from eaggr.enums.model import Model
from eaggr.enums.dggs_shape_type import DggsShapeType
from eaggr.enums.dggs_shape_location import DggsShapeLocation
from eaggr.enums.shape_string_format import ShapeStringFormat
from eaggr.exceptions.eaggr_exception import EaggrException
from eaggr.shapes.lat_long_point import LatLongPoint
from eaggr.shapes.lat_long_linestring import LatLongLinestring
from eaggr.shapes.lat_long_polygon import LatLongPolygon
from eaggr.shapes.dggs_cell import DggsCell
from eaggr.shapes.dggs_linestring import DggsLinestring
from eaggr.shapes.dggs_polygon import DggsPolygon
from eaggr.enums.dggs_analysis_type import DggsAnalysisType
from eaggr.shapes.dggs_shape import DggsShape


## Sets up test data for ISEA4T spatial analysis unit tests
class DggsTestDataTriangular:
    # Initialise the base shapes for spatial analysis testing on a triangular grid 
    # Create a cell
    dggs_shape_cell = DggsShape(DggsCell("0700"), DggsShapeLocation.ONE_FACE)
    # Create a linestring
    dggs_linestring = DggsLinestring()
    dggs_linestring.add_cell(DggsCell("0701"))
    dggs_linestring.add_cell(DggsCell("0702"))
    dggs_linestring.add_cell(DggsCell("0703"))
    dggs_shape_linestring = DggsShape(dggs_linestring, DggsShapeLocation.ONE_FACE)
    # Create a polygon with an inner ring
    dggs_outer_linestring = DggsLinestring()
    dggs_outer_linestring.add_cell(DggsCell("07111"))
    dggs_outer_linestring.add_cell(DggsCell("0702"))
    dggs_outer_linestring.add_cell(DggsCell("0701"))
    dggs_outer_linestring.add_cell(DggsCell("07333"))
    dggs_polygon = DggsPolygon(dggs_outer_linestring)
    dggs_inner_linestring = DggsLinestring()
    dggs_inner_linestring.add_cell(DggsCell("07312"))
    dggs_inner_linestring.add_cell(DggsCell("07302"))
    dggs_inner_linestring.add_cell(DggsCell("07321"))
    dggs_polygon.add_inner_ring(dggs_inner_linestring)
    dggs_polygon = DggsPolygon(dggs_outer_linestring)
    dggs_shape_polygon = DggsShape(dggs_polygon, DggsShapeLocation.ONE_FACE)
    # Initialise comparison shapes from cells, for spatial analysis testing
    # Create a eaggr shape from a child cell of the base cell 
    child_dggs_shape_cell = DggsShape(DggsCell("07000"), DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a parent cell of the base cell
    parent_dggs_shape_cell = DggsShape(DggsCell("070"), DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a cell that is disjoint from the base cell
    disjoint_dggs_shape_cell = DggsShape(DggsCell("0711"), DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a cell that shares an edge with the base cell
    edge_sharing_dggs_shape_cell = DggsShape(DggsCell("0701"), DggsShapeLocation.ONE_FACE)
    # Initialise comparison shapes from linestrings, for spatial analysis testing
    # Create a eaggr shape from a linestring that is a substring of the base linestring
    substring_dggs_linestring = DggsLinestring()
    substring_dggs_linestring.add_cell(DggsCell("0701"))
    substring_dggs_linestring.add_cell(DggsCell("0702"))
    substring_dggs_shape_linestring = DggsShape(substring_dggs_linestring, DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a linestring that is disjoint from the base linestring
    disjoint_dggs_linestring = DggsLinestring()
    disjoint_dggs_linestring.add_cell(DggsCell("0721"))
    disjoint_dggs_linestring.add_cell(DggsCell("0722"))
    disjoint_dggs_linestring.add_cell(DggsCell("0723"))
    disjoint_dggs_shape_linestring = DggsShape(disjoint_dggs_linestring, DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a linestring that is a superstring of the base linestring
    superstring_dggs_linestring = DggsLinestring()
    superstring_dggs_linestring.add_cell(DggsCell("0733"))
    superstring_dggs_linestring.add_cell(DggsCell("0701"))
    superstring_dggs_linestring.add_cell(DggsCell("0702"))
    superstring_dggs_linestring.add_cell(DggsCell("0703"))
    superstring_dggs_linestring.add_cell(DggsCell("0711"))
    superstring_dggs_shape_linestring = DggsShape(superstring_dggs_linestring, DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a linestring whose end touches the end of the base linestring
    ends_touching_dggs_linestring = DggsLinestring()
    ends_touching_dggs_linestring.add_cell(DggsCell("0733"))
    ends_touching_dggs_linestring.add_cell(DggsCell("0732"))
    ends_touching_dggs_linestring.add_cell(DggsCell("0701"))
    ends_touching_dggs_shape_linestring = DggsShape(ends_touching_dggs_linestring, DggsShapeLocation.ONE_FACE)
    # Initialise comparison shapes from polygons, for spatial analysis testing
    # Create a eaggr shape from a polygon with no inner rings, that lies within the interior of the base polygon
    interior_polygon_dggs_outer_linestring = DggsLinestring()
    interior_polygon_dggs_outer_linestring.add_cell(DggsCell("07101"))
    interior_polygon_dggs_outer_linestring.add_cell(DggsCell("07132"))
    interior_polygon_dggs_outer_linestring.add_cell(DggsCell("07001"))
    interior_polygon_dggs_polygon = DggsPolygon(interior_polygon_dggs_outer_linestring)
    interior_polygon_dggs_shape_polygon = DggsShape(interior_polygon_dggs_polygon, DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a polygon with no inner rings, that is disjoint from the base polygon
    disjoint_polygon_dggs_outer_linestring = DggsLinestring()
    disjoint_polygon_dggs_outer_linestring.add_cell(DggsCell("0721"))
    disjoint_polygon_dggs_outer_linestring.add_cell(DggsCell("0722"))
    disjoint_polygon_dggs_outer_linestring.add_cell(DggsCell("0723"))
    disjoint_polygon_dggs_polygon = DggsPolygon(disjoint_polygon_dggs_outer_linestring)
    disjoint_polygon_dggs_shape_polygon = DggsShape(disjoint_polygon_dggs_polygon, DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a polygon with no inner rings, that contains the base polygon
    contains_polygon_dggs_outer_linestring = DggsLinestring()
    contains_polygon_dggs_outer_linestring.add_cell(DggsCell("07111"))
    contains_polygon_dggs_outer_linestring.add_cell(DggsCell("07222"))
    contains_polygon_dggs_outer_linestring.add_cell(DggsCell("07333"))
    contains_polygon_dggs_polygon = DggsPolygon(contains_polygon_dggs_outer_linestring)
    contains_polygon_dggs_shape_polygon = DggsShape(contains_polygon_dggs_polygon, DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a polygon with no inner rings, that shares an edge with the base polygon
    edge_sharing_polygon_dggs_outer_linestring = DggsLinestring()
    edge_sharing_polygon_dggs_outer_linestring.add_cell(DggsCell("0702"))
    edge_sharing_polygon_dggs_outer_linestring.add_cell(DggsCell("0701"))
    edge_sharing_polygon_dggs_outer_linestring.add_cell(DggsCell("0722"))
    edge_sharing_polygon_dggs_polygon = DggsPolygon(edge_sharing_polygon_dggs_outer_linestring)
    edge_sharing_polygon_dggs_shape_polygon = DggsShape(edge_sharing_polygon_dggs_polygon, DggsShapeLocation.ONE_FACE)


## Sets up test data for ISEA4T spatial analysis unit tests
class DggsTestDataHexagonal:
    # Initialise the base shapes for spatial analysis testing on a triangular grid 
    # Create a cell
    dggs_shape_cell = DggsShape(DggsCell("07021,1"), DggsShapeLocation.ONE_FACE)
    # Create a linestring
    dggs_linestring = DggsLinestring()
    dggs_linestring.add_cell(DggsCell("07021,2"))
    dggs_linestring.add_cell(DggsCell("07023,1"))
    dggs_linestring.add_cell(DggsCell("07023,3"))
    dggs_shape_linestring = DggsShape(dggs_linestring, DggsShapeLocation.ONE_FACE)
    # Create a polygon with an inner ring
    dggs_outer_linestring = DggsLinestring()
    dggs_outer_linestring.add_cell(DggsCell("07020,2"))
    dggs_outer_linestring.add_cell(DggsCell("07024,0"))
    dggs_outer_linestring.add_cell(DggsCell("07024,4"))
    dggs_outer_linestring.add_cell(DggsCell("07023,4"))
    dggs_outer_linestring.add_cell(DggsCell("07020,3"))
    dggs_polygon = DggsPolygon(dggs_outer_linestring)
    dggs_inner_linestring = DggsLinestring()
    dggs_inner_linestring.add_cell(DggsCell("07021,2"))
    dggs_inner_linestring.add_cell(DggsCell("07023,1"))
    dggs_inner_linestring.add_cell(DggsCell("07023,3"))
    dggs_polygon.add_inner_ring(dggs_inner_linestring)
    dggs_shape_polygon = DggsShape(dggs_polygon, DggsShapeLocation.ONE_FACE)
    # Initialise comparison shapes from cells, for spatial analysis testing
    # Create a eaggr shape from a partially overlapping child cell of the base cell 
    overlapping_child_dggs_shape_cell = DggsShape(DggsCell("07031,4"), DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a cell that is disjoint from the base cell
    disjoint_dggs_shape_cell = DggsShape(DggsCell("07023,1"), DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a linestring that is disjoint from the base linestring
    disjoint_dggs_linestring = DggsLinestring()
    disjoint_dggs_linestring.add_cell(DggsCell("07020,2"))
    disjoint_dggs_linestring.add_cell(DggsCell("07023,0"))
    disjoint_dggs_linestring.add_cell(DggsCell("07024,1"))
    disjoint_dggs_linestring.add_cell(DggsCell("07024,4"))
    disjoint_dggs_shape_linestring = DggsShape(disjoint_dggs_linestring, DggsShapeLocation.ONE_FACE)
    # Create a eaggr shape from a polygon with no inner rings, that overlaps the base polygon
    overlapping_polygon_dggs_outer_linestring = DggsLinestring()
    overlapping_polygon_dggs_outer_linestring.add_cell(DggsCell("07032,3"))
    overlapping_polygon_dggs_outer_linestring.add_cell(DggsCell("07033,4"))
    overlapping_polygon_dggs_outer_linestring.add_cell(DggsCell("07022,3"))
    overlapping_polygon_dggs_polygon = DggsPolygon(overlapping_polygon_dggs_outer_linestring)
    overlapping_polygon_dggs_shape_polygon = DggsShape(overlapping_polygon_dggs_polygon, DggsShapeLocation.ONE_FACE)
    #Create a eaggr shape from a polygon with no inner rings, that is disjoint from the base polygon
    disjoint_polygon_dggs_outer_linestring = DggsLinestring()
    disjoint_polygon_dggs_outer_linestring.add_cell(DggsCell("07020,0"))
    disjoint_polygon_dggs_outer_linestring.add_cell(DggsCell("07020,1"))
    disjoint_polygon_dggs_outer_linestring.add_cell(DggsCell("07021,0"))
    disjoint_polygon_dggs_polygon = DggsPolygon(disjoint_polygon_dggs_outer_linestring)
    disjoint_polygon_dggs_shape_polygon = DggsShape(disjoint_polygon_dggs_polygon, DggsShapeLocation.ONE_FACE)
        
        
## Unit tests for the Eaggr class.
class TestDggsPython(unittest.TestCase):
    
    _PROJ_PATH = os.path.join(os.path.dirname(__file__), '..', '..')

    def test_get_version(self):
        version = Eaggr(Model.ISEA4T).get_version()
        self.assertEqual(version, "v2.0")

    def test_convert_point_to_dggs_cell(self):
        # Create the lat/long points
        lat_long_point1 = LatLongPoint(1.234, 2.345, 3.884);
        lat_long_point2 = LatLongPoint(3.456, 4.567, 3.884);
        # Initialise the DGGS model
        dggs = Eaggr(Model.ISEA4T)
        # Convert the first lat/long point
        dggs_cell = dggs.convert_point_to_dggs_cell(lat_long_point1)
        self.assertTrue(type(dggs_cell) is DggsCell)
        self.assertEqual(dggs_cell.get_cell_id(), "07231131111113100331001")
        # Convert the second lat/long point
        dggs_cell = dggs.convert_point_to_dggs_cell(lat_long_point2)
        self.assertTrue(type(dggs_cell) is DggsCell)
        self.assertEqual(dggs_cell.get_cell_id(), "07012000001303022011321")

    def test_convert_shapes_to_dggs_shapes(self):
        # Create the lat/long shapes
        point1 = LatLongPoint(1.234, 2.345, 3.884)
        point2 = LatLongPoint(3.456, 4.567, 3.884)
        linestring = LatLongLinestring()
        linestring.add_point(point1)
        linestring.add_point(point2)
        polygon = LatLongPolygon(linestring)
        polygon.add_inner_ring(linestring)
        polygon.add_inner_ring(linestring)
        # Add the lat/long shapes to the input list
        lat_long_shapes = []
        lat_long_shapes.append(point1)
        lat_long_shapes.append(point2)
        lat_long_shapes.append(linestring)
        lat_long_shapes.append(polygon)
        # Convert the lat/long shapes
        dggs = Eaggr(Model.ISEA4T)
        dggs_shapes = dggs.convert_shapes_to_dggs_shapes(lat_long_shapes)
        # Check the number of shapes returned
        self.assertEqual(len(dggs_shapes), 4)
        # Check the first shape
        self.assertEqual(dggs_shapes[0].get_location(), DggsShapeLocation.ONE_FACE)
        self.assertEqual(dggs_shapes[0].get_shape_type(), DggsShapeType.CELL)
        self.assertTrue(type(dggs_shapes[0].get_shape()) is DggsCell)
        self.assertEqual(dggs_shapes[0].get_shape().get_cell_id(), "07231131111113100331001")
        # Check the second shape
        self.assertEqual(dggs_shapes[1].get_location(), DggsShapeLocation.ONE_FACE)
        self.assertEqual(dggs_shapes[1].get_shape_type(), DggsShapeType.CELL)
        self.assertTrue(type(dggs_shapes[1].get_shape()) is DggsCell)
        self.assertEqual(dggs_shapes[1].get_shape().get_cell_id(), "07012000001303022011321")
        # Check the third shape
        self.assertEqual(dggs_shapes[2].get_location(), DggsShapeLocation.NO_LOCATION)
        self.assertEqual(dggs_shapes[2].get_shape_type(), DggsShapeType.LINESTRING)
        self.assertTrue(type(dggs_shapes[2].get_shape()) is DggsLinestring)
        cells = dggs_shapes[2].get_shape().get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), "07231131111113100331001")
        self.assertEqual(cells[1].get_cell_id(), "07012000001303022011321")
        # Check the fourth shape
        self.assertEqual(dggs_shapes[3].get_location(), DggsShapeLocation.NO_LOCATION)
        self.assertTrue(dggs_shapes[3].get_shape_type() is DggsShapeType.POLYGON)
        self.assertTrue(type(dggs_shapes[3].get_shape()) is DggsPolygon)
        cells = dggs_shapes[3].get_shape().get_outer_ring().get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), "07231131111113100331001")
        self.assertEqual(cells[1].get_cell_id(), "07012000001303022011321")
        rings = dggs_shapes[3].get_shape().get_inner_rings()
        self.assertEqual(len(rings), 2)
        cells = rings[0].get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), "07231131111113100331001")
        self.assertEqual(cells[1].get_cell_id(), "07012000001303022011321")
        cells = rings[1].get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), "07231131111113100331001")
        self.assertEqual(cells[1].get_cell_id(), "07012000001303022011321")
        # Convert empty list
        dggs_shapes = dggs.convert_shapes_to_dggs_shapes([])
        self.assertEqual(len(dggs_shapes), 0)

    def test_convert_shape_string_to_dggs_shapes(self):
        wkt_string = ('GEOMETRYCOLLECTION('
                      'MULTIPOINT(2.345 1.234, 4.567 3.456), '
                      'LINESTRING(2.345 1.234, 4.567 3.456), '
                      'POLYGON((2.345 1.234, 4.567 3.456), (2.345 1.234, 4.567 3.456), (2.345 1.234, 4.567 3.456)))')
        # Convert the shape string
        dggs = Eaggr(Model.ISEA4T)
        dggs_shapes = dggs.convert_shape_string_to_dggs_shapes(wkt_string, ShapeStringFormat.WKT, 3.884)
        # Check the number of points returned
        self.assertEqual(len(dggs_shapes), 4)
        # Check the first shape
        self.assertEqual(dggs_shapes[0].get_location(), DggsShapeLocation.ONE_FACE)
        self.assertEqual(dggs_shapes[0].get_shape_type(), DggsShapeType.CELL)
        self.assertTrue(type(dggs_shapes[0].get_shape()) is DggsCell)
        self.assertEqual(dggs_shapes[0].get_shape().get_cell_id(), "07231131111113100331001")
        # Check the second shape
        self.assertEqual(dggs_shapes[1].get_location(), DggsShapeLocation.ONE_FACE)
        self.assertEqual(dggs_shapes[1].get_shape_type(), DggsShapeType.CELL)
        self.assertTrue(type(dggs_shapes[1].get_shape()) is DggsCell)
        self.assertEqual(dggs_shapes[1].get_shape().get_cell_id(), "07012000001303022011321")
        # Check the third shape
        self.assertEqual(dggs_shapes[2].get_location(), DggsShapeLocation.NO_LOCATION)
        self.assertEqual(dggs_shapes[2].get_shape_type(), DggsShapeType.LINESTRING)
        self.assertTrue(type(dggs_shapes[2].get_shape()) is DggsLinestring)
        cells = dggs_shapes[2].get_shape().get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), "07231131111113100331001")
        self.assertEqual(cells[1].get_cell_id(), "07012000001303022011321")
        # Check the fourth shape
        self.assertEqual(dggs_shapes[3].get_location(), DggsShapeLocation.NO_LOCATION)
        self.assertEqual(dggs_shapes[3].get_shape_type(), DggsShapeType.POLYGON)
        self.assertTrue(type(dggs_shapes[3].get_shape()) is DggsPolygon)
        cells = dggs_shapes[3].get_shape().get_outer_ring().get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), "07231131111113100331001")
        self.assertEqual(cells[1].get_cell_id(), "07012000001303022011321")
        rings = dggs_shapes[3].get_shape().get_inner_rings()
        self.assertEqual(len(rings), 2)
        cells = rings[0].get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), "07231131111113100331001")
        self.assertEqual(cells[1].get_cell_id(), "07012000001303022011321")
        cells = rings[1].get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), "07231131111113100331001")
        self.assertEqual(cells[1].get_cell_id(), "07012000001303022011321")

    def test_convert_dggs_cell_to_point(self):
        # Create the DGGS cells
        dggs_cell1 = DggsCell("07231131111113100331001")
        dggs_cell2 = DggsCell("07012000001303022011321")
        # Initialise the DGGS model
        dggs = Eaggr(Model.ISEA4T)
        # Convert the first DGGS cell
        lat_long_point = dggs.convert_dggs_cell_to_point(dggs_cell1)
        self.failUnlessAlmostEqual(1.234, lat_long_point.get_latitude(), places=3)
        self.failUnlessAlmostEqual(2.345, lat_long_point.get_longitude(), places=3)
        self.failUnlessAlmostEqual(5.804, lat_long_point.get_accuracy(), places=3)
        # Convert the second DGGS cell
        lat_long_point = dggs.convert_dggs_cell_to_point(dggs_cell2)
        self.failUnlessAlmostEqual(3.456, lat_long_point.get_latitude(), places=3)
        self.failUnlessAlmostEqual(4.567, lat_long_point.get_longitude(), places=3)
        self.failUnlessAlmostEqual(5.804, lat_long_point.get_accuracy(), places=3)

    def test_convert_dggs_cells_to_points(self):
        # Create the DGGS cells
        dggs_cells = [DggsCell("07231131111113100331001"), DggsCell("07012000001303022011321")]
        # Convert the DGGS cells
        dggs = Eaggr(Model.ISEA4T)
        lat_long_points = dggs.convert_dggs_cells_to_points(dggs_cells)
        # Check the number of points returned
        self.assertEqual(len(lat_long_points), 2)
        # Check the first point
        point_index = 0
        self.failUnlessAlmostEqual(1.234, lat_long_points[point_index].get_latitude(), places=3)
        self.failUnlessAlmostEqual(2.345, lat_long_points[point_index].get_longitude(), places=3)
        self.failUnlessAlmostEqual(5.804, lat_long_points[point_index].get_accuracy(), places=3)
        # Check the second point
        point_index += 1
        self.failUnlessAlmostEqual(3.456, lat_long_points[point_index].get_latitude(), places=3)
        self.failUnlessAlmostEqual(4.567, lat_long_points[point_index].get_longitude(), places=3)
        self.failUnlessAlmostEqual(5.804, lat_long_points[point_index].get_accuracy(), places=3)
        # Convert empty list
        lat_long_points = dggs.convert_dggs_cells_to_points([])
        self.assertEqual(len(lat_long_points), 0)

    def test_convert_dggs_cells_to_shape_string(self):
        # Create the DGGS cells
        dggs_cells = [DggsCell("07231131111113100331001"), DggsCell("07012000001303022011321")]
        # Convert the DGGS cells
        dggs = Eaggr(Model.ISEA4T)
        shape_string = dggs.convert_dggs_cells_to_shape_string(dggs_cells, ShapeStringFormat.WKT)
        # Split up the string and test the values numerically because values in the string can vary slightly
        string_data = re.split(' \(| |,|\)', shape_string)
        # Check the shape type
        self.assertEqual('MULTIPOINT', string_data[0])
        # Check the first point
        self.failUnlessAlmostEqual(2.345, float(string_data[1]), places=3)
        self.failUnlessAlmostEqual(1.234, float(string_data[2]), places=3)
        # Check the second point
        self.failUnlessAlmostEqual(4.567, float(string_data[3]), places=3)
        self.failUnlessAlmostEqual(3.456, float(string_data[4]), places=3)

    def test_convert_dggs_cell_outline_to_shape_string(self):
        # Create the DGGS cells
        dggs_cell = DggsCell("07231131111113100331001")
        # Convert the DGGS cells
        dggs = Eaggr(Model.ISEA4T)
        shape_string = dggs.convert_dggs_cell_outline_to_shape_string(dggs_cell, ShapeStringFormat.WKT)
        # Split up the string and test the values numerically because values in the string can vary slightly
        string_data = re.split(' \(\(| |,| |,|\)\)', shape_string)
        # Check the shape type
        self.assertEqual('POLYGON', string_data[0])
        # Check the first point
        self.failUnlessAlmostEqual(2.3449866, float(string_data[1]), places=7)
        self.failUnlessAlmostEqual(1.2340080, float(string_data[2]), places=7)
        # Check the second point
        self.failUnlessAlmostEqual(2.3450042, float(string_data[3]), places=7)
        self.failUnlessAlmostEqual(1.2339790, float(string_data[4]), places=7)
        # Check the second point
        self.failUnlessAlmostEqual(2.3450218, float(string_data[5]), places=7)
        self.failUnlessAlmostEqual(1.2340036, float(string_data[6]), places=7)

    def test_get_dggs_cell_parents(self):
        # Create the DGGS cell
        dggs_cell = DggsCell("07012212222221011101013")
        # Get its parent cell
        dggs = Eaggr(Model.ISEA4T)
        parents = dggs.get_dggs_cell_parents(dggs_cell)
        # Check the parent cell
        self.assertEqual(len(parents), 1)
        self.assertEqual(parents[0].get_cell_id(), "0701221222222101110101")

    def test_get_dggs_cell_children(self):
        # Create the DGGS cell
        dggs_cell = DggsCell("0701221222222101110101")
        # Get its child cells
        dggs = Eaggr(Model.ISEA4T)
        children = dggs.get_dggs_cell_children(dggs_cell)
        # Check the child cells
        self.assertEqual(len(children), 4)
        self.assertEqual(children[0].get_cell_id(), "07012212222221011101010")
        self.assertEqual(children[1].get_cell_id(), "07012212222221011101011")
        self.assertEqual(children[2].get_cell_id(), "07012212222221011101012")
        self.assertEqual(children[3].get_cell_id(), "07012212222221011101013")
        
    def test_get_dggs_cell_siblings(self):
        # Create the DGGS cell
        dggs_cell = DggsCell("07012212222221011101010")
        # Get its sibling cells
        dggs = Eaggr(Model.ISEA4T)
        siblings = dggs.get_dggs_cell_siblings(dggs_cell)
        # Check the sibling cells
        self.assertEqual(len(siblings), 3)
        self.assertEqual(siblings[0].get_cell_id(), "07012212222221011101011")
        self.assertEqual(siblings[1].get_cell_id(), "07012212222221011101012")
        self.assertEqual(siblings[2].get_cell_id(), "07012212222221011101013")

    def test_get_bounding_dggs_cell(self):
        # Create DGGS cells at the same resolution
        dggs_cells = [DggsCell("07001"),
                      DggsCell("07002"),
                      DggsCell("07003")]
        # Get their bounding cell
        dggs = Eaggr(Model.ISEA4T)
        bounding_cell = dggs.get_bounding_dggs_cell(dggs_cells)
        # Check the bounding cell
        self.assertEqual(bounding_cell.get_cell_id(), "0700")
        # Create DGGS cells at different resolutions
        dggs_cells = [DggsCell("0700123123"),
                      DggsCell("0700231231231"),
                      DggsCell("07003123123123123123123")]
        # Get their bounding cell
        bounding_cell = dggs.get_bounding_dggs_cell(dggs_cells)
        # Check the bounding cell
        self.assertEqual(bounding_cell.get_cell_id(), "0700")

    def test_create_dggs_kml_file(self):
        # Create the input DGGS cells
        dggs_cells = [DggsCell("07231131111113100331032"), DggsCell("07012000001303022011321")]
        # Get the directory path for the KML files
        test_data_dir = os.path.join(self._PROJ_PATH, 'TestData')
        # Create a KML file for the DGGS cells
        dggs = Eaggr(Model.ISEA4T)
        kml_filename = os.path.join(test_data_dir, 'ActualKmlFile.kml')
        dggs.create_dggs_kml_file(kml_filename, dggs_cells)
        # Check the contents of the output KML file
        with open(os.path.join(test_data_dir, 'ExpectedDggsLibraryTestOutput.kml'), 'r') as expected_file:
            expected_kml = expected_file.read()
        with open(kml_filename, 'r') as actual_file:
            actual_kml = actual_file.read()
        os.remove(kml_filename)
        # Remove end of line characters from the strings
        expected_kml = expected_kml.replace('\r', '').replace('\n', '')
        actual_kml = actual_kml.replace('\r', '').replace('\n', '')
        self.assertEqual(expected_kml, actual_kml)

    def test_convert_shapes_to_dggs_shapes_isea3h(self):
        # Create the lat/long shapes
        point1 = LatLongPoint(1.234, 2.345, 3.884)
        point2 = LatLongPoint(3.456, 4.567, 3.884)
        linestring = LatLongLinestring()
        linestring.add_point(point1)
        linestring.add_point(point2)
        polygon = LatLongPolygon(linestring)
        polygon.add_inner_ring(linestring)
        polygon.add_inner_ring(linestring)
        # Add the lat/long shapes to the input list
        lat_long_shapes = []
        lat_long_shapes.append(point1)
        lat_long_shapes.append(point2)
        lat_long_shapes.append(linestring)
        lat_long_shapes.append(polygon)
        # Convert the lat/long shapes
        dggs = Eaggr(Model.ISEA3H)
        dggs_shapes = dggs.convert_shapes_to_dggs_shapes(lat_long_shapes)
        # Check the number of shapes returned
        self.assertEqual(len(dggs_shapes), 4)
        # Check the first shape
        self.assertEqual(dggs_shapes[0].get_location(), DggsShapeLocation.ONE_FACE)
        self.assertEqual(dggs_shapes[0].get_shape_type(), DggsShapeType.CELL)
        self.assertTrue(type(dggs_shapes[0].get_shape()) is DggsCell)
        self.assertEqual(dggs_shapes[0].get_shape().get_cell_id(), '0728-549628,-522499')
        # Check the second shape
        self.assertEqual(dggs_shapes[1].get_location(), DggsShapeLocation.ONE_FACE)
        self.assertEqual(dggs_shapes[1].get_shape_type(), DggsShapeType.CELL)
        self.assertTrue(type(dggs_shapes[1].get_shape()) is DggsCell)
        self.assertEqual(dggs_shapes[1].get_shape().get_cell_id(), '0728-592991,-296234')
        # Check the third shape
        self.assertEqual(dggs_shapes[2].get_location(), DggsShapeLocation.NO_LOCATION)
        self.assertEqual(dggs_shapes[2].get_shape_type(), DggsShapeType.LINESTRING)
        self.assertTrue(type(dggs_shapes[2].get_shape()) is DggsLinestring)
        cells = dggs_shapes[2].get_shape().get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), '0728-549628,-522499')
        self.assertEqual(cells[1].get_cell_id(), '0728-592991,-296234')
        # Check the fourth shape
        self.assertEqual(dggs_shapes[3].get_location(), DggsShapeLocation.NO_LOCATION)
        self.assertTrue(dggs_shapes[3].get_shape_type() is DggsShapeType.POLYGON)
        self.assertTrue(type(dggs_shapes[3].get_shape()) is DggsPolygon)
        cells = dggs_shapes[3].get_shape().get_outer_ring().get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), '0728-549628,-522499')
        self.assertEqual(cells[1].get_cell_id(), '0728-592991,-296234')
        rings = dggs_shapes[3].get_shape().get_inner_rings()
        self.assertEqual(len(rings), 2)
        cells = rings[0].get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), '0728-549628,-522499')
        self.assertEqual(cells[1].get_cell_id(), '0728-592991,-296234')
        cells = rings[1].get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0].get_cell_id(), '0728-549628,-522499')
        self.assertEqual(cells[1].get_cell_id(), '0728-592991,-296234')

    ## Test to ensure multiple instances of the DGGS API can be used without conflicting
    #  with each other.
    def test_multiple_instances(self):
        # Create a point to convert
        lat_long_point = LatLongPoint(1.234, 2.345, 3.884);
        # Create two different DGGS models
        isea4tDggs = Eaggr(Model.ISEA4T)
        isea3hDggs = Eaggr(Model.ISEA3H)
        # Convert the point to a ISEA4T cell
        dggs_cell = isea4tDggs.convert_point_to_dggs_cell(lat_long_point)
        self.assertEqual(dggs_cell.get_cell_id(), "07231131111113100331001")
        # Convert the point to a ISEA3H cell
        dggs_cell = isea3hDggs.convert_point_to_dggs_cell(lat_long_point)
        self.assertEqual(dggs_cell.get_cell_id(), "0728-549628,-522499")
        # Now delete the ISEA4T model and ensure the ISEA3H model can still be used
        del isea4tDggs
        dggs_cell = isea3hDggs.convert_point_to_dggs_cell(lat_long_point)
        self.assertEqual(dggs_cell.get_cell_id(), "0728-549628,-522499")

    def test_compare_dggs_shapes_equals(self):
        dggs = Eaggr(Model.ISEA4T)
        # Test the EQUALS spatial analysis
        # Compare the cell with itself
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.dggs_shape_cell, DggsAnalysisType.EQUALS))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.disjoint_dggs_shape_cell, DggsAnalysisType.EQUALS))
        # Compare the linestring with itself
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.dggs_shape_linestring, DggsAnalysisType.EQUALS))
        # Compare the linestring with a disjoint linestring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.disjoint_dggs_shape_linestring, DggsAnalysisType.EQUALS))
        # Compare the polygon with itself
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.dggs_shape_polygon, DggsAnalysisType.EQUALS))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.EQUALS))
        
    def test_compare_dggs_shapes_contains(self):
        dggs = Eaggr(Model.ISEA4T)
        # Test the CONTAINS spatial analysis
        # Compare the cell with a child cell
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.child_dggs_shape_cell, DggsAnalysisType.CONTAINS))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.disjoint_dggs_shape_cell, DggsAnalysisType.CONTAINS))
        # Compare the linestring with a substring
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.substring_dggs_shape_linestring, DggsAnalysisType.CONTAINS))
        # Compare the linestring with a disjoint linestring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.disjoint_dggs_shape_linestring, DggsAnalysisType.CONTAINS))
        # Compare the polygon with a polygon in its interior
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.interior_polygon_dggs_shape_polygon, DggsAnalysisType.CONTAINS))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.CONTAINS))
        
    def test_compare_dggs_shapes_within(self):
        dggs = Eaggr(Model.ISEA4T)
        # Test the WITHIN spatial analysis
        # Compare the cell with a parent cell
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.parent_dggs_shape_cell, DggsAnalysisType.WITHIN))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.disjoint_dggs_shape_cell, DggsAnalysisType.WITHIN))
        # Compare the linestring with a super-string
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.superstring_dggs_shape_linestring, DggsAnalysisType.WITHIN))
        # Compare the linestring with a disjoint linestring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.disjoint_dggs_shape_linestring, DggsAnalysisType.WITHIN))
        # Compare the polygon with a polygon that contains it
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.contains_polygon_dggs_shape_polygon, DggsAnalysisType.WITHIN))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.WITHIN))
        
    def test_compare_dggs_shapes_touches(self):
        dggs = Eaggr(Model.ISEA4T)
        # Test the TOUCHES spatial analysis
        # Compare the cell with an edge sharing cell
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.edge_sharing_dggs_shape_cell, DggsAnalysisType.TOUCHES))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.disjoint_dggs_shape_cell, DggsAnalysisType.TOUCHES))
        # Compare the linestring with a linestring that touches its end
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.ends_touching_dggs_shape_linestring, DggsAnalysisType.TOUCHES))
        # Compare the linestring with a disjoint linestring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.disjoint_dggs_shape_linestring, DggsAnalysisType.TOUCHES))
        # Compare the polygon with a polygon that shares an edge of the outer ring
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.edge_sharing_polygon_dggs_shape_polygon, DggsAnalysisType.TOUCHES))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.TOUCHES))
        
    def test_compare_dggs_shapes_disjoint(self):
        dggs = Eaggr(Model.ISEA4T)
        # Test the DISJOINT spatial analysis
        # Compare the cell with a disjoint cell
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.disjoint_dggs_shape_cell, DggsAnalysisType.DISJOINT))
        # Compare the cell with an edge sharing cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.edge_sharing_dggs_shape_cell, DggsAnalysisType.DISJOINT))
        # Compare the linestring with a disjoint linestring 
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.disjoint_dggs_shape_linestring, DggsAnalysisType.DISJOINT))
        # Compare the linestring with a linestring that touches its end
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.ends_touching_dggs_shape_linestring, DggsAnalysisType.DISJOINT))
        # Compare the polygon with a disjoint polygon
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.DISJOINT))
        # Compare the polygon with a polygon that shares an edge of the outer ring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.edge_sharing_polygon_dggs_shape_polygon, DggsAnalysisType.DISJOINT))
        
    def test_compare_dggs_shapes_intersects(self):
        dggs = Eaggr(Model.ISEA4T)
        # Test the INTERSECTS spatial analysis
        # Compare the cell with an edge sharing cell
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.edge_sharing_dggs_shape_cell, DggsAnalysisType.INTERSECTS))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.disjoint_dggs_shape_cell, DggsAnalysisType.INTERSECTS))
        # Compare the linestring with a linestring that touches its end
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.ends_touching_dggs_shape_linestring, DggsAnalysisType.INTERSECTS))
        # Compare the linestring with a disjoint linestring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.disjoint_dggs_shape_linestring, DggsAnalysisType.INTERSECTS))
        # Compare the polygon with a polygon that shares an edge of the outer ring
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.edge_sharing_polygon_dggs_shape_polygon, DggsAnalysisType.INTERSECTS))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.INTERSECTS))
        
    def test_compare_dggs_shapes_covers(self):
        dggs = Eaggr(Model.ISEA4T)
        # Test the COVERS spatial analysis
        # Compare the cell with a child cell
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.child_dggs_shape_cell, DggsAnalysisType.COVERS))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.disjoint_dggs_shape_cell, DggsAnalysisType.COVERS))
        # Compare the linestring with a substring
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.substring_dggs_shape_linestring, DggsAnalysisType.COVERS))
        # Compare the linestring with a disjoint linestring    
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.disjoint_dggs_shape_linestring, DggsAnalysisType.COVERS))
        # Compare the polygon with a polygon in its interior
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.interior_polygon_dggs_shape_polygon, DggsAnalysisType.COVERS))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.COVERS))
        
    def test_compare_dggs_shapes_covered_by(self):
        dggs = Eaggr(Model.ISEA4T)
        # Test the COVERED_BY spatial analysis
        # Compare the cell with a parent cell
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.parent_dggs_shape_cell, DggsAnalysisType.COVERED_BY))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_cell, DggsTestDataTriangular.disjoint_dggs_shape_cell, DggsAnalysisType.COVERED_BY))
        # Compare the linestring with a super-string
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.superstring_dggs_shape_linestring, DggsAnalysisType.COVERED_BY))
        # Compare the linestring with a disjoint linestring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_linestring, DggsTestDataTriangular.disjoint_dggs_shape_linestring, DggsAnalysisType.COVERED_BY))
        # Compare the polygon with a polygon that contains it
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.contains_polygon_dggs_shape_polygon, DggsAnalysisType.COVERED_BY))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataTriangular.dggs_shape_polygon, DggsTestDataTriangular.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.COVERED_BY))
        
    def test_compare_dggs_shapes_crosses(self):
        dggs = Eaggr(Model.ISEA3H)
        # Test the CROSSES spatial analysis
        # Two cells can never cross as they are treated as polygons
        # Compare the cell with an overlapping child cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_cell, DggsTestDataHexagonal.overlapping_child_dggs_shape_cell, DggsAnalysisType.CROSSES))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_cell, DggsTestDataHexagonal.disjoint_dggs_shape_cell, DggsAnalysisType.CROSSES))
        # Compare the linestring with a linestring that crosses it at a single point
        crossing_dggs_linestring = DggsLinestring()
        crossing_dggs_linestring.add_cell(DggsCell("07022,3"))
        crossing_dggs_linestring.add_cell(DggsCell("07023,1"))
        crossing_dggs_linestring.add_cell(DggsCell("07024,0"))
        crossing_dggs_shape_linestring = DggsShape(crossing_dggs_linestring, DggsShapeLocation.ONE_FACE)
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_linestring, crossing_dggs_shape_linestring, DggsAnalysisType.CROSSES))
        # Compare the linestring with a disjoint linestring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_linestring, DggsTestDataHexagonal.disjoint_dggs_shape_linestring, DggsAnalysisType.CROSSES))
        # A polygon can never cross a polygon
        # Compare the polygon with a partially overlapping polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_polygon, DggsTestDataHexagonal.overlapping_polygon_dggs_shape_polygon, DggsAnalysisType.CROSSES))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_polygon, DggsTestDataHexagonal.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.CROSSES))
        
    def test_compare_dggs_shapes_overlaps(self):
        dggs = Eaggr(Model.ISEA3H)
        # Test the OVERLAPS spatial analysis
        # Compare the cell with an overlapping child cell
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_cell, DggsTestDataHexagonal.overlapping_child_dggs_shape_cell, DggsAnalysisType.OVERLAPS))
        # Compare the cell with a disjoint cell
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_cell, DggsTestDataHexagonal.disjoint_dggs_shape_cell, DggsAnalysisType.OVERLAPS))
        # Compare the linestring with a linestring that partially overlaps
        overlapping_dggs_linestring = DggsLinestring()
        overlapping_dggs_linestring.add_cell(DggsCell("07021,4"))
        overlapping_dggs_linestring.add_cell(DggsCell("07021,2"))
        overlapping_dggs_linestring.add_cell(DggsCell("07023,1"))
        overlapping_dggs_shape_linestring = DggsShape(overlapping_dggs_linestring, DggsShapeLocation.ONE_FACE)
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_linestring, overlapping_dggs_shape_linestring, DggsAnalysisType.OVERLAPS))
        # Compare the linestring with a disjoint linestring
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_linestring, DggsTestDataHexagonal.disjoint_dggs_shape_linestring, DggsAnalysisType.OVERLAPS))
        # Compare the polygon with a partially overlapping polygon
        self.assertTrue(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_polygon, DggsTestDataHexagonal.overlapping_polygon_dggs_shape_polygon, DggsAnalysisType.OVERLAPS))
        # Compare the polygon with a disjoint polygon
        self.assertFalse(dggs.compare_dggs_shapes(DggsTestDataHexagonal.dggs_shape_polygon, DggsTestDataHexagonal.disjoint_polygon_dggs_shape_polygon, DggsAnalysisType.OVERLAPS))
        