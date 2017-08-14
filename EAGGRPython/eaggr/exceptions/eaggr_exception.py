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


## Exception thrown if a DGGS DLL function fails.
class EaggrException(Exception):

    ## Stores an error message and the value returned by the DGGS DLL function.
    def __init__(self, return_code, message):
        # Call the base class constructor with the parameters it needs
        super(EaggrException, self).__init__(message)
        # Store the return code given by the DGGS DLL function
        self._return_code = return_code

    ## @return Return code given by the DGGS API function.
    def get_return_code(self):
        return self._return_code
