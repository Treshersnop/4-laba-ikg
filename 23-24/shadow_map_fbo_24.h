#ifndef SHADOWMAPFBO_H
#define SHADOWMAPFBO_H

#include <GL/glew.h>

class ShadowMapFBO //������ ������ �����
    //��������� ��� FBO, ������� ����� ����������� ��� ��������� �����
{
public:
    ShadowMapFBO();

    ~ShadowMapFBO();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();

    void BindForReading(GLenum TextureUnit);

private:
    GLuint m_fbo;//������� FBO
    GLuint m_shadowMap;//�������� �� ��������
};

#endif /* SHADOWMAPFBO_H */