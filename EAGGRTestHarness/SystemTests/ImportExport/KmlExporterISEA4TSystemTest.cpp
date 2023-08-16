//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: System Tests
//
//------------------------------------------------------
/// @file KmlExporterISEA4TSystemTest.cpp
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
#include "Src/Model/IGridIndexer/HierarchicalGridIndexer.hpp"
#include "Src/Model/IGrid/IHierarchicalGrid/Aperture4TriangleGrid.hpp"
#include "Src/LatLong/SphericalAccuracyPoint.hpp"

#include "TestUtilities/KmlFileMatcher.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <Windows.h>
    #define IS_ON_WINDOWS true
#endif

using namespace EAGGR::Model;
using namespace EAGGR::Model::Projection;
using namespace EAGGR::Model::PolyhedralGlobe;
using namespace EAGGR::Model::Grid::HierarchicalGrid;
using namespace EAGGR::Model::GridIndexer;
using namespace EAGGR::ImportExport;

/// Set this flag to rewrite the expected KML files - use if the expected output changes.
static const bool RECREATE_KML_FILES = false;

void CheckKMLFile(const std::string& a_expectedFilePath, const std::ostringstream& a_actualString)
{
    struct stat buffer;   
    const bool fileExists = (stat (a_expectedFilePath.c_str(), &buffer) == 0); 

    ASSERT_TRUE(fileExists) << "Expected file does not exist\n";

    // Read expected output from file
    std::ifstream expectedFile;
    expectedFile.open(a_expectedFilePath);

    std::istringstream actualStream(a_actualString.str());

    EAGGR::TestUtilities::KmlFileMatcher matcher;
    bool filesMatch = matcher.Compare(expectedFile, actualStream);
    if (!filesMatch)
    {
        SCOPED_TRACE(matcher.GetLastErrorMessage());
        ASSERT_TRUE(filesMatch);
    }
}

/// Ensures tests cannot pass with the RECREATE_KML_FILES flag set.
SYSTEM_TEST(KMLExporterISEA4T, CheckRecreateKmlFiles)
{
  ASSERT_FALSE(RECREATE_KML_FILES);
  struct stat buffer;   
  const bool fileExists = (stat ("OpenEAGGR/EAGGRTestHarness", &buffer) == 0); 

  // Helpful debugging statements
#if defined(IS_ON_WINDOWS)
  TCHAR cwdPath[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, cwdPath);
#else
  const char* cwdPath = "Unknown";
#endif
  ASSERT_TRUE(fileExists) << "Current working directory is set up wrong:\n\t"
      "When checking out be sure to name the repo \"OpenEAGGR\", CWD should be the folder above \"OpenEAGGR\".\n"
      "Current path is: \"" << cwdPath << "\"\n it should not include any open eaggr files.";

}

SYSTEM_TEST(KMLExporterISEA4T, IcosahedronAllFaces)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture4TriangleGrid grid;
  HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  std::unique_ptr<Cell::ICell> face0 = indexer.CreateCell("00");
  std::unique_ptr<Cell::ICell> face1 = indexer.CreateCell("01");
  std::unique_ptr<Cell::ICell> face2 = indexer.CreateCell("02");
  std::unique_ptr<Cell::ICell> face3 = indexer.CreateCell("03");
  std::unique_ptr<Cell::ICell> face4 = indexer.CreateCell("04");
  std::unique_ptr<Cell::ICell> face5 = indexer.CreateCell("05");
  std::unique_ptr<Cell::ICell> face6 = indexer.CreateCell("06");
  std::unique_ptr<Cell::ICell> face7 = indexer.CreateCell("07");
  std::unique_ptr<Cell::ICell> face8 = indexer.CreateCell("08");
  std::unique_ptr<Cell::ICell> face9 = indexer.CreateCell("09");
  std::unique_ptr<Cell::ICell> face10 = indexer.CreateCell("10");
  std::unique_ptr<Cell::ICell> face11 = indexer.CreateCell("11");
  std::unique_ptr<Cell::ICell> face12 = indexer.CreateCell("12");
  std::unique_ptr<Cell::ICell> face13 = indexer.CreateCell("13");
  std::unique_ptr<Cell::ICell> face14 = indexer.CreateCell("14");
  std::unique_ptr<Cell::ICell> face15 = indexer.CreateCell("15");
  std::unique_ptr<Cell::ICell> face16 = indexer.CreateCell("16");
  std::unique_ptr<Cell::ICell> face17 = indexer.CreateCell("17");
  std::unique_ptr<Cell::ICell> face18 = indexer.CreateCell("18");
  std::unique_ptr<Cell::ICell> face19 = indexer.CreateCell("19");
  {
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
  }
  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  const std::string outputFilePath("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TIcosahedronAllFacesKmlExport.kml");

  if (RECREATE_KML_FILES)
  {
    std::ofstream outputFile(outputFilePath);
    exporter.Export(outputFile);
    outputFile.close();
  }

  CheckKMLFile(outputFilePath, output);
}

SYSTEM_TEST(KMLExporterISEA4T, AllFacesResolution1)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture4TriangleGrid grid;
  HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  for (int face = 0; face < globe.GetNoOfFaces(); ++face)
  {
    for (int partition = 0; partition < grid.GetNumChildren(); ++partition)
    {
      std::stringstream cellId;
      cellId << std::setw(2) << std::setfill('0') << face;
      cellId << partition;

      std::unique_ptr<Cell::ICell> cell = indexer.CreateCell(cellId.str());
      exporter.AddCell(*cell);
    }
  }

// Export the KML
  std::ostringstream output;
  exporter.Export(output);

  const std::string outputFilePath("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TAllFacesResolution1.kml");

  if (RECREATE_KML_FILES)
  {
    std::ofstream outputFile(outputFilePath);
    exporter.Export(outputFile);
    outputFile.close();
  }

  CheckKMLFile(outputFilePath, output);
}

SYSTEM_TEST(KMLExporterISEA4T, Face3Resolution2)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture4TriangleGrid grid;
  HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  static const int FACE = 3;

  for (int partition1 = 0; partition1 < grid.GetNumChildren(); ++partition1)
  {
    for (int partition2 = 0; partition2 < grid.GetNumChildren(); ++partition2)
    {
      std::stringstream cellId;
      cellId << std::setw(2) << std::setfill('0') << FACE;
      cellId << partition1 << partition2;

      std::unique_ptr<Cell::ICell> cell = indexer.CreateCell(cellId.str());
      exporter.AddCell(*cell);
    }
  }

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream outputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TFace3Resolution2.kml");
    exporter.Export(outputFile);
    outputFile.close();
  }

  CheckKMLFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TFace3Resolution2.kml", output);
}

SYSTEM_TEST(KMLExporterISEA4T, Face3Resolution3)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture4TriangleGrid grid;
  HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

  KmlExporter exporter(&projection, &indexer);

  static const int FACE = 3;

  for (int partition1 = 0; partition1 < grid.GetNumChildren(); ++partition1)
  {
    for (int partition2 = 0; partition2 < grid.GetNumChildren(); ++partition2)
    {
      for (int partition3 = 0; partition3 < grid.GetNumChildren(); ++partition3)
      {
        std::stringstream cellId;
        cellId << std::setw(2) << std::setfill('0') << FACE;
        cellId << partition1 << partition2 << partition3;

        std::unique_ptr<Cell::ICell> cell = indexer.CreateCell(cellId.str());
        exporter.AddCell(*cell);
      }
    }
  }

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream outputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TFace3Resolution3.kml");
    exporter.Export(outputFile);
    outputFile.close();
  }

  CheckKMLFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TFace3Resolution3.kml", output);
}

SYSTEM_TEST(KMLExporterISEA4T, Cities100m2Accuracy)
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
  Aperture4TriangleGrid grid;
  HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

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
    std::ofstream outputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TCities100m2Accuracy.kml");
    exporter.Export(outputFile);
    outputFile.close();
  }

  CheckKMLFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TCities100m2Accuracy.kml", output);
}

SYSTEM_TEST(KMLExporterISEA4T, CellGridTest)
{
  // Five metre intervals at 51.0, 0.0 as determined from http://www.movable-type.co.uk/scripts/latlong.html
  const double latitudeFiveMetreSpacing = 0.000045;
  const double longitudeFiveMetreSpacing = 0.000072;

  const double baseLatitude = 51.0;
  const double baseLongitude = 0.0;

  Icosahedron globe;
  Snyder projection(&globe);
  Aperture4TriangleGrid grid;
  HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

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

      // Cell ids at this resolution are very long.  We want to view the ids in google earth
      // to ensure that all points in a cell have the same id.  To aid this only show the
      // last 5 digits.
      exporter.AddLocation(point, cellId.substr(cellId.length() - 5));
    }
  }

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  if (RECREATE_KML_FILES)
  {
    std::ofstream outputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TGrid100m2Accuracy.kml");
    exporter.Export(outputFile);
    outputFile.close();
  }

  CheckKMLFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TGrid100m2Accuracy.kml", output);
}

SYSTEM_TEST(KMLExporterISEA4T, ParentsTest)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture4TriangleGrid grid;
  HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

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
    std::ofstream outputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TParents.kml");
    exporter.Export(outputFile);
    outputFile.close();
  }

  CheckKMLFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TParents.kml", output);
}

SYSTEM_TEST(KMLExporterISEA4T, ChildrenTest)
{
  Icosahedron globe;
  Snyder projection(&globe);
  Aperture4TriangleGrid grid;
  HierarchicalGridIndexer indexer(&grid, globe.GetNoOfFaces() - 1U);

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
    std::ofstream outputFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TChildren.kml");
    exporter.Export(outputFile);
    outputFile.close();
  }

  CheckKMLFile("OpenEAGGR/EAGGRTestHarness/TestData/ISEA4TChildren.kml", output);
}
