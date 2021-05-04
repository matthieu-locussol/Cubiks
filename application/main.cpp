#include <SFML/Window.hpp>
#include <glbinding/gl/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/File.h>
#include <globjects/Texture.h>
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

    auto vertexShaderData = globjects::Shader::sourceFromFile("shaders/texture.vert.glsl");
    auto fragmentShaderData = globjects::Shader::sourceFromFile("shaders/texture.frag.glsl");

    auto vertexShader = globjects::Shader::create(gl::GL_VERTEX_SHADER);
    vertexShader->setSource(vertexShaderData.get());
    auto fragmentShader = globjects::Shader::create(gl::GL_FRAGMENT_SHADER);
    fragmentShader->setSource(fragmentShaderData.get());

    program->attach(vertexShader.get(), fragmentShader.get());

    const std::array<float, 180> vertices{
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    auto buffer = globjects::Buffer::create();
    buffer->setData(vertices, gl::GL_STATIC_DRAW);

    auto vao = globjects::VertexArray::create();

    auto positionAttribute = vao->binding(0);
    positionAttribute->setAttribute(0);
    positionAttribute->setBuffer(buffer.get(), 0, 5 * sizeof(float));
    positionAttribute->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);

    auto textureAttribute = vao->binding(1);
    textureAttribute->setAttribute(1);
    textureAttribute->setBuffer(buffer.get(), 0, sizeof(glm::vec3));
    textureAttribute->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(float));

    vao->enable(0);
    vao->enable(1);



    auto texture = globjects::Texture::create(gl::GL_TEXTURE_2D);
    texture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
    texture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
    texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
    texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);

    texture->image2D(0, gl::GL_RGBA8, glm::ivec2(512), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);
    texture->clearImage(0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, glm::ivec4(255, 255, 255, 255));
    texture->generateMipmap();



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