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


## Defines a DGGS cell.
class DggsCell(object):

    ## Sets the cell ID of the DGGS cell.
    #  @param cell_id ID of the cell as a string.
    def __init__(self, cell_id):
        self._cell_id = cell_id

    ## @return ID of the cell as a string.
    def get_cell_id(self):
        return self._cell_id

    ## @return True if cells have the same ID, otherwise false.
    def __eq__(self, other):
        return self._cell_id == other.get_cell_id()

    ## @return Opposite of the == operator.
    def __ne__(self, other):
        return not (self == other)
