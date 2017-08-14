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

import threading
import unittest
from eaggr.eaggr import Eaggr
from eaggr.enums.model import Model
from eaggr.shapes.lat_long_point import LatLongPoint
from eaggr.shapes.dggs_cell import DggsCell

## Tests for using the Eaggr class in a multithreaded environment.
class TestDggsPython(unittest.TestCase):

    def convert_point_to_dggs_cell_in_thread(self, latitude, longitude):
        # Create the lat/long points
        lat_long_point = LatLongPoint(latitude, longitude, 1E-5);
        # Initialise the DGGS model
        dggs = Eaggr(Model.ISEA4T)
        # Convert the lat/long point
        dggs_cell = dggs.convert_point_to_dggs_cell(lat_long_point)
        # Convert back to a lat/long point
        converted_point = dggs.convert_dggs_cell_to_point(dggs_cell)
        # Check converted point is the same as the original
        self.failUnlessAlmostEqual(lat_long_point.get_latitude(), converted_point.get_latitude(), places=3)
        self.failUnlessAlmostEqual(lat_long_point.get_longitude(), converted_point.get_longitude(), places=3)
        
    def test_multithread(self):
        # Initialise the thread objects
        for latitude in range(-16, 16):
            for longitude in range(-34, 34):
                dggsRunners = []
                dggsRunners.append(threading.Thread(self.convert_point_to_dggs_cell_in_thread(5 * latitude, 5 * longitude)))
        # Start the threads
        for runner in dggsRunners:
            runner.start()
        # Join the threads to wait for completion
        for runner in dggsRunners:
            runner.join()
