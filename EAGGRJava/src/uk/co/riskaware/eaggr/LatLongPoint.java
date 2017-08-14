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

/**
 * Class to represent a Lat/Long point. The DGGS library uses the WGS84 coordinate system.
 * 
 * @author i_bush
 *
 */
public class LatLongPoint {

    private final double latitude;
    private final double longitude;
    private final double accuracy;

    /**
     * Constructor
     * 
     * @param latitude
     *            The latitude in degrees
     * @param longitude
     *            The longitude in degrees
     * @param accuracy
     *            The accuracy in metres squared
     */
    public LatLongPoint(final double latitude, final double longitude, final double accuracy) {

        this.latitude = latitude;
        this.longitude = longitude;
        this.accuracy = accuracy;
    }

    /**
     * Package internal constructor from a native point
     * 
     * @param latLongPoint
     *            The native point object
     */
    LatLongPoint(final uk.co.riskaware.eaggr.DggsNativeLatLongTypes.LatLongPoint latLongPoint) {
        latitude = latLongPoint.latitude;
        longitude = latLongPoint.longitude;
        accuracy = latLongPoint.accuracy;
    }

    /**
     * @return The latitude in degrees
     */
    public double getLatitude() {
        return latitude;
    }

    /**
     * @return The longitude in degrees
     */
    public double getLongitude() {
        return longitude;
    }

    /**
     * @return The accuracy in metres squared
     */
    public double getAccuracy() {
        return accuracy;
    }

    /**
     * Package internal conversion to a native point
     * 
     * @param nativePoint
     *            The native point object to be populated with this point's data
     */
    void toNativeLatLongPoint(final DggsNativeLatLongTypes.LatLongPoint nativePoint) {
        nativePoint.latitude = latitude;
        nativePoint.longitude = longitude;
        nativePoint.accuracy = accuracy;
    }
}
