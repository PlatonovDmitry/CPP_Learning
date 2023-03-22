#include "wall.h"

double Wall::GetHeight() const {
    return height_;
}
    
double Wall::GetWidth() const {
    return width_;
}

void Wall::SetColor(Color color) {
    color_ = color;
}
    
Wall::Color Wall::GetColor() const {
    return color_;
}