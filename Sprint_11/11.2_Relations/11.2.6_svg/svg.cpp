#include "svg.h"

namespace svg {

using namespace std::literals;

std::ostream& operator<<(std::ostream& out, StrokeLineCap line_cap) {
    switch (line_cap)
    {
        case StrokeLineCap::BUTT :
            out << "butt"sv;
            break;
        case StrokeLineCap::ROUND :
            out << "round"sv;
            break;
        case StrokeLineCap::SQUARE :
            out << "square"sv;
            break;
    default:
        break;
    }
    
    return out;
}  

std::ostream& operator<<(std::ostream& out, StrokeLineJoin line_join) {
    switch (line_join)
    {
        case StrokeLineJoin::ARCS :
            out << "arcs"sv;
            break;
        case StrokeLineJoin::BEVEL :
            out << "bevel"sv;
            break;
        case StrokeLineJoin::MITER :
            out << "miter"sv;
            break;
        case StrokeLineJoin::MITER_CLIP :
            out << "miter-clip"sv;
            break;
        case StrokeLineJoin::ROUND :
            out << "round"sv;
            break;
    default:
        break;
    }
    
    return out;
}

std::ostream& operator<<(std::ostream& out, std::monostate color){
    (void)color;
    out << "none"sv;
    return out;
}

std::ostream& operator<<(std::ostream& out, Rgb color){
    out << "rgb("sv << int(color.red) << ","sv << int(color.green) << ","sv << int(color.blue) <<")"sv;
    return out;
}

std::ostream& operator<<(std::ostream& out, Rgba color){
    out << "rgba("sv << int(color.red) << ","sv << int(color.green) << ","sv << int(color.blue) << ","sv << color.opacity <<")"sv;
    return out;
}

std::ostream& operator<<(std::ostream& out, Color color){
    std::visit(
        [&out](auto color){out << color;},
        color
    );
    return out;
}

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    RenderAttrs(out);
    out << "/>"sv;
}

// ---------- Polyline ------------------

Polyline& Polyline::AddPoint(Point point){
    points_.push_back(point);
    return *this;
}

void Polyline::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<polyline points=\""sv ;
    bool first = true;
    for(auto cur_point: points_){
        if(first){
            first = false;
        } else{
            out << " "sv;    
        }

        out << cur_point.x << ","sv << cur_point.y;
    }
    out << "\""sv;
    RenderAttrs(out);

    out << "/>"sv;
}

// ---------- Text ------------------

    Text& Text::SetPosition(Point pos){
        pos_ = pos;
        return *this;
    }

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& Text::SetOffset(Point offset){
        offset_ = offset;
        return *this;
    }

    // Задаёт размеры шрифта (атрибут font-size)
    Text& Text::SetFontSize(uint32_t size){
        size_ = size;
        return *this;
    }

    // Задаёт название шрифта (атрибут font-family)
    Text& Text::SetFontFamily(std::string font_family){
        font_family_ = font_family;
        return *this;
    }

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& Text::SetFontWeight(std::string font_weight){
        font_weight_ = font_weight;
        return *this;
    }

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& Text::SetData(std::string data){

        size_t replace_pos = data.find_first_of('&');
        while(replace_pos != std::string::npos){
            data = data.replace(replace_pos, 1, "&amp;");
            replace_pos = data.find_first_of('&');
        }

        replace_pos = data.find_first_of('\"');
        while(replace_pos != std::string::npos){
            data = data.replace(replace_pos, 1, "&quot;");
            replace_pos = data.find_first_of('\"');
        }

        replace_pos = data.find_first_of('\'');
        while(replace_pos != std::string::npos){
            data = data.replace(replace_pos, 1, "&apos;");
            replace_pos = data.find_first_of('\'');
        }

        replace_pos = data.find_first_of('<');
        while(replace_pos != std::string::npos){
            data = data.replace(replace_pos, 1, "&lt;");
            replace_pos = data.find_first_of('<');
        }

        replace_pos = data.find_first_of('>');
        while(replace_pos != std::string::npos){
            data = data.replace(replace_pos, 1, "&gt;");
            replace_pos = data.find_first_of('>');
        }

        data_ = data;
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<text"sv ;
        RenderAttrs(out);
        out << " x= \""sv << pos_.x << "\" "sv ;
        out << "y= \""sv << pos_.y << "\" "sv ;
        out << "dx= \""sv << offset_.x << "\" "sv ;
        out << "dy= \""sv << offset_.y << "\" "sv ;
        out << "font-size= \""sv << size_ << "\""sv ;

        if(font_family_.size()>0){
            out << " font-family= \""sv << font_family_ << "\""sv ;
        }

        if(font_weight_.size()>0){
            out << " font-weight= \""sv << font_weight_ << "\""sv ;
        }
        
        out << ">"sv << data_ << "</text>"sv;
    }

// ---------- Document ------------------

// Добавляет в svg-документ объект-наследник svg::Object
void Document::AddPtr(std::unique_ptr<Object>&& obj){
    objects_.push_back(move(obj));
}

// Выводит в ostream svg-представление документа
void Document::Render(std::ostream& out) const{

    RenderContext ctx(out, 2, 2);

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;

    for(size_t i=0; i<objects_.size(); ++i){
        objects_[i]->Render(ctx);
    }

    std::cout << "</svg>"sv;
}

}  // namespace svg