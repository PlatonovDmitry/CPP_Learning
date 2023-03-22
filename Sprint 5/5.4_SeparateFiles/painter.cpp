#include "painter.h"
#include "wall.h"

void Painter::Paint(Wall& wall, Wall::Color color) const {
       wall.SetColor(color);
    }
