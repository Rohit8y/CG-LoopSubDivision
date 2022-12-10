#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMatrix4x4>

#include "shadertypes.h"
#include "mesh/mesh.h"


/**
 * Struct that contains all the settings of the program. Initialised with a
 * number of default values.
 */
typedef struct Settings {
  bool modelLoaded = false;
  bool wireframeMode = true;
  bool phongShadingRender = false;
  bool isophotesRender = false;
  bool renderBasicModel = true;
  int frequencyIsophotes = 0;
  int colorStripeCode = 0;


  float FoV = 80;
  float dispRatio = 16.0f / 9.0f;
  float rotAngle = 0.0f;

  bool uniformUpdateRequired = true;

  int selectedVertex = -1;
  Mesh currentMesh;

  ShaderType currentShader = ShaderType::PHONG;
  ShaderType isophotesShader = ShaderType::ISOPHOTES;

  QMatrix4x4 modelViewMatrix, projectionMatrix;
  QMatrix3x3 normalMatrix;
} Settings;

#endif  // SETTINGS_H
