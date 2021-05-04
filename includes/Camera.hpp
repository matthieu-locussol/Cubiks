#ifndef CUBIKS_CAMERA_HPP
#define CUBIKS_CAMERA_HPP

class Camera {
public:
    Camera() = default;
    Camera(Camera&&) = default;
    Camera(const Camera&) = delete;

    Camera& operator=(Camera&&) = default;
    Camera& operator=(const Camera&) = delete;



    ~Camera() = default;

private:
};

#endif // CUBIKS_CAMERA_HPP
