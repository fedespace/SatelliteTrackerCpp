# Satellite Tracker Application



## Table of Contents
- [1. Introduction](#1-introduction)
    - [1.1 Scope of the Project](#11-scope-of-the-project)
    - [1.2 Design and Implementation](#12-design-and-implementation)
    - [1.3 Assumptions made in this version](#13-assumptions-made-in-this-version)
    - [1.4 Tech Stack & Dependencies](#14-tech-stack--dependencies)
    - [1.5 Backend Logic](#15-backend-logic)
    - [1.6 Frontend Design](#16-frontend-design)
- [2. Log](#2-log)
    - [2.1 CMake and program execution](#21-cmake-and-program-execution)
    - [2.2 Core math primitives](#22-core-math-primitives-structures)
    - [2.3 Validation using try and catch](#23-validation-using-try-and-catch)
    - [2.4 TLE parsing and validation](#24-tle-parsing-and-validation)
    - [2.5 TLE SGP4 propagator](#25-tle-sgp4-propagator)
        - [2.5.1 TlePropagator.h](#251-tlepropagatorh)
        - [2.5.2 Vallado's SGP4 Propagator](#252-vallados-sgp4-propagator)
        - [2.5.3 SGP4Propagator.h and .cpp](#253-sgp4propagatorh-and-cpp)
        - [2.5.4 Delta Minutes](#254-delta-minutes)
        - [2.5.5 Calling the Propagator](#255-calling-the-propagator)
        - [2.5.6 TEME to ECEF](#256-teme-to-ecef)


## 1. Introduction

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


## 2. Log

### 2.1 CMake and program execution
Some of the dependencies for the project are C++ language, that leads to the need of a native compiler (like clang) and a "middleman", CMake.\
CMake needs a CMakeList.txt file that will be placed inside the main folder. In this file, it will be needed to specify minimum version for cmake, the project's name and language used together with its version, and the executable file (usually main.cpp). In case there are external libraries used in the project, there will be an extra section in the file that will detail this.\
In order to compile and create an executable, it is necessary to run the cmake command twice: the first time to tell the machine that we are considering the current folder (main directory) as the source and the buiding folder will be specified with a name, and the second time to build the program through clang, the compiler. Note that the first step is needed only if we are changing the environment variables in the CMakeList.txt file, otherwise the second command is sufficient.\
Overall these are the commands that will make the execution of the program possible:
- `cmake -S . -B <name-of-build-folder>`: set the current folder as source, the specified folder as build folder;
- `cmake --build <name-of-build-folder>`: build the project in the specified folder;
- `./<name-of-project-in-cmakelist-file>`: execute the program with name specified in the CMakeList.txt.

### 2.2 Core math primitives structures
1. Vector3D.h: initialise the vector in the Vector3D header. It will have 3 components {X, Y, Z} which are doubles. It is possible to include in the header file also the norm calculation of that vector as a method inside the structure;
2. TimeUTC.h: initialise the time structure with values of year, month, day, hour and minute as integer, and second as double. As for the Vector3D, a method is also introduced to validate the object (i.e. months must be <= 12, hours <= 24);
3. OrbitState.h: this structure will include 3 different components and each of them will be either a Vector3D or TimeUTC structure. The first component is the position 3D vector, the second one is the velocity 3D vector and the last one is the epoch defined as TimeUTC struct.

It is important to recall that to access one single field of each structure, the following notation must be used: `orbitstate.position.x`. This will return the X component of the position vector of the defined `orbitstate`, name of the variable of the type OrbitState.

### 2.3 Validation using try and catch
Exceptions are written inside a chosen function in terms of "if - throw" statements. If the condition is met (i.e. January results in having 34 days), then the proper exception is called via the throw method (in this case it will be "invalid_argument").\
``` 
if (day < 1 || day > 31) {
    throw std::invalid_argument("Invalid day.");
}
```
Validating inputs means there is a possibility they could result invalid. In this case an error should be raised and it would be ideal to know what is the reason for the error.\
\
The statement we can make use of is the "try - catch". In the try bit we develop the necessary code. The catch bit is called only if the try fails and will return the nature of the error. It is also important to return 1 as a value instead of the usual 0.\
```
try {
    IF STATEMENT WITH THROW
} 
catch(const std::exception& e) {
    std::cerr << e.what();
    return 1;
}
```
The nature of the error comes from what is defined in the function that failed inside the try piece. For example, if the error inside the validate function is "invalid_argument" and the reason for it, then calling the nature of the exception will return exactly that, which means in this case "invalid day.".

### 2.4 TLE parsing and validation
- Step 1: a header file is created in the domain section, called Tle.h. It will describe the structure TLE. It has attribute of name, line1 and line2. The <string> module will need to be included to make this possible;
- Step 2: a file TleParser.h will be created in the section "tle" of the repo. This performs two basic checks: line1 and line2 must be min 69 characters in size and line1 must start with '1' (char, not string "1"), line2 with '2'. The result of this parser will still be a tle object, hence:
```
inline Tle TleParser( <arguments> ) { < validation logic > }
```
- NB: since we have to include the Tle.h that we created at step1, there is no need to add again the <string> module, since that will already be included in that file.

### 2.5 TLE SGP4 propagator
#### 2.5.1 TlePropagator.h
A header file called TlePropagator.h is needed and will be stored in the space "propagation" of the repo. It will initialise the class (which details a behaviour, unlike structures that detail just data). Everytime we initialise a class, we need to define public API so that the class will be able to be used in other files of the main project, like the main.cpp file. To achieve this, the keyword "public" can be added as the very first line of the class element. In addition, we will also introduce the virtual destructor and vtable/pointers:
```
virtual ~TlePropagator() = default;
virtual OrbitState propagate (
    // This will be the input to give which are different from the cartesian coordinates, they are TLE
    const Tle& tle,
    const TimeUTC& targetTime
) const = 0;
```
The two arguments that are needed to run this type of propagator are the TLE itself and the final time we need to propagate to.
#### 2.5.2 Vallado's SGP4 propagator
Since there are already many SGP4 propagator versions available open source, there is no need to write it from scratch. It can be taken from a chosen source. In this case from Vallado's public repo, using the header and cpp files. This version is also used by NASA, ESA, Orekit... These files will be saved in a specific folder called "external", where all the external physic codes are stored and remain untouched.
- NB: an amendment is needed to make the propagator work. Change line 2253 of the SGP4.cpp file since this code would be sutable for C and not C++.\
To be able to use the external library, CMakeList.txt must be updated too:
```
add_library(sgp4
    external/sgp4/sgp4.cpp
)
target_include_directories(sgp4 PUBLIC external/sgp4)
target_link_libraries(SatelliteTracker PRIVATE sgp4)
```
#### 2.5.3 SGP4Propagator.h and .cpp
It is time now to link the external library fully functioning to the SGP4Propagator.h that we will create. This propagator will be of type TlePropagator which means will have as inputs the TLE and the final epoch. Since this is a header file, we are not going to detail the logic and process here. Instead, we are creating a separate file where everything will be detailed in terms of logic, and that is as best-practice the .cpp file, lying in the same folder.\
In this last file, it is important to import the external library in this way:
```
extern "C" {
    #include "../../external/sgp4/sgp4.h"
}
```
To know which arguments are needed, it is only required to double check in the source file of the SGP4 propagator by Vallado.
#### 2.5.4 Delta minutes
This particular propagator works using the time difference from TLE epoch and final epoch expressed in minutes. Therefore, a separate function is needed and that is going to be stored in the TimeUtils.h file. It is simple enough to utilise MJD2000 for example, so that both the times are converted in linear scale with respect to a specific and unique reference, so that they can then be subtracted to each other. This function has to return a double value.\
From the TLE it is possible to get the last two digits of the year and the day of the year (all expressed as strings), so a bit of parsing is needed. A function ```epoch2MJD2000_TLE(int year, double dayFrac)``` has been created using the two inputs convertede in int and double as arguments.\
For the final epoch, it is of structure TimeUTC hence it will have integer values for year, month, day, hour and minute and double seconds. The function ```epoch2MJD2000(const TimeUTC& targetTime)``` has been created instead with the object as argument.
#### 2.5.5 Calling the propagator
After all these steps are completed, the propagator can be called via ```SGP4Funcs::sgp4(satrec, m, r, v);```, where m is the difference of time in minutes as a double. Two vectors, position and velocity, will be returned and they will be expressed in TEME reference frame. This means that we will need to compute rotations to translate the vectors in ECEF frame to then get the geodetic coordinates and therefore latitude and longitude, needed to plot the groundtrack.
#### 2.5.6 TEME to ECEF
Since the orientation of the Earth in space depends on the absolute time we are considering, we need to compute the rotation angle that will then be applied to the vectors using the target UTC time we have used for the propagation and converted to seconds (same process as we did for the previous step, using then MJD2000), and the rotation velocity of the Earth.\
Then we will rotate the position vector around the Z-axis but an angle of minus theta. This because we are rotating the coordinate frame itself, e.g the Earth eastwards.



<!-- NOTE: introduce function in TimeUtils that checks if t1 is after t0, i.e. if final epoch is after the initial instant or TLE time so that dt can be positive value. -->





<!-- add "reference" with the external sgp4 link and cleestrak as source of truth -->



![Application Mindmap](diagram/SatelliteTracker.svg)