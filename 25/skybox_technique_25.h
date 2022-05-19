#pragma once
#ifndef SKYBOX_TECHNIQUE_H
#define	SKYBOX_TECHNIQUE_H

#include "technique_25.h"
#include "math_3d_25.h"


class SkyboxTechnique : public Technique {
public:

    SkyboxTechnique();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);//������� WVP ��� �������������� ���� ��� �����
    void SetTextureUnit(unsigned int TextureUnit);//��������, ������� ����� �������������

    virtual ~SkyboxTechnique();

private:

    GLuint m_WVPLocation;
    GLuint m_textureLocation;
};


#endif	/* SKYBOX_TECHNIQUE_H */