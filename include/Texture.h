#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:
    explicit Texture(const char* path);
    ~Texture();

    void bind(unsigned int textureUnit) const;
    bool isValid() const;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

private:
    unsigned int id = 0;
    bool valid = false;
};

#endif