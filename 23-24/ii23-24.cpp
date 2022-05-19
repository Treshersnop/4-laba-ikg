#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline_24.h"
#include "camera_24.h"
#include "texture_24.h"
#include "lighting_technique_24.h"
#include "glut_backend_24.h"
#include "util_24.h"
#include "mesh_24.h"
#include "shadow_map_fbo_24.h"
#include "shadow_map_technique_24.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

class Main : public ICallbacks
{
public:

    Main()
    {
        m_pLightingEffect = NULL;
        m_pShadowMapEffect = NULL;
        m_pGameCamera = NULL;
        m_pMesh = NULL;
        m_pQuad = NULL;
        m_scale = 0.0f;
        m_pGroundTex = NULL;

        m_spotLight.AmbientIntensity = 0.1f;
        m_spotLight.DiffuseIntensity = 0.9f;
        m_spotLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_spotLight.Attenuation.Linear = 0.01f;
        m_spotLight.Position = Vector3f(-20.0, 20.0, 1.0f);
        m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
        m_spotLight.Cutoff = 20.0f;
    }

    virtual ~Main()
    {
        SAFE_DELETE(m_pLightingEffect);
        SAFE_DELETE(m_pShadowMapEffect);
        SAFE_DELETE(m_pGameCamera);
        SAFE_DELETE(m_pMesh);
        SAFE_DELETE(m_pQuad);
        SAFE_DELETE(m_pGroundTex);
    }

    bool Init()
    {
        Vector3f Pos(3.0f, 8.0f, -10.0f);
        Vector3f Target(0.0f, -0.2f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
            return false;
        }

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

        m_pLightingEffect = new LightingTechnique();

        if (!m_pLightingEffect->Init()) //настройка части LightingTechnique
        {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pLightingEffect->Enable();
        m_pLightingEffect->SetSpotLights(1, &m_spotLight);
        m_pLightingEffect->SetTextureUnit(0);
        m_pLightingEffect->SetShadowMapTextureUnit(1);
        
        m_pShadowMapEffect = new ShadowMapTechnique();

        if (!m_pShadowMapEffect->Init()) {
            printf("Error initializing the shadow map technique\n");
            return false;
        }

        m_pQuad = new Mesh();

        if (!m_pQuad->LoadMesh("D:\\Content\\quad.obj")) {
            return false;
        }

        m_pGroundTex = new Texture(GL_TEXTURE_2D, "D:\\kotk.png");

        if (!m_pGroundTex->Load()) {
            return false;
        }

        m_pMesh = new Mesh();

        return m_pMesh->LoadMesh("D:\\Content\\phoenix_ugv.md2");
    }

    void Run()
    {
        GLUTBackendRun(this);
    }

    virtual void RenderSceneCB()
    {
        m_pGameCamera->OnRender();//камера
        m_scale += 0.5f;//коэффициент масштабирования, который используется для вращения меша

        ShadowMapPass();//проход для теней, перед проходом рендера
        RenderPass();

        glutSwapBuffers();
    }

    virtual void ShadowMapPass()
    {
        m_shadowMapFBO.BindForWriting();//привязка FBO

        glClear(GL_DEPTH_BUFFER_BIT); //очистка буфера глубины

        m_pShadowMapEffect->Enable();

        Pipeline p;
        p.Scale(0.1f, 0.1f, 0.1f);
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 5.0f);
        p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
        p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
        m_pShadowMapEffect->SetWVP(p.GetWVPTrans());
        m_pMesh->Render();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    /*Проход рендера начинается с очистки и буфера глубины и буфера цвета,
      заменяем метод теней на свет и привязываем карту теней для чтения в модуль текстур 1.
      Далее мы рендерим плоскость так, что бы она служила землей, на которую падает тень.*/
    virtual void RenderPass()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//очистка буфера глубины в начале нового кадра

        m_pLightingEffect->Enable();

        m_shadowMapFBO.BindForReading(GL_TEXTURE1);

        Pipeline p;
        p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
        p.Scale(10.0f, 10.0f, 10.0f);
        p.WorldPos(0.0f, 0.0f, 1.0f);
        p.Rotate(90.0f, 0.0f, 0.0f);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());

        m_pLightingEffect->SetWVP(p.GetWVPTrans());
        m_pLightingEffect->SetWorldMatrix(p.GetWorldTrans());
        p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
        m_pLightingEffect->SetLightWVP(p.GetWVPTrans());
        m_pLightingEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
        m_pGroundTex->Bind(GL_TEXTURE0);
        m_pQuad->Render();

        p.Scale(0.1f, 0.1f, 0.1f);
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 3.0f);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        m_pLightingEffect->SetWVP(p.GetWVPTrans());
        m_pLightingEffect->SetWorldMatrix(p.GetWorldTrans());
        p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
        m_pLightingEffect->SetLightWVP(p.GetWVPTrans());

        m_pMesh->Render();
    }

    virtual void IdleCB()
    {
        RenderSceneCB();
    }

    virtual void SpecialKeyboardCB(int Key, int x, int y)
    {
        m_pGameCamera->OnKeyboard(Key);
    }


    virtual void KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
        case 'q':
            glutLeaveMainLoop();
            break;
        }
    }


    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

private:

    LightingTechnique* m_pLightingEffect;
    ShadowMapTechnique* m_pShadowMapEffect;
    Camera* m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    Mesh* m_pMesh;
    Mesh* m_pQuad;
    ShadowMapFBO m_shadowMapFBO;
    Texture* m_pGroundTex;
};


int main(int argc, char** argv)
{
    Magick::InitializeMagick(nullptr);
    GLUTBackendInit(argc, argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Karimova tutorial 23-24")) {
        return 1;
    }

    Main* pApp = new Main();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;

    return 0;
}