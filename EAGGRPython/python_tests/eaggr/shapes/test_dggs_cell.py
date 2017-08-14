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


## Unit tests for the DggsCell class.
class TestDggsCell(unittest.TestCase):

    def test_getters(self):
        cell_id = '0123'
        cell = DggsCell(cell_id)
        self.assertEqual(cell.get_cell_id(), cell_id)

    def test_operators(self):
        orig_cell = DggsCell('0123')
        same_cell = DggsCell('0123')
        diff_cell = DggsCell('3210')
        self.assertTrue(orig_cell == same_cell)
        self.assertFalse(orig_cell == diff_cell)
        self.assertEqual(orig_cell, same_cell)
        self.assertFalse(orig_cell != same_cell)
        self.assertTrue(orig_cell != diff_cell)
        self.assertNotEqual(orig_cell, diff_cell)
