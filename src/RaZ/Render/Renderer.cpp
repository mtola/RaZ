#include "GL/glew.h"
#include "RaZ/Render/Renderer.hpp"

namespace Raz {

namespace {

inline constexpr const char* recoverGlErrorStr(unsigned int errorCode) {
  switch (errorCode) {
    case GL_INVALID_ENUM:      return "Unrecognized error code (Invalid enum)";
    case GL_INVALID_VALUE:     return "Numeric argument out of range (Invalid value)";
    case GL_INVALID_OPERATION: return "Operation illegal in current state (Invalid operation)";
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    case GL_OUT_OF_MEMORY:     return "Not enough memory left (Out of memory)";
    case GL_NO_ERROR:          return "No error";
    default:                   return "Unknown error";
  }
}

} // namespace

void Renderer::initialize() {
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK)
    std::cerr << "Error: Failed to initialize GLEW." << std::endl;
  else
    s_isInitialized = true;
}

void Renderer::enable(unsigned int code) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glEnable(code);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::disable(unsigned int code) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glDisable(code);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::bindBuffer(unsigned int type, unsigned int index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glBindBuffer(type, index);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::bindBufferBase(unsigned int type, unsigned int bindingIndex, unsigned int bufferIndex) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glBindBufferBase(type, bindingIndex, bufferIndex);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::bindTexture(unsigned int type, unsigned int index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glBindTexture(type, index);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::resizeViewport(int xOrigin, int yOrigin, unsigned int width, unsigned int height) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glViewport(xOrigin, yOrigin, width, height);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::generateBuffers(unsigned int count, unsigned int* index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glGenBuffers(count, index);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::deleteBuffers(unsigned int count, unsigned int* index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glDeleteBuffers(count, index);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

unsigned int Renderer::createProgram() {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  const unsigned int programIndex = glCreateProgram();

#if !defined(NDEBUG)
  checkErrors();
#endif

  return programIndex;
}

int Renderer::getProgramInfo(unsigned int index, unsigned int infoType) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  int res;
  glGetProgramiv(index, infoType, &res);

#if !defined(NDEBUG)
  checkErrors();
#endif

  return res;
}

bool Renderer::isProgramLinked(unsigned int index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  const bool isLinked = getProgramInfo(index, GL_LINK_STATUS);

#if !defined(NDEBUG)
  checkErrors();
#endif

  return isLinked;
}

void Renderer::linkProgram(unsigned int index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glLinkProgram(index);

  if (!isProgramLinked(index)) {
    std::array<char, 512> infoLog {};

    glGetProgramInfoLog(index, static_cast<int>(infoLog.size()), nullptr, infoLog.data());
    std::cerr << "Error: Shader program link failed (ID " << index << ").\n" << infoLog.data() << std::endl;
  }

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::useProgram(unsigned int index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glUseProgram(index);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::deleteProgram(unsigned int index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glDeleteProgram(index);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

unsigned int Renderer::createShader(ShaderType type) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  const unsigned int shaderIndex = glCreateShader(static_cast<unsigned int>(type));

#if !defined(NDEBUG)
  checkErrors();
#endif

  return shaderIndex;
}

void Renderer::attachShader(unsigned int programIndex, unsigned int shaderIndex) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glAttachShader(programIndex, shaderIndex);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::detachShader(unsigned int programIndex, unsigned int shaderIndex) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glDetachShader(programIndex, shaderIndex);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::deleteShader(unsigned int index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glDeleteShader(index);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

int Renderer::recoverUniformLocation(unsigned int programIndex, const char* uniformName) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  const int location = glGetUniformLocation(programIndex, uniformName);

#if !defined(NDEBUG)
  checkErrors();

  if (location == -1)
    std::cerr << "Warning: Uniform '" << uniformName << "' unrecognized." << std::endl;
#endif

  return location;
}

void Renderer::activateTexture(unsigned int index) {
  assert("Error: The Renderer must be initialized before calling its functions." && isInitialized());

  glActiveTexture(GL_TEXTURE0 + index);

#if !defined(NDEBUG)
  checkErrors();
#endif
}

void Renderer::checkErrors() {
  while (true) {
    const unsigned int errorCode = glGetError();

    if (errorCode == GL_NO_ERROR)
      break;

    std::cerr << "OpenGL error - " << recoverGlErrorStr(errorCode) << " (code " << errorCode << ")\n";
  };

  std::cerr << std::flush;
}

} // namespace Raz
