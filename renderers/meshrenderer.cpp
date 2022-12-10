#include "meshrenderer.h"

/**
 * @brief MeshRenderer::MeshRenderer Creates a new mesh renderer.
 */
MeshRenderer::MeshRenderer() : meshIBOSize(0) {}

/**
 * @brief MeshRenderer::~MeshRenderer Deconstructor.
 */
MeshRenderer::~MeshRenderer() {
  gl->glDeleteVertexArrays(1, &vao);

  gl->glDeleteBuffers(1, &meshCoordsBO);
  gl->glDeleteBuffers(1, &meshNormalsBO);
  gl->glDeleteBuffers(1, &meshIndexBO);
}

/**
 * @brief MeshRenderer::initShaders Initializes the shaders used to shade a
 * mesh.
 */
void MeshRenderer::initShaders() {
  shaders.insert(ShaderType::PHONG, constructDefaultShader("phong"));
  shaders.insert(ShaderType::ISOPHOTES, constructDefaultShader("isophotes"));

}

/**
 * @brief MeshRenderer::initBuffers Initializes the buffers. Uses indexed
 * rendering. The coordinates and normals are passed into the shaders.
 */
void MeshRenderer::initBuffers() {
  gl->glGenVertexArrays(1, &vao);
  gl->glBindVertexArray(vao);

  gl->glGenBuffers(1, &meshCoordsBO);
  gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
  gl->glEnableVertexAttribArray(0);
  gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  gl->glGenBuffers(1, &meshNormalsBO);
  gl->glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
  gl->glEnableVertexAttribArray(1);
  gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  gl->glGenBuffers(1, &meshIndexBO);
  gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);

  gl->glBindVertexArray(0);
}

/**
 * @brief MeshRenderer::updateBuffers Updates the buffers based on the provided
 * mesh.
 * @param mesh The mesh to update the buffer contents with.
 */
void MeshRenderer::updateBuffers(Mesh& mesh) {
  QVector<QVector3D>& vertexCoords = mesh.getVertexCoords();
  QVector<QVector3D>& vertexNormals = mesh.getVertexNorms();
  QVector<unsigned int>& polyIndices = mesh.getPolyIndices();

  gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
  gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * vertexCoords.size(),
                   vertexCoords.data(), GL_STATIC_DRAW);

  gl->glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
  gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * vertexNormals.size(),
                   vertexNormals.data(), GL_STATIC_DRAW);

  gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);
  gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   sizeof(unsigned int) * polyIndices.size(),
                   polyIndices.data(), GL_STATIC_DRAW);

  meshIBOSize = polyIndices.size();
}

/**
 * @brief MeshRenderer::updateUniforms Updates the uniforms in the shader.
 */
void MeshRenderer::updateUniforms() {
   QOpenGLShaderProgram* shader;
  if (settings->renderBasicModel && !settings->phongShadingRender && !settings->isophotesRender){
  shader = shaders[settings->currentShader];
  }
  else if (settings->phongShadingRender && settings->renderBasicModel){
  shader = shaders[settings->currentShader];
  }
  else if (settings->isophotesRender && settings->renderBasicModel){
  shader = shaders[settings->isophotesShader];
  }
  uniModelViewMatrix = shader->uniformLocation("modelviewmatrix");
  uniProjectionMatrix = shader->uniformLocation("projectionmatrix");
  uniNormalMatrix = shader->uniformLocation("normalmatrix");

  gl->glUniformMatrix4fv(uniModelViewMatrix, 1, false,
                         settings->modelViewMatrix.data());
  gl->glUniformMatrix4fv(uniProjectionMatrix, 1, false,
                         settings->projectionMatrix.data());
  gl->glUniformMatrix3fv(uniNormalMatrix, 1, false,
                         settings->normalMatrix.data());
  // Uniforms for frequency and color of stripes
  if (settings->isophotesRender){
     frequencyLocation = shader->uniformLocation("frequency");
     stripeColorLocation = shader->uniformLocation("stripesCode");

    qDebug() << stripeColorLocation << settings->colorStripeCode;
     shader->setUniformValue(frequencyLocation,settings->frequencyIsophotes);
     shader->setUniformValue(stripeColorLocation,settings->colorStripeCode);

     //int stripeCodeLocation = shader->uniformLocation("stripesCode");
     //gl->glUniform1f(frequencyLocation,settings->frequencyIsophotes);
  }


}

/**
 * @brief MeshRenderer::draw Draw call.
 */
void MeshRenderer::draw() {
    gl->glClearColor(0.0, 0.0, 0.0, 1.0);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    qDebug() << "draw basic model" << settings->renderBasicModel;
    qDebug() << "draw called phong:" << settings->phongShadingRender;
    qDebug() << "draw called iso:" << settings->isophotesRender;

    if (settings->renderBasicModel && !settings->phongShadingRender && !settings->isophotesRender){
        drawPhong();
    }
    else if (settings->phongShadingRender && settings->renderBasicModel){
        drawPhong();
    }
    else if (settings->isophotesRender && settings->renderBasicModel){
        drawIsophotes();
    }

}
/**
 * @brief MeshRenderer::drawPhong Draw phong shader.
 */
void MeshRenderer::drawPhong(){

    shaders[settings->currentShader]->bind();

    if (settings->uniformUpdateRequired) {
      updateUniforms();
      settings->uniformUpdateRequired = false;
    }

    gl->glBindVertexArray(vao);
    gl->glDrawElements(GL_TRIANGLES, meshIBOSize, GL_UNSIGNED_INT, nullptr);

    gl->glBindVertexArray(0);

    shaders[settings->currentShader]->release();
}
/**
 * @brief MeshRenderer::drawPhong Draw Isophotes.
 */
void MeshRenderer::drawIsophotes(){

    shaders[settings->isophotesShader]->bind();

    if (settings->uniformUpdateRequired) {
      updateUniforms();
      settings->uniformUpdateRequired = false;
    }

    gl->glBindVertexArray(vao);
    gl->glDrawElements(GL_TRIANGLES, meshIBOSize, GL_UNSIGNED_INT, nullptr);

    gl->glBindVertexArray(0);

    shaders[settings->isophotesShader]->release();
}
