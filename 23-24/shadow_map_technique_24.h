#ifndef SHADOW_MAP_TECHNIQUE_H
#define	SHADOW_MAP_TECHNIQUE_H

#include "technique_24.h"
#include "math_3d_24.h"

class ShadowMapTechnique : public Technique {

public:
    ShadowMapTechnique();
    virtual bool Init();
    void SetWVP(const Matrix4f& WVP);
    void SetTextureUnit(unsigned int TextureUnit);

private:

    GLuint m_WVPLocation;
    GLuint m_textureLocation;
};

#endif	/* SHADOW_MAP_TECHNIQUE_H */