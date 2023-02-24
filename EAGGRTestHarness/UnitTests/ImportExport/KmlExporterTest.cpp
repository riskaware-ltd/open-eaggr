//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGRTestHarness
//
// subsystem: Unit Tests
//
//------------------------------------------------------
/// @file KmlExporterTest.cpp
///
/// Tests for the EAGGR::ImportExport::KmlExporter class.
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

#include "TestUtilities/KmlTestGridIndexer.hpp"
#include "TestUtilities/KmlTestCell.hpp"
#include "TestUtilities/KmlFileMatcher.hpp"

using namespace EAGGR::Model;
using namespace EAGGR::Model::Projection;
using namespace EAGGR::Model::PolyhedralGlobe;
using namespace EAGGR::Model::GridIndexer;
using namespace EAGGR::ImportExport;

UNIT_TEST(KmlExporter, Export)
{
  Icosahedron globe;
  Snyder projection(&globe);
  EAGGR::TestUtilities::KmlTestGridIndexer indexer;

  KmlExporter exporter(&projection, &indexer);

  // Initialise the parameters for cell 1
  FaceCoordinate centre1(0U, 0.0, 0.0, 1.0);
  FaceCoordinate vertex0_1(0U, 0.0, 0.0, 1.0);
  FaceCoordinate vertex1_1(0U, 1E-3, 0.0, 1.0);
  FaceCoordinate vertex2_1(0U, 0.0, 1E-3, 1.0);

  std::list<FaceCoordinate> vertices1;
  vertices1.push_back(vertex0_1);
  vertices1.push_back(vertex1_1);
  vertices1.push_back(vertex2_1);

  indexer.AddCell("Test Cell ID 1", centre1, vertices1);
  EAGGR::TestUtilities::KmlTestCell cell1("Test Cell ID 1");

  // Initialise the parameters for cell 2
  FaceCoordinate centre2(1U, 0.0, 0.0, 1.0);
  FaceCoordinate vertex0_2(1U, 0.0, 0.0, 1.0);
  FaceCoordinate vertex1_2(1U, 0.0, 1E-3, 1.0);
  FaceCoordinate vertex2_2(1U, 1E-3, 0.0, 1.0);

  std::list<FaceCoordinate> vertices2;
  vertices2.push_back(vertex0_2);
  vertices2.push_back(vertex1_2);
  vertices2.push_back(vertex2_2);

  indexer.AddCell("Test Cell ID 2", centre2, vertices2);
  EAGGR::TestUtilities::KmlTestCell cell2("Test Cell ID 2");

  // Add the cells to the exporter
  exporter.AddCell(cell1);
  exporter.AddCell(cell2);

  // Export the KML
  std::ostringstream output;
  exporter.Export(output);

  // Uncomment these lines to write the output to file - use if the expected output changes
  /*std::ofstream ouputFile("TestData/ExpectedKmlExport.kml");
   exporter.Export(ouputFile);
   ouputFile.close();*/

  // Read expected output from file
  std::ifstream expectedFile("OpenEAGGR/EAGGRTestHarness/TestData/ExpectedKmlExport.kml");
  if (!expectedFile.is_open())
  {
    // Unable to open file containing expected KML
    FAIL();
  }

  // Ensure the KML is as expected
  EAGGR::TestUtilities::KmlFileMatcher matcher;
  std::istringstream actualStream(output.str());
  bool filesMatch = matcher.Compare(expectedFile, actualStream);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }

  // Export the shapes to a KML file
  char filename[] = "OpenEAGGR/EAGGRTestHarness/TestData/ActualKmlExport.kml";
  exporter.ExportToFile(filename);

  // Read the KML from the generated file
  std::ifstream actualFile;
  actualFile.open(filename);

  expectedFile.close();

  std::ifstream expectedFile2("OpenEAGGR/EAGGRTestHarness/TestData/ExpectedKmlExport.kml");
  filesMatch = matcher.Compare(expectedFile2, actualFile);

  if (!filesMatch)
  {
    SCOPED_TRACE(matcher.GetLastErrorMessage());
    ASSERT_TRUE(filesMatch);
  }

  // Close the KML files
  expectedFile2.close();
  actualFile.close();

  // Delete the generated KML file
  std::remove(filename);
}
