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
    double conversionFactor = M_PI/180.0;
    double latRad = lat*conversionFactor;
    double lonRad = lon*conversionFactor;

    // Computation of vector elements
    double N = a / std::sqrt(1.0 - e2*std::pow(std::sin(latRad),2));
    double rx = (N + alt) * std::cos(latRad)*std::cos(lonRad);
    double ry = (N + alt) * std::cos(latRad)*std::sin(lonRad);
    double rz = (N * (1.0-e2) + alt) * std::sin(latRad);

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
    double conversionFactor = M_PI/180.0;
    double latRad = locationLat*conversionFactor;
    double lonRad = locationLon*conversionFactor;

    // Compute the delta components
    Vector3D range;
    range.x = r1.x - r2.x;
    range.y = r1.y - r2.y;
    range.z = r1.z - r2.z;

    double e = -std::sin(lonRad)*range.x + std::cos(lonRad)*range.y;
    double n = -std::sin(latRad)*std::cos(lonRad)*range.x - std::sin(latRad)*std::sin(lonRad)*range.y + std::cos(latRad)*range.z;
    double u = std::cos(latRad)*std::cos(lonRad)*range.x + std::cos(latRad)*std::sin(lonRad)*range.y + std::sin(latRad)*range.z;


    Vector3D r;
    r.x = e;
    r.y = n;
    r.z = u;

    
    return r;
}

std::vector<double> ENU2ElAz(Vector3D r_enu) {
    double elevation = std::asin(r_enu.z / r_enu.norm()); 
    double azimuth = std::atan2(r_enu.x, r_enu.y);

    std::vector<double> elAz = {elevation * 180.0 / M_PI, azimuth * 180.0/ M_PI};

    return elAz;
}



std::vector<PassPrediction> passTimes(Tle tle, TimeUTC tstart, TimeUTC tend, double lat, double lon, double alt, double elevationMask, double step) {
    // Inputs: time [UTC], coordinate [deg, m], step [s]
    
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
    double finalDelta = timeE_min - time_TLE_min;

    // Initialise variables
    Matrix3x3 R;
    double r[3], v[3];
    double jd, jdfrac;

    // Initialize structure that will be returned.
    std::vector<WindowElevation> window;

    // While loop for the propagation and grountrack
    while (tsince <= finalDelta) {
        // Propagation
        SGP4Funcs::sgp4(satrec, tsince, r, v); // r TEME is in [km]
        
        // Julian days
        TimeUTC time_c = MJD20002epoch(tsince + time_TLE_min);
        SGP4Funcs::jday_SGP4(
            time_c.year, time_c.month, time_c.day, time_c.hour, time_c.minute, time_c.second,
            jd, jdfrac
        );
        double jd_full = jd + jdfrac;

        // Get gstime using Vallado's library
        double gstime = SGP4Funcs::gstime_SGP4(jd_full); // [rad]

        // Compute again the new rotation matrix
        R = rotation_teme2ecef(gstime);

        // Conversion from r_sat in TEME to ECEF
        Vector3D r_teme = {r[0], r[1], r[2]};
        Vector3D r_ecef = rotateZ(R, r_teme); // [km]

        // Conversion from kilometers to meters 
        Vector3D r_ecef_m; // [m]
        r_ecef_m.x = r_ecef.x * 1000.0;
        r_ecef_m.y = r_ecef.y * 1000.0;
        r_ecef_m.z = r_ecef.z * 1000.0;

        // Conversion of the range vector from ECEF to ENU
        Vector3D r_enu = r_ENU(lat, lon, r_ecef_m, gs_ecef);

        // Compute elevation obs-sat [deg] from r_enu
        double elevation = ENU2ElAz(r_enu)[0];

        WindowElevation singlePoint = {time_c, elevation};
        window.push_back(singlePoint);

        tsince += step/60.0; // [min]
    }

        
    std::vector<PassPrediction> prediction;
    bool pass = false;
    double max_elevation;
    PassPrediction singlePass = {};

    // Define elevation at the step - 1 for comparison 
    double e_old = window[0].elevation;

    int index = 0;
    double t0, t1, e0, e1, sharpness, sharpnessScore, maxElScore, pass_score;
    size_t i_maxEl;
    std::string qos;
    
    for(int iter = 1; iter < window.size(); iter++) {

        double e = window[iter].elevation;
        TimeUTC t = window[iter].time;

        if (!pass && e > elevationMask && e_old < e) {
            singlePass.AOS = window[iter - 1].time;
            singlePass.max_el = e;
            pass = true;
        }

        // Update max elevation
        if (pass && e > singlePass.max_el) {
            singlePass.max_el = e;
            singlePass.time_maxEl = t;
        }

        if (pass && e < elevationMask && e_old > e) {
            singlePass.LOS = window[iter - 1].time;
            singlePass.id = index;
            singlePass.duration = (epoch2mins(singlePass.LOS) - epoch2mins(singlePass.AOS));
            
            // Check if pass is considered visible from observer (iterating each min)
            double visibility_time = epoch2mins(singlePass.AOS);
            bool isSatelliteInLight;
            bool isDark;
            for (double i = visibility_time; visibility_time < epoch2mins(singlePass.LOS); visibility_time++) {
                // Is satellite in the light
                TimeUTC vt = MJD20002epoch(visibility_time);
                Vector3D s_m = r_sat(tle, vt); // [m]
                Vector3D s_km;
                s_km.x = s_m.x / 1000.0;
                s_km.y = s_m.y / 1000.0;
                s_km.z = s_m.z / 1000.0;
                double jd_v, jdf_v;
                SGP4Funcs::jday_SGP4(
                    vt.year, vt.month, vt.day, vt.hour, vt.minute, vt.second,
                    jd_v, jdf_v
                );
                double jd_full_v = jd_v + jdf_v;
                isSatelliteInLight = satInLight(s_km, jd_full_v);

                // Is the sky dark enough
                std::vector<double> s_eci = sun_eci(jd_full_v);
                isDark = observer_darkness(s_eci, gs_ecef, vt, -6.0);
                // Combining the two checks
                if (isSatelliteInLight && isDark) {
                    singlePass.passIsVisible = true;
                    break;
                }
            }

            prediction.push_back(singlePass);
            singlePass = {};
            index++;
            pass = false;
        }

        auto it = std::find_if(window.begin(), window.end(), [&](const WindowElevation& w) {
            return w.elevation == singlePass.max_el;
        });
        if (it != window.end()) {
            i_maxEl = std::distance(window.begin(), it);
        }
        if (i_maxEl >= 12 && i_maxEl + 12 < window.size()) {
            e0 = window[i_maxEl-12].elevation;
            e1 = window[i_maxEl+12].elevation;
            t0 = epoch2mins(window[i_maxEl-12].time) - 1.0;
            t1 = epoch2mins(window[i_maxEl-12].time) + 1.0;
        }
        sharpness = (e1 - e0) / ((t1 - t0)*60); // el divided by seconds
        if (sharpness < 0.05) {
            sharpnessScore = 0.0;
        } else if (sharpness >= 0.05 && sharpness >= 0.3) {
            sharpnessScore = (sharpness - 0.05) / (0.3 - 0.05) * 100;
        } else if (sharpness > 0.3) {
            sharpnessScore = 100.0;
        }
        if (singlePass.max_el < 10.0) {
            maxElScore = 0.0;
        } else {
            maxElScore = (singlePass.max_el - 10) / 80 * 100;
        }
        pass_score = 0.7*maxElScore + 0.3*sharpnessScore;
        if (pass_score < 25) {
            qos = "POOR";
        } else if (pass_score >= 25 && pass_score < 50) {
            qos = "FAIR";
        } else if (pass_score >= 50 && pass_score < 75) {
            qos = "GOOD";
        } else {
            qos = "EXCELLENT";
        }
        singlePass.qos = qos;
        e_old = e;

    }

    return prediction;
}

std::vector<double> sun_eci(double jd) {
    // J2000 centuries
    double jd_century = (jd - 2451545.0) / 36525.0;
    // Sun mean longitude [deg]
    double lon_M = 280.460 + 36000.771 * jd_century;
    lon_M = fmod(lon_M, 360.0);
    if (lon_M < 0) {
        lon_M += 360.0;
    }
    // Sun mean anomaly [deg]
    double M = 357.528 + 35999.050 * jd_century;
    M = fmod(M, 360.0);
    if (M < 0) {
        M += 360.0;
    }
    // Eclictic longitude
    double l_ecl = lon_M + 1.915 * std::sin(M*M_PI/180.0) + 0.020 * std::sin(2*M*M_PI/180.0);
    // Ecliptic angle
    double eps = 23.439 - 0.0000004 * jd_century;
    // Create vector
    double l_ecl_rad = l_ecl * M_PI/180.0;
    double eps_rad = eps * M_PI/180.0;
    // ECI sun vector: negligible detail for the purpose even if sat is TEME
    std::vector<double> sun_ECI = {
        std::cos(l_ecl_rad),
        std::cos(eps_rad) * std::sin(l_ecl_rad),
        std::sin(eps_rad) * std::sin(l_ecl_rad)
    };

    return sun_ECI;
}

bool satInLight(Vector3D sat, double jd) {
    std::vector<double> sun_ECI = sun_eci(jd);
    std::vector<double> sat_TEME = {
        sat.x,
        sat.y,
        sat.z
    };

    // Check eclipsis with cylindrical shadow model
    bool inLight;
    double dot_product_vectors = std::inner_product(sat_TEME.begin(), sat_TEME.end(), sun_ECI.begin(), 0.0);
    // dot product < 0: satellite is on opposite side of the Earth
    if (dot_product_vectors < 0) {
        // Shadow
        double sun = std::sqrt(sun_ECI[0]*sun_ECI[0]+sun_ECI[1]*sun_ECI[1]+sun_ECI[2]*sun_ECI[2]);
        double sat = std::sqrt(sat_TEME[0]*sat_TEME[0]+sat_TEME[1]*sat_TEME[1]+sat_TEME[2]*sat_TEME[2]);
        double cos = dot_product_vectors / (sun*sat);
        double perpDistance = sat * std::sqrt(1 - cos*cos); // sat distance from the earth-sun axis
        double e_radius = 6378.137;
        if (perpDistance < e_radius) {
            inLight = false; //eclipsed
        } else {
            inLight = true; //light is on the sat
        }
    } else {
    inLight = true; 
    }

    return inLight;
}

bool observer_darkness(std::vector<double> sun_ECI, Vector3D r_gs, TimeUTC t, int threshold) {
    // Inputs: sun_ECI [km], r_gs [m]
    bool darkness;

    // Conversion of ECI sun vector to ECEF
    double jd, jdfrac;
    SGP4Funcs::jday_SGP4(
            t.year, t.month, t.day, t.hour, t.minute, t.second,
            jd, jdfrac
        );
    double jd_full = jd + jdfrac;
    double gstime = SGP4Funcs::gstime_SGP4(jd_full); // [rad]
    Matrix3x3 rotation = rotation_teme2ecef(gstime);
    Vector3D s_eci = {sun_ECI[0], sun_ECI[1], sun_ECI[2]};
    Vector3D s_ecef = rotateZ(rotation, s_eci); // [km]
    
    // Normalize vectors sun and observer to get directions only (convert r_gs from m to km)
    double s = s_ecef.norm();
    std::vector<double> r_obs = {r_gs.x/1000.0, r_gs.y/1000.0, r_gs.z/1000.0};
    double o = std::sqrt(r_obs[0]*r_obs[0] + r_obs[1]*r_obs[1] + r_obs[2]*r_obs[2]);
    std::vector<double> s_norm = {s_ecef.x/s, s_ecef.y/s, s_ecef.z/s};
    std::vector<double> o_norm = {r_obs[0]/o, r_obs[1]/o, r_obs[2]/o};

    // Compute dot product to find angle between the two vectors
    double sun_elevation = std::asin(std::inner_product(s_norm.begin(), s_norm.end(), o_norm.begin(), 0.0));
    double s_elevation_deg = sun_elevation * 180/M_PI;

    if (s_elevation_deg < threshold) {
        darkness = true;
    } else {
        darkness = false;
    }

    return darkness;
}