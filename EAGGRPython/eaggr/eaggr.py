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

import os
import sys
import platform

from ctypes import (
    cdll, CDLL, c_void_p, c_char_p, c_ushort, c_int, c_double,
    POINTER, byref, create_string_buffer, cast, c_bool)

# Top level of the namespace is different in this file in Python 2
if (sys.version_info < (3, 0)):
    from exceptions.eaggr_exception import EaggrException
    from enums.dggs_return_code import DggsReturnCode
    from enums.dggs_analysis_type import DggsAnalysisType
    from enums.model import check_model
    from enums.shape_string_format import check_shape_string_format
    from shapes.native_structures import (
        get_LAT_LONG_SHAPE_array, get_LAT_LONG_POINT_array, LAT_LONG_POINT, LAT_LONG_SHAPE,
        get_DGGS_CELL_array, DGGS_CELL, DGGS_SHAPE, DGGS_SHAPE_DATA)
else:
    from eaggr.exceptions.eaggr_exception import EaggrException
    from eaggr.enums.dggs_return_code import DggsReturnCode
    from eaggr.enums.dggs_analysis_type import DggsAnalysisType
    from eaggr.enums.model import check_model
    from eaggr.enums.shape_string_format import check_shape_string_format
    from eaggr.shapes.native_structures import (
        get_LAT_LONG_SHAPE_array, get_LAT_LONG_POINT_array, LAT_LONG_POINT, LAT_LONG_SHAPE,
        get_DGGS_CELL_array, DGGS_CELL, DGGS_SHAPE, DGGS_SHAPE_DATA)

## Python API for the EAGGR library.
class Eaggr(object):

    ## Number of characters in EAGGR DLL's version string (including the null
    #  terminating character).
    _EAGGR_VERSION_STRING_LENGTH = 5

    ## Maximum number of parents a DGGS cell can have.
    _EAGGR_MAX_PARENT_CELLS = 3
    ## Maximum number of children a DGGS cell can have.
    _EAGGR_MAX_CHILD_CELLS = 7
    ## Maximum number of children a DGGS cell can have.
    _EAGGR_MAX_SIBLING_CELLS = 15

    ## Handle to the Proj.4 library DLL.
    _proj_dll_handle = None
    ## Handle to the GDAL library DLL.
    _gdal_dll_handle = None
    ## Handle to the EAGGR DLL.
    _eaggr_dll_handle = None
    ## Reference to the ctypes library object. Used to call the functions in the EAGGR DLL.
    _eaggr_dll = None

    ## Loads the necessary DLLs and opens a handle to the EAGGR library.
    #  @param dggs_model Specifies the type of DGGS model to use (enums.Model).
    #  @throw EaggrException Thrown if unable to open a handle to the EAGGR library.
    #  @throw IOError Thrown if one of the DLLs cannot be found.
    def __init__(self, dggs_model):
        # Check input argument
        check_model(dggs_model)
        # Open the DLLs if they are not already open
        if Eaggr._eaggr_dll is None:
            self._open_dlls()
        # Open a handle to the DGGS model
        self._dggs_handle = c_void_p()
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_OpenDggsHandle')
        func.argtypes = [c_int, POINTER(c_void_p)]
        func.restype = c_int
        return_code = func(dggs_model, byref(self._dggs_handle))
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())

    ## Closes the handle to the model and frees the resources allocated to the DLLs.
    def __del__(self):
        if Eaggr._eaggr_dll is not None and self._dggs_handle is not None:
            # Call the DLL to close the handle to the DGGS
            func = getattr(Eaggr._eaggr_dll, 'EAGGR_CloseDggsHandle')
            func.argtypes = [c_void_p]
            func.restype = c_int
            # Return code is ignored - cannot do anything about it in a destructor
            func(self._dggs_handle)

    ## Gets the version of the DLL as a string.
    #  @return Version of the DLL as a string.
    #  @throw EaggrException Thrown if unable to get the version number.
    def get_version(self):
        # Set up the arguments to the DLL function
        version = create_string_buffer(Eaggr._EAGGR_VERSION_STRING_LENGTH)
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_GetVersion')
        func.argtypes = [c_char_p]
        func.restype = c_int
        return_code = func(version)
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, "Failed to get version number of EAGGR DLL")
        return version.value.decode('utf-8')

    ## Converts a single point in lat / long coordinates into a DGGS cell.
    #  @param point Point defined by lat / long coordinates.
    #  @return DGGS cell.
    #  @throw EaggrException Thrown if unable to convert the lat / long point.
    def convert_point_to_dggs_cell(self, point):
        # Set up the arguments to the DLL function (create array of one point)
        lat_long_points = get_LAT_LONG_POINT_array([point])
        no_of_points = c_ushort(1)
        output_cells = (DGGS_CELL * no_of_points.value)()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_ConvertPointsToDggsCells')
        func.argtypes = [c_void_p, POINTER(LAT_LONG_POINT), c_ushort, POINTER(DGGS_CELL)]
        func.restype = c_int
        return_code = func(self._dggs_handle, lat_long_points, no_of_points, output_cells)
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Return the first (and only) DGGS cell in the list
        return output_cells[0].to_dggs_cell()

    ## Converts a list of shapes in lat / long coordinates into a list of
    #  shapes defined by DGGS cells.
    #  @param shapes List of shapes defined by lat / long coordinates.
    #  @return List of DGGS shapes, with the same number of elements as the input list.
    #  @throw EaggrException Thrown if unable to convert the lat / long shapes.
    def convert_shapes_to_dggs_shapes(self, shapes):
        # Set up the arguments to the DLL function
        lat_long_shapes = get_LAT_LONG_SHAPE_array(shapes)
        no_of_shapes = c_ushort(len(shapes))
        output_shapes = POINTER(DGGS_SHAPE)()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_ConvertShapesToDggsShapes')
        func.argtypes = [c_void_p, POINTER(LAT_LONG_SHAPE), c_ushort,
                         POINTER(POINTER(DGGS_SHAPE))]
        func.restype = c_int
        return_code = func(self._dggs_handle, lat_long_shapes, no_of_shapes, byref(output_shapes))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Process the output data
        dggs_shapes = []
        for shape_index in range(0, no_of_shapes.value):
            dggs_shapes.append(output_shapes[shape_index].to_dggs_shape())
        # Deallocate memory for the output data
        self._deallocate_dggs_shapes(output_shapes, no_of_shapes.value)
        # Return the array of DGGS shapes
        return dggs_shapes

    ## Converts a shape string into a list of shapes defined by DGGS cells.
    #  @param string String containing shape information in one of the supported formats.
    #  @param string_format Format used for the string (enums.ShapeStringFormat).
    #  @param accuracy Defines an area of accuracy (in metres squared) to use for every point in the string.
    #  @return List containing the input shapes defined by DGGS cells.
    #  @throw EaggrException Thrown if unable to convert the shape string.
    def convert_shape_string_to_dggs_shapes(self, string, string_format, accuracy):
        # Check requested string format is valid
        check_shape_string_format(string_format)
        # Set up the arguments to the DLL function
        try:
            shape_string = cast(string.encode('utf-8'), c_char_p).value
        except AttributeError:
            raise ValueError('Invalid shape string')
        output_shapes = POINTER(DGGS_SHAPE)()
        no_of_shapes = c_ushort()
        try:
            input_accuracy = c_double(accuracy)
        except TypeError:
            raise ValueError('Invalid accuracy value')
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_ConvertShapeStringToDggsShapes')
        func.argtypes = [c_void_p, c_char_p, c_int, c_double,
                         POINTER(POINTER(DGGS_SHAPE)), POINTER(c_ushort)]
        func.restype = c_int
        return_code = func(self._dggs_handle,
                           shape_string,
                           string_format,
                           input_accuracy,
                           byref(output_shapes),
                           byref(no_of_shapes))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Process the output data
        dggs_shapes = []
        for shape_index in range(0, no_of_shapes.value):
            dggs_shapes.append(output_shapes[shape_index].to_dggs_shape())
        # Deallocate memory for output data
        self._deallocate_dggs_shapes(output_shapes, no_of_shapes.value)
        # Return the array of DGGS shapes
        return dggs_shapes

    ## Converts a single DGGS cell into a point with lat / long coordinates.
    #  @param cell DGGS cell.
    #  @return Lat / long point.
    #  @throw EaggrException Thrown if unable to convert the DGGS cell.
    def convert_dggs_cell_to_point(self, cell):
        # Call convert_dggs_cells_to_points() with just one cell
        lat_long_points = self.convert_dggs_cells_to_points([cell])
        return lat_long_points[0]

    ## Converts a list of DGGS cells into a list of points with lat / long coordinates.
    #  @param cells List of DGGS cells.
    #  @return List of lat / long points.
    #  @throw EaggrException Thrown if unable to convert the DGGS cells.
    def convert_dggs_cells_to_points(self, cells):
        # Set up the arguments to the DLL function
        dggs_cells = get_DGGS_CELL_array(cells)
        no_of_cells = c_ushort(len(cells))
        output_points = (LAT_LONG_POINT * no_of_cells.value)()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_ConvertDggsCellsToPoints')
        func.argtypes = [c_void_p, POINTER(DGGS_CELL), c_ushort,
                         POINTER(LAT_LONG_POINT)]
        func.restype = c_int
        return_code = func(self._dggs_handle, dggs_cells, no_of_cells, output_points)
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Process the output data
        lat_long_points = []
        for point_index in range(0, no_of_cells.value):
            lat_long_points.append(output_points[point_index].to_lat_long_point())
        # Return the array of DGGS shapes
        return lat_long_points

    ## Converts a list of DGGS cells into a string of lat / long points.
    #  @param cells List of DGGS cells.
    #  @param string_format Format to use for the output string (enums.ShapeStringFormat).
    #  @return String defining the DGGS cells in lat / long coordinates.
    #  @throw EaggrException Thrown if unable to convert the DGGS cells.
    def convert_dggs_cells_to_shape_string(self, cells, string_format):
        # Check requested string format is valid
        check_shape_string_format(string_format)
        # Set up the arguments to the DLL function
        dggs_cells = get_DGGS_CELL_array(cells)
        no_of_cells = c_ushort(len(cells))
        output_string = c_char_p()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_ConvertDggsCellsToShapeString')
        func.argtypes = [c_void_p, POINTER(DGGS_CELL), c_ushort, c_int, POINTER(c_char_p)]
        func.restype = c_int
        return_code = func(self._dggs_handle, dggs_cells, no_of_cells, string_format, byref(output_string))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Store the string from native memory
        returnString = cast(output_string, c_char_p).value.decode('utf-8')
        # Free the native string memory
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_DeallocateString')
        func.argtypes = [c_void_p, POINTER(c_char_p)]
        func.restype = c_int
        return_code = func(self._dggs_handle, byref(output_string))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Return the shape string
        return returnString

    ## Converts a cell area to a shape string containing a lat / long polygon.
    #  @param cell DGGS cell to get the shape string for.
    #  @param string_format Format to use for the output string (enums.ShapeStringFormat).
    #  @return String defining the DGGS cell shape.
    #  @throw EaggrException Thrown if unable to get the DGGS cell shape.
    def convert_dggs_cell_outline_to_shape_string(self, cell, string_format):
        # Check requested string format is valid
        check_shape_string_format(string_format)
        # Set up the arguments to the DLL function
        dggs_cell = DGGS_CELL()
        dggs_cell.from_dggs_cell(cell)
        output_string = c_char_p()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_ConvertDggsCellOutlineToShapeString')
        func.argtypes = [c_void_p, DGGS_CELL, c_int, POINTER(c_char_p)]
        func.restype = c_int
        return_code = func(self._dggs_handle, dggs_cell, string_format, byref(output_string))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Store the string from native memory
        returnString = cast(output_string, c_char_p).value.decode('utf-8')
        # Free the native string memory
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_DeallocateString')
        func.argtypes = [c_void_p, POINTER(c_char_p)]
        func.restype = c_int
        return_code = func(self._dggs_handle, byref(output_string))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Return the shape string
        return returnString

    ## Outputs the parents of the specified cell.
    #
    #  Parent cells are defined as the cells in the resolution below which share
    #  some or all of the area of the specified cell.
    #  @param cell DGGS cell to find the parents of.
    #  @return Parent cells.
    #  @throw  EaggrException Thrown if unable to get the parent cells.
    def get_dggs_cell_parents(self, cell):
        # Set up the arguments to the DLL function
        dggs_cell = DGGS_CELL()
        dggs_cell.from_dggs_cell(cell)
        output_cells = (DGGS_CELL * Eaggr._EAGGR_MAX_PARENT_CELLS)()
        no_of_parents = c_ushort()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_GetDggsCellParents')
        func.argtypes = [c_void_p, DGGS_CELL, POINTER(DGGS_CELL), POINTER(c_ushort)]
        func.restype = c_int
        return_code = func(self._dggs_handle, dggs_cell, output_cells, byref(no_of_parents))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Process the output data
        parents_cells = []
        for cell_index in range(0, no_of_parents.value):
            parents_cells.append(output_cells[cell_index].to_dggs_cell())
        # Return the array of parent cells
        return parents_cells

    ## Outputs the children of the specified cell.
    #
    #  Child cells are defined as the cells in the resolution above which share
    #  some or all of the area of the specified cell.
    #  @param cell DGGS cell to find the children of.
    #  @return Child cells.
    #  @throw EaggrException Thrown if unable to get the parent cells.
    def get_dggs_cell_children(self, cell):
        # Set up the arguments to the DLL function
        dggs_cell = DGGS_CELL()
        dggs_cell.from_dggs_cell(cell)
        output_cells = (DGGS_CELL * Eaggr._EAGGR_MAX_CHILD_CELLS)()
        no_of_children = c_ushort()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_GetDggsCellChildren')
        func.argtypes = [c_void_p, DGGS_CELL, POINTER(DGGS_CELL), POINTER(c_ushort)]
        func.restype = c_int
        return_code = func(self._dggs_handle, dggs_cell, output_cells, byref(no_of_children))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Process the output data
        child_cells = []
        for cell_index in range(0, no_of_children.value):
            child_cells.append(output_cells[cell_index].to_dggs_cell())
        # Return the array of children cells
        return child_cells

    ## Outputs the siblings of the specified cell.
    #
    #  Siblings cells are defined as those cells in the same resolution that share 
    #  area with the cells in the resolution above where these share area with the 
    #  specified cell.
    #  @param cell DGGS cell to find the siblings of.
    #  @return Sibling cells.
    #  @throw  EaggrException Thrown if unable to get the sibling cells.
    def get_dggs_cell_siblings(self, cell):
        # Set up the arguments to the DLL function
        dggs_cell = DGGS_CELL()
        dggs_cell.from_dggs_cell(cell)
        output_cells = (DGGS_CELL * Eaggr._EAGGR_MAX_SIBLING_CELLS)()
        no_of_siblings = c_ushort()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_GetDggsCellSiblings')
        func.argtypes = [c_void_p, DGGS_CELL, POINTER(DGGS_CELL), POINTER(c_ushort)]
        func.restype = c_int
        return_code = func(self._dggs_handle, dggs_cell, output_cells, byref(no_of_siblings))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Process the output data
        siblings_cells = []
        for cell_index in range(0, no_of_siblings.value):
            siblings_cells.append(output_cells[cell_index].to_dggs_cell())
        # Return the array of sibling cells
        return siblings_cells

    ## Outputs the highest resolution cell that contains all the given cells.
    #  @param cells List of DGGS cells to find the bounding cell of.
    #  @return Cell containing all of the cells in the input list.
    #  @throw EaggrException Thrown if unable to get the bounding cell.
    def get_bounding_dggs_cell(self, cells):
        # Set up the arguments to the DLL function
        dggs_cells = get_DGGS_CELL_array(cells)
        no_of_cells = c_ushort(len(cells))
        output_cell = DGGS_CELL()
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_GetBoundingDggsCell')
        func.argtypes = [c_void_p, POINTER(DGGS_CELL), c_ushort, POINTER(DGGS_CELL)]
        func.restype = c_int
        return_code = func(self._dggs_handle, dggs_cells, no_of_cells, byref(output_cell))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())
        # Return the array of DGGS shapes
        return output_cell.to_dggs_cell()

    ## Creates a KML file for displaying the given cells on mapping applications,
    #  such as Google Earth.
    #  @param filename Filename of the KML file to be created.
    #  @param cells List of DGGS cells to include in the KML file.
    #  @throw EaggrException Thrown if unable to create the KML file.
    def create_dggs_kml_file(self, filename, cells):
        # Set up the arguments to the DLL function
        try:
            kml_filename = cast(filename.encode('utf-8'), c_char_p).value
        except AttributeError:
            raise ValueError('Invalid filename for KML file')
        dggs_cells = get_DGGS_CELL_array(cells)
        no_of_cells = c_ushort(len(cells))
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_CreateDggsKmlFile')
        func.argtypes = [c_void_p, c_char_p, POINTER(DGGS_CELL), c_ushort]
        func.restype = c_int
        return_code = func(self._dggs_handle, kml_filename, dggs_cells, no_of_cells)
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())

    ## Opens handles to the DGGS DLL and its dependencies.
    #  @note Because the DLLs used by the EAGGR library are not in the run directory or the path
    #        they must be manually loaded beforehand so the EAGGR library DLL can find them.
    #  @throw IOError Thrown if unable to open a handle to one of the DLLs.
    def _open_dlls(self):
        # DLL directories and filenames depend on the platform
        if (platform.system() == 'Windows'):
            proj_library_name = 'libproj-9.dll'
            gdal_library_name = 'libgdal-20.dll'
            dggs_library_name = 'eaggr.dll'
            if platform.architecture()[0] == '64bit':
                gcc_library_name = 'libgcc_s_seh-1.dll'
                dll_dir = os.path.join(os.path.dirname(__file__), 'dlls', 'win32-x86-64')
            else:
                gcc_library_name = 'libgcc_s_dw2-1.dll'
                dll_dir = os.path.join(os.path.dirname(__file__), 'dlls', 'win32-x86')
        else:
            proj_library_name = 'libproj.so'
            gdal_library_name = 'libgdal.so'
            dggs_library_name = 'libeaggr.so'
            dll_dir = os.path.join(os.path.dirname(__file__), 'dlls', 'linux-x86-64')
        # Check the chosen DLL directory exists
        if not os.path.exists(dll_dir):
            raise IOError('Cannot find DLL directory - ' + dll_dir)
        # Load MinGW DLLs in the correct order (required on Windows only)
        if (platform.system() == 'Windows'):
            thread_path = os.path.join(dll_dir, 'libwinpthread-1.dll')
            if not os.path.isfile(thread_path):
                raise IOError('Cannot find the Windows thread library - ' + thread_path)
            Eaggr._proj_dll_handle = cdll.LoadLibrary(thread_path)
            gcc_path = os.path.join(dll_dir, gcc_library_name)
            if not os.path.isfile(gcc_path):
                raise IOError('Cannot find the GCC library - ' + gcc_path)
            Eaggr._proj_dll_handle = cdll.LoadLibrary(gcc_path)
            stdc_path = os.path.join(dll_dir, 'libstdc++-6.dll')
            if not os.path.isfile(stdc_path):
                raise IOError('Cannot find the std C++ library - ' + stdc_path)
            Eaggr._proj_dll_handle = cdll.LoadLibrary(stdc_path)
        # Load Proj4 and GDAL library DLLs
        proj_path = os.path.join(dll_dir, proj_library_name)
        if not os.path.isfile(proj_path):
            raise IOError('Cannot find the Proj4 library - ' + proj_path)
        Eaggr._proj_dll_handle = cdll.LoadLibrary(proj_path)
        gdal_path = os.path.join(dll_dir, gdal_library_name)
        if not os.path.isfile(gdal_path):
            raise IOError('Cannot find the GDAL library - ' + gdal_path)
        Eaggr._gdal_dll_handle = cdll.LoadLibrary(gdal_path)
        # Now load the EAGGR DLL
        dggs_path = os.path.join(dll_dir, dggs_library_name)
        if not os.path.isfile(dggs_path):
            raise IOError('Cannot find the EAGGR library - ' + dggs_path)
        Eaggr._eaggr_dll_handle = cdll.LoadLibrary(dggs_path)
        Eaggr._eaggr_dll = CDLL(dggs_path)
        if Eaggr._eaggr_dll is None:
            raise IOError('Unable to open handle to EAGGR DLL')

    ## Deallocates the memory used by the DGGS shapes that are output from the DLL.
    #  @param output_shapes Array containing the DGGS shapes.
    #  @param no_of_shapes Number of shapes in the array.
    #  @throw EaggrException Thrown if unable to deallocate the memory for the DGGS shapes.
    def _deallocate_dggs_shapes(self, output_shapes, no_of_shapes):
        # Configure and call the DLL function
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_DeallocateDggsShapes')
        func.argtypes = [c_void_p, POINTER(POINTER(DGGS_SHAPE)), c_ushort]
        func.restype = c_int
        return_code = func(self._dggs_handle, byref(output_shapes), no_of_shapes)
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, self._get_last_error_message())

    ## Outputs a message for the last error to occur.
    #  @return Error message string.
    #  @throw EaggrException Thrown if unable to get the last error message.
    def _get_last_error_message(self):
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_GetLastErrorMessage')
        func.argtypes = [c_void_p, POINTER(c_char_p), POINTER(c_ushort)]
        func.restype = c_int
        error_message = c_char_p()
        message_length = c_ushort()
        return_code = func(self._dggs_handle, byref(error_message), byref(message_length))
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException("Error occurred, but failed to get error message")
        # Store the string from native memory
        returnString = cast(error_message, c_char_p).value.decode('utf-8')
        # Free the native string memory
        func = getattr(Eaggr._eaggr_dll, 'EAGGR_DeallocateString')
        func.argtypes = [c_void_p, POINTER(c_char_p)]
        func.restype = c_int
        return_code = func(self._dggs_handle, byref(error_message))
        # Check the return code
        if return_code != DggsReturnCode.DGGS_SUCCESS:
            raise EaggrException(return_code, "Failed to free native string memory.")
        # Return the shape string
        return returnString


## Outputs the result of a spatial analysis between two shapes.
    #
    #  Two DGGS_Shapes are compared using a DGGS_AnalysisType spatial analysis type.
    #  @param baseShape The base DGGSShape object used in the spatial analysis
    #  @param comparisonShape The comparison DGGSShape object used in the spatial analysis
    #  @param analysisType The spatial analysis type, taken from the dggs_shape_type enum
    #  @return flag indicating result of spatial analysis
    #  @throw  EaggrException Thrown if unable to get shape comparison
    def compare_dggs_shapes(self, baseShape, comparisonShape, analysisType):
        try:
            # Set up the arguments to the DLL function
            base_dggs_shape = DGGS_SHAPE()
            comparison_dggs_shape = DGGS_SHAPE()
            dggs_shape_analysis_type = analysisType      
            base_dggs_shape.from_dggs_shape(baseShape)
            comparison_dggs_shape.from_dggs_shape(comparisonShape)
            func_output = c_bool()
            # Configure and call the DLL function
            func = getattr(Eaggr._eaggr_dll, 'EAGGR_CompareShapes')
            func.argtypes = [c_void_p, c_int, POINTER(DGGS_SHAPE), POINTER(DGGS_SHAPE), POINTER(c_bool)]
            func.restype = c_int
            return_code = func(self._dggs_handle, dggs_shape_analysis_type, base_dggs_shape, comparison_dggs_shape, byref(func_output))
            output_boolean = func_output.value
            # Check the return code
            if return_code != DggsReturnCode.DGGS_SUCCESS:
                raise EaggrException(return_code, self._get_last_error_message())
            # Return the boolean indicating the result of the shape comparison
            return output_boolean
    
        except TypeError:
            raise ValueError("Analysis type must be CONTAINS, COVERED_BY, COVERS, CROSSES, DISJOINT, " 
                             "EQUALS, INTERSECTS, OVERLAPS, TOUCHES or  WITHIN")
