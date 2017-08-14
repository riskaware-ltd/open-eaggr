OpenEAGGR Python Library
=========================

Provides a library for converting between shapes made up of WGS84 points with
latitude and longitude coordinates and cells on a Discrete Global Grid System
(DGGS).

Two DGGS models are supported:

 * ISEA4T with hierarchical cell indexing
 * ISEA3H with offset cell indexing

The library also provides methods for querying individual cells, such as
finding the parent and child cells of a particular cell.
