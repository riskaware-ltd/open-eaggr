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

from ctypes import c_double, c_ushort, c_int, c_char, c_char_p, POINTER, Structure, Union, Array, cast
from eaggr.enums.dggs_shape_type import DggsShapeType
from eaggr.shapes.lat_long_point import LatLongPoint
from eaggr.shapes.lat_long_linestring import LatLongLinestring
from eaggr.shapes.lat_long_polygon import LatLongPolygon
from eaggr.shapes.dggs_cell import DggsCell
from eaggr.shapes.dggs_linestring import DggsLinestring
from eaggr.shapes.dggs_polygon import DggsPolygon
from eaggr.shapes.dggs_shape import DggsShape


def get_LAT_LONG_SHAPE_array(shapes):
    if not isinstance(shapes, list):
        raise ValueError("Argument must be a list containing only LatLongPoint, "
                         "LatLongLinestring and LatLongPolygon objects")
    output_array = (LAT_LONG_SHAPE * len(shapes))()
    array_index = 0
    for shape in shapes:
        if isinstance(shape, LatLongPoint):
            point = LAT_LONG_POINT()
            point.from_lat_long_point(shape)
            output_array[array_index] = LAT_LONG_SHAPE(0, LAT_LONG_SHAPE_DATA(m_point=point))
        elif isinstance(shape, LatLongLinestring):
            linestring = LAT_LONG_LINESTRING()
            linestring.from_lat_long_linestring(shape)
            output_array[array_index] = LAT_LONG_SHAPE(1, LAT_LONG_SHAPE_DATA(m_linestring=linestring))
        elif isinstance(shape, LatLongPolygon):
            polygon = LAT_LONG_POLYGON()
            polygon.from_lat_long_polygon(shape)
            output_array[array_index] = LAT_LONG_SHAPE(2, LAT_LONG_SHAPE_DATA(m_polygon=polygon))
        else:
            raise ValueError("Element " + str(array_index) + " in the list is not a "
                             "LatLongPoint, LatLongLinestring or LatLongPolygon object")
        array_index += 1
    return output_array


def get_LAT_LONG_POINT_array(points):
    if not isinstance(points, list):
        raise ValueError("Argument must be a list containing only LatLongPoint objects")
    output_array = (LAT_LONG_POINT * len(points))()
    array_index = 0
    for point in points:
        if isinstance(point, LatLongPoint):
            output_array[array_index] = LAT_LONG_POINT()
            output_array[array_index].from_lat_long_point(point)
        else:
            raise ValueError("Element " + str(array_index) + " in the list is not a "
                             "LatLongPoint object")
        array_index += 1
    return output_array


class LAT_LONG_POINT(Structure):

    _fields_ = [('m_latitude', c_double),
                ('m_longitude', c_double),
                ('m_accuracy', c_double)]

    def from_lat_long_point(self, lat_long_point):
        if not isinstance(lat_long_point, LatLongPoint):
            raise ValueError("Argument must be a LatLongPoint object")
        try:
            self.m_latitude = lat_long_point.get_latitude()
            self.m_longitude = lat_long_point.get_longitude()
            self.m_accuracy = lat_long_point.get_accuracy()
        except TypeError:
            raise ValueError("Latitude, longitude and accuracy values for a "
                             "LatLongPoint object must all be numeric")

    def to_lat_long_point(self):
        return LatLongPoint(self.m_latitude, self.m_longitude, self.m_accuracy)


class LAT_LONG_LINESTRING(Structure):

    _fields_ = [('m_points', POINTER(LAT_LONG_POINT)),
                ('m_noOfPoints', c_ushort)]

    def from_lat_long_linestring(self, lat_long_linestring):
        if not isinstance(lat_long_linestring, LatLongLinestring):
            raise ValueError("Argument must be a LatLongLinestring object")
        no_of_points = len(lat_long_linestring.get_points())
        self.m_noOfPoints = no_of_points
        self.m_points = (LAT_LONG_POINT * no_of_points)()
        array_index = 0
        for point in lat_long_linestring.get_points():
            self.m_points[array_index].from_lat_long_point(point)
            array_index += 1


class LAT_LONG_POLYGON(Structure):

    _fields_ = [('m_outerRing', LAT_LONG_LINESTRING),
                ('m_innerRings', POINTER(LAT_LONG_LINESTRING)),
                ('m_noOfInnerRings', c_ushort)]

    def from_lat_long_polygon(self, lat_long_polygon):
        if not isinstance(lat_long_polygon, LatLongPolygon):
            raise ValueError("Argument must be a LatLongPolygon object")
        self.m_outerRing = LAT_LONG_LINESTRING()
        self.m_outerRing.from_lat_long_linestring(lat_long_polygon.get_outer_ring())
        no_of_inner_rings = len(lat_long_polygon.get_inner_rings())
        self.m_noOfInnerRings = no_of_inner_rings
        self.m_innerRings = (LAT_LONG_LINESTRING * no_of_inner_rings)()
        array_index = 0
        for ring in lat_long_polygon.get_inner_rings():
            self.m_innerRings[array_index].from_lat_long_linestring(ring)
            array_index += 1


class LAT_LONG_SHAPE_DATA(Union):

    _fields_ = [('m_point', LAT_LONG_POINT),
                ('m_linestring', LAT_LONG_LINESTRING),
                ('m_polygon', LAT_LONG_POLYGON)]


class LAT_LONG_SHAPE(Structure):

    _fields_ = [('m_type', c_int),
                ('m_data', LAT_LONG_SHAPE_DATA)]


def get_DGGS_CELL_array(dggs_cells):
    if not isinstance(dggs_cells, list):
        raise ValueError("Argument must be a list containing only DggsCell objects")
    output_array = (DGGS_CELL * len(dggs_cells))()
    array_index = 0
    for cell in dggs_cells:
        if not isinstance(cell, DggsCell):
            raise ValueError("Element " + str(array_index) + " in the list is not a DggsCell object")
        output_array[array_index] = DGGS_CELL()
        output_array[array_index].from_dggs_cell(cell)
        array_index += 1
    return output_array


class DGGS_CELL(Array):

    _type_ = c_char
    _length_ = 43

    def from_dggs_cell(self, dggs_cell):
        if not isinstance(dggs_cell, DggsCell):
            raise ValueError("Argument must be a DggsCell object")
        cell_id = dggs_cell.get_cell_id().encode('utf-8')
        for id_index in range(0, len(cell_id)):
            self[id_index] = cell_id[id_index]

    def to_dggs_cell(self):
        return DggsCell(cast(self, c_char_p).value.decode('utf-8'))


class DGGS_LINESTRING(Structure):

    _fields_ = [('m_cells', POINTER(DGGS_CELL)),
                ('m_noOfCells', c_ushort)]
    
    def from_dggs_linestring(self, dggs_linestring):
        if not isinstance(dggs_linestring, DggsLinestring):
            raise ValueError("Argument must be a DggsLinestring object")
        self.m_noOfCells = len(dggs_linestring.get_cells())
        self.m_cells = (DGGS_CELL * self.m_noOfCells)()
        for cell_index in range(0, self.m_noOfCells):
            self.m_cells[cell_index] = DGGS_CELL()
            self.m_cells[cell_index].from_dggs_cell(dggs_linestring.get_cells()[cell_index])

    def to_dggs_linestring(self):
        dggs_linestring = DggsLinestring()
        for cell_index in range(0, self.m_noOfCells):
            dggs_linestring.add_cell(self.m_cells[cell_index].to_dggs_cell())
        return dggs_linestring


class DGGS_POLYGON(Structure):

    _fields_ = [('m_outerRing', DGGS_LINESTRING),
                ('m_innerRings', POINTER(DGGS_LINESTRING)),
                ('m_noOfInnerRings', c_ushort)]

    def from_dggs_polygon(self, dggs_polygon):
        if not isinstance(dggs_polygon, DggsPolygon):
            raise ValueError("Argument must be a DggsPolygon object")
        self.m_noOfInnerRings = len(dggs_polygon.get_inner_rings())
        self.m_outerRing = DGGS_LINESTRING()
        self.m_outerRing.from_dggs_linestring(dggs_polygon.get_outer_ring()) 
        self.m_innerRings = (DGGS_LINESTRING * self.m_noOfInnerRings)()     
        for ring_index in range(0, self.m_noOfInnerRings):
            self.m_innerRings[ring_index] = DGGS_LINESTRING()
            self.m_innerRings[ring_index].from_dggs_linestring(dggs_polygon.get_inner_rings()[ring_index])
        
    def to_dggs_polygon(self):
        outer_ring = self.m_outerRing.to_dggs_linestring()
        dggs_polygon = DggsPolygon(outer_ring)
        for ring_index in range(0, self.m_noOfInnerRings):
            inner_ring = self.m_innerRings[ring_index].to_dggs_linestring()
            dggs_polygon.add_inner_ring(inner_ring)
        return dggs_polygon


class DGGS_SHAPE_DATA(Union):

    _fields_ = [('m_cell', DGGS_CELL),
                ('m_linestring', DGGS_LINESTRING),
                ('m_polygon', DGGS_POLYGON)]

class DGGS_SHAPE(Structure):

    _fields_ = [('m_type', c_int),
                ('m_data', DGGS_SHAPE_DATA),
                ('m_location', c_int)]
   
    def from_dggs_shape(self, dggs_shape):
        if not isinstance(dggs_shape, DggsShape):
            raise ValueError("Argument must be a DggsShape object")
        try:
            self.m_type = dggs_shape.get_shape_type()
            self.m_location = dggs_shape.get_location()
            shape_type = dggs_shape.get_shape_type()
            if shape_type == DggsShapeType.CELL:
                self.m_data.m_cell = dggs_shape.get_shape().get_cell_id().encode('utf-8')            
            elif shape_type == DggsShapeType.LINESTRING:
                dggs_linestring = dggs_shape.get_shape()
                linestring = DGGS_LINESTRING()
                linestring.from_dggs_linestring(dggs_linestring)
                self.m_data.m_linestring.m_cells = linestring.m_cells
                self.m_data.m_linestring.m_noOfCells = linestring.m_noOfCells
            elif shape_type == DggsShapeType.POLYGON:
                dggs_polygon = dggs_shape.get_shape()
                polygon=DGGS_POLYGON()
                polygon.from_dggs_polygon(dggs_polygon)
                self.m_data.m_polygon.m_outerRing = polygon.m_outerRing
                self.m_data.m_polygon.m_innerRings = polygon.m_innerRings
                self.m_data.m_polygon.m_noOfInnerRings = polygon.m_noOfInnerRings
            else:
                raise ValueError("DGGS_SHAPE Structure contains unrecognised shape type")
        except TypeError:
            raise ValueError("Shape type and location must be integers and the shape data "
                            "must be a DGGSCell, DGGSLinestring or DGGSPolygon")

    def to_dggs_shape(self):
        if self.m_type == DggsShapeType.CELL:
            # For some reason it is not possible to call to_dggs_cell() on the DGGS_CELL object here,
            # so we must create the object with its constructor instead
            return DggsShape(DggsCell(self.m_data.m_cell.decode('utf-8')), self.m_location)
        elif self.m_type == DggsShapeType.LINESTRING:
            return DggsShape(self.m_data.m_linestring.to_dggs_linestring(), self.m_location)
        elif self.m_type == DggsShapeType.POLYGON:
            return DggsShape(self.m_data.m_polygon.to_dggs_polygon(), self.m_location)
        else:
            raise ValueError("DGGS_SHAPE Structure contains unrecognised shape type")
        
    