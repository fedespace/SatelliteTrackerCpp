# Satellite Tracker Application



## Table of Contents
- [1. Introduction](#1-introduction)
  - [1.1 Scope of the Project](#11-scope-of-the-project)
  - [1.2 Design and Implementation](#12-design-and-implementation)
  - [1.3 Assumptions made in this version](#13-assumptions-made-in-this-version)
  - [1.4 Tech Stack & Dependencies](#14-tech-stack--dependencies)
  - [1.5 Backend Logic](#15-backend-logic)
  - [1.6 Frontend Design](#16-frontend-design)



### 1.1 Scope of the Project
The Satellite Tracker project is developed to help the user locate specific satellites - given chosen inputs - on the Earth map, showing the relative groundtrack.\
By computing precisely latitude and longitude coordinates (from position and velocity vectors) at a target time, it is possible to also define when the selected object is in visibility (FOV) from a location on the Earth, such as a Ground Station, and therefore to compute AOS and LOS.\
An alarm feature is also implemented.

### 1.2 Design and Implementation
* Step 1: Define structures for the core math primitives: 3D Vectors, 3x3 Matrix, Reference Frames and Time;
* Step 2: Create functions as part of the Time Utils, such as computing minutes passed since epoch, and as part of the Reference Frame handling such as rotation around Z-axis;
* Step 3: Create model structures: Orbital State, TLE;
* Step 4: Create an abstract propagator with destructor. Integrate SGP4 propagator from external source (Vallado's Git repository) and implement it;
* Step 5: Transform TEME vectors (result from the SGP4 propagator) to ECEF vectors;
* Step 6: Converting ECEF vectors to geodetic (latitude, longitude and altitude);
* Step 7: Compute the grountrack given start and end epoch;
* Step 8: Create the Observer model, generated from coordinates of the station and elevation mask;
* Step 9: Calculate visibility windows, providing AOS and LOS given a specific location on Earth;
* Step 10: Introduce API layer so that Backend is connected to Frontend allowing users to send HTTP requests with inputs and allowing the application to parse those inputs, returning a result;
* Step 11: Data serialization utilising JSON files;
* Step 12: Web Frontend creation via HTML, CSS and JS;
* Step 13: Tests and validations against references.

### 1.3 Assumptions made in this version
* Orbital dynamics:
    - No other force is considered beyond what the external SGP4 model already includes;
    - TLEs checksum is not validated (future implementation);
    - The spacecraft is not performing maneuvers. No attitude sensors and actuators are considered.
* Reference frames and geometry:
    - Earth rotation is considered by rotating vectors around the Z-axis, with constant Earth rotation rate;
    - No polar motion, nor nutation/precession is assumed;
    - WGS model is used and no custom geopotential has been modelled.
* Time system:
    - UTC is the scale used for every considered epoch, assuming it is equivalent to TAI (Atomic Time), TT ( Terrestrial Time) and UT1 (Earth Rotation Time) and treated as continuous (no leap seconds);
* Units:
    - Distance: [km];
    - Velocity: [km/s];
    - Time: [mins] for SGP4 propagator, [s] for user-facing input/output;
    - Angles: [rad] in the backend, [deg] in the frontend;
    - Coordinate System: Z-axis is aligned with Earth rotation axis.

### 1.4 Tech Stack & Dependencies
* Platforms / Environments:
    - VSCode;
    - clang;
    - Docker;
* Programming Languages:
    - C++17 or C++20 for the Backend;
    - HTML/CSS/JS for the Frontend;
    - Leaflet, Google Maps or Mapbox for map visualisation;
    - Plotly.js and Chart.js for charting;
    - JSON handling;
    - REST API;
* Build System and External Libraries:
    - CMake;
    - Vallado's SGP4 propagator (link here);

### 1.5 Backend Logic
* Step 1: Handle user request via REST API, extracting inputs and validating parameters;
* Step 2: Parse TLE, handle time converting the epoch to minutes since TLE epoch, and call propagator by providing TLE and target epoch;
* Step 3: Convert the coordinates from TEME to ECEF and from ECEF to geodetic;
* Step 4: Compute ground track points and AOS/LOS;
* Step 5: Return JSON file with status code, orbital state, passes, track points and errors if a step fails.

### 1.6 Frontend Design
The application works on desktop devices. Real-time validation on all the inputs provided will raise warnings in case of invalidity. All the units of measurements are always displayed on the screen at all time.\
The screen is split between Input section and Output section:
* Inputs: 
    - Object definition: NORAD or TLE or mission or operator;
    - Time: Target Epoch in static mode (only looking for the position at a specific instant) or Start and End Epoch in visibility mode and grountrack mode (adding also step size in the latter);
    - Observer/Ground Station data (optional) as latitude, longitude and altitude. There is the possibility to select a location name instead of providing coordinates;
    - Submit button.
* Outputs:
    - Map visualisation (static point or grountrack);
    - Current position in ECEF reference;
    - Alerts for visibility and AOS/LOS;
    - Downloadable JSON files of pass times;
    - Zoom available on the map.
* Footer:
    - Details and notes about the selected object;
    - Info and useful links.




<!-- add "reference" with the external sgp4 link and cleestrak as source of truth -->



![Application Mindmap](diagram/SatelliteTracker.svg)