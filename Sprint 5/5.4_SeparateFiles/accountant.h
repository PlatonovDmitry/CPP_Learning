#pragma once
#include "square_calculation.h"
#include "wall.h"

#include <utility>

class Accountant {
public:
    template <class BuildingObject>
    double CalcPaintNeeded(const BuildingObject& building_object) const;
    template <class BuildingObject>
    double CalcBricksNeeded(const BuildingObject& building_object) const;
};