#include "RaZ/Render/Scene.hpp"

namespace Raz {

void Scene::render() const {
  for (const auto& model : m_models)
    model->draw();
}

void Scene::updateLights() const {
  // TODO: Temporary workaround, remove ASAP
  m_models.front()->getMaterial().getShaderProgram().use();
  const GLuint programIndex = m_models.front()->getMaterial().getShaderProgram().getIndex();

  const GLint uniLightCountLocation = glGetUniformLocation(programIndex, "uniLightCount");
  glUniform1ui(uniLightCountLocation, m_lights.size());

  for (std::size_t lightIndex = 0; lightIndex < m_lights.size(); ++lightIndex) {
    const std::string locationBase = "uniLight[" + std::to_string(lightIndex) + "].";
    const std::string posLocation = locationBase + "position";
    const std::string dirLocation = locationBase + "direction";
    const std::string colorLocation = locationBase + "color";
    const std::string angleLocation = locationBase + "angle";

    const GLint uniPositionLocation = glGetUniformLocation(programIndex, posLocation.c_str());
    glUniform4fv(uniPositionLocation, 1, m_lights[lightIndex].getPosition().getData().data());

    const GLint uniDirectionLocation = glGetUniformLocation(programIndex, dirLocation.c_str());
    glUniform3fv(uniDirectionLocation, 1, m_lights[lightIndex].getDirection().getData().data());

    const GLint uniColorLocation = glGetUniformLocation(programIndex, colorLocation.c_str());
    glUniform3fv(uniColorLocation, 1, m_lights[lightIndex].getColor().getData().data());

    const GLint uniAngleLocation = glGetUniformLocation(programIndex, angleLocation.c_str());
    glUniform1f(uniAngleLocation, m_lights[lightIndex].getAngle());
  }
}

} // namespace Raz
