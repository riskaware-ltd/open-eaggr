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


## Checks the input corresponds to one of the DggsShapeLocation values.
#  @param value Input to check.
#  @throw ValueError Thrown if the input is not a DGGS shape location.
def check_dggs_shape_location(value):
    if not (isinstance(value, int) and
            value >= DggsShapeLocation.ONE_FACE and value <= DggsShapeLocation.NO_LOCATION):
        raise ValueError('DGGS shape location must be one of the DggsShapeLocation values')


## Different DGGS shape location types.
class DggsShapeLocation:

    ## Shape sits entirely on one face.
    ONE_FACE = 0
    ## Shape spans two faces.
    TWO_FACES = 1
    ## Shape spans more than two faces.
    MANY_FACES = 2
    ## Shape location only applies to cells - this value is used for linestrings and polygons.
    NO_LOCATION = 3
