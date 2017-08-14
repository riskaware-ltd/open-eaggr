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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.junit.Test;

import uk.co.riskaware.eaggr.enums.AnalysisType;
import uk.co.riskaware.eaggr.enums.DggsModel;
import uk.co.riskaware.eaggr.enums.DggsShapeLocation;
import uk.co.riskaware.eaggr.enums.DggsShapeType;
import uk.co.riskaware.eaggr.enums.ShapeStringFormat;
import uk.co.riskaware.eaggr.testutilities.DggsTestDataHexagonal;
import uk.co.riskaware.eaggr.testutilities.DggsTestDataTriangular;
import uk.co.riskaware.eaggr.testutilities.TestUtilities;

public class EaggrTest {

    @Test
    public void constructorTest() throws EaggrException, EaggrLibraryException {
        // Should construct with no exceptions
        new Eaggr(DggsModel.ISEA4T);
        new Eaggr(DggsModel.ISEA3H);
    }

    @Test
    public void getVersionTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final String versionString = dggs.getVersion();

        assertEquals("v2.0", versionString);
    }

    @Test
    public void convertPointToDggsCellTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final LatLongPoint latLongPoint1 = new LatLongPoint(1.234, 2.345, 3.879);
        final LatLongPoint latLongPoint2 = new LatLongPoint(3.456, 4.567, 3.879);

        DggsCell dggsCell = dggs.convertPointToDggsCell(latLongPoint1);
        assertEquals("07231131111113100331001", dggsCell.getCellId());

        dggsCell = dggs.convertPointToDggsCell(latLongPoint2);
        assertEquals("07012000001303022011321", dggsCell.getCellId());
    }

    @Test
    public void convertShapesToDggsShapesTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final LatLongShape[] latLongShapes = new LatLongShape[4];

        latLongShapes[0] = new LatLongShape(new LatLongPoint(1.234, 2.345, 3.879));
        latLongShapes[1] = new LatLongShape(new LatLongPoint(3.456, 4.567, 3.879));

        final LatLongLinestring linestring = new LatLongLinestring();
        linestring.addPoint(new LatLongPoint(1.234, 2.345, 3.879));
        linestring.addPoint(new LatLongPoint(3.456, 4.567, 3.879));
        latLongShapes[2] = new LatLongShape(linestring);

        final LatLongLinestring outerRing = new LatLongLinestring();
        outerRing.addPoint(new LatLongPoint(1.234, 2.345, 3.879));
        outerRing.addPoint(new LatLongPoint(3.456, 4.567, 3.879));

        final LatLongLinestring innerRing1 = new LatLongLinestring();
        innerRing1.addPoint(new LatLongPoint(1.234, 2.345, 3.879));
        innerRing1.addPoint(new LatLongPoint(3.456, 4.567, 3.879));

        final LatLongLinestring innerRing2 = new LatLongLinestring();
        innerRing2.addPoint(new LatLongPoint(1.234, 2.345, 3.879));
        innerRing2.addPoint(new LatLongPoint(3.456, 4.567, 3.879));

        final LatLongPolygon polygon = new LatLongPolygon(outerRing);
        polygon.addInnerRing(innerRing1);
        polygon.addInnerRing(innerRing2);
        latLongShapes[3] = new LatLongShape(polygon);

        DggsShape[] dggsShapes = dggs.convertShapesToDggsShapes(latLongShapes);

        assertEquals(4, dggsShapes.length);

        assertEquals(DggsShapeType.CELL, dggsShapes[0].getShapeType());
        assertEquals(DggsShapeLocation.ONE_FACE, dggsShapes[0].getShapeLocation());
        assertEquals("07231131111113100331001", dggsShapes[0].getCell().getCellId());

        assertEquals(DggsShapeType.CELL, dggsShapes[1].getShapeType());
        assertEquals(DggsShapeLocation.ONE_FACE, dggsShapes[1].getShapeLocation());
        assertEquals("07012000001303022011321", dggsShapes[1].getCell().getCellId());

        assertEquals(DggsShapeType.LINESTRING, dggsShapes[2].getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, dggsShapes[2].getShapeLocation());

        final List<DggsCell> linestringCellIds = dggsShapes[2].getLinestring().getLinestringCellIds();
        assertEquals(2, linestringCellIds.size());
        assertEquals("07231131111113100331001", linestringCellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", linestringCellIds.get(1).getCellId());

        assertEquals(DggsShapeType.POLYGON, dggsShapes[3].getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, dggsShapes[3].getShapeLocation());

        final List<DggsCell> outerRingCellIds = dggsShapes[3].getPolygon().getOuterRing().getLinestringCellIds();
        assertEquals(2, outerRingCellIds.size());

        assertEquals("07231131111113100331001", outerRingCellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", outerRingCellIds.get(1).getCellId());

        final List<DggsLinestring> innerDggsRings = dggsShapes[3].getPolygon().getInnerRings();
        assertEquals(2, innerDggsRings.size());

        final List<DggsCell> innerRing1CellIds = innerDggsRings.get(0).getLinestringCellIds();
        assertEquals("07231131111113100331001", innerRing1CellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", innerRing1CellIds.get(1).getCellId());

        final List<DggsCell> innerRing2CellIds = innerDggsRings.get(1).getLinestringCellIds();
        assertEquals("07231131111113100331001", innerRing2CellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", innerRing2CellIds.get(1).getCellId());

        // Convert an empty list of lat/long shapes
        dggsShapes = dggs.convertShapesToDggsShapes(new LatLongShape[] {});
        assertEquals(0, dggsShapes.length);
    }

    @Test
    public void convertShapesToDggsShapesPolygonNoInnerRingsTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final LatLongShape[] latLongShapes = new LatLongShape[1];

        final LatLongLinestring outerRing = new LatLongLinestring();
        outerRing.addPoint(new LatLongPoint(1.234, 2.345, 3.879));
        outerRing.addPoint(new LatLongPoint(3.456, 4.567, 3.879));

        final LatLongPolygon polygon = new LatLongPolygon(outerRing);
        latLongShapes[0] = new LatLongShape(polygon);

        final DggsShape[] dggsShapes = dggs.convertShapesToDggsShapes(latLongShapes);

        assertEquals(1, dggsShapes.length);

        assertEquals(DggsShapeType.POLYGON, dggsShapes[0].getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, dggsShapes[0].getShapeLocation());

        final List<DggsCell> outerRingCellIds = dggsShapes[0].getPolygon().getOuterRing().getLinestringCellIds();
        assertEquals(2, outerRingCellIds.size());

        assertEquals("07231131111113100331001", outerRingCellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", outerRingCellIds.get(1).getCellId());

        final List<DggsLinestring> innerDggsRings = dggsShapes[0].getPolygon().getInnerRings();
        assertEquals(0, innerDggsRings.size());
    }

    @Test(expected = EaggrLibraryException.class)
    public void convertShapesToDggsShapesInvalidPointTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final LatLongShape[] invalidShapes = new LatLongShape[1];
        invalidShapes[0] = new LatLongShape(new LatLongPoint(91.0, 2.345, 3.879));

        dggs.convertShapesToDggsShapes(invalidShapes);
    }

    @Test
    public void convertShapeStringToDggsShapesTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final String wktText = "GEOMETRYCOLLECTION(MULTIPOINT(2.345 1.234, 4.567 3.456),LINESTRING(2.345 1.234, 4.567 3.456),POLYGON((2.345 1.234, 4.567 3.456), (2.345 1.234, 4.567 3.456), (2.345 1.234, 4.567 3.456)))";

        final DggsShape[] dggsShapes = dggs.convertShapeStringToDggsShapes(wktText, ShapeStringFormat.WKT, 3.879);

        assertEquals(4, dggsShapes.length);

        assertEquals(DggsShapeType.CELL, dggsShapes[0].getShapeType());
        assertEquals(DggsShapeLocation.ONE_FACE, dggsShapes[0].getShapeLocation());
        assertEquals("07231131111113100331001", dggsShapes[0].getCell().getCellId());

        assertEquals(DggsShapeType.CELL, dggsShapes[1].getShapeType());
        assertEquals(DggsShapeLocation.ONE_FACE, dggsShapes[1].getShapeLocation());
        assertEquals("07012000001303022011321", dggsShapes[1].getCell().getCellId());

        assertEquals(DggsShapeType.LINESTRING, dggsShapes[2].getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, dggsShapes[2].getShapeLocation());

        final List<DggsCell> linestringCellIds = dggsShapes[2].getLinestring().getLinestringCellIds();
        assertEquals(2, linestringCellIds.size());
        assertEquals("07231131111113100331001", linestringCellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", linestringCellIds.get(1).getCellId());

        assertEquals(DggsShapeType.POLYGON, dggsShapes[3].getShapeType());
        assertEquals(DggsShapeLocation.NO_LOCATION, dggsShapes[3].getShapeLocation());

        final List<DggsCell> outerRingCellIds = dggsShapes[3].getPolygon().getOuterRing().getLinestringCellIds();
        assertEquals(2, outerRingCellIds.size());

        assertEquals("07231131111113100331001", outerRingCellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", outerRingCellIds.get(1).getCellId());

        final List<DggsLinestring> innerDggsRings = dggsShapes[3].getPolygon().getInnerRings();
        assertEquals(2, innerDggsRings.size());

        final List<DggsCell> innerRing1CellIds = innerDggsRings.get(0).getLinestringCellIds();
        assertEquals("07231131111113100331001", innerRing1CellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", innerRing1CellIds.get(1).getCellId());

        final List<DggsCell> innerRing2CellIds = innerDggsRings.get(1).getLinestringCellIds();
        assertEquals("07231131111113100331001", innerRing2CellIds.get(0).getCellId());
        assertEquals("07012000001303022011321", innerRing2CellIds.get(1).getCellId());
    }

    @Test(expected = EaggrLibraryException.class)
    public void convertShapeStringToDggsShapesInvalidPointTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final String wktText = "GEOMETRYCOLLECTION(MULTIPOINT(2.345 91.0, 4.567 3.456))";

        dggs.convertShapeStringToDggsShapes(wktText, ShapeStringFormat.WKT, 3.879);
    }

    @Test
    public void convertDggsCellToPointTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final DggsCell cell1 = new DggsCell("07231131111113100331032");
        final DggsCell cell2 = new DggsCell("07012000001303022011321");

        final double tolerance = 1E-3;

        LatLongPoint point = dggs.convertDggsCellToPoint(cell1);
        assertEquals(1.234, point.getLatitude(), tolerance);
        assertEquals(2.345, point.getLongitude(), tolerance);
        assertEquals(5.804, point.getAccuracy(), tolerance);

        point = dggs.convertDggsCellToPoint(cell2);
        assertEquals(3.456, point.getLatitude(), tolerance);
        assertEquals(4.567, point.getLongitude(), tolerance);
        assertEquals(5.804, point.getAccuracy(), tolerance);
    }

    @Test
    public void convertDggsCellsToPointsTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final DggsCell[] cellIds = new DggsCell[] { new DggsCell("07231131111113100331032"),
                new DggsCell("07012000001303022011321") };

        LatLongPoint[] points = dggs.convertDggsCellsToPoints(cellIds);

        assertEquals(cellIds.length, points.length);

        final double tolerance = 1E-3;

        assertEquals(1.234, points[0].getLatitude(), tolerance);
        assertEquals(2.345, points[0].getLongitude(), tolerance);
        assertEquals(5.804, points[0].getAccuracy(), tolerance);

        assertEquals(3.456, points[1].getLatitude(), tolerance);
        assertEquals(4.567, points[1].getLongitude(), tolerance);
        assertEquals(5.804, points[1].getAccuracy(), tolerance);

        // Convert an empty list of DGGS cells
        points = dggs.convertDggsCellsToPoints(new DggsCell[] {});
        assertEquals(0, points.length);
    }

    @Test
    public void convertDggsShapesToShapeStringTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final DggsCell[] cellIds = new DggsCell[] { new DggsCell("07231131111113100331032"),
                new DggsCell("07012000001303022011321") };

        final String shapeString = dggs.convertDggsCellsToShapeString(cellIds, ShapeStringFormat.WKT);

        // Values in the string may vary due to numerical precision - extract them using a regex and compare them as
        // doubles
        final Pattern pattern = Pattern
                .compile("MULTIPOINT \\((\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*),(\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*)\\)");

        final Matcher matcher = pattern.matcher(shapeString);

        final double[] coordinates = new double[4];

        assertTrue(matcher.matches());
        coordinates[0] = Double.parseDouble(matcher.group(1));
        coordinates[1] = Double.parseDouble(matcher.group(2));
        coordinates[2] = Double.parseDouble(matcher.group(3));
        coordinates[3] = Double.parseDouble(matcher.group(4));

        final double tolerance = 1E-3;

        assertEquals(2.345, coordinates[0], tolerance);
        assertEquals(1.234, coordinates[1], tolerance);
        assertEquals(4.567, coordinates[2], tolerance);
        assertEquals(3.456, coordinates[3], tolerance);
    }

    @Test
    public void convertDggsCellOutlineToShapeStringTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final DggsCell cellId = new DggsCell("07231131111113100331032");

        final String shapeString = dggs.convertDggsCellOutlineToShapeString(cellId, ShapeStringFormat.WKT);

        // Values in the string may vary due to numerical precision - extract them using a regex and compare them as
        // doubles
        final Pattern pattern = Pattern.compile(
                "POLYGON \\(\\((\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*),(\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*),(\\d+\\.?\\d*)\\s+(\\d+\\.?\\d*)\\)\\)");

        final Matcher matcher = pattern.matcher(shapeString);

        final double[] coordinates = new double[6];

        assertTrue(matcher.matches());
        coordinates[0] = Double.parseDouble(matcher.group(1));
        coordinates[1] = Double.parseDouble(matcher.group(2));
        coordinates[2] = Double.parseDouble(matcher.group(3));
        coordinates[3] = Double.parseDouble(matcher.group(4));
        coordinates[4] = Double.parseDouble(matcher.group(5));
        coordinates[5] = Double.parseDouble(matcher.group(6));

        final double tolerance = 1E-3;

        assertEquals(2.3449866, coordinates[0], tolerance);
        assertEquals(1.2340080, coordinates[1], tolerance);
        assertEquals(2.3450042, coordinates[2], tolerance);
        assertEquals(1.2339790, coordinates[3], tolerance);
        assertEquals(2.3450218, coordinates[4], tolerance);
        assertEquals(1.2340036, coordinates[5], tolerance);
    }

    @Test
    public void getCellParentsTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA3H);

        // Cell with one parent
        DggsCell[] parentCells = dggs.getCellParents(new DggsCell("01033,6"));

        assertEquals(1, parentCells.length);
        assertEquals("01022,3", parentCells[0].getCellId());

        // Cell with three parents
        parentCells = dggs.getCellParents(new DggsCell("01035,2"));

        assertEquals(3, parentCells.length);
        assertEquals("01023,1", parentCells[0].getCellId());
        assertEquals("01024,1", parentCells[1].getCellId());
        assertEquals("01023,0", parentCells[2].getCellId());
    }

    @Test
    public void getCellChildrenTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA3H);

        final DggsCell[] childCells = dggs.getCellChildren(new DggsCell("01031,3"));

        assertEquals(7, childCells.length);
        assertEquals("01043,4", childCells[0].getCellId());
        assertEquals("01042,4", childCells[1].getCellId());
        assertEquals("01043,5", childCells[2].getCellId());
        assertEquals("01044,4", childCells[3].getCellId());
        assertEquals("01043,3", childCells[4].getCellId());
        assertEquals("01042,5", childCells[5].getCellId());
        assertEquals("01044,5", childCells[6].getCellId());
    }

    @Test
    public void getCellSiblingsTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA3H);

        final DggsCell[] siblingCells = dggs.getCellSiblings(new DggsCell("01054,2"));

        assertEquals(15, siblingCells.length);
        assertEquals("01054,3", siblingCells[0].getCellId());
        assertEquals("01053,3", siblingCells[1].getCellId());
        assertEquals("01054,4", siblingCells[2].getCellId());
        assertEquals("01055,3", siblingCells[3].getCellId());
        assertEquals("01055,4", siblingCells[4].getCellId());
        assertEquals("01055,2", siblingCells[5].getCellId());
        assertEquals("01054,1", siblingCells[6].getCellId());
        assertEquals("01053,1", siblingCells[7].getCellId());
        assertEquals("01055,1", siblingCells[8].getCellId());
        assertEquals("01054,0", siblingCells[9].getCellId());
        assertEquals("01055,0", siblingCells[10].getCellId());
        assertEquals("01053,2", siblingCells[11].getCellId());
        assertEquals("01052,2", siblingCells[12].getCellId());
        assertEquals("01052,3", siblingCells[13].getCellId());
        assertEquals("01052,1", siblingCells[14].getCellId());
    }

    @Test
    public void getBoundingCellTest() throws EaggrException, EaggrLibraryException {
        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final DggsCell[] cellIds = new DggsCell[] { new DggsCell("07001"), new DggsCell("07002"),
                new DggsCell("07003") };

        DggsCell boundingCell = dggs.getBoundingCell(cellIds);

        assertEquals("0700", boundingCell.getCellId());
    }

    @Test
    public void createKmlFileTest() throws EaggrException, EaggrLibraryException, IOException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);

        final DggsCell[] cellIds = new DggsCell[] { new DggsCell("07231131111113100331001"),
                new DggsCell("07012000001303022011321") };

        final String outputKmlFileName = "../EAGGRJava/testData/EaggrTestOutput.kml";

        dggs.createKmlFile(outputKmlFileName, cellIds);

        final String expectedKmlFileName = "../EAGGRJava/testData/ExpectedEaggrLibraryTestOutput.kml";

        // Ensure the files are identical
        final String actualOutput = TestUtilities
                .removeEndOfLineCharacters(new String(Files.readAllBytes(Paths.get(outputKmlFileName))));
        final String expectedOutput = TestUtilities
                .removeEndOfLineCharacters(new String(Files.readAllBytes(Paths.get(expectedKmlFileName))));

        assertEquals(expectedOutput, actualOutput);

        final File outputKmlFile = new File(outputKmlFileName);
        outputKmlFile.delete();
    }

    @Test(expected = EaggrLibraryException.class)
    public void cellIdTooLongTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        try {
            final DggsCell[] cellIds = new DggsCell[] { new DggsCell("0123012301230123012301230123012301230123012") };

            dggs.convertDggsCellsToPoints(cellIds);
        } catch (final EaggrException e) {
            // Discrepancy between number of characters exceeded between eclipse and ant build.
            // Probably related to similar issue in the library itself.
            // For now only check that the exception message starts as we expect.
            assertTrue(e.getMessage().startsWith("DGGS Exception: Cell ID exceeds maximum length "));
            throw e;
        }
    }

    @Test
    public void compareShapesEqualsTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the cell shape with itself
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DGGS_SHAPE_CELL, AnalysisType.EQUALS);
        assertTrue(comparisonResult);

        // Compare the shape cell with a disjoint shape cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.EQUALS);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Compare the shape linestring with itself
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DGGS_SHAPE_LINESTRING, AnalysisType.EQUALS);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a disjoint shape linestring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.EQUALS);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the shape polygon with itself
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DGGS_SHAPE_POLYGON, AnalysisType.EQUALS);
        assertTrue(comparisonResult);

        // Compare the polygon with a disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.EQUALS);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesContainsTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the shape cell with a shape cell containing a child cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.CHILD_DGGS_SHAPE_CELL, AnalysisType.CONTAINS);
        assertTrue(comparisonResult);

        // Compare the shape cell with a disjoint shape cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.CONTAINS);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Compare the shape linestring with another shape linestring, containing a substring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.SUBSTRING_DGGS_SHAPE_LINESTRING, AnalysisType.CONTAINS);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a different shape linestring, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.CONTAINS);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the polygon shape with a different shape containing a polygon found in the interior of the first
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.INTERIOR_DGGS_SHAPE_POLYGON, AnalysisType.CONTAINS);
        assertTrue(comparisonResult);

        // Compare the shape polygon with a shape containing a disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.CONTAINS);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesWithinTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the shape cell with a shape cell containing a parent cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.PARENT_DGGS_SHAPE_CELL, AnalysisType.WITHIN);
        assertTrue(comparisonResult);

        // Compare the shape cell with a different shape cell containing a disjoint cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.WITHIN);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Compare the shape linestring with another shape linestring, containing a superstring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.SUPERSTRING_DGGS_SHAPE_LINESTRING, AnalysisType.WITHIN);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a different shape, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.WITHIN);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the shape polygon with a different shape polygon containing the polygon that contains the first
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.CONTAINS_DGGS_SHAPE_POLYGON, AnalysisType.WITHIN);
        assertTrue(comparisonResult);

        // Compare the shape polygon with a different shape polygon, containing the disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.WITHIN);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesTouchesTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the shape cell with a shape cell containing a edge cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.EDGE_SHARING_DGGS_SHAPE_CELL, AnalysisType.TOUCHES);
        assertTrue(comparisonResult);

        // Compare the shape cell with a different shape cell containing a disjoint cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.TOUCHES);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Construct a DGGS shape from a linestring that touches the end of the linestring with one of its ends
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.ENDS_TOUCHING_DGGS_SHAPE_LINESTRING, AnalysisType.TOUCHES);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a different shape, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.TOUCHES);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the shape polygon with a different shape polygon, containing the polygon that shares an edge with the
        // first
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.EDGE_SHARING_DGGS_SHAPE_POLYGON, AnalysisType.TOUCHES);
        assertTrue(comparisonResult);

        // Compare the shape polygon with a different shape polygon, containing the disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.TOUCHES);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesDisjointTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the shape cell with a shape cell containing a disjoint cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.DISJOINT);
        assertTrue(comparisonResult);

        // Compare the shape cell with a different shape cell containing an edge sharing cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.EDGE_SHARING_DGGS_SHAPE_CELL, AnalysisType.DISJOINT);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Compare the shape linestring with a different shape, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.DISJOINT);
        assertTrue(comparisonResult);

        // Compare the shape linestring with another shape, containing a linestring that touches the end of the first
        // with an end
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.ENDS_TOUCHING_DGGS_SHAPE_LINESTRING, AnalysisType.DISJOINT);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the shape polygon with a different shape polygon, containing the disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.DISJOINT);
        assertTrue(comparisonResult);

        // Compare the shape polygon with a different shape polygon, containing the polygon that shares an edge with the
        // first
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.EDGE_SHARING_DGGS_SHAPE_POLYGON, AnalysisType.DISJOINT);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesIntersectsTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the shape cell with a different shape cell containing an edge sharing cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.EDGE_SHARING_DGGS_SHAPE_CELL, AnalysisType.INTERSECTS);
        assertTrue(comparisonResult);

        // Compare the shape cell with a shape cell containing a disjoint cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.INTERSECTS);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Compare the shape linestring with another shape, containing a linestring that touches the end of the first
        // with an end
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.ENDS_TOUCHING_DGGS_SHAPE_LINESTRING, AnalysisType.INTERSECTS);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a different shape, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.INTERSECTS);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the shape polygon with a different shape polygon, containing the polygon that shares an edge with the
        // first
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.EDGE_SHARING_DGGS_SHAPE_POLYGON, AnalysisType.INTERSECTS);
        assertTrue(comparisonResult);

        // Compare the shape polygon with a different shape polygon, containing the disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.INTERSECTS);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesCoversTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the shape cell with a different shape cell containing a child cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.CHILD_DGGS_SHAPE_CELL, AnalysisType.COVERS);
        assertTrue(comparisonResult);

        // Compare the shape cell with a shape cell containing a disjoint cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.COVERS);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Compare the shape linestring with another shape linestring, containing a substring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.SUBSTRING_DGGS_SHAPE_LINESTRING, AnalysisType.COVERS);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a different shape, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.COVERS);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the shape polygon with a different shape polygon, containing the polygon that lies within the
        // interior of the first
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.INTERIOR_DGGS_SHAPE_POLYGON, AnalysisType.COVERS);
        assertTrue(comparisonResult);

        // Compare the shape polygon with a different shape polygon, containing the disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.COVERS);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesCoveredByTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA4T);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the shape cell with a different shape cell containing a parent cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.PARENT_DGGS_SHAPE_CELL, AnalysisType.COVERED_BY);
        assertTrue(comparisonResult);

        // Compare the shape cell with a shape cell containing a disjoint cell
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_CELL,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.COVERED_BY);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Compare the shape linestring with another shape linestring, containing a superstring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.SUPERSTRING_DGGS_SHAPE_LINESTRING, AnalysisType.COVERED_BY);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a different shape, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_LINESTRING,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.COVERED_BY);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the shape polygon with a different shape polygon, containing the polygon that contains the first
        // polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.CONTAINS_DGGS_SHAPE_POLYGON, AnalysisType.COVERED_BY);
        assertTrue(comparisonResult);

        // Compare the shape polygon with a different shape polygon, containing the disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataTriangular.DGGS_SHAPE_POLYGON,
                DggsTestDataTriangular.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.COVERED_BY);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesCrossesTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA3H);
        boolean comparisonResult;

        // Test the cell shape
        // Two cells can never cross as they are treated as polygons

        // Compare the shape cell with a different shape cell containing an overlapping child cell
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_CELL,
                DggsTestDataHexagonal.OVERLAPPING_CHILD_DGGS_SHAPE_CELL, AnalysisType.CROSSES);
        assertFalse(comparisonResult);

        // Compare the shape cell with a shape cell containing a disjoint cell
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_CELL,
                DggsTestDataHexagonal.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.CROSSES);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Construct a DGGS shape from a linestring that crosses the base linestring at a single point
        final ArrayList<DggsCell> crossingLinestringCellIds = new ArrayList<DggsCell>();
        final DggsCell crossingLinestringDggsCell1 = new DggsCell("00022,3");
        final DggsCell crossingLinestringDggsCell2 = new DggsCell("00023,1");
        final DggsCell crossingLinestringDggsCell3 = new DggsCell("00024,0");
        crossingLinestringCellIds.add(crossingLinestringDggsCell1);
        crossingLinestringCellIds.add(crossingLinestringDggsCell2);
        crossingLinestringCellIds.add(crossingLinestringDggsCell3);
        final DggsLinestring crossingDggsLinestring = new DggsLinestring(crossingLinestringCellIds);
        final DggsShape crossingDggsShapeLineString = new DggsShape(crossingDggsLinestring);

        // Compare the shape linestring with another shape linestring, containing a linestring that crosses the first
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_LINESTRING, crossingDggsShapeLineString,
                AnalysisType.CROSSES);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a different shape, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_LINESTRING,
                DggsTestDataHexagonal.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.CROSSES);
        assertFalse(comparisonResult);

        // Test the polygon shape
        // A polygon can never cross another polygon

        // Compare the shape polygon with a different shape polygon, containing the polygon that overlaps the first
        // polygon
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_POLYGON,
                DggsTestDataHexagonal.OVERLAPPING_DGGS_SHAPE_POLYGON, AnalysisType.CROSSES);
        assertFalse(comparisonResult);

        // Compare the shape polygon with a different shape polygon, containing the disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_POLYGON,
                DggsTestDataHexagonal.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.CROSSES);
        assertFalse(comparisonResult);
    }

    @Test
    public void compareShapesOverlapsTest() throws EaggrException, EaggrLibraryException {

        final Eaggr dggs = new Eaggr(DggsModel.ISEA3H);
        boolean comparisonResult;

        // Test the cell shape

        // Compare the shape cell with a different shape cell containing an overlapping child cell
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_CELL,
                DggsTestDataHexagonal.OVERLAPPING_CHILD_DGGS_SHAPE_CELL, AnalysisType.OVERLAPS);
        assertTrue(comparisonResult);

        // Compare the shape cell with a shape cell containing a disjoint cell
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_CELL,
                DggsTestDataHexagonal.DISJOINT_DGGS_SHAPE_CELL, AnalysisType.OVERLAPS);
        assertFalse(comparisonResult);

        // Test the linestring shape

        // Construct a DGGS shape from a linestring that partially overlaps the base linestring
        final ArrayList<DggsCell> overlappingLinestringCellIds = new ArrayList<DggsCell>();
        final DggsCell overlappingLinestringDggsCell1 = new DggsCell("00021,4");
        final DggsCell overlappingLinestringDggsCell2 = new DggsCell("00021,2");
        final DggsCell overlappingLinestringDggsCell3 = new DggsCell("00023,1");
        overlappingLinestringCellIds.add(overlappingLinestringDggsCell1);
        overlappingLinestringCellIds.add(overlappingLinestringDggsCell2);
        overlappingLinestringCellIds.add(overlappingLinestringDggsCell3);
        final DggsLinestring overlappingDggsLinestring = new DggsLinestring(overlappingLinestringCellIds);
        final DggsShape overlappingDggsShapeLineString = new DggsShape(overlappingDggsLinestring);

        // Compare the shape linestring with another shape linestring, containing a linestring that partially overlaps
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_LINESTRING,
                overlappingDggsShapeLineString, AnalysisType.OVERLAPS);
        assertTrue(comparisonResult);

        // Compare the shape linestring with a different shape, containing a disjoint linestring
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_LINESTRING,
                DggsTestDataHexagonal.DISJOINT_DGGS_SHAPE_LINESTRING, AnalysisType.OVERLAPS);
        assertFalse(comparisonResult);

        // Test the polygon shape

        // Compare the shape polygon with a different shape polygon, containing the polygon that overlaps the first
        // polygon
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_POLYGON,
                DggsTestDataHexagonal.OVERLAPPING_DGGS_SHAPE_POLYGON, AnalysisType.OVERLAPS);
        assertTrue(comparisonResult);

        // Compare the shape polygon with a different shape polygon, containing the disjoint polygon
        comparisonResult = dggs.compareShapes(DggsTestDataHexagonal.DGGS_SHAPE_POLYGON,
                DggsTestDataHexagonal.DISJOINT_DGGS_SHAPE_POLYGON, AnalysisType.OVERLAPS);
        assertFalse(comparisonResult);
    }
}
