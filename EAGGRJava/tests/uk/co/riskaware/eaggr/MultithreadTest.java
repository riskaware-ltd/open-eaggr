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

import static org.junit.Assert.assertEquals;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import uk.co.riskaware.eaggr.enums.DggsModel;

public class MultithreadTest {

    private static final long TIMEOUT = 5000;
    private static final double TOLERANCE = 1E-7;

    private class DggsRunnable implements Runnable {

        private Throwable t;
        private LatLongPoint point;
        private LatLongPoint convertedPoint;

        public DggsRunnable(LatLongPoint point) {
            this.point = point;
        }

        public Throwable getError() {
            return t;
        }

        public boolean hasError() {
            return t != null;
        }

        public LatLongPoint getPoint() {
            return point;
        }

        public LatLongPoint getConvertedPoint() {
            return convertedPoint;
        }

        @Override
        public void run() {
            try {
                final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

                DggsCell dggsCell = dggs.convertPointToDggsCell(point);

                convertedPoint = dggs.convertDggsCellToPoint(dggsCell);
            } catch (Throwable t) {
                this.t = t;
            }
        }
    }

    @Test
    public void multipleThreadTest() throws Throwable {

        double accuracy = 1E-5;

        List<DggsRunnable> dggsRunners = new ArrayList<DggsRunnable>();

        for (double latitude = -80; latitude <= 80; latitude += 5) {
            for (double longitude = -170; longitude <= 170; longitude += 5) {
                dggsRunners.add(new DggsRunnable(new LatLongPoint(latitude, longitude, accuracy)));
            }
        }

        List<Thread> threads = new ArrayList<Thread>();

        for (DggsRunnable dggsRunner : dggsRunners) {
            Thread thread = new Thread(dggsRunner);
            threads.add(thread);
            thread.run();
        }

        for (Thread thread : threads) {
            thread.join(TIMEOUT);
        }

        for (DggsRunnable dggsRunner : dggsRunners) {
            if (dggsRunner.hasError()) {
                throw dggsRunner.getError();
            }

            assertEquals(dggsRunner.getPoint().getLatitude(), dggsRunner.getConvertedPoint().getLatitude(), TOLERANCE);
            assertEquals(dggsRunner.getPoint().getLongitude(), dggsRunner.getConvertedPoint().getLongitude(),
                    TOLERANCE);
        }
    }
}
