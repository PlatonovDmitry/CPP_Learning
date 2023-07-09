#pragma once
#include "texture.h"

#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE};

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type):
    type_(type) {
        
    }

    void SetPosition(Point pos) {
        pos_ = pos;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = move(texture);
    }

    // Рисует фигуру на указанном изображении
	// В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        Size canvas_size = GetImageSize(image);
        for(int y=0; y<size_.height; ++y){
            int canvas_y = y+pos_.y;
            if(canvas_y<0 || canvas_y >= canvas_size.height){
                continue;
            }
            for(int x=0; x<size_.width; ++x){
                int canvas_x = x+pos_.x;
                if(canvas_x < 0 || canvas_x >= canvas_size.width){
                    continue;
                }

                if(type_ == ShapeType::ELLIPSE && !IsPointInEllipse({x,y}, size_)){
                    continue;
                }

                // char new_pix = texture_ == nullptr ? '.' : texture_->GetPixelColor({x, y});
                // image[canvas_y] = image[canvas_y].replace(canvas_x, 1, &new_pix);
                image[canvas_y][canvas_x] = texture_ == nullptr ? '.' : texture_->GetPixelColor({x, y});
            }
        }
    }
private:
    ShapeType type_;
    Point pos_;
    Size size_;
    std::shared_ptr<Texture> texture_;
};