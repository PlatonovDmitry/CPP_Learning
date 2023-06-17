#include <algorithm>
#include <iostream>
#include <list>
#include <string>


using namespace std;

class Editor {
public:
    Editor()
    :cursor_(text_.end()){

    };
    // сдвинуть курсор влево
    void Left(){
        if(cursor_ != text_.begin()){
            --cursor_;
        }
    }
    // сдвинуть курсор вправо 
    void Right(){
        if(cursor_ != text_.end()){
            ++cursor_;
        }
    }
    // вставить символ token
    void Insert(char token){
        text_.insert(cursor_, token);
    }
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1){
        auto last_pos = GetLastPosition(tokens);
        bufer_.assign(cursor_, last_pos);
        cursor_ = text_.erase(cursor_, last_pos);
    }
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1){
        auto last_pos = GetLastPosition(tokens);
        bufer_.assign(cursor_, last_pos);
    }
    // вставить содержимое буфера в текущую позицию курсора
    void Paste(){
        text_.insert(cursor_, bufer_.begin(), bufer_.end());
    }
    // получить текущее содержимое текстового редактора
    string GetText() const{
        string output;
        for(auto cur_char: text_){
            output+= cur_char;
        }
        return output;
    }

private:
    list<char> text_;
    list<char> bufer_;
    list<char>::iterator cursor_;

    list<char>::iterator GetLastPosition(size_t tokens){
        auto output = cursor_;
        for(size_t i = 0; i < tokens; ++i){
            if (output == text_.end()){
                break;
            }
            ++output;
        }

        return output;
    }
};

int main() {
    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
}