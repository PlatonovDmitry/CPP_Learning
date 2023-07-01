#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
            size_ = GetImageSize(image_);
    }

    Size GetSize() const {
        return size_;
    }

    char GetPixelColor(Point p) const {
        if(p.x < 0 || p.x >= size_.width){
            return '.';
        }

        if(p.y < 0 || p.y >= size_.height){
            return '.';
        }

        return image_[p.y].at(p.x);
    }

private:
    Image image_;
    Size size_;
};