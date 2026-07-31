#include "PassPrediction.h"
#include <cmath>

// ========================

Vector3D r_gs(double lat, double lon, double alt) {
    // Inputs: latitude and longitude give in degrees, altitude given in m
    // Output: vector components given in m

    // Defining constant of WGS84
    double a = 6378137.0; // [m]
    double e2 = 0.00669437999014;

    // Conversion of lat and lon to radiants
    double conversionFactor = M_PI/180;
    double latRad = lat*conversionFactor;
    double lonRad = lon*conversionFactor;

    // Computation of vector elements
    double N = a / std::sqrt(1 - e2*std::pow(std::sin(latRad),2));
    double rx = (N + alt) * std::cos(latRad)*std::cos(lonRad);
    double ry = (N + alt) * std::cos(latRad)*std::sin(lonRad);
    double rz = (N * (1-e2) + alt) * std::sin(latRad);

    // Create the vector
    Vector3D r_gs;
    r_gs.x = rx;
    r_gs.y = ry;
    r_gs.z = rz;

    return r_gs;
}


Vector3D r_sat(Tle tle, TimeUTC t) {
    double tleEpoch = epoch2MJD2000_TLE(tle);
    double nowEpoch = epoch2mins(t);
    // Fetch param from TLE
    TleParam parameters = parseTLE(tle);
    gravconsttype WGS = parameters.whichconst;
    char opsmode = parameters.opsmode;
    std::array<char,6> satn = parameters.satn;
    double epoch = parameters.epoch;
    double bstar = parameters.bstar;
    double ndot = parameters.ndot;
    double nddot = parameters.nddot;
    double ecco = parameters.ecco;
    double argpo = parameters.argpo;
    double inclo = parameters.inclo;
    double mo = parameters.mo;
    double no_kozai = parameters.no_kozai;
    double nodeo = parameters.nodeo;
    elsetrec satrec;
    SGP4Funcs::sgp4init (
        WGS, opsmode, satn.data(), epoch, bstar, ndot, nddot, ecco, argpo, inclo, mo, no_kozai, nodeo, 
        satrec
    );
    // Conversion of timeUTC to "time since TLE epoch (minutes)"
    double time_TLE_min = epoch2MJD2000_TLE(tle);
    double t0_min = epoch2mins(t);
    double tsince = t0_min - time_TLE_min; // start value of the tsince
    double radius_earth = satrec.radiusearthkm;

    // Initialise variables
    double r[3], v[3];
    Vector3D r_ecef_km;
    double jd, jdfrac;
    // Start the propagation
    SGP4Funcs::sgp4(satrec, tsince, r, v);
    SGP4Funcs::jday_SGP4(
            t.year, t.month, t.day, t.hour, t.minute, t.second,
            jd, jdfrac
        );
    double jd_full = jd + jdfrac;
    double gstime = SGP4Funcs::gstime_SGP4(jd_full); // [rad]
    Matrix3x3 R = rotation_teme2ecef(gstime);
    Vector3D r_teme = {r[0], r[1], r[2]};
    r_ecef_km = rotateZ(R, r_teme); //[km]
    
    Vector3D r_ecef_m;
    r_ecef_m.x = r_ecef_km.x * 1000.0;
    r_ecef_m.y = r_ecef_km.y * 1000.0;
    r_ecef_m.z = r_ecef_km.z * 1000.0;

    return r_ecef_m;
}