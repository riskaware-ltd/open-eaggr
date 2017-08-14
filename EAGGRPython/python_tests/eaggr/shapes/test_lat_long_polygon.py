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
from eaggr.shapes.lat_long_polygon import LatLongPolygon


## Unit tests for the LatLongPolygon class.
class LatLongPolygonTest(unittest.TestCase):

    def test_getters(self):
        # Create LatLongPolygon object
        orig_points = [
            LatLongPoint(1.0, 2.0, 3.0),
            LatLongPoint(4.0, 5.0, 6.0),
            LatLongPoint(7.0, 8.0, 9.0)]
        linestring = LatLongLinestring()
        linestring.add_point(orig_points[0])
        polygon = LatLongPolygon(linestring)
        linestring = LatLongLinestring()
        linestring.add_point(orig_points[1])
        polygon.add_inner_ring(linestring)
        linestring = LatLongLinestring()
        linestring.add_point(orig_points[2])
        polygon.add_inner_ring(linestring)
        # Test LatLongPolygon object
        outer_ring_points = polygon.get_outer_ring().get_points()
        self.assertEqual(outer_ring_points[0], orig_points[0])
        inner_rings = polygon.get_inner_rings()
        self.assertEqual(len(inner_rings), 2)
        inner_ring_points = inner_rings[0].get_points()
        self.assertEqual(inner_ring_points[0], orig_points[1])
        inner_ring_points = inner_rings[1].get_points()
        self.assertEqual(inner_ring_points[0], orig_points[2])

    def test_errors(self):
        # Error with outer ring
        with self.assertRaises(ValueError) as cm:
            LatLongPolygon('Not a LatLongLinestring')
        # Error with inner ring
        polygon = LatLongPolygon(LatLongLinestring())
        with self.assertRaises(ValueError) as cm:
            polygon.add_inner_ring('Not a LatLongLinestring')
