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

import sys
import unittest
import xmlrunner

## Runs all tests in the modules of "tests" package that match the pattern "test_*.py".
#  Results are output to an xUnit compatible XML file in a directory called "reportsXX",
#  where "XX" is the version number of the Python interpreter used to run the tests.
if __name__ == '__main__':
    report_dir = 'reports' + str(sys.version_info[0]) + str(sys.version_info[1])
    loader = unittest.TestLoader()
    tests = loader.discover('.')
    runner = xmlrunner.XMLTestRunner(output=report_dir)
    runner.run(tests)
