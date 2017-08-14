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
import sys
import unittest
import xmlrunner
from eaggr.eaggr import Eaggr
from eaggr.enums.dggs_return_code import DggsReturnCode
from eaggr.enums.dggs_shape_location import DggsShapeLocation
from eaggr.enums.dggs_shape_type import DggsShapeType
from eaggr.enums.model import Model
from eaggr.enums.shape_string_format import ShapeStringFormat
from eaggr.exceptions.eaggr_exception import EaggrException
from eaggr.shapes.dggs_cell import DggsCell
from eaggr.shapes.dggs_linestring import DggsLinestring
from eaggr.shapes.dggs_polygon import DggsPolygon
from eaggr.shapes.dggs_shape import DggsShape
from eaggr.shapes.lat_long_point import LatLongPoint
from eaggr.shapes.lat_long_linestring import LatLongLinestring
from eaggr.shapes.lat_long_polygon import LatLongPolygon


class DggsPythonWheelTest(unittest.TestCase):
    """Tests to ensure the DGGS Python Library wheel package has installed successfully."""

    _EXPECTED_VERSION = '2.0'

    def test_package_version(self):
        """Checks the version of the DGGS Python package."""
        import eaggr
        self.assertEqual(eaggr.__version__, self._EXPECTED_VERSION)

    def test_wheel_install(self):
        """Calls a function in the main class to ensure the source files and the
        DLLs have been installed properly.
        """
        version = Eaggr(Model.ISEA4T).get_version()
        self.assertEqual(version, 'v' + self._EXPECTED_VERSION)

    def test_import_classes(self):
        """Calls the constructor of every client-facing class in the library to
        ensure all the classes are successfully imported. Without this test some
        of the imports would be unused and possibly optimised out.
        """
        # Enumerated Types
        DggsReturnCode()
        DggsShapeLocation()
        DggsShapeType()
        Model()
        ShapeStringFormat()
        # Exceptions
        EaggrException(DggsReturnCode.DGGS_SUCCESS, '')
        # Shapes
        DggsCell('0123')
        DggsLinestring()
        DggsPolygon(DggsLinestring())
        DggsShape(DggsLinestring(), DggsShapeLocation.NO_LOCATION)
        LatLongPoint(0.0, 0.0, 0.0)
        LatLongLinestring()
        LatLongPolygon(LatLongLinestring())
        # Main class
        Eaggr(Model.ISEA3H)


if __name__ == '__main__':
    proj_path = os.path.join(os.path.dirname(__file__), '..')
    report_dir = 'reports' + str(sys.version_info[0]) + str(sys.version_info[1])
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output=os.path.join(proj_path, report_dir)))
