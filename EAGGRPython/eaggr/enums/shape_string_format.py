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


## Checks the input corresponds to one of the ShapeStringFormat values.
#  @param value Input to check.
#  @throw ValueError Thrown if the input is not a shape string format.
def check_shape_string_format(value):
    if not (isinstance(value, int) and
            value >= ShapeStringFormat.WKT and value <= ShapeStringFormat.GEO_JSON):
        raise ValueError('Shape string format must be one of the ShapeStringFormat values')


## Different shape string formats supported by the library.
class ShapeStringFormat:

    WKT = 0
    GEO_JSON = 1
