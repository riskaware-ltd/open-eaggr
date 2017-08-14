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
from eaggr.enums.dggs_return_code import DggsReturnCode
from eaggr.exceptions.eaggr_exception import EaggrException


## Unit tests for the EaggrException class.
class TestLatLongPoint(unittest.TestCase):

    def test_getters(self):
        orig_return_code = DggsReturnCode.DGGS_SUCCESS
        orig_message = 'Test message'
        exception = EaggrException(orig_return_code, orig_message)
        self.assertEqual(orig_return_code, exception.get_return_code())
        self.assertEqual(orig_message, str(exception))
