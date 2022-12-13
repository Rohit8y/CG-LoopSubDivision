#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <QOpenGLShaderProgram>

#include "../mesh/mesh.h"
#include "renderer.h"

/**
 * @brief The MeshRenderer class is responsible for rendering a mesh. Only
 * renders triangle meshes.
 */
class MeshRenderer : public Renderer {
 public:
  MeshRenderer();
  ~MeshRenderer() override;

  void updateUniforms();
  void updateBuffers(Mesh& m);
  void draw();
  void drawPhong();
  void drawIsophotes();
  void drawVertexSelection();

 protected:
  void initShaders() override;
  void initBuffers() override;

 private:
  GLuint vao;
  GLuint meshCoordsBO, meshNormalsBO, meshIndexBO, selectedVertexBO;
  int meshIBOSize;

  // Uniforms
  GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix, frequencyLocation, stripeColorLocation;
};

#endif  // MESHRENDERER_H
