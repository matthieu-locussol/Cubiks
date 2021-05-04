#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <SFML/Window.hpp>
#include <glbinding/gl/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <globjects/Texture.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/File.h>
#include <globjects/globjects.h>
#include <iostream>

#include <Camera.hpp>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
float delta = 0.0f;
float lastFrame = 0.0f;

int main(int argc, char* argv[])
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Cubiks", sf::Style::Default,
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
        // TOP FACE
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        // BOTTOM FACE
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        // FRONT FACE
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        // BACK FACE
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        // RIGHT FACE
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        // LEFT FACE
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f};

    const std::array<int, 6> indices = {
        0, 1, 3,
        1, 2, 3};

    auto buffer = globjects::Buffer::create();
    buffer->bind(gl::GL_ARRAY_BUFFER);
    buffer->setData(vertices, gl::GL_STATIC_DRAW);

    auto vao = globjects::VertexArray::create();

    auto positionAttribute = vao->binding(0);
    positionAttribute->setAttribute(0);
    positionAttribute->setBuffer(buffer.get(), 0, 5 * sizeof(float));
    positionAttribute->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);

    auto textureAttribute = vao->binding(1);
    textureAttribute->setAttribute(1);
    textureAttribute->setBuffer(buffer.get(), 0, 5 * sizeof(float));
    textureAttribute->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(float));

    vao->enable(0);
    vao->enable(1);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/cube.png", &width, &height, &nrChannels, 0);

    auto texture = globjects::Texture::create(gl::GL_TEXTURE_2D);
    texture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
    texture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
    texture->image2D(0, gl::GL_RGBA, glm::ivec2(width, height), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data);
    texture->generateMipmap();

    stbi_image_free(data);

    gl::glEnable(gl::GL_DEPTH_TEST);

    sf::Clock clock;

    while (window.isOpen()) {
        float currentFrame = clock.getElapsedTime().asSeconds();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved) {
                if (firstMouse) {
                    lastX = event.mouseMove.x;
                    lastY = event.mouseMove.y;
                    firstMouse = false;
                }

                glm::vec2 offset(event.mouseMove.x - lastX, lastY - event.mouseMove.y);

                lastX = event.mouseMove.x;
                lastY = event.mouseMove.y;

                camera.lookCamera(offset, false);
            }
            else if (event.type == sf::Event::MouseWheelMoved) {
                glm::vec2 offset(event.mouseWheel.delta, event.mouseWheel.delta);
                camera.zoomCamera(offset);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Z) {
                    camera.moveCamera(Camera::FORWARD, delta);
                }
                else if (event.key.code == sf::Keyboard::Q) {
                    camera.moveCamera(Camera::LEFT, delta);
                }
                else if (event.key.code == sf::Keyboard::S) {
                    camera.moveCamera(Camera::BACKWARD, delta);
                }
                else if (event.key.code == sf::Keyboard::D) {
                    camera.moveCamera(Camera::RIGHT, delta);
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        program->use();

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

        program->setUniform("view", view);
        program->setUniform("model", model);
        program->setUniform("projection", projection);

        vao->drawArrays(gl::GL_TRIANGLES, 0, 36);

        program->release();

        window.display();
    }

    return 0;
}