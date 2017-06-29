//
//  Test.cpp
//  Texture
//
//  Created by zhufu on 2017/3/28.
//
//

#include "Test.h"

Test* Test::create()
{
    Test* test = new(std::nothrow) Test();
    if(test && test->init())
    {
        test->autorelease();
        return test;
    }
    else
    {
        delete test;
        test = nullptr;
        return nullptr;
    }
}

bool Test::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    initCommand();
    loadShader();
    loadTexture();
    loadRectangle();
    
    return true;
}

void Test::flipVertical(int width, int height, unsigned char* arr)
{
    int index = 0, f_index, cycle=height/2;
    char buf;
    for (int i = 0; i < cycle; i++)
    {
        for (int j = 0; j < width*Test::Format_RGBA; j++)
        {
            //当前像素
            index = i*width*Test::Format_RGBA + j;
            //需要交换的像素
            f_index = (height - 1 - i)*width*Test::Format_RGBA + j;
            //缓存当前像素
            buf = arr[index];
            //交换像素
            arr[index] = arr[f_index];
            //交换回像素
            arr[f_index] = buf;
        }
    }
}


void Test::loadShader()
{
    _glProgram = new GLProgram();
    _glProgram->initWithFilenames("shader/myVertexShader.vsh", "shader/myFragmentShader.fsh");
    _glProgram->link();
}

void Test::loadTexture()
{
    Image *image = new Image;
    std::string imagePath = FileUtils::getInstance()->fullPathForFilename("HelloWorld.png");
    image->initWithImageFile(imagePath);
    unsigned char *imageData = image->getData();
    int width = image->getWidth();
    int height = image->getHeight();
    flipVertical(width, height, imageData);
    
    CCLOG("width %d", width);
    
    //别忘了释放image内存
    //    CC_SAFE_DELETE(image);
    
    glGenTextures(1, &TEXTUREID);
    glBindTexture(GL_TEXTURE_2D, TEXTUREID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Test::loadRectangle()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    //make and bind VBO;
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    //put the rectangle vertices(XYZ) and texture coordinates (UV) into the vbo
    GLfloat textureData[] = {
        //  X     Y     Z       U     V
        -1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f,-1.0f, 0.0f,   0.0f, 0.0f,
        1.0f,-1.0f , 0.0f,   1.0f, 0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
    
    //connect the xyz to the "vert" arrribute of the vertex shader
    glEnableVertexAttribArray(_glProgram->getAttribLocation("vert"));
    glVertexAttribPointer(_glProgram->getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    
    glEnableVertexAttribArray(_glProgram->getAttribLocation("vertTexCoord"));
    glVertexAttribPointer(_glProgram->getAttribLocation("vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3*sizeof(GLfloat)));
}

void Test::initCommand()
{
    _command.init(getLocalZOrder());
    _command.func = CC_CALLBACK_0(Test::onDraw, this);
}

void Test::draw(Renderer *renderer, const Mat4 &transform, uint32_t platform)
{
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void Test::onDraw()
{
    //clear everything
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //bind the program (the shaders)
    _glProgram->use();
    
    //bind the texture and set the "tex" uniform in the fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TEXTUREID);
    GLuint tex = glGetUniformLocation(_glProgram->getProgram(), "tex");
    glUniform1i(tex, 0);
    
    //bind the vao
    glBindVertexArray(_vao);
    
    //draw the vao
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
