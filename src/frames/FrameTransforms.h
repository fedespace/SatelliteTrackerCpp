#pragma once
#include <cmath>
#include "../domain/Vector3D.h"
#include "../domain/Lla.h"
#include "../domain/Radec.h"


// ===============================

// Rotation rate of the Earth
constexpr double EARTH_ROT_RATE_RAD_S = 7.2921159e-5;

// Initiate the Matrix 3x3 used for rotation of the vector
struct Matrix3x3 {
    double m[3][3]; 
};

// Creation of the rotation matrix around Z-axis
Matrix3x3 rotationMatrixZ(double angle);

// Performing the matrix-vector product
Vector3D rotateZ(const Matrix3x3 R, const Vector3D v);

// rECEF to LLA conversion
Radec r2radec(const Vector3D rECEF);

// Using Vallado's library to convert ECEF to LL
// const double small = 0.00000001;         // small value for tolerances
// 		const double re = 6378.137;
// 		const double eesqrd = 0.006694385000;     // eccentricity of earth sqrd
// 		double magr, decl, rtasc, olddelta, temp, sintemp, s, c = 0.0;
// 		int i;

// 		// ---------------------------  implementation   -----------------------
// 		magr = astMath::mag(recef);

// 		// ---------------------- find longitude value  ------------------------
// 		temp = sqrt(recef[0] * recef[0] + recef[1] * recef[1]);
// 		if (fabs(temp) < small)
// 			rtasc = astMath::sgn(recef[2]) * pi * 0.5;
// 		else
// 			rtasc = atan2(recef[1], recef[0]);

// 		lon = rtasc;
// 		if (fabs(lon) >= pi)   // mod it ?
// 		{
// 			if (lon < 0.0)
// 				lon = twopi + lon;
// 			else
// 				lon = lon - twopi;

// 		}
// 		decl = asin(recef[2] / magr);
// 		latgd = decl;

// 		// ----------------- iterate to find geodetic latitude -----------------
// 		i = 1;
// 		olddelta = latgd + 10.0;

// 		while ((fabs(olddelta - latgd) >= small) && (i<10))
// 		{
// 			olddelta = latgd;
// 			sintemp = sin(latgd);
// 			c = re / (sqrt(1.0 - eesqrd * sintemp * sintemp));
// 			latgd = atan((recef[2] + c*eesqrd*sintemp) / temp);
// 			i = i + 1;
// 		}

// 		if ((pi * 0.5 - fabs(latgd)) > pi / 180.0)  // 1 deg
// 			hellp = (temp / cos(latgd)) - c;
// 		else
// 		{
// 			s = c * (1.0 - eesqrd);
// 			hellp = recef[2] / sin(latgd) - s;
// 		}

// 		gc_gd(latgc, eFrom, latgd);
