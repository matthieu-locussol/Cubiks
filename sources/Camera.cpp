#include <Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_movementSpeed(DEFAULT_SPEED),
      m_mouseSensitivity(DEFAULT_SENSITIVITY),
      m_zoom(DEFAULT_ZOOM)
{
    m_position = position;
    m_worldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;

    this->updateVectors();
}

float Camera::getZoom() const
{
    return m_zoom;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::moveCamera(Movement direction, float delta)
{
    float velocity = m_movementSpeed * delta;

    switch (direction) {
    case FORWARD:
        m_position += m_front * velocity;
        break;
    case BACKWARD:
        m_position -= m_front * velocity;
        break;
    case LEFT:
        m_position -= m_right * velocity;
        break;
    case RIGHT:
        m_position += m_right * velocity;
        break;
    }
}

void Camera::lookCamera(glm::vec2 offset, bool pitchConstraint)
{
    offset *= m_mouseSensitivity;

    m_yaw += offset.x;
    m_pitch += offset.y;

    if (pitchConstraint) {
        m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
    }

    this->updateVectors();
}

void Camera::zoomCamera(glm::vec2 offset)
{
    m_zoom -= offset.y;
    m_zoom = glm::clamp(m_zoom, 1.0f, 45.0f);
}

void Camera::updateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}