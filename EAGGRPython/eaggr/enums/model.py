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


## Checks the input corresponds to one of the Model values.
#  @param value Input to check.
#  @throw ValueError Thrown if the input is not a DGGS model type.
def check_model(value):
    if not (isinstance(value, int) and
            value >= Model.ISEA4T and value <= Model.ISEA3H):
        raise ValueError('DGGS model type must be one of the Model values')


## Used to specify the model type to use for the DGGS.
class Model:

    ISEA4T = 0
    ISEA3H = 1
