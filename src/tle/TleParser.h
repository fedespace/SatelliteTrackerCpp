#pragma once
#include "../domain/Tle.h"
#include <stdexcept>
#include <iostream>

// ===============================

Tle validate(Tle& tle);
Tle standardise(Tle& tle);