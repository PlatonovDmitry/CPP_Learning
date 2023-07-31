#pragma once

#include "json.h"

#include <optional>
#include <variant>
#include <stack>

namespace json{

    class Builder{
    public:

        class ItemContext{
        public:
            ItemContext(Builder* builder): builder_(builder) { }
            ItemContext(const ItemContext& parent): builder_(parent.builder_) { }

            ItemContext Value(Node value);
            ItemContext Key(std::string key);
            ItemContext StartDict();
            ItemContext StartArray();
            ItemContext EndDict();
            ItemContext EndArray();
            Node Build();

        private:
            Builder* builder_;
        };

        class DictItemContext: public ItemContext{
        public:
            class ArrayItemContext: public ItemContext{
            public:
                ArrayItemContext(Builder* builder): ItemContext(builder) { }
                ArrayItemContext(const ItemContext& parent): ItemContext(parent) { }

                ArrayItemContext Value(Node value){
                    return ArrayItemContext(ItemContext::Value(value));
                }
                DictItemContext StartDict(){
                    return DictItemContext(ItemContext::StartDict());
                }
                ArrayItemContext StartArray(){
                    return ArrayItemContext(ItemContext::StartArray());
                }

                ItemContext Key(std::string key) = delete;
                ItemContext EndDict() = delete;
                Node Build() = delete;
            };

            class KeyItemContext: public ItemContext{
            public:        
                KeyItemContext(Builder* builder): ItemContext(builder) { }
                KeyItemContext(const ItemContext& parent): ItemContext(parent) { }

                DictItemContext Value(Node value){
                    return DictItemContext(ItemContext::Value(value));
                }
                DictItemContext StartDict(){
                    return DictItemContext(ItemContext::StartDict());
                }
                ArrayItemContext StartArray(){
                    return ArrayItemContext(ItemContext::StartArray());
                }

                ItemContext Key(std::string key) = delete;
                ItemContext EndDict() = delete;
                ItemContext EndArray() = delete;
                Node Build() = delete;
            };

            DictItemContext(Builder* builder): ItemContext(builder) { }
            DictItemContext(const ItemContext& parent): ItemContext(parent) { }

            KeyItemContext Key(std::string key){
                return KeyItemContext(ItemContext::Key(key));
            }

            ItemContext Value(Node value) = delete;
            ItemContext StartDict() = delete;
            ItemContext StartArray() = delete;
            ItemContext EndArray() = delete;
            Node Build() = delete;
        };
        
        ItemContext Value(Node value);
        DictItemContext::KeyItemContext Key(std::string key);
        DictItemContext StartDict();
        DictItemContext::ArrayItemContext StartArray();
        ItemContext EndDict();
        ItemContext EndArray();
        Node Build();

    private:
        std::stack<Node*> node_stack_;
        Node root_;
        std::optional<std::string> opened_key_;
    };
}