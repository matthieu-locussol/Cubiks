#ifndef CUBIKS_CAMERA_HPP
#define CUBIKS_CAMERA_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    constexpr static float DEFAULT_YAW = -90.0f;
    constexpr static float DEFAULT_PITCH = 0.0f;
    constexpr static float DEFAULT_SPEED = 2.5f;
    constexpr static float DEFAULT_SENSITIVITY = 0.1f;
    constexpr static float DEFAULT_ZOOM = 45.0f;

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH);
    Camera(Camera&&) = default;
    Camera(const Camera&) = delete;

    Camera& operator=(Camera&&) = default;
    Camera& operator=(const Camera&) = delete;

    float getZoom() const;
    glm::mat4 getViewMatrix() const;

    void moveCamera(Movement direction, float delta);
    void lookCamera(glm::vec2 offset, bool pitchConstraint = true);
    void zoomCamera(glm::vec2 offset);

    ~Camera() = default;

private:
    void updateVectors();

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;

    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;
};

#endif // CUBIKS_CAMERA_HPP
