/*
 * Copyright (c) Riskaware 2015
 *
 * This file is part of OpenEAGGR.
 * 
 * OpenEAGGR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OpenEAGGR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * A copy of the GNU Lesser General Public License is available in COPYING.LESSER
 * or can be found at <http://www.gnu.org/licenses/>.
 */

package uk.co.riskaware.eaggr;

import uk.co.riskaware.eaggr.enums.DggsShapeLocation;
import uk.co.riskaware.eaggr.enums.DggsShapeType;

/**
 * Class to represent a DGGS shape
 * 
 * @author i_bush
 *
 */
public class DggsShape {

    private final DggsShapeType shapeType;
    private final DggsShapeLocation shapeLocation;

    private final DggsCell cell;
    private final DggsLinestring linestring;
    private final DggsPolygon polygon;

    /**
     * Package internal constructor using a native DGGS shape object
     * 
     * @param nativeShape
     *            The native DGGS shape
     * @throws EaggrException
     *             If the native shape is not supported
     */
    DggsShape(final DggsNativeDggsTypes.DggsShape nativeShape) throws EaggrException {
        shapeType = DggsShapeType.fromNativeCode(nativeShape.shapeType);
        shapeLocation = DggsShapeLocation.fromNativeCode(nativeShape.shapeLocation);

        switch (shapeType) {
            case CELL:
                // Need to trim the string as the byte array is padded with nulls
                cell = new DggsCell(new String(nativeShape.shapeData.cell).trim());
                linestring = null;
                polygon = null;
                break;
            case LINESTRING:
                linestring = new DggsLinestring(nativeShape.shapeData.linestring);
                cell = null;
                polygon = null;
                break;
            case POLYGON:
                polygon = new DggsPolygon(nativeShape.shapeData.polygon);
                cell = null;
                linestring = null;
                break;
            default:
                throw new EaggrException("Unrecognised shape type " + shapeType + ".");
        }
    }

    /**
     * Package internal constructor using a DGGS cell object
     * 
     * @param dggsCell
     *            A DGGS cell object
     */
    public DggsShape(final DggsCell dggsCell) {
        this.shapeType = DggsShapeType.CELL;
        this.shapeLocation = DggsShapeLocation.ONE_FACE;
        this.cell = dggsCell;
        this.linestring = null;
        this.polygon = null;
    }

    /**
     * Package internal constructor using a DGGS linestring object
     * 
     * @param dggsLinestring
     *            A DGGS linestring object
     */
    public DggsShape(final DggsLinestring dggsLinestring) {
        this.shapeType = DggsShapeType.LINESTRING;
        this.shapeLocation = DggsShapeLocation.NO_LOCATION;
        this.linestring = dggsLinestring;
        this.cell = null;
        this.polygon = null;
    }

    /**
     * Package internal constructor using a DGGS polygon object
     * 
     * @param dggsPolygon
     *            A DGGS polygon shape
     */
    public DggsShape(final DggsPolygon dggsPolygon) {
        this.shapeType = DggsShapeType.POLYGON;
        this.shapeLocation = DggsShapeLocation.NO_LOCATION;
        this.polygon = dggsPolygon;
        this.cell = null;
        this.linestring = null;
    }

    /**
     * @return The shape type
     */
    public DggsShapeType getShapeType() {
        return shapeType;
    }

    /**
     * @return The shape location
     */
    public DggsShapeLocation getShapeLocation() {
        return shapeLocation;
    }

    /**
     * @return The DGGS cell or null if this shape is not a CELL type
     */
    public DggsCell getCell() {
        return cell;
    }

    /**
     * @return The DGGS linestring or null if this shape is not a LINESTRING type
     */
    public DggsLinestring getLinestring() {
        return linestring;
    }

    /**
     * @return The DGGS polygon or null if this shape is not a POLYGON type
     */
    public DggsPolygon getPolygon() {
        return polygon;
    }
}
