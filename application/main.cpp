#include <SFML/Window.hpp>
#include <glbinding/gl/gl.h>
#include <glm/vec3.hpp>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/File.h>
#include <globjects/globjects.h>

int main(int argc, char* argv[])
{
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

    globjects::init(nullptr);

    auto program = globjects::Program::create();

    auto vertexShaderData = globjects::Shader::sourceFromFile("shaders/triangle.vert.glsl");
    auto fragmentShaderData = globjects::Shader::sourceFromFile("shaders/triangle.frag.glsl");

    auto vertexShader = globjects::Shader::create(gl::GL_VERTEX_SHADER);
    vertexShader->setSource(vertexShaderData.get());
    auto fragmentShader = globjects::Shader::create(gl::GL_FRAGMENT_SHADER);
    fragmentShader->setSource(fragmentShaderData.get());

    program->attach(vertexShader.get(), fragmentShader.get());

    const std::array<glm::vec3, 3> vertices{{glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f)}};

    auto buffer = globjects::Buffer::create();
    buffer->setData(vertices, gl::GL_STATIC_DRAW);

    auto vao = globjects::VertexArray::create();
    auto binding = vao->binding(1);
    binding->setAttribute(0);
    binding->setBuffer(buffer.get(), 0, sizeof(glm::vec3));
    binding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    vao->enable(0);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        program->use();
        vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 3);
        program->release();

        window.display();
    }

    return 0;
}