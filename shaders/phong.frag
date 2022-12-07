#version 410
// Fragment shader

layout(location = 0) in vec3 vertcoords_fs;
layout(location = 1) in vec3 vertnormal_fs;

out vec4 fColor;

const vec3 lightPos = vec3(3.0, 0.0, 2.0);
const vec3 lightCol = vec3(1.0);
const vec3 matSpecCol = vec3(1.0);
const vec3 camerapos = vec3(0.0);

const float matAmbientCoeff = 0.2;
const float matDiffuseCoeff = 0.6;
const float matSpecularCoeff = 0.5;

// Basic phong shading
vec3 phongShading(vec3 matCol, vec3 coords, vec3 normal) {
  vec3 surfToLight = normalize(lightPos - coords);
  vec3 surfToCamera = normalize(camerapos - coords);

  if (!gl_FrontFacing) {
    // Make the inside a darker shade.
    normal *= -1;
    matCol *= 0.4;
  }

  float diffuseCoeff = max(0.0, dot(surfToLight, normal));

  vec3 reflected = 2 * diffuseCoeff * normal - surfToLight;
  float specularCoeff = max(0.0, dot(reflected, surfToCamera));

  vec3 compCol = min(1.0, matAmbientCoeff + matDiffuseCoeff * diffuseCoeff) *
                 lightCol * matCol;
  compCol += matSpecularCoeff * specularCoeff * lightCol * matSpecCol;

  return compCol;
}

void main() {
  vec3 matcolour = vec3(0.53, 0.80, 0.87);
  vec3 col =
      phongShading(matcolour, vertcoords_fs, vertnormal_fs);
  fColor = vec4(col, 1.0);
}
