#include "config.h"

class Material {
    public:
        Material(const char* filename);
        ~Material(); // destructor (frees up resources)
        void use(int unit);
    private:
        unsigned int texture; // acts like an ID number
};