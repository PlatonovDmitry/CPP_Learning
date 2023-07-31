#include "json_builder.h"

#include <stdexcept>

using namespace std;

namespace json{

    Builder::ItemContext Builder::ItemContext::Value(Node value){
        return builder_->Value(value);
    }
    Builder::ItemContext Builder::ItemContext::Key(std::string key){
        return builder_->Key(key);
    }
    Builder::ItemContext Builder::ItemContext::StartDict(){
        return builder_->StartDict();
    }
    Builder::ItemContext Builder::ItemContext::StartArray(){
        return builder_->StartArray();
    }
    Builder::ItemContext Builder::ItemContext::EndDict(){
        return builder_->EndDict();
    }
    Builder::ItemContext Builder::ItemContext::EndArray(){
        return builder_->EndArray();
    }
    Node Builder::ItemContext::Build(){
        return builder_->Build();
    }


    Builder::ItemContext Builder::Value(Node value){
        if(root_.IsNull()){
            root_ = value;
            //root_ptr_=&root_;
        } else if(opened_key_.has_value()){
            node_stack_.top()->AsDict()[opened_key_.value()] = value;
            opened_key_.reset();
        } else if(node_stack_.size() >0 && node_stack_.top()->IsArray()) {
            node_stack_.top()->AsArray().push_back(value);
        } else {
            throw std::logic_error("NotAllowed Value() called!"s);
        }

        return ItemContext{this};
    }

    Builder::DictItemContext::KeyItemContext Builder::Key(std::string key){
        if(node_stack_.size() > 0 && 
            node_stack_.top()->IsDict() && 
            !opened_key_.has_value())
        {
            opened_key_= key;   
        } else {
            throw std::logic_error("NotAllowed Key() called!");
        }

        return Builder::DictItemContext::KeyItemContext{this};
    }

    Builder::DictItemContext Builder::StartDict(){
        if(root_.IsNull()){
            root_= move(Dict{});
            //root_ptr_ = &root_;
            node_stack_.push(&root_);
        }else if(opened_key_.has_value()){
            node_stack_.top()->AsDict()[opened_key_.value()] = Dict{};
            node_stack_.push(&node_stack_.top()->AsDict()[opened_key_.value()]);
            opened_key_.reset();
        } else if(node_stack_.size() > 0 && node_stack_.top()->IsArray()) {
            node_stack_.top()->AsArray().push_back(Dict{});
            node_stack_.push(&node_stack_.top()->AsArray().back());
        } else {
            throw std::logic_error("NotAllowed StartDict() called!");
        }

        return DictItemContext(this);
    }

    Builder::DictItemContext::ArrayItemContext Builder::StartArray(){
        if(root_.IsNull()){
            root_ = Array{};
            node_stack_.push(&root_);
        } else if(opened_key_.has_value()){
            node_stack_.top()->AsDict()[opened_key_.value()] = Array{};
            node_stack_.push(&node_stack_.top()->AsDict()[opened_key_.value()]);
            opened_key_.reset();
        } else if(node_stack_.size() > 0 && node_stack_.top()->IsArray()) {
            node_stack_.top()->AsArray().push_back(Array{});
            node_stack_.push(&node_stack_.top()->AsArray().back());
        } else {
            throw std::logic_error("NotAllowed StartArray() called!");
        }

        return Builder::DictItemContext::ArrayItemContext(this);
    }

    Builder::ItemContext Builder::EndDict(){
        if(node_stack_.size() > 0 && node_stack_.top()->IsDict() && !opened_key_.has_value()){
            node_stack_.pop();
        } else{
            throw std::logic_error("NotAllowed EndDict() called!");
        }

        return ItemContext(this);
    }

    Builder::ItemContext Builder::EndArray(){
        if(node_stack_.size() > 0 && node_stack_.top()->IsArray()){
            node_stack_.pop();
        } else{
            throw std::logic_error("NotAllowed EndArray() called!");
        }

        return ItemContext(this);
    }

    Node Builder::Build(){
        if(root_.IsNull() || node_stack_.size() > 0){
            throw std::logic_error("NotAllowed Build() called!");
        }

        return root_;
    }
}