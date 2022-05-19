#pragma once
#ifndef SKYBOX_H
#define	SKYBOX_H

#include "camera_25.h"
#include "skybox_technique_25.h"
#include "cubemap_texture_25.h"
#include "mesh_25.h"

class SkyBox
{
public:
    SkyBox(const Camera* pCamera, const PersProjInfo& p);

    ~SkyBox();
    //инициализация единожды
    bool Init(const string& Directory,
        const string& PosXFilename,
        const string& NegXFilename,
        const string& PosYFilename,
        const string& NegYFilename,
        const string& PosZFilename,
        const string& NegZFilename);

    void Render();//для использования во время приложения

private:
    SkyboxTechnique* m_pSkyboxTechnique;
    const Camera* m_pCamera;
    CubemapTexture* m_pCubemapTex;
    Mesh* m_pMesh;
    PersProjInfo m_persProjInfo;
};

#endif	/* SKYBOX_H */