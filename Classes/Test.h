//
//  Test.h
//  Texture
//
//  Created by zhufu on 2017/3/28.
//
//

#ifndef Test_h
#define Test_h

#include <stdio.h>
USING_NS_CC;

class Test : public Node
{
public:
    
    enum Format {
        Format_Grayscale = 1, /**< one channel: grayscale */
        Format_GrayscaleAlpha = 2, /**< two channels: grayscale and alpha */
        Format_RGB = 3, /**< three channels: red, green, blue */
        Format_RGBA = 4 /**< four channels: red, green, blue, alpha */
    };
    
    static Test* create();
    virtual bool init() override;
    virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t platform) override;
    void onDraw();
    
    void loadShader();
    void loadTexture();
    void loadRectangle();
    void initCommand();
    void flipVertical(int width, int height, unsigned char* arr);
    
private:
    GLuint TEXTUREID;
    GLProgram* _glProgram;
    GLuint _vao;
    CustomCommand _command;
};

#endif /* Test_h */
