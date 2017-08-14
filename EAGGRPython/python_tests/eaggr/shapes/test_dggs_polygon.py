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
from eaggr.shapes.dggs_cell import DggsCell
from eaggr.shapes.dggs_linestring import DggsLinestring
from eaggr.shapes.dggs_polygon import DggsPolygon


## Unit tests for the DggsPolygon class.
class DggsPolygonTest(unittest.TestCase):

    def test_getters(self):
        # Create DggsPolygon object
        orig_cells = [
            DggsCell('0000'),
            DggsCell('1111'),
            DggsCell('2222')]
        linestring = DggsLinestring()
        linestring.add_cell(orig_cells[0])
        polygon = DggsPolygon(linestring)
        linestring = DggsLinestring()
        linestring.add_cell(orig_cells[1])
        polygon.add_inner_ring(linestring)
        linestring = DggsLinestring()
        linestring.add_cell(orig_cells[2])
        polygon.add_inner_ring(linestring)
        # Test DggsPolygon object
        outer_ring_cells = polygon.get_outer_ring().get_cells()
        self.assertEqual(outer_ring_cells[0], orig_cells[0])
        inner_rings = polygon.get_inner_rings()
        self.assertEqual(len(inner_rings), 2)
        inner_ring_cells = inner_rings[0].get_cells()
        self.assertEqual(inner_ring_cells[0], orig_cells[1])
        inner_ring_cells = inner_rings[1].get_cells()
        self.assertEqual(inner_ring_cells[0], orig_cells[2])

    def test_errors(self):
        # Error with outer ring
        with self.assertRaises(ValueError) as cm:
            DggsPolygon('Not a DggsLinestring')
        # Error with inner ring
        polygon = DggsPolygon(DggsLinestring())
        with self.assertRaises(ValueError) as cm:
            polygon.add_inner_ring('Not a DggsLinestring')
