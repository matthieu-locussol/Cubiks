#include <SFML/Window.hpp>
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
// #include <glm/vec3.hpp>

int main(int argc, char *argv[]) {
  sf::Window window(sf::VideoMode(1200, 800), "Cubiks", sf::Style::Default,
                    sf::ContextSettings(32));
  window.setVerticalSyncEnabled(true);
  window.setActive(true);

  glbinding::Binding::initialize();
  gl::glClearColor(0.6f, 0.5f, 0.8f, 1.0f);

  //   glm::vec3(-1.0f, 0.0f, 0.0f);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    window.display();
  }

  return 0;
}