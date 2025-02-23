#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        float angularTurret;

        float translateX2, translateY2;
        float scaleX2, scaleY2;
        float angularStep2;
        float angularTurret2;


        std::vector<float> vec;
        float t;

        struct Bullet{
            glm::vec2 position;
            glm::vec2 velocity;
            bool ok;
        };
        
        Bullet bullets[20];

        int hp1, hp2;

        float cloudX, cloudX2, cloudX3;

        float smokelife1, smokeY1, smokelife2, smokeY2;
    };
}   // namespace m1
