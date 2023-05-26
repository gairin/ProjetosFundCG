#pragma once
#include "Sprite.h"
class Meteor: public Sprite {
    private:
        int m;
    public:
        Meteor() {}
        void update();
        void respawn(int m);
        void destroy();
};

