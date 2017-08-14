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


## Unit tests for the DggsLinestring class.
class TestDggsLinestring(unittest.TestCase):

    def test_getters(self):
        orig_cells = [
            DggsCell('0000'),
            DggsCell('1111')]
        linestring = DggsLinestring()
        linestring.add_cell(orig_cells[0])
        linestring.add_cell(orig_cells[1])
        cells = linestring.get_cells()
        self.assertEqual(len(cells), 2)
        self.assertEqual(cells[0], orig_cells[0])
        self.assertEqual(cells[1], orig_cells[1])

    def test_errors(self):
        linestring = DggsLinestring()
        with self.assertRaises(ValueError) as cm:
            linestring.add_cell('Not a DggsCell')
