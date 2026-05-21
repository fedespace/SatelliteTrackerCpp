#include "sgp4Propagator.h"

// ========================

std::vector<GroundTrack> propagate (Tle tle, TimeUTC start, TimeUTC end, double step) {

    // Init of the structure "satrec"
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

        // Conversion of timeUTC to "time since epoch (minutes)"
        double timeS_min = epoch2mins(start);
        double timeE_min = epoch2mins(end);
        double deltaT = timeE_min - timeS_min;

        // While loop for the propagation and grountrack
        double tsince = step; // in the first iteration that's equal to the step
        double r[3], v[3];
        double jd, jdfrac;
        double t = timeS_min; // the time that will keep incrementing in the groundtrack loop
        double radius_earth = satrec.radiusearthkm;
        double lat, lon;
        std::vector<double> lat_vec, lon_vec;
        std::vector<GroundTrack> gt;
        while (tsince <= deltaT) {
            SGP4Funcs::sgp4(satrec, tsince, r, v);
            double t1 = t + step; // [min]
            TimeUTC t1_components = MJD20002epoch(t1); // [year, month, day, hour, minute, seconds]
            SGP4Funcs::jday_SGP4(
                t1_components.year, t1_components.month, t1_components.day, t1_components.hour, t1_components.minute, t1_components.second,
                jd, jdfrac
            );
            double jd_full = jd + jdfrac;
            double gstime = SGP4Funcs::gstime_SGP4(jd_full); // [rad]
            Matrix3x3 R = rotationMatrixZ(gstime);
            Vector3D r_teme = {r[0], r[1], r[2]};
            Vector3D r_ecef = rotateZ(R, r_teme);
            ecef2ll(r_ecef, radius_earth, lat, lon);
            lat_vec.push_back(lat);
            lon_vec.push_back(lon);
            GroundTrack single_point = {t1_components, lat, lon};
            gt.push_back(single_point);
            
            // Update t for next iteration
            t += step; 
            tsince += step;

        }

        return gt;

}