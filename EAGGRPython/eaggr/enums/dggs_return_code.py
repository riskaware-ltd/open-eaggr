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


## Value returned by the functions in the DLL. Indicates if the function
#  completed successfully, or the reason the function failed.
class DggsReturnCode:

    DGGS_SUCCESS = 0
    DGGS_NOT_IMPLEMENTED = 1
    DGGS_INVALID_HANDLE = 2
    DGGS_INVALID_PARAM = 3
    DGGS_NULL_POINTER = 4
    DGGS_MODEL_ERROR = 5
    DGGS_CELL_LENGTH_TOO_LONG = 6
    DGGS_MEMORY_ALLOCATION_FAILURE = 7
    DGGS_UNKNOWN_ERROR = 8
