//
// Created by marti on 22/05/2024.
//

#ifndef MAIN_GUIVARIABLES_H
#define MAIN_GUIVARIABLES_H
#include "glm/vec3.hpp"


class GuiVariables {
public:
    GuiVariables() = delete;

    enum ChunkDrawMode
    {
        None,
        Chunks,
        Blocks,
    };

    enum TerrainDrawMode
    {
        Disabled,
        Fill,
        Wireframe,
    };


    static int s_ChunkDrawMode;
    static int s_TerrainDrawMode;
    static bool s_WireframeTerrain;
    static bool s_DisplayNormals;
    static bool s_EnableSSBOCaching;
    static bool s_EnableEmptyBlockCaching;
    static bool s_DrawLODs[5];
    static bool s_StationaryCam;
    static bool s_Octaves[6];
    static float s_LODdistance;
    static int s_RenderDistance;

    // Atmosphere
    static bool s_UseEnvTexture;
    static float s_BetaRayleigh[3];
    static float s_BetaMie;
    static float s_AtmosphereRadius;
    static float s_PlanetRadius;
    static int s_NumberOfSamples;
    static int s_NumberOfLightSamples;
};


#endif //MAIN_GUIVARIABLES_H
