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

from eaggr.shapes.dggs_cell import DggsCell


## Defines a linestring made up of DGGS cells.
class DggsLinestring(object):

    ## Initially the linestring contains no cells.
    #  @note Call add_cell() to add cells to the linestring.
    def __init__(self):
        self._cells = []

    ## Adds a cell to the linestring.
    #  @note Cells must be added to the linestring in the correct order.
    #  @param cell Cell to add to the linestring.
    #  @throw ValueError Thrown if input argument is not a DggsCell object.
    def add_cell(self, cell):
        if not isinstance(cell, DggsCell):
            raise ValueError("Input argument to add_cell() must be a DggsCell")
        self._cells.append(cell)

    ## @return List of cells that make up the linestring.
    def get_cells(self):
        return self._cells
