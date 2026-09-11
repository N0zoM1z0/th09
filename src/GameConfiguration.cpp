#include "GameConfiguration.hpp"

#include <string.h>

void GameConfiguration::Initialize()
{
    memset(this, 0, sizeof(GameConfiguration));
    this->version = 0x90003;
    this->padXAxis = 600;
    this->padYAxis = 600;
    this->musicMode = 1;
    this->valueAF = 1;
    this->difficulty = 1;
    this->valueAC = 0;
    this->valueAD = 0;
    this->windowed = 0;
    this->valueB2 = 0;
    this->controllerMapping.bindings[0] = g_ControllerMapping.primaryBindings[0];
    this->controllerMapping.bindings[1] = g_ControllerMapping.primaryBindings[1];
    this->controllerMapping.bindings[2] = g_ControllerMapping.primaryBindings[2];
    this->controllerMapping.bindings[3] = g_ControllerMapping.secondaryBindings[0];
    this->controllerMapping.bindings[4] = g_ControllerMapping.secondaryBindings[1];
    this->controllerMapping.bindings[5] = g_ControllerMapping.secondaryBindings[2];
    this->valueB3 = 2;
    this->musicVolume = 100;
    this->sfxVolume = 80;
    this->valueB4 = 0;
    this->valueB5 = 0;
    this->valueB6 = 0;
    this->valueB7 = 0;
    this->valueB8 = 1;
    this->valueB9 = 2;
}

GameConfiguration::GameConfiguration()
{
    this->Initialize();
}
