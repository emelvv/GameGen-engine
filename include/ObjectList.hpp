#pragma once
#include <vector>
#include "Object.hpp"



namespace Engine {
    class ObjectList {
    private:
        std::vector<Object> objects; // Вектор для хранения объектов

    public:

        ObjectList() {
            // Конструктор по умолчанию
        }

        void Add(const Object& obj) {
            objects.push_back(obj);
        }

        void Pop(int index) {
            objects.erase(objects.begin() + index);
        }

        Object& operator[](int index) {
            return objects[index];
        }

        int Len() const {
            return objects.size();
        }

        void DrawAll() {
            for (int i = 0; i < this->Len(); i++) {
                this->objects[i].Draw();
            }
        }
    };
}