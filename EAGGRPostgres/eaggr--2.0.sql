-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION eaggr" to load this file. \quit
CREATE OR REPLACE FUNCTION EAGGR_Version() RETURNS text
AS '$libdir/lib_eaggr_postgres', 'EAGGR_Version'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION EAGGR_ToCells(text, double precision, text) RETURNS text
AS '$libdir/lib_eaggr_postgres', 'EAGGR_ToCells'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION EAGGR_ToCellArray(text) RETURNS text[]
AS '$libdir/lib_eaggr_postgres', 'EAGGR_ToCellArray'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION EAGGR_GetBoundingCell(text[], text) RETURNS text
AS '$libdir/lib_eaggr_postgres', 'EAGGR_GetBoundingCell'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION EAGGR_CellGeometry(text, text) RETURNS text
AS '$libdir/lib_eaggr_postgres', 'EAGGR_CellGeometry'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION EAGGR_CellToPoint(text, text) RETURNS text
AS '$libdir/lib_eaggr_postgres', 'EAGGR_CellToPoint'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION EAGGR_ShapeComparison(text, text, text, text) RETURNS bool
AS '$libdir/lib_eaggr_postgres', 'EAGGR_ShapeComparison'
LANGUAGE C IMMUTABLE STRICT;
