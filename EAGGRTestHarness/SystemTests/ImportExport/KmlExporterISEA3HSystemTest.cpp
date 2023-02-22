//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: System Tests
//
//------------------------------------------------------
/// @file KmlExporterISEA3HSystemTest.cpp
///
/// System tests to visualise DGGS data
///
/// This file is part of OpenEAGGR.
///
/// OpenEAGGR is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// OpenEAGGR is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Lesser General Public License for more details.
///
/// A copy of the GNU Lesser General Public License is available in COPYING.LESSER
/// or can be found at <http://www.gnu.org/licenses/>.
//------------------------------------------------------

#include "TestMacros.hpp"

#include <fstream>
#include <sstream>

#include "Src/ImportExport/KmlExporter.hpp"
#include "Src/Model/IProjection/Snyder.hpp"
#include "Src/Model/IPolyhedralGlobe/Icosahedron.hpp"
#include "Src/Model/IGridIndexer/OffsetGridIndexer.hpp"
#include "Src/Model/IGrid/IOffsetGrid/Aperture3HexagonGrid.hpp"
#include "Src/LatLong/SphericalAccuracyPoint.hpp"

#include "TestUtilities/KmlFileMatcher.hpp"

using namespace EAGGR::Model;
using namespace EAGGR::Model::Projection;
using namespace EAGGR::Model::PolyhedralGlobe;
using namespace EAGGR::Model::Grid::OffsetGrid;
using namespace EAGGR::Model::GridIndexer;
using namespace EAGGR::ImportExport;

/// Set this flag to rewrite the expected KML files - use if the expected output changes.
static const bool RECREATE_KML_FILES = false;

/// Ensures tests cannot pass with the RECREATE_KML_FILES flag set.
SYSTEM_TEST(KMLExporterISEA3H, CheckRecreateKmlFiles)
{
  ASSERT_FALSE(RECREATE_KML_FILES);
}

SYSTEM_TEST(KMLExporterISEA3H, IcosahedronAllFaces)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture3HexagonGrid grid;
  OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  std::unique_ptr<Cell::ICell> face0 = indexer.CreateCell("00000,0");
  std::unique_ptr<Cell::ICell> face1 = indexer.CreateCell("01000,0");
  std::unique_ptr<Cell::ICell> face2 = indexer.CreateCell("02000,0");
  std::unique_ptr<Cell::ICell> face3 = indexer.CreateCell("03000,0");
  std::unique_ptr<Cell::ICell> face4 = indexer.CreateCell("04000,0");
  std::unique_ptr<Cell::ICell> face5 = indexer.CreateCell("05000,0");
  std::unique_ptr<Cell::ICell> face6 = indexer.CreateCell("06000,0");
  std::unique_ptr<Cell::ICell> face7 = indexer.CreateCell("07000,0");
  std::unique_ptr<Cell::ICell> face8 = indexer.CreateCell("08000,0");
  std::unique_ptr<Cell::ICell> face9 = indexer.CreateCell("09000,0");
  std::unique_ptr<Cell::ICell> face10 = indexer.CreateCell("10000,0");
  std::unique_ptr<Cell::ICell> face11 = indexer.CreateCell("11000,0");
  std::unique_ptr<Cell::ICell> face12 = indexer.CreateCell("12000,0");
  std::unique_ptr<Cell::ICell> face13 = indexer.CreateCell("13000,0");
  std::unique_ptr<Cell::ICell> face14 = indexer.CreateCell("14000,0");
  std::unique_ptr<Cell::ICell> face15 = indexer.CreateCell("15000,0");
  std::unique_ptr<Cell::ICell> face16 = indexer.CreateCell("16000,0");
  std::unique_ptr<Cell::ICell> face17 = indexer.CreateCell("17000,0");
  std::unique_ptr<Cell::ICell> face18 = indexer.CreateCell("18000,0");
  std::unique_ptr<Cell::ICell> face19 = indexer.CreateCell("19000,0");

  // Add the cells to the exporter
  exporter.AddCell(*face0);
  exporter.AddCell(*face1);
  exporter.AddCell(*face2);
  exporter.AddCell(*face3);
  exporter.AddCell(*face4);
  exporter.AddCell(*face5);
  exporter.AddCell(*face6);
  exporter.AddCell(*face7);
  exporter.AddCell(*face8);
  exporter.AddCell(*face9);
  exporter.AddCell(*face10);
  exporter.AddCell(*face11);
  exporter.AddCell(*face12);
  exporter.AddCell(*face13);
  exporter.AddCell(*face14);
  exporter.AddCell(*face15);
  exporter.AddCell(*face16);
  exporter.AddCell(*face17);
  exporter.AddCell(*face18);
  exporter.AddCell(*face19);

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream ouputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HIcosahedronAllFacesKmlExport.kml");
    exporter.Export(ouputFile);
    ouputFile.close();
  }

  // Read expected output from file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HIcosahedronAllFacesKmlExport.kml");

  std::istringstream actualStream(output.str());

  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }
}

SYSTEM_TEST(KMLExporterISEA3H, AllFacesResolution1)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture3HexagonGrid grid;
  OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  std::unique_ptr<Cell::ICell> cell0 = indexer.CreateCell("00010,0");
  std::unique_ptr<Cell::ICell> cell1 = indexer.CreateCell("01010,0");
  std::unique_ptr<Cell::ICell> cell2 = indexer.CreateCell("02010,0");
  std::unique_ptr<Cell::ICell> cell3 = indexer.CreateCell("03010,0");
  std::unique_ptr<Cell::ICell> cell4 = indexer.CreateCell("04010,0");
  std::unique_ptr<Cell::ICell> cell5 = indexer.CreateCell("05010,0");
  std::unique_ptr<Cell::ICell> cell6 = indexer.CreateCell("06010,0");
  std::unique_ptr<Cell::ICell> cell7 = indexer.CreateCell("07010,0");
  std::unique_ptr<Cell::ICell> cell8 = indexer.CreateCell("08010,0");
  std::unique_ptr<Cell::ICell> cell9 = indexer.CreateCell("09010,0");
  std::unique_ptr<Cell::ICell> cell10 = indexer.CreateCell("10010,0");
  std::unique_ptr<Cell::ICell> cell11 = indexer.CreateCell("11010,0");
  std::unique_ptr<Cell::ICell> cell12 = indexer.CreateCell("12010,0");
  std::unique_ptr<Cell::ICell> cell13 = indexer.CreateCell("13010,0");
  std::unique_ptr<Cell::ICell> cell14 = indexer.CreateCell("14010,0");
  std::unique_ptr<Cell::ICell> cell15 = indexer.CreateCell("15010,0");
  std::unique_ptr<Cell::ICell> cell16 = indexer.CreateCell("16010,0");
  std::unique_ptr<Cell::ICell> cell17 = indexer.CreateCell("17010,0");
  std::unique_ptr<Cell::ICell> cell18 = indexer.CreateCell("18010,0");
  std::unique_ptr<Cell::ICell> cell19 = indexer.CreateCell("19010,0");

  // Add the cells to the exporter
  exporter.AddCell(*cell0);
  exporter.AddCell(*cell1);
  exporter.AddCell(*cell2);
  exporter.AddCell(*cell3);
  exporter.AddCell(*cell4);
  exporter.AddCell(*cell5);
  exporter.AddCell(*cell6);
  exporter.AddCell(*cell7);
  exporter.AddCell(*cell8);
  exporter.AddCell(*cell9);
  exporter.AddCell(*cell10);
  exporter.AddCell(*cell11);
  exporter.AddCell(*cell12);
  exporter.AddCell(*cell13);
  exporter.AddCell(*cell14);
  exporter.AddCell(*cell15);
  exporter.AddCell(*cell16);
  exporter.AddCell(*cell17);
  exporter.AddCell(*cell18);
  exporter.AddCell(*cell19);

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream ouputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HAllFacesResolution1.kml");
    exporter.Export(ouputFile);
    ouputFile.close();
  }

  // Read expected output from file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HAllFacesResolution1.kml");

  std::istringstream actualStream(output.str());

  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }
}

SYSTEM_TEST(KMLExporterISEA3H, Face3Resolution2)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture3HexagonGrid grid;
  OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  std::unique_ptr<Cell::ICell> cell0 = indexer.CreateCell("03020,0");
  std::unique_ptr<Cell::ICell> cell1 = indexer.CreateCell("03020,1");
  std::unique_ptr<Cell::ICell> cell2 = indexer.CreateCell("03020,-1");
  std::unique_ptr<Cell::ICell> cell3 = indexer.CreateCell("03021,-1");
  std::unique_ptr<Cell::ICell> cell4 = indexer.CreateCell("03021,0");
  std::unique_ptr<Cell::ICell> cell5 = indexer.CreateCell("0302-1,-1");
  std::unique_ptr<Cell::ICell> cell6 = indexer.CreateCell("0302-1,0");

  // Add the cells to the exporter
  exporter.AddCell(*cell0);
  exporter.AddCell(*cell1);
  exporter.AddCell(*cell2);
  exporter.AddCell(*cell3);
  exporter.AddCell(*cell4);
  exporter.AddCell(*cell5);
  exporter.AddCell(*cell6);

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream ouputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HFace3Resolution2.kml");
    exporter.Export(ouputFile);
    ouputFile.close();
  }

  // Read expected output from file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HFace3Resolution2.kml");

  std::istringstream actualStream(output.str());

  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }
}

SYSTEM_TEST(KMLExporterISEA3H, Face3Resolution3)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture3HexagonGrid grid;
  OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  std::unique_ptr<Cell::ICell> cell0 = indexer.CreateCell("0303-2,-1");
  std::unique_ptr<Cell::ICell> cell1 = indexer.CreateCell("0303-2,1");
  std::unique_ptr<Cell::ICell> cell2 = indexer.CreateCell("0303-1,-2");
  std::unique_ptr<Cell::ICell> cell3 = indexer.CreateCell("0303-1,-1");
  std::unique_ptr<Cell::ICell> cell4 = indexer.CreateCell("0303-1,0");
  std::unique_ptr<Cell::ICell> cell5 = indexer.CreateCell("0303-1,1");
  std::unique_ptr<Cell::ICell> cell6 = indexer.CreateCell("0303-1,2");
  std::unique_ptr<Cell::ICell> cell7 = indexer.CreateCell("03030,-2");
  std::unique_ptr<Cell::ICell> cell8 = indexer.CreateCell("03030,-1");
  std::unique_ptr<Cell::ICell> cell9 = indexer.CreateCell("03030,0");
  std::unique_ptr<Cell::ICell> cell10 = indexer.CreateCell("03030,1");
  std::unique_ptr<Cell::ICell> cell11 = indexer.CreateCell("03030,2");
  std::unique_ptr<Cell::ICell> cell12 = indexer.CreateCell("03031,-1");
  std::unique_ptr<Cell::ICell> cell13 = indexer.CreateCell("03031,0");
  std::unique_ptr<Cell::ICell> cell14 = indexer.CreateCell("03031,1");
  std::unique_ptr<Cell::ICell> cell15 = indexer.CreateCell("03032,0");

  // Add the cells to the exporter
  exporter.AddCell(*cell0);
  exporter.AddCell(*cell1);
  exporter.AddCell(*cell2);
  exporter.AddCell(*cell3);
  exporter.AddCell(*cell4);
  exporter.AddCell(*cell5);
  exporter.AddCell(*cell6);
  exporter.AddCell(*cell7);
  exporter.AddCell(*cell8);
  exporter.AddCell(*cell9);
  exporter.AddCell(*cell10);
  exporter.AddCell(*cell11);
  exporter.AddCell(*cell12);
  exporter.AddCell(*cell13);
  exporter.AddCell(*cell14);
  exporter.AddCell(*cell15);

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream ouputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HFace3Resolution3.kml");
    exporter.Export(ouputFile);
    ouputFile.close();
  }

  // Read expected output from file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HFace3Resolution3.kml");

  std::istringstream actualStream(output.str());

  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }
}

SYSTEM_TEST(KMLExporterISEA3H, Cities100m2Accuracy)
{
  static const unsigned short NO_OF_CITIES = 4U;
  double coordData[NO_OF_CITIES][2] =
  {
    // Lat, Long
    { 51.5072, -0.1275}, // London
    { -33.8650, 151.2094}, // Sydney
    { 1.3000, 103.8000}, // Singapore
    { -22.9068, -43.1729}  // Rio de Janeiro
  };

  Icosahedron globe;
  Snyder projection(&globe);
  Aperture3HexagonGrid grid;
  OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  for (int index = 0; index < NO_OF_CITIES; ++index)
  {
    EAGGR::LatLong::Wgs84AccuracyPoint point(coordData[index][0], coordData[index][1],
        100.0);

    EAGGR::CoordinateConversion::CoordinateConverter converter;
    EAGGR::LatLong::SphericalAccuracyPoint sphericalPoint = converter.ConvertWGS84ToSphere(point);

    FaceCoordinate faceCoord = projection.GetFaceCoordinate(sphericalPoint);
    std::unique_ptr<Cell::ICell> cell = indexer.GetCell(faceCoord);

    exporter.AddCell(*cell);
  }

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream ouputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HCities100m2Accuracy.kml");
    exporter.Export(ouputFile);
    ouputFile.close();
  }

  // Read expected output from file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HCities100m2Accuracy.kml");

  std::istringstream actualStream(output.str());

  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }
}

SYSTEM_TEST(KMLExporterISEA3H, GridTest)
{
  // Five metre intervals at 51.0, 0.0 as determined from http://www.movable-type.co.uk/scripts/latlong.html
  const double latitudeFiveMetreSpacing = 0.000045;
  const double longitudeFiveMetreSpacing = 0.000072;

  const double baseLatitude = 51.0;
  const double baseLongitude = 0.0;

  Icosahedron globe;
  Snyder projection(&globe);
  Aperture3HexagonGrid grid;
  OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer, false);

  for (int latIndex = 0; latIndex < 10; ++latIndex)
  {
    for (int longIndex = 0; longIndex < 10; ++longIndex)
    {
      EAGGR::LatLong::Wgs84AccuracyPoint point(
          baseLatitude + (latIndex * latitudeFiveMetreSpacing),
          baseLongitude + (longIndex * longitudeFiveMetreSpacing),
          100.0);

      EAGGR::CoordinateConversion::CoordinateConverter converter;
      EAGGR::LatLong::SphericalAccuracyPoint sphericalPoint = converter.ConvertWGS84ToSphere(point);

      FaceCoordinate faceCoord = projection.GetFaceCoordinate(sphericalPoint);
      std::unique_ptr<Cell::ICell> cell = indexer.GetCell(faceCoord);

      exporter.AddCell(*cell);

      std::string cellId = cell->GetCellId();
      exporter.AddLocation(point, cellId);
    }
  }

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream ouputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HGrid100m2Accuracy.kml");
    exporter.Export(ouputFile);
    ouputFile.close();
  }

  // Read expected output from file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HGrid100m2Accuracy.kml");

  std::istringstream actualStream(output.str());

  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }
}

SYSTEM_TEST(KMLExporterISEA3H, ParentsTest)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture3HexagonGrid grid;
  OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  EAGGR::LatLong::Wgs84AccuracyPoint point(51.0, 0.0, 100.0);

  EAGGR::CoordinateConversion::CoordinateConverter converter;
  EAGGR::LatLong::SphericalAccuracyPoint sphericalPoint = converter.ConvertWGS84ToSphere(point);

  FaceCoordinate faceCoord = projection.GetFaceCoordinate(sphericalPoint);
  std::unique_ptr<Cell::ICell> cell = indexer.GetCell(faceCoord);

  exporter.AddCell(*cell);

  std::vector< std::unique_ptr<Cell::ICell> > parentCells;
  indexer.GetParents(*cell, parentCells);

  for (std::vector< std::unique_ptr<Cell::ICell> >::const_iterator parent = parentCells.begin();
      parent != parentCells.end();
      ++parent)
  {
    exporter.AddCell(**parent);
  }

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream ouputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HParents.kml");
    exporter.Export(ouputFile);
    ouputFile.close();
  }

  // Read expected output from file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HParents.kml");

  std::istringstream actualStream(output.str());

  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }
}

SYSTEM_TEST(KMLExporterISEA3H, ChildrenTest)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture3HexagonGrid grid;
  OffsetGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  EAGGR::LatLong::Wgs84AccuracyPoint point(51.0, 0.0, 100.0);

  EAGGR::CoordinateConversion::CoordinateConverter converter;
  EAGGR::LatLong::SphericalAccuracyPoint sphericalPoint = converter.ConvertWGS84ToSphere(point);

  FaceCoordinate faceCoord = projection.GetFaceCoordinate(sphericalPoint);
  std::unique_ptr<Cell::ICell> cell = indexer.GetCell(faceCoord);

  exporter.AddCell(*cell);

  std::vector< std::unique_ptr<Cell::ICell> > childCells;
  indexer.GetChildren(*cell, childCells);

  for (std::vector< std::unique_ptr<Cell::ICell> >::const_iterator child = childCells.begin();
      child != childCells.end();
      ++child)
  {
    exporter.AddCell(**child);
  }

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream ouputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HChildren.kml");
    exporter.Export(ouputFile);
    ouputFile.close();
  }

  // Read expected output from file
  std::ifstream expectedFile;
  expectedFile.open("OpenEAGGR/EAGGRTestHarness/TestData/ISEA3HChildren.kml");

  std::istringstream actualStream(output.str());

  EAGGR::TestUtilities::KmlFileMatcher matcher;
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }
}
