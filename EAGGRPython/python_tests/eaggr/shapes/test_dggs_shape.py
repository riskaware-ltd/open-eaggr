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

import unittest
from eaggr.enums.dggs_shape_location import DggsShapeLocation
from eaggr.enums.dggs_shape_type import DggsShapeType
from eaggr.shapes.dggs_cell import DggsCell
from eaggr.shapes.dggs_linestring import DggsLinestring
from eaggr.shapes.dggs_polygon import DggsPolygon
from eaggr.shapes.dggs_shape import DggsShape


## Unit tests for the DggsShape class.
class TestDggsShape(unittest.TestCase):

    def test_getters(self):
        orig_cells = [
            DggsCell('0000'),
            DggsCell('1111')]
        # Cell
        shape = DggsShape(orig_cells[0], DggsShapeLocation.ONE_FACE)
        self.assertEqual(shape.get_shape_type(), DggsShapeType.CELL)
        self.assertEqual(shape.get_shape(), orig_cells[0])
        self.assertEqual(shape.get_location(), DggsShapeLocation.ONE_FACE)
        # Linestring
        linestring = DggsLinestring()
        linestring.add_cell(orig_cells[1])
        shape = DggsShape(linestring, DggsShapeLocation.NO_LOCATION)
        self.assertEqual(shape.get_shape_type(), DggsShapeType.LINESTRING)
        linestring_cells = shape.get_shape().get_cells()
        self.assertEqual(len(linestring_cells), 1)
        self.assertEqual(linestring_cells[0], orig_cells[1])
        self.assertEqual(shape.get_location(), DggsShapeLocation.NO_LOCATION)
        # Polygon
        shape = DggsShape(DggsPolygon(linestring), DggsShapeLocation.NO_LOCATION)
        self.assertEqual(shape.get_shape_type(), DggsShapeType.POLYGON)
        outer_ring_cells = shape.get_shape().get_outer_ring().get_cells()
        self.assertEqual(len(outer_ring_cells), 1)
        self.assertEqual(outer_ring_cells[0], orig_cells[1])
        self.assertEqual(shape.get_location(), DggsShapeLocation.NO_LOCATION)

    def test_errors(self):
        # Invalid shape
        with self.assertRaises(ValueError) as cm:
            DggsShape('Invalid shape', DggsShapeLocation.NO_LOCATION)
        # Invalid location type
        with self.assertRaises(ValueError) as cm:
            DggsShape(DggsCell('0000'), 'Not a DggsShapeLocation')
