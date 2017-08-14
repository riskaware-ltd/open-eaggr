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
from eaggr.shapes.lat_long_point import LatLongPoint
from eaggr.shapes.lat_long_linestring import LatLongLinestring


## Unit tests for the LatLongLinestring class.
class TestLatLongLinestring(unittest.TestCase):

    def test_getters(self):
        orig_points = [
            LatLongPoint(1.0, 2.0, 3.0),
            LatLongPoint(4.0, 5.0, 6.0)]
        linestring = LatLongLinestring()
        linestring.add_point(orig_points[0])
        linestring.add_point(orig_points[1])
        points = linestring.get_points()
        self.assertEqual(len(points), 2)
        self.assertEqual(orig_points[0], points[0])
        self.assertEqual(orig_points[1], points[1])

    def test_errors(self):
        linestring = LatLongLinestring()
        with self.assertRaises(ValueError) as cm:
            linestring.add_point('Not a LatLongPoint')
