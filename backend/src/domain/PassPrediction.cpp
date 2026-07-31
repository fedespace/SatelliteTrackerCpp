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

Vector3D r_ENU(double locationLat, double locationLon, Vector3D r1, Vector3D r2) {
    // Inputs: latitude and longitude give in degrees. Conversion to radiants
    double conversionFactor = M_PI/180;
    double latRad = locationLat*conversionFactor;
    double lonRad = locationLon*conversionFactor;

    // Compute the delta components
    Vector3D range;
    range.x = r1.x - r2.x;
    range.y = r1.y - r2.y;
    range.z = r1.z - r2.z;

    double e = -std::sin(lonRad)*range.x + std::cos(lonRad)*range.y;
    double n = -std::sin(latRad)*std::cos(lonRad)*range.x - std::sin(latRad)*std::sin(lonRad)*range.y + std::cos(latRad)*range.z;
    double u = std::cos(latRad)*std::cos(lonRad)*range.x - std::cos(latRad)*std::sin(lonRad)*range.y + std::sin(latRad)*range.z;

    Vector3D r;
    r.x = e;
    r.y = n;
    r.z = u;
    
    return r;
}

std::vector<double> ENU2ElAz(Vector3D r_enu) {
    double elevation = std::asin(r_enu.z / r_enu.norm()); 
    double azimuth = std::atan2(r_enu.x, r_enu.y);

    std::vector<double> elAz = {elevation * 180 / M_PI, azimuth * 180/ M_PI};

    return elAz;
}

std::vector<PassPrediction> passTimes(Tle tle, TimeUTC tstart, TimeUTC tend, double lat, double lon, double alt, double elevationMask, double step) {
    
    // Get ECEF vector for GS
    Vector3D gs_ecef = r_gs(lat, lon, alt);

    // Propagate the orbit giving a step [sec]
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
    double timeS_min = epoch2mins(tstart);
    double timeE_min = epoch2mins(tend);
    double tsince = timeS_min - time_TLE_min; // start value of the tsince
    double radius_earth = satrec.radiusearthkm;
    double finalDelta = timeE_min - time_TLE_min;

    // Initialise variables
    double r[3], v[3];
    double jd, jdfrac;

    // Initialize elevation @ tstart
    SGP4Funcs::sgp4(satrec, tsince, r, v);
    SGP4Funcs::jday_SGP4(
        tstart.year, tstart.month, tstart.day, tstart.hour, tstart.minute, tstart.second,
        jd, jdfrac
    );
    double jd_full = jd + jdfrac;
    double gstime = SGP4Funcs::gstime_SGP4(jd_full); // [rad]
    Matrix3x3 R = rotation_teme2ecef(gstime);
    Vector3D r_teme = {r[0], r[1], r[2]};
    Vector3D r_ecef = rotateZ(R, r_teme);
    Vector3D r_ecef_m;
    r_ecef_m.x = r_ecef.x * 1000.0;
    r_ecef_m.y = r_ecef.y * 1000.0;
    r_ecef_m.z = r_ecef.z * 1000.0;

    Vector3D r_enu = r_ENU(lat, lon, r_ecef_m, gs_ecef);
    double elevation_old = ENU2ElAz(r_enu)[0]; // this is at initial step to use as a comparison


    // Initialize structure that will be returned.
    std::vector<WindowElevation> window;

    // While loop for the propagation and grountrack
    while (tsince <= finalDelta) {
        SGP4Funcs::sgp4(satrec, tsince, r, v);
        TimeUTC time_c = MJD20002epoch(tsince+time_TLE_min); // [year, month, day, hour, minute, seconds]
        SGP4Funcs::jday_SGP4(
            time_c.year, time_c.month, time_c.day, time_c.hour, time_c.minute, time_c.second,
            jd, jdfrac
        );
        double jd_full = jd + jdfrac;
        double gstime = SGP4Funcs::gstime_SGP4(jd_full); // [rad]
        Matrix3x3 R = rotation_teme2ecef(gstime);
        Vector3D r_teme = {r[0], r[1], r[2]};
        Vector3D r_ecef = rotateZ(R, r_teme);
        Vector3D r_ecef_m;
        r_ecef_m.x = r_ecef.x * 1000.0;
        r_ecef_m.y = r_ecef.y * 1000.0;
        r_ecef_m.z = r_ecef.z * 1000.0;

        Vector3D r_enu = r_ENU(lat, lon, r_ecef_m, gs_ecef);
        double elevation = ENU2ElAz(r_enu)[0];

        WindowElevation singlePoint = {time_c, elevation};
        window.push_back(singlePoint);

        tsince += step/60.0; // [min]
    }

        
    std::vector<PassPrediction> prediction;
    bool pass = false;
    double max_elevation;
    PassPrediction singlePass;
    
    for(int iter = 1; iter < window.size(); iter++) {

        double e = window[iter].elevation;
        TimeUTC t = window[iter].time;
        double e_old = window[iter-1].elevation;

        if (!pass && e >= elevationMask && e_old < elevationMask) {
            // Interpolating
            double frac = ((elevationMask - e_old) / (e - e_old)) * step; // that's usually fractions of seconds if step is low
            singlePass.AOS = t;
            singlePass.AOS.second += frac;
            pass = true;
        }

        // Update max elevation
        if (pass && e > e_old) {
            singlePass.max_el = e;
        }

        if (pass && e <= elevationMask && e_old > elevationMask) {
            double frac = ((elevationMask - e_old) / (e - e_old)) * step;
            singlePass.LOS = t;
            singlePass.LOS.second += frac;
            pass = false;
            prediction.push_back(singlePass);
            singlePass = {};
        }

    }

    return prediction;
}