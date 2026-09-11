#pragma once

#pragma pack(push, 1)
struct ControllerBinding
{
    unsigned int v00;
    unsigned int v04;
    unsigned int v08;
    unsigned int v0C;
    unsigned short v10;
};

struct SerializedControllerMapping
{
    ControllerBinding bindings[6];
};

struct ControllerMapping
{
    ControllerBinding primaryBindings[3];
    unsigned char unknown_0036[0x58];
    ControllerBinding secondaryBindings[3];
};

struct GameConfiguration
{
    SerializedControllerMapping controllerMapping;
    unsigned char unknown_006C[0x38];
    unsigned int version;
    unsigned short padXAxis;
    unsigned short padYAxis;
    unsigned char valueAC;
    unsigned char valueAD;
    unsigned char musicMode;
    unsigned char valueAF;
    unsigned char difficulty;
    unsigned char windowed;
    unsigned char valueB2;
    unsigned char valueB3;
    unsigned char valueB4;
    unsigned char valueB5;
    unsigned char valueB6;
    unsigned char valueB7;
    unsigned char valueB8;
    unsigned char valueB9;
    unsigned char musicVolume;
    unsigned char sfxVolume;
    unsigned char unknown_00BC[0x0C];
    unsigned int options;

    GameConfiguration();
    void Initialize();
};
#pragma pack(pop)

typedef char ControllerBindingSizeCheck[(sizeof(ControllerBinding) == 0x12) ? 1 : -1];
typedef char SerializedMappingSizeCheck[(sizeof(SerializedControllerMapping) == 0x6C) ? 1 : -1];
typedef char ControllerMappingSizeCheck[(sizeof(ControllerMapping) == 0xC4) ? 1 : -1];
typedef char GameConfigurationSizeCheck[(sizeof(GameConfiguration) == 0xCC) ? 1 : -1];

extern ControllerMapping g_ControllerMapping;
