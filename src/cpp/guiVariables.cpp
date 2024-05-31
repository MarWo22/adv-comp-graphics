//
// Created by marti on 22/05/2024.
//

#include "guiVariables.h"

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