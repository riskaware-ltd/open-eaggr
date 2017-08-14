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

from eaggr.enums.dggs_shape_type import DggsShapeType
from eaggr.enums.dggs_shape_location import check_dggs_shape_location
from eaggr.shapes.dggs_cell import DggsCell
from eaggr.shapes.dggs_linestring import DggsLinestring
from eaggr.shapes.dggs_polygon import DggsPolygon


## Shape defined by DGGS cells.
#
#  The class wraps one of a DggsCell, DggsLinestring or DggsPolygon 
#  (depending on the type of the lat/long shape that was coverted by the library).
#  The type of shape stored can be found from the get_shape_type() method.
class DggsShape(object):

    ## Sets the shape, along with its type and location.
    #  @param shape Must be DggsCell, DggsLinestring or DggsPolygon.
    #  @param location Must be one of the DggsShapeLocation values.
    #  @throw ValueError Thrown if input arguments are not one of the expected types.
    def __init__(self, shape, location):
        if isinstance(shape, DggsCell):
            self._shape_type = DggsShapeType.CELL
        elif isinstance(shape, DggsLinestring):
            self._shape_type = DggsShapeType.LINESTRING
        elif isinstance(shape, DggsPolygon):
            self._shape_type = DggsShapeType.POLYGON
        else:
            raise ValueError("First input argument to DggsShape() must be DggsCell, DggsLinestring or DggsPolygon")
        self._shape = shape
        check_dggs_shape_location(location)
        self._location = location

    ## @return One of the DggsShapeType values.
    def get_shape_type(self):
        return self._shape_type

    ## @return Object of either DggsCell, DggsLinestring or DggsPolygon.
    def get_shape(self):
        return self._shape

    ## Returns a value indicating where the shape sits in relation to the faces
    #  of the polyhedral globe.
    #  @return One of the DggsShapeLocation values.
    def get_location(self):
        return self._location
