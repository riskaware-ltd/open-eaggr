//------------------------------------------------------
// Copyright (c) Riskaware 2015
//------------------------------------------------------
//
// system: EAGGR
//
// subsystem: Projection
//
//------------------------------------------------------
/// @file Snyder.cpp
/// 
/// Implements the EAGGR::Model::Projection::Snyder class.
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

#include <cmath>
#include <string>
#include <sstream>

#include "Snyder.hpp"

#include "Src/Utilities/RadianMacros.hpp"
#include "Src/LatLong/SphericalAccuracyPoint.hpp"
#include "Src/EAGGRException.hpp"

using namespace EAGGR::Utilities::Maths;

namespace EAGGR
{
  namespace Model
  {
    namespace Projection
    {
      Snyder::Snyder(const PolyhedralGlobe::IPolyhedralGlobe * const a_pGlobe)
          : m_pGlobe(a_pGlobe)
      {
      }

      FaceCoordinate Snyder::GetFaceCoordinate(const LatLong::SphericalAccuracyPoint a_point) const
      {
        // Note: All angles in this method are in radians (because cmath functions use radians)

        // Margin around the edges of the polyhedron to ensure that points near the edge do not fall
        // between two faces. The margin is needed due cumulative inaccuracies in the calculations.
        /// @todo Find a better way of coping with inaccuracies, because currently systems that use a
        ///       different number of bits to store doubles could calculate different faces for the
        ///       same point.
        static const Radians EDGE_MARGIN = 0.0000000001;

        // Get the point's lat and long coordinates
        const Radians phi = a_point.GetLatitudeInRadians();
        const Radians lambda = a_point.GetLongitudeInRadians();

        // Variables used in step 4 (outside the while loop)
        Radians z = 0.0, Az = 0.0, AzAdjustment = 0.0, q = 0.0;

        // Constants that depend on face index
        Radians g = 0.0, G = 0.0, theta = 0.0;

        // Test each face in turn
        bool foundFace = false;
        FaceIndex faceIndex = 0U;
        while (!foundFace)
        {
          // Should always find a face, but just in case
          if (faceIndex >= m_pGlobe->GetNoOfFaces())
          {
            std::stringstream stream;
            stream << "Impossible transform: Point (" << a_point.GetLatitude() << ", "
                << a_point.GetLongitude() << ") is not located on any face";
            throw EAGGRException(stream.str());
          }

          // Get the geographic centre of the face
          const LatLong::Point faceCentre = m_pGlobe->GetFaceCentre(faceIndex);
          const Radians phi0 = faceCentre.GetLatitudeInRadians();
          const Radians lambda0 = faceCentre.GetLongitudeInRadians();

          // Get spherical constants for face
          g = m_pGlobe->Get_g();
          G = m_pGlobe->Get_G();
          theta = m_pGlobe->GetTheta();

          // Step 1 - Calculate z and Az

          // Equation 13: Calculate the spherical distance (z) of the point from the geographic centre of the hexagon
          z = acos((sin(phi0) * sin(phi)) + (cos(phi0) * cos(phi) * cos(lambda - lambda0)));

          // If z exceeds g, point is too far from centre of the face and located on another face
          if (z > g + EDGE_MARGIN)
          {
            // Try next face
            faceIndex++;
            continue;
          }

          // Equation 14: Calculate the azimuth (Az) of the point from the geographic centre of the hexagon
          Az = atan2(
              cos(phi) * sin(lambda - lambda0),
              (cos(phi0) * sin(phi)) - (sin(phi0) * cos(phi) * cos(lambda - lambda0)));

          // Step 2 - Work out which section of the face we are in

          // Initial adjustment to give "some" vertex an Az of 0
          Az += m_pGlobe->GetOrientationOfFace(faceIndex);

          // Adjust Az for the point to fall within the range of 0 and the angle between the vertices
          AzAdjustment = AdjustAz(theta, Az);

          // Step 3

          // Equation 9: Calculate q.
          q = atan(tan(g) / (cos(Az) + (sin(Az) * Cot(theta))));

          // If z exceeds q, it will not fit on this polygon and is located on another one
          if (z > q + EDGE_MARGIN)
          {
            // Try next face
            faceIndex++;
          }
          else
          {
            foundFace = true;
          }
        }

        // Step 4 - Apply equations (5)–(8) and (10)–(12) in order

        // Equation 5 (Let R = 1 until the final scaling of the map)
        const double RPrime = m_pGlobe->GetRPrimeRelativeToR();

        // Equation 6
        const Radians H = acos((sin(Az) * sin(G) * cos(g)) - (cos(Az) * cos(G)));

        // Equation 7
        // Note: pi * R^2 / 180 degrees gives 1 so can be omitted from the equation
        const double AG = Az + G + H - DEGREES_IN_RAD(180);

        // Equation 8
        Radians AzPrime = atan2(
            2.0 * AG,
            (Squared(RPrime) * Squared(tan(g))) - (2.0 * AG * Cot(theta)));

        // Equation 10
        const double dPrime = RPrime * tan(g) / (cos(AzPrime) + (sin(AzPrime) * Cot(theta)));

        // Equation 11
        const double f = dPrime / (2.0 * RPrime * sin(q / 2.0));

        // Equation 12
        const double rho = 2.0 * RPrime * f * sin(z / 2.0);

        // Remove the adjustment amount from Step 2
        AzPrime -= AzAdjustment;

        // Calculate rectangular coordinates (as a fraction of the radius of earth)

        // Equation 15
        const double x = rho * sin(AzPrime);
        // Equation 16
        const double y = rho * cos(AzPrime);

        // Get the conversion ratio to make coordinates relative to the edge length of the globe
        const double earthRadiusRelativeToEdgeLength = 1 / GetEdgeLengthRelativeToR();

        // Enter results into the face coordinate object
        FaceCoordinate faceCoordinate(
            faceIndex,
            x * earthRadiusRelativeToEdgeLength,
            y * earthRadiusRelativeToEdgeLength,
            GetAccuracyArea(a_point.GetAccuracy()));

        return (faceCoordinate);
      }

      LatLong::SphericalAccuracyPoint Snyder::GetLatLongPoint(
          const FaceCoordinate a_coordinate) const
      {
        // Note: All angles in this method are in radians (except in the lat / long point)

        // Get the face on which the point is located
        const FaceIndex faceIndex = a_coordinate.GetFaceIndex();

        // Convert face coordinate to x and y offsets (in terms of R)
        const double edgeLengthRelativeToEarthRadius = GetEdgeLengthRelativeToR();
        const double x = a_coordinate.GetXOffset() * edgeLengthRelativeToEarthRadius;
        const double y = a_coordinate.GetYOffset() * edgeLengthRelativeToEarthRadius;

        // Get spherical constants for face
        const Radians g = m_pGlobe->Get_g();
        const Radians G = m_pGlobe->Get_G();
        const Radians theta = m_pGlobe->GetTheta();

        // Equation 17
        Radians AzPrime = atan2(x, y);

        // Equation 18 (Pythagoras Theorem)
        const double rho = sqrt(Squared(x) + Squared(y));

        // Adjust Az' for the point to fall within the range of 0 and the angle between the vertices
        const Radians AzAdjustment = AdjustAz(theta, AzPrime);

        // Equation 5 (Ultimately R terms will cancel out so for convenience let R = 1)
        const double RPrime = m_pGlobe->GetRPrimeRelativeToR();

        // Equation 19
        const double AG = Squared(RPrime) * Squared(tan(g)) / (2 * (Cot(AzPrime) + Cot(theta)));

        // Iteration converges even to 10^-9 degrees in 3 to 4 cycles
        static const double ITERATION_ACCURACY = pow(10, -9.0);

        // Iterate through equations (6) and (20)-(22) in order, with Az' as the first approximation
        Radians approxAz = AzPrime;
        Radians deltaAz = 0.0;
        do
        {
          // Equation 6
          const Radians H = acos(sin(approxAz) * sin(G) * cos(g) - cos(approxAz) * cos(G));

          // Equation 20 (180 degrees / (pi * R^2) gives 1 so can be omitted from the equation
          const Radians FunctionAz = AG - G - H - approxAz + DEGREES_IN_RAD(180);

          // Equation 21
          const Radians DerivativeAz = ((cos(approxAz) * sin(G) * cos(g) + sin(approxAz) * cos(G))
              / sin(H)) - 1.0;

          // Equation 22
          deltaAz = -1.0 * FunctionAz / DerivativeAz;

          // Calculate next approximation
          approxAz += deltaAz;

        }
        while (std::abs(deltaAz) > ITERATION_ACCURACY);
        Radians Az = approxAz;

        // Equation 9
        const Radians q = atan(tan(g) / (cos(Az) + (sin(Az) * Cot(theta))));

        // Equation 10
        const double dPrime = RPrime * tan(g) / (cos(AzPrime) + (sin(AzPrime) * Cot(theta)));

        // Equation 11
        const double f = dPrime / (2.0 * RPrime * sin(q / 2.0));

        // Equation 23
        const Radians z = 2 * asin(rho / (2 * RPrime * f));

        // Remove the adjustment amount
        Az -= AzAdjustment;

        // Adjust Az to allow for the orientation of the face
        Az -= m_pGlobe->GetOrientationOfFace(faceIndex);

        // Get the geographic centre of the face
        const LatLong::Point faceCentre = m_pGlobe->GetFaceCentre(faceIndex);
        const Radians phi0 = faceCentre.GetLatitudeInRadians();
        const Radians lambda0 = faceCentre.GetLongitudeInRadians();

        // Convert to final lat / long based on the centre and orientation of the face
        // Equations taken from: http://www.movable-type.co.uk/scripts/latlong.html
        const Radians phi = asin((sin(phi0) * cos(z)) + (cos(phi0) * sin(z) * cos(Az)));
        const Radians lambda = lambda0
            + atan2(sin(Az) * sin(z) * cos(phi0), cos(z) - sin(phi0) * sin(phi));

        // Convert to degrees to get latitude and longitude
        const Degrees latitude = RADIANS_IN_DEG(phi);
        Degrees longitude = RADIANS_IN_DEG(lambda);

        // Wrap around the world if we have exceeded the maximum or minimum longitude
        longitude = LatLong::Point::WrapLongitude(longitude);

        // Convert lat / long to degrees and enter results into the point object
        LatLong::SphericalAccuracyPoint point(
            latitude,
            longitude,
            GetAccuracyAngle(a_coordinate.GetAccuracy()));

        return (point);
      }

      Radians Snyder::AdjustAz(const Radians a_theta, Radians & a_Az) const
      {
        // Calculate the adjustment amount
        const Radians angleBetweenVertices = 2.0 * (DEGREES_IN_RAD(90) - a_theta);

        // Adjust Az for the point to fall within the range of 0 and the angle between the vertices
        short noOfAdjustments = 0;
        while (a_Az < 0.0)
        {
          a_Az += angleBetweenVertices;
          noOfAdjustments++;
        }
        while (a_Az > angleBetweenVertices)
        {
          a_Az -= angleBetweenVertices;
          noOfAdjustments--;
        }

        // Return the adjustment amount so it can be removed later
        return (angleBetweenVertices * static_cast<Radians>(noOfAdjustments));
      }

      double Snyder::GetEdgeLengthRelativeToR() const
      {
        return (m_pGlobe->GetRPrimeRelativeToR() * m_pGlobe->GetEdgeLengthRelativeToRPrime());
      }

      double Snyder::GetAccuracyArea(const Utilities::Maths::Degrees a_accuracyAngle) const
      {
        // Area of Sphere Cap = 2 * pi * Height of Cap * Radius of Earth

        // Height of Cap = R * ( 1 - cos(Accuracy Angle) )

        // Therefore:
        // Area of Sphere Cap = 2 * pi * ( 1 - cos(Accuracy Angle) ) * Radius of Earth ^ 2

        // Total Area of Sphere = 4 * pi * Radius of Earth ^ 2

        // Therefore:
        // ( Area of Sphere Cap / Total Area of Sphere ) = 0.5 * ( 1 - cos(Accuracy Angle) )

        double accuracyRelativeToAreaOfSphere = 0.5
            * (Utilities::Maths::OneMinusCos(DEGREES_IN_RAD(a_accuracyAngle)));

        // Area of Face = Total Area of Sphere / No of Faces
        // Therefore:
        // ( Area of Sphere Cap / Area of Face ) = 0.5 * ( 1 - cos(Accuracy Angle) ) * No of Faces
        double accuracyRelativeToAreaOfFace = accuracyRelativeToAreaOfSphere
            * static_cast<double>(m_pGlobe->GetNoOfFaces());

        // Limit maximum area to the area of one face
        if (accuracyRelativeToAreaOfFace > 1.0)
        {
          accuracyRelativeToAreaOfFace = 1.0;
        }

        return (accuracyRelativeToAreaOfFace);
      }

      Utilities::Maths::Degrees Snyder::GetAccuracyAngle(const double a_accuracyArea) const
      {
        // Reversing the equation from GetAccuracyArea() gives:
        // Accuracy Angle = acos( 1 - 2 * (Area of Sphere Cap / Total Area of Face) / No of Faces )

        const Utilities::Maths::Radians accuracyAngle = acos(
            1.0 - (2.0 * a_accuracyArea / static_cast<double>(m_pGlobe->GetNoOfFaces())));

        return (RADIANS_IN_DEG(accuracyAngle));
      }
    }
  }
}
