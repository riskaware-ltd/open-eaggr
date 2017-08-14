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


## Unit tests for the LatLongPoint class.
class TestLatLongPoint(unittest.TestCase):

    def test_getters(self):
        latitude = 1.0
        longitude = 2.0
        accuracy = 3.0
        point = LatLongPoint(latitude, longitude, accuracy)
        self.assertEqual(point.get_latitude(), latitude)
        self.assertEqual(point.get_longitude(), longitude)
        self.assertEqual(point.get_accuracy(), accuracy)

    def test_operators(self):
        orig_point = LatLongPoint(1.0, 2.0, 3.0)
        same_point = LatLongPoint(1.0, 2.0, 3.0)
        diff_point = LatLongPoint(4.0, 5.0, 6.0)
        self.assertTrue(orig_point == same_point)
        self.assertFalse(orig_point == diff_point)
        self.assertEqual(orig_point, same_point)
        self.assertFalse(orig_point != same_point)
        self.assertTrue(orig_point != diff_point)
        self.assertNotEqual(orig_point, diff_point)
