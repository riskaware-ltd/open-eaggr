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

from eaggr.shapes.lat_long_point import LatLongPoint


## Defines a linestring made up of WGS84 lat/long points.
class LatLongLinestring(object):

    ## Initially the linestring contains no points.
    #  @note Call add_point() to add points to the linestring.
    def __init__(self):
        self._points = []

    ## Adds a point to the linestring.
    #  @note Points must be added to the linestring in the correct order.
    #  @param point Point to add to the linestring.
    #  @throw ValueError Thrown if input argument is not a LatLongPoint object.
    def add_point(self, point):
        if not isinstance(point, LatLongPoint):
            raise ValueError("Input argument to add_point() must be a LatLongPoint")
        self._points.append(point)

    ## @return List of points that make up the linestring.
    def get_points(self):
        return self._points
