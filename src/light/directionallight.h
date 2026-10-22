#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "light/light.h"
class Directionallight : public Light
{
public:
    Directionallight(const SceneLightData *light);
private:
    const SceneLightData *l_light;
};

#endif // DIRECTIONALLIGHT_H
