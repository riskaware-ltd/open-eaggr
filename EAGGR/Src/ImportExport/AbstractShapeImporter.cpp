//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Import / Export
//
//------------------------------------------------------
/// @file AbstractShapeImporter.cpp
/// 
/// Implements the EAGGR::Utilities::ImportExport::AbstractShapeImporter class.
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

#include <sstream>

#include "AbstractShapeImporter.hpp"

#include "Src/LatLong/Wgs84AccuracyPoint.hpp"
#include "Src/LatLong/Wgs84Linestring.hpp"
#include "Src/LatLong/Wgs84Polygon.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::LatLong;

namespace EAGGR
{
  namespace ImportExport
  {
    AbstractShapeImporter::AbstractShapeImporter(const double a_accuracy)
        : m_shapeIndex(0U), m_accuracy(a_accuracy)
    {
    }

    AbstractShapeImporter::~AbstractShapeImporter()
    {
      // Delete all the pointers to the shape objects
      for (std::vector<LatLongShape>::const_iterator iter = m_shapes.begin();
          iter != m_shapes.end(); ++iter)
      {
        LatLongShape llShape = *iter;
        const IShape* pShape = llShape.GetShapeData();
        delete pShape;
      }
    }

    LatLongShape AbstractShapeImporter::GetNextShape()
    {
      LatLongShape & shape = m_shapes[m_shapeIndex];
      ++m_shapeIndex;
      return shape;
    }

    bool AbstractShapeImporter::HasNext() const
    {
      return m_shapeIndex < m_shapes.size();
    }

    void AbstractShapeImporter::ExtractShapesFromGeometry(const OGRGeometry* const a_pGeometry)
    {
      OGRwkbGeometryType geometryType = wkbFlatten(a_pGeometry->getGeometryType());

      // If the geometry is a collection of geometries
      switch (geometryType)
      {
        case wkbGeometryCollection:
        case wkbMultiPoint:
        case wkbMultiLineString:
        case wkbMultiPolygon:
        {
          const OGRGeometryCollection* pGeometryCollection =
              static_cast<const OGRGeometryCollection*>(a_pGeometry);

          // Loop over the geometries and call this method iteratively to extract the shapes from each
          for (int geometryIndex = 0; geometryIndex < pGeometryCollection->getNumGeometries();
              ++geometryIndex)
          {
            ExtractShapesFromGeometry(pGeometryCollection->getGeometryRef(geometryIndex));
          }
          break;
        }
        case wkbPoint:
        {
          const OGRPoint* pPoint = static_cast<const OGRPoint*>(a_pGeometry);

          LatLongShape shape(
              WGS84_ACCURACY_POINT,
              new LatLong::Wgs84AccuracyPoint(pPoint->getY(), pPoint->getX(), m_accuracy));

          m_shapes.push_back(shape);
          break;
        }
        case wkbLineString:
        {
          const OGRLineString* pLineString = static_cast<const OGRLineString *>(a_pGeometry);

          OGRPointIterator* pIterator = pLineString->getPointIterator();
          OGRPoint pPoint;

          LatLong::Wgs84Linestring* pWgs84Linestring = new LatLong::Wgs84Linestring();
          while (pIterator->getNextPoint(&pPoint))
          {
            pWgs84Linestring->AddAccuracyPoint(pPoint.getY(), pPoint.getX(), m_accuracy);
          }

          LatLongShape shape(WGS84_LINESTRING, pWgs84Linestring);

          m_shapes.push_back(shape);
          break;
        }
        case wkbPolygon:
        {
          LatLong::Wgs84Polygon* pWgs84Polygon = new LatLong::Wgs84Polygon();

          const OGRPolygon* pPolygon = static_cast<const OGRPolygon *>(a_pGeometry);
          OGRPoint pPoint;

          // Store the outer (exterior) ring of the polygon
          OGRPointIterator* pOuterRingIterator = pPolygon->getExteriorRing()->getPointIterator();
          while (pOuterRingIterator->getNextPoint(&pPoint))
          {
            pWgs84Polygon->AddAccuracyPointToOuterRing(pPoint.getY(), pPoint.getX(), m_accuracy);
          }

          // Store the inner (interior) rings of the polygon
          const unsigned short noOfInnerRings = pPolygon->getNumInteriorRings();
          for (unsigned short ringIndex = 0U; ringIndex < noOfInnerRings; ringIndex++)
          {
            pWgs84Polygon->CreateInnerRing();

            OGRPointIterator* pInnerRingIterator =
                pPolygon->getInteriorRing(ringIndex)->getPointIterator();
            while (pInnerRingIterator->getNextPoint(&pPoint))
            {
              pWgs84Polygon->AddAccuracyPointToInnerRing(
                  ringIndex,
                  pPoint.getY(),
                  pPoint.getX(),
                  m_accuracy);
            }
          }

          // Add polygon to the shapes vector
          LatLongShape shape(WGS84_POLYGON, pWgs84Polygon);
          m_shapes.push_back(shape);

          break;
        }
        default:
          std::stringstream stream;
          stream << "Unsupported geometry: " << geometryType << " in shape string importer.";
          throw EAGGRException(stream.str());
      }
    }
  }
}
