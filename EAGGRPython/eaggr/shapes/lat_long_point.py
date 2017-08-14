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


## WGS84 lat/long coordinate with an accuracy in metres squared.
class LatLongPoint(object):

    ## Sets the latitude, longitude and accuracy of the point.
    #  @param latitude Latitude of the point (in degrees).
    #  @param longitude Longitude of the point (in degrees).
    #  @param accuracy Accuracy of the point (in metres squared).
    def __init__(self, latitude, longitude, accuracy):
        self._latitude = latitude
        self._longitude = longitude
        self._accuracy = accuracy

    ## @return Latitude of the point (in degrees).
    def get_latitude(self):
        return self._latitude

    ## @return Longitude of the point (in degrees).
    def get_longitude(self):
        return self._longitude

    ## @return Accuracy of the point (in metres squared).
    def get_accuracy(self):
        return self._accuracy

    ## @return True if points have the same latitude, longitude and accuracy values,
    #          otherwise false.
    def __eq__(self, other):
        return (self._latitude == other.get_latitude() and
                self._longitude == other.get_longitude() and
                self._accuracy == other.get_accuracy())

    ## @return Opposite of the == operator.
    def __ne__(self, other):
        return not (self == other)
