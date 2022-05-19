#ifndef SHADOWMAPFBO_H
#define SHADOWMAPFBO_H

#include <GL/glew.h>

class ShadowMapFBO //объект буфера кадра
    //интерфейс для FBO, который будет использован для наложения теней
{
public:
    ShadowMapFBO();

    ~ShadowMapFBO();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();

    void BindForReading(GLenum TextureUnit);

private:
    GLuint m_fbo;//текущий FBO
    GLuint m_shadowMap;//казатель на текстуру
};

#endif /* SHADOWMAPFBO_H */