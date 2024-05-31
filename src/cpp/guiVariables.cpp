//
// Created by marti on 22/05/2024.
//

#include "guiVariables.h"

// Terrain
int GuiVariables::s_ChunkDrawMode = 0;
int GuiVariables::s_TerrainDrawMode = 1;
bool GuiVariables::s_WireframeTerrain = false;
bool GuiVariables::s_DisplayNormals = false;
bool GuiVariables::s_EnableSSBOCaching = true;
bool GuiVariables::s_EnableEmptyBlockCaching = true;
bool GuiVariables::s_DrawLODs[] = {true, true, true, true, true};
bool GuiVariables::s_StationaryCam = true;
bool GuiVariables::s_Octaves[] = {false, false, false, false, false, false};
float GuiVariables::s_LODdistance = 10.f;
int GuiVariables::s_RenderDistance = 16;

// Atmosphere
bool GuiVariables::s_UseEnvTexture = true;
float GuiVariables::s_BetaRayleigh[3] = {3.8e-6f, 13.5e-6f, 33.1e-6f};
float GuiVariables::s_BetaMie = 21e-6f;
float GuiVariables::s_PlanetRadius = 6360e3;
float GuiVariables::s_AtmosphereRadius = 6420e3;
int GuiVariables::s_NumberOfSamples = 16;
int GuiVariables::s_NumberOfLightSamples = 8;
bool GuiVariables::s_ComputeRayleigh = false;

