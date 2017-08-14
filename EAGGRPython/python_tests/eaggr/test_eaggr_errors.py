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
from eaggr.eaggr import Eaggr
from eaggr.enums.model import Model
from eaggr.enums.shape_string_format import ShapeStringFormat
from eaggr.exceptions.eaggr_exception import EaggrException
from eaggr.shapes.lat_long_point import LatLongPoint
from eaggr.shapes.dggs_cell import DggsCell
from eaggr.enums.dggs_return_code import DggsReturnCode


## Unit tests for the error handling in the Eaggr class.
class TestDggsErrors(unittest.TestCase):

    def test_convert_shapes_to_dggs_shapes(self):
        dggs = Eaggr(Model.ISEA4T)
        # Wrong input types
        try:
            dggs.convert_shapes_to_dggs_shapes('Not a list')
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e),
                             ('Argument must be a list containing only LatLongPoint, '
                              'LatLongLinestring and LatLongPolygon objects'))
        try:
            dggs.convert_shapes_to_dggs_shapes(['Not a LatLongShape'])
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e),
                             ('Element 0 in the list is not a LatLongPoint, LatLongLinestring '
                              'or LatLongPolygon object'))
        # Points with invalid values
        try:
            dggs.convert_shapes_to_dggs_shapes([LatLongPoint(None, 0.0, 1.0)])
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e),
                             ('Latitude, longitude and accuracy values for a LatLongPoint object '
                              'must all be numeric'))
        try:
            dggs.convert_shapes_to_dggs_shapes([LatLongPoint(360.0, 360.0, 0.0)])
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             ('EAGGR Exception: Latitude is greater than maximum allowed '
                              'for a lat/long point.'))
        # Empty list of shapes (not actually an error)
        lat_long_shapes = dggs.convert_shapes_to_dggs_shapes([])
        self.assertEqual(lat_long_shapes, [])

    def test_convert_shape_string_to_dggs_shapes(self):
        wkt_string = 'LINESTRING(2.345 1.234, 4.567 3.456)'
        string_format = ShapeStringFormat.WKT
        accuracy = 3.884
        dggs = Eaggr(Model.ISEA4T)
        # Invalid input types
        try:
            dggs.convert_shape_string_to_dggs_shapes(None, string_format, accuracy)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Invalid shape string')
        try:
            dggs.convert_shape_string_to_dggs_shapes(wkt_string, 2, accuracy)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e),
                             'Shape string format must be one of the ShapeStringFormat values')
        try:
            dggs.convert_shape_string_to_dggs_shapes(wkt_string, string_format, None)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Invalid accuracy value')

        # Empty shape string
        try:
            dggs.convert_shape_string_to_dggs_shapes('', string_format, accuracy)
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             'EAGGR Exception: Unsupported WKT: . WKT string is not valid.')
        # Syntax error in shape string
        try:
            dggs.convert_shape_string_to_dggs_shapes('LINESTRING(2.345 1.234, 4.567 X)',
                                                     string_format, accuracy)
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             ('EAGGR Exception: Unsupported WKT: LINESTRING(2.345 1.234, 4.567 X). '
                              'WKT string is not valid.'))
        # Invalid accuracy value
        try:
            dggs.convert_shape_string_to_dggs_shapes(wkt_string, string_format, 0.0)
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e), 'EAGGR Exception: Accuracy value must be > 0')

    def test_convert_dggs_cells_to_points(self):
        dggs = Eaggr(Model.ISEA4T)
        # Wrong input type
        try:
            dggs.convert_dggs_cells_to_points('Not a list')
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Argument must be a list containing only DggsCell objects')
        try:
            dggs.convert_dggs_cells_to_points(['Not a DggsCell'])
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Element 0 in the list is not a DggsCell object')
        # Invalid cell ID
        try:
            dggs.convert_dggs_cells_to_points([DggsCell('XXXXXX')])
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             "EAGGR Exception: Invalid face index, 'XX', must be positive integer")
        # Empty list of cells (not actually an error)
        lat_long_shapes = dggs.convert_dggs_cells_to_points([])
        self.assertEqual(lat_long_shapes, [])

    def test_convert_dggs_cells_to_shape_string(self):
        dggs_cells = [DggsCell('01230123')]
        string_format = ShapeStringFormat.WKT
        dggs = Eaggr(Model.ISEA4T)
        # Wrong input type
        try:
            dggs.convert_dggs_cells_to_shape_string('Not a list', string_format)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Argument must be a list containing only DggsCell objects')
        try:
            dggs.convert_dggs_cells_to_shape_string(['Not a DggsCell'], string_format)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Element 0 in the list is not a DggsCell object')
        # Invalid cell ID
        try:
            dggs.convert_dggs_cells_to_shape_string([DggsCell('XXXXXX')], string_format)
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             "EAGGR Exception: Invalid face index, 'XX', must be positive integer")
        # Empty list of cells (not actually an error)
        shape_string = dggs.convert_dggs_cells_to_shape_string([], string_format)
        self.assertEqual(shape_string, 'MULTIPOINT EMPTY')
        # Invalid shape string format
        try:
            dggs.convert_dggs_cells_to_shape_string(dggs_cells, 2)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e),
                             'Shape string format must be one of the ShapeStringFormat values')

    def test_get_dggs_cell_parents(self):
        dggs = Eaggr(Model.ISEA4T)
        # Wrong input type
        try:
            dggs.get_dggs_cell_parents('Not a DggsCell')
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Argument must be a DggsCell object')
        # Invalid cell ID
        try:
            dggs.get_dggs_cell_parents(DggsCell('XXXXXX'))
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             "EAGGR Exception: Invalid face index, 'XX', must be positive integer")
        # Cell representing a face - no parent cell
        try:
            dggs.get_dggs_cell_parents(DggsCell('01'))
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            ## @todo Improve the error message given by the DLL for this error.
            #        Clearly there is a wrap-around issue with the resolution value.
            #        Issue #17 raised for this
            self.assertEqual(str(e),
                             ('EAGGR Exception: Unable to create cell at resolution 65535 '
                              'as it is greater than the upper limit (40).'))

    def test_get_dggs_cell_children(self):
        dggs = Eaggr(Model.ISEA4T)
        # Wrong input type
        try:
            dggs.get_dggs_cell_children('Not a DggsCell')
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Argument must be a DggsCell object')
        # Invalid cell ID
        try:
            dggs.get_dggs_cell_children(DggsCell('XXXXXX'))
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             "EAGGR Exception: Invalid face index, 'XX', must be positive integer")
        # Children which exceed the maximum resolution
        try:
            dggs.get_dggs_cell_children(
                DggsCell('012301230123012301230123012301230123012301'))
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             ('EAGGR Exception: Unable to create cell at resolution 41 '
                              'as it is greater than the upper limit (40).'))
            
    def test_get_dggs_cell_siblings(self):
        dggs = Eaggr(Model.ISEA4T)
        # Wrong input type
        try:
            dggs.get_dggs_cell_siblings('Not a DggsCell')
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Argument must be a DggsCell object')
        # Invalid cell ID
        try:
            dggs.get_dggs_cell_siblings(DggsCell('XXXXXX'))
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             "EAGGR Exception: Invalid face index, 'XX', must be positive integer")
        # Cell representing a face - no parent cell
        try:
            dggs.get_dggs_cell_parents(DggsCell('01'))
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            ## @todo Improve the error message given by the DLL for this error.
            #        Clearly there is a wrap-around issue with the resolution value.
            #        Issue #17 raised for this
            self.assertEqual(str(e),
                             ('EAGGR Exception: Unable to create cell at resolution 65535 '
                              'as it is greater than the upper limit (40).'))
        # Cell exceeds maximum resolution
        try:
            dggs.get_dggs_cell_children(
                DggsCell('012301230123012301230123012301230123012301'))
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             ('EAGGR Exception: Unable to create cell at resolution 41 '
                              'as it is greater than the upper limit (40).'))

    def test_get_bounding_dggs_cell(self):
        dggs = Eaggr(Model.ISEA4T)
        # Wrong input type
        try:
            dggs.get_bounding_dggs_cell('Not a list')
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Argument must be a list containing only DggsCell objects')
        try:
            dggs.get_bounding_dggs_cell(['Not a DggsCell'])
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Element 0 in the list is not a DggsCell object')
        # Invalid cell ID
        try:
            dggs.get_bounding_dggs_cell([DggsCell('XXXXXX')])
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             "EAGGR Exception: Invalid face index, 'XX', must be positive integer")
        # Empty list of cells
        try:
            dggs.get_bounding_dggs_cell([])
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e), 'EAGGR Exception: No cells provided to determine uniqueness')
        # Cells on different faces - no bounding cell
        try:
            dggs.get_bounding_dggs_cell([DggsCell('0100'), DggsCell('0200')])
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_INVALID_PARAM)
            self.assertEqual(str(e), 'Supplied cells do not have a common parent.')

    def test_create_dggs_kml_file(self):
        dggs = Eaggr(Model.ISEA4T)
        kml_filename = 'KmlFile.kml'
        dggs_cells = [DggsCell('0123')]
        # Wrong input types
        try:
            dggs.create_dggs_kml_file(None, dggs_cells)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Invalid filename for KML file')
        try:
            dggs.create_dggs_kml_file(kml_filename, 'Not a list')
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Argument must be a list containing only DggsCell objects')
        try:
            dggs.create_dggs_kml_file(kml_filename, ['Not a DggsCell'])
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Element 0 in the list is not a DggsCell object')
        ## @todo Fix the DLL so that this error is handled.
        """
        # Directory does not exist
        try:
            eaggr.create_dggs_kml_file(os.path.join('not', 'a', 'directory'), dggs_cells)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), '???')
        """
        # Invalid cell ID
        try:
            dggs.create_dggs_kml_file(kml_filename, [DggsCell('XXXXXX')])
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             "EAGGR Exception: Invalid face index, 'XX', must be positive integer")

    def test_get_dggs_cell_shape_string(self):
        dggs_cells = DggsCell('01230123')
        string_format = ShapeStringFormat.WKT
        dggs = Eaggr(Model.ISEA4T)
        # No cell
        try:
            dggs.convert_dggs_cell_outline_to_shape_string(None, string_format)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e), 'Argument must be a DggsCell object')
        # Invalid cell ID
        try:
            dggs.convert_dggs_cell_outline_to_shape_string(DggsCell('XXXXXX'), string_format)
            self._check_exception_thrown()
        except EaggrException as e:
            self.assertEqual(e.get_return_code(), DggsReturnCode.DGGS_MODEL_ERROR)
            self.assertEqual(str(e),
                             "EAGGR Exception: Invalid face index, 'XX', must be positive integer")
        # Invalid shape string format
        try:
            dggs.convert_dggs_cell_outline_to_shape_string(dggs_cells, 2)
            self._check_exception_thrown()
        except ValueError as e:
            self.assertEqual(str(e),
                             'Shape string format must be one of the ShapeStringFormat values')

    def _check_exception_thrown(self):
        self.fail('Should not reach this line - code before should throw an exception')
