#pragma once

#include <vector>

#include "components/simple_scene.h"
//#include "lab_m1/Tema2/tema2_camera.h"
#include "components/transform.h"



namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         struct ViewportArea
         {
             ViewportArea() : x(0), y(0), width(1), height(1) {}
             ViewportArea(int x, int y, int width, int height)
                 : x(x), y(y), width(width), height(height) {}
             int x;
             int y;
             int width;
             int height;
         };

        Tema2();
        ~Tema2();

        void Init() override;

        Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
        bool Collision(glm::vec3 dronePos, glm::vec3 objectPos, float droneRadius, float objectRadius, float objectHeight);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderScene(float deltaTimeSeconds);

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;



     protected:
         float angularStep;
		 float translateX, translateY, translateZ;
         float droneRotationAngle;
         std::vector<glm::vec3> treePositions;
         std::vector<glm::vec3> cubePositions;
         std::vector<glm::vec3> destPositions;
         std::vector<glm::vec3> cloudPositions;
         int hp, count, previousCount;
         int ok, ok2;
		 int hpDrone;
         int alertHp;
         ViewportArea miniViewportArea;
		 gfxc::Camera* camera;
		 gfxc::Camera* minimapCamera;
    };
}   // namespace m1
