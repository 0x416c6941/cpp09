#pragma once

#include <stdint.h>
#include <Data.hpp>

class Data;

class Serializer {
    private:
        Serializer();
        Serializer(const Serializer & src);
        Serializer & operator = (const Serializer & src);

    public:
        virtual ~Serializer();

        static uintptr_t serialize(Data * ptr);
        static Data * deserialize(uintptr_t raw);
};
