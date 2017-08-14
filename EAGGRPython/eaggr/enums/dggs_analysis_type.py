"""
Copyright (c) Riskaware 2016

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


## Different DGGS analysis types.
class DggsAnalysisType:

    CONTAINS = 0 
    COVERED_BY = 1
    COVERS = 2
    CROSSES = 3 
    DISJOINT = 4 
    EQUALS = 5
    INTERSECTS = 6 
    OVERLAPS = 7
    TOUCHES = 8 
    WITHIN = 9
