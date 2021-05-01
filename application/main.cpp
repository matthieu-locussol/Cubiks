#include <SFML/Window.hpp>
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <iostream>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main(int argc, char *argv[]) {
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.antialiasingLevel = 4;
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  settings.attributeFlags = sf::ContextSettings::Core;

  sf::Window window(sf::VideoMode(1200, 800), "Cubiks", sf::Style::Default,
                    settings);
  window.setVerticalSyncEnabled(true);
  window.setActive(true);

  glbinding::Binding::initialize(nullptr);

  int success;
  char infoLog[512];

  unsigned int vertexShader = gl::glCreateShader(gl::GL_VERTEX_SHADER);
  gl::glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  gl::glCompileShader(vertexShader);

  gl::glGetShaderiv(vertexShader, gl::GL_COMPILE_STATUS, &success);
  if (!success) {
    gl::glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int fragmentShader = gl::glCreateShader(gl::GL_FRAGMENT_SHADER);
  gl::glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  gl::glCompileShader(fragmentShader);

  gl::glGetShaderiv(fragmentShader, gl::GL_COMPILE_STATUS, &success);
  if (!success) {
    gl::glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int shaderProgram = gl::glCreateProgram();
  gl::glAttachShader(shaderProgram, vertexShader);
  gl::glAttachShader(shaderProgram, fragmentShader);
  gl::glLinkProgram(shaderProgram);

  gl::glDeleteShader(vertexShader);
  gl::glDeleteShader(fragmentShader);

  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f  // top
  };

  unsigned int VBO, VAO;
  gl::glGenVertexArrays(1, &VAO);
  gl::glGenBuffers(1, &VBO);

  gl::glBindVertexArray(VAO);

  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, VBO);
  gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                   gl::GL_STATIC_DRAW);

  gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(float),
                            (void *)0);
  gl::glEnableVertexAttribArray(0);

  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);
  gl::glBindVertexArray(0);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    gl::glUseProgram(shaderProgram);
    gl::glBindVertexArray(VAO);
    gl::glDrawArrays(gl::GL_TRIANGLES, 0, 3);

    window.display();
  }

  gl::glDeleteVertexArrays(1, &VAO);
  gl::glDeleteBuffers(1, &VBO);
  gl::glDeleteProgram(shaderProgram);

  return 0;
}