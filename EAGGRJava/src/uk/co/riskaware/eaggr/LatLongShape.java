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

import uk.co.riskaware.eaggr.enums.LatLongShapeType;

/**
 * Class to represent a Lat/Long shape
 * 
 * @author i_bush
 *
 */
public class LatLongShape {

    private final LatLongShapeType shapeType;

    private final LatLongPoint point;
    private final LatLongLinestring linestring;
    private final LatLongPolygon polygon;

    /**
     * Constructor for a shape representing a point
     * 
     * @param point
     *            The Lat/Long point
     */
    public LatLongShape(final LatLongPoint point) {
        shapeType = LatLongShapeType.POINT;

        this.point = point;
        linestring = null;
        polygon = null;
    }

    /**
     * Constructor for a shape representing a linestring
     * 
     * @param linestring
     *            The Lat/Long linestring
     */
    public LatLongShape(final LatLongLinestring linestring) {
        shapeType = LatLongShapeType.LINESTRING;

        point = null;
        this.linestring = linestring;
        polygon = null;
    }

    /**
     * Constructor for a shape representing a polygon
     * 
     * @param polygon
     *            The Lat/Long polygon
     */
    public LatLongShape(final LatLongPolygon polygon) {
        shapeType = LatLongShapeType.POLYGON;

        point = null;
        linestring = null;
        this.polygon = polygon;
    }

    /**
     * Package internal conversion to a native Lat/Long shape object
     * 
     * @param nativeShape
     *            The native shape object that will be populated with this shape's data
     * @throws EaggrException
     *             If the shape type is not supported
     */
    void toNativeLatLongShape(final DggsNativeLatLongTypes.LatLongShape nativeShape) throws EaggrException {

        nativeShape.latLongShapeType = shapeType.ordinal();

        switch (shapeType) {
            case POINT:
                point.toNativeLatLongPoint(nativeShape.latLongShapeData.point);

                nativeShape.latLongShapeData.setType(DggsNativeLatLongTypes.LatLongPoint.class);
                break;
            case LINESTRING:
                linestring.toNativeLatLongLinestring(nativeShape.latLongShapeData.linestring);

                nativeShape.latLongShapeData.setType(DggsNativeLatLongTypes.LatLongLinestring.class);
                break;
            case POLYGON:
                polygon.toNativeLatLongPolygon(nativeShape.latLongShapeData.polygon);

                nativeShape.latLongShapeData.setType(DggsNativeLatLongTypes.LatLongPolygon.class);
                break;
            default:
                throw new EaggrException("Unrecognised shape type " + shapeType + ".");
        }
    }

    /**
     * @return The shape type
     */
    public LatLongShapeType getShapeType() {
        return shapeType;
    }

    /**
     * @return The point or null if this shape is not a POINT type
     */
    public LatLongPoint getPoint() {
        return point;
    }

    /**
     * @return The linestring or null if this shape is not a LINESTRING type
     */
    public LatLongLinestring getLinestring() {
        return linestring;
    }

    /**
     * @return The polygon or null if this shape is not a POLYGON type
     */
    public LatLongPolygon getPolygon() {
        return polygon;
    }
}
