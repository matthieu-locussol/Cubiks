#ifndef CUBIKS_BLOCK_HPP
#define CUBIKS_BLOCK_HPP

class Block {
public:
    Block() = default;
    Block(Block&&) = default;
    Block(const Block&) = delete;

    Block& operator=(Block&&) = default;
    Block& operator=(const Block&) = delete;



    ~Block() = default;

    enum FaceType {
        FACE_TOP,
        FACE_BOTTOM,
        FACE_FRONT,
        FACE_BACK,
        FACE_RIGHT,
        FACE_LEFT,

        FACES_NUMBER
    };

private:

};

#endif // CUBIKS_BLOCK_HPP
