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

from eaggr.shapes.dggs_linestring import DggsLinestring


## Defines a DGGS polygon made up of an outer ring and zero or more inner rings.
#  The outer and inner rings are linestrings made up of DGGS cells.
class DggsPolygon(object):

    ## Sets the outer ring of the polygon. Initially the polygon has no inner rings.
    #  @note Call add_inner_ring() to add inner rings to the polygon.
    #  @param outer_ring DggsLinestring defining the outer ring of the polygon.
    #  @throw ValueError Thrown if input argument is not a DggsLinestring object.
    def __init__(self, outer_ring):
        if not isinstance(outer_ring, DggsLinestring):
            raise ValueError("Input argument to DggsPolygon() must be a DggsLinestring")
        self._outer_ring = outer_ring
        self._inner_rings = []

    ## Adds an inner ring to the polygon.
    #  @param inner_ring DggsLinestring defining a new inner ring for the polygon.
    #  @throw ValueError Thrown if input argument is not a DggsLinestring object.
    def add_inner_ring(self, inner_ring):
        if not isinstance(inner_ring, DggsLinestring):
            raise ValueError("Input argument to add_inner_ring() must be a DggsLinestring")
        self._inner_rings.append(inner_ring)

    ## @return DggsLinestring defining the outer ring of the polygon.
    def get_outer_ring(self):
        return self._outer_ring

    ## @return List of DggsLinestring objects which define the inner rings of the polygon.
    def get_inner_rings(self):
        return self._inner_rings
