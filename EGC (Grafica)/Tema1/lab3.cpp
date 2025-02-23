#include "lab_m1/Tema1/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 1;

    // Initialize tx and ty (the translation steps)
    translateX = 150;
    translateY = 0;
	translateX2 = 900;
	translateY2 = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;
	scaleX2 = 1;
	scaleY2 = 1;

    // Initialize angular
    angularStep = 0;
    angularTurret = 0;
	angularStep2 = 0;
	angularTurret2 = 0;

	hp1 = 3;
	hp2 = 3;

    cloudX = 0;
    cloudX2 = 0;
    cloudX3 = 0;

    smokelife1 = 3.0f;
	smokeY1 = 0;
    smokelife2 = 3.0f;
    smokeY2 = 0;
    
	for (int i = 0; i < 20; i++) {
		bullets[i].position = glm::vec2(0, 0);
		bullets[i].velocity = glm::vec2(0, 0);
		bullets[i].ok = false;
	}

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(50.0 / 255.0, 169.0 / 255.0, 50.0 / 255.0), true);
    AddMeshToList(square1);

	for (float x = 0; x < 1281; x += 1.0f) {
		vec.push_back(35 * sin((3.14 / 400) * x) + 45 * sin((3.14 / 300) * x) + 60 * sin((3.14 / 220) * x) + 20 * sin((3.14 / 100) * x) + 200);
	}
    
    {
        vector<VertexFormat> verticesTank
        {
            VertexFormat(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.7f, 0.5f)),  //1
            VertexFormat(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.7f, 0.5f)),
            VertexFormat(glm::vec3(0.7f, 0.4f, 0.0f), glm::vec3(0.8f, 0.7f, 0.5f)), 
            VertexFormat(glm::vec3(-0.7f, 0.4f, 0.0f), glm::vec3(0.8f, 0.7f, 0.5f)),

            VertexFormat(glm::vec3(-0.6f, -0.2f, 0.0f), glm::vec3(0.4f, 0.3f, 0.2f)), //2
            VertexFormat(glm::vec3(0.6f, -0.2f, 0.0f), glm::vec3(0.4f, 0.3f, 0.2f)),
            VertexFormat(glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(0.4f, 0.3f, 0.2f)),
            VertexFormat(glm::vec3(-0.8f, 0.0f, 0.0f), glm::vec3(0.4f, 0.3f, 0.2f)),

        };

        vector<unsigned int> indicesTank =
        {
            0, 1, 2,
            0, 2, 3,

            4, 5, 6,
            4, 6, 7
        };

        int startIndex = verticesTank.size();
        for (int i = 0; i <= 30; ++i) {
            float angle = 2.0f * M_PI * i / 30;
            float radius = 0.3f;
			float x = radius * cos(angle);
			float y = 0.4f + radius * sin(angle);
            verticesTank.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.8f, 0.7f, 0.5f)));
        }
        for (int i = 1; i < 30; ++i) {
			indicesTank.push_back(startIndex);
			indicesTank.push_back(startIndex + i);
			indicesTank.push_back(startIndex + i + 1);
        }

        meshes["tank"] = new Mesh("tank");
		meshes["tank"]->InitFromData(verticesTank, indicesTank);
    }

    {
        vector<VertexFormat> verticesTank2
        {
			VertexFormat(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.0f, 0.0f)),  //1
			VertexFormat(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.0f, 0.0f)),
			VertexFormat(glm::vec3(0.7f, 0.4f, 0.0f), glm::vec3(0.6f, 0.0f, 0.0f)), 
			VertexFormat(glm::vec3(-0.7f, 0.4f, 0.0f), glm::vec3(0.6f, 0.0f, 0.0f)),

			VertexFormat(glm::vec3(-0.6f, -0.2f, 0.0f), glm::vec3(0.4f, 0.0f, 0.0f)), //2
			VertexFormat(glm::vec3(0.6f, -0.2f, 0.0f), glm::vec3(0.4f, 0.0f, 0.0f)),
			VertexFormat(glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(0.4f, 0.0f, 0.0f)),
			VertexFormat(glm::vec3(-0.8f, 0.0f, 0.0f), glm::vec3(0.4f, 0.0f, 0.0f)),
        };

		vector<unsigned int> indicesTank2 =
		{
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7
		};

		int startIndex = verticesTank2.size();
        for (int i = 0; i <= 30; ++i) {
            float angle = 2.0f * M_PI * i / 30;
            float radius = 0.3f;
            float x = radius * cos(angle);
            float y = 0.4f + radius * sin(angle);
            verticesTank2.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.6f, 0.0f, 0.0f)));
        }
		for (int i = 1; i < 30; ++i) {
			indicesTank2.push_back(startIndex);
			indicesTank2.push_back(startIndex + i);
			indicesTank2.push_back(startIndex + i + 1);
		}

		meshes["tank2"] = new Mesh("tank2");
		meshes["tank2"]->InitFromData(verticesTank2, indicesTank2);
    }

    {
        vector<VertexFormat> verticesTurret
        {
			VertexFormat(glm::vec3(0.1f, 0.4f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
			VertexFormat(glm::vec3(0.7f, 0.4f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
			VertexFormat(glm::vec3(0.7f, 0.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
			VertexFormat(glm::vec3(0.1f, 0.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
		};

		vector<unsigned int> indicesTurret =
		{
			0, 1, 2,
			0, 2, 3
		};

		meshes["turret"] = new Mesh("turret");
		meshes["turret"]->InitFromData(verticesTurret, indicesTurret);
    }

    {
        vector<VertexFormat> verticesTurret2
        {
            VertexFormat(glm::vec3(-0.7f, 0.4f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
            VertexFormat(glm::vec3(-0.1f, 0.4f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
            VertexFormat(glm::vec3(-0.1f, 0.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
            VertexFormat(glm::vec3(-0.7f, 0.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)),
        };

        vector<unsigned int> indicesTurret2 =
        {
            0, 1, 2,
            0, 2, 3
        };

        meshes["turret2"] = new Mesh("turret2");
        meshes["turret2"]->InitFromData(verticesTurret2, indicesTurret2);
    }

    {
        vector<VertexFormat> verticesBullet;
		for (int i = 0; i <= 20; ++i) {
			float angle = 2.0f * M_PI * i / 20;
            float radius = 0.1f;
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			verticesBullet.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
		}

        vector<unsigned int> indicesBullet;
		for (int i = 1; i < 20; ++i) {
			indicesBullet.push_back(0);
			indicesBullet.push_back(i);
			indicesBullet.push_back(i + 1);
		}

        meshes["bullet"] = new Mesh("bullet");
        meshes["bullet"]->InitFromData(verticesBullet, indicesBullet);
    }

    {
        vector<VertexFormat> verticesSun;
        for (int i = 0; i <= 60; ++i) {
            float angle = 2.0f * M_PI * i / 60;
            float radius = 60.0f;
            float x = 1280 - radius * cos(angle);
            float y = 720 - radius * sin(angle);
            verticesSun.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(1.0f, 1.0f, 0.35f)));
        }

        vector<unsigned int> indicesSun;
        for (int i = 1; i < 60; ++i) {
            indicesSun.push_back(0);
            indicesSun.push_back(i);
            indicesSun.push_back(i + 1);
        }

        meshes["sun"] = new Mesh("sun");
        meshes["sun"]->InitFromData(verticesSun, indicesSun);
    }

    {
        vector<VertexFormat> verticesCloud;
        vector<unsigned int> indicesCloud;
        for (int i = 0; i <= 100; ++i) {
            float angle = 2.0f * M_PI * i / 100;
            float radius = 30.0f;
            float x = 4 + radius * cos(angle);
            float y = 23 + radius * sin(angle);
            verticesCloud.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
        }
		for (int i = 1; i < 100; ++i) {
			indicesCloud.push_back(0);
			indicesCloud.push_back(i);
			indicesCloud.push_back(i + 1);
		}
        int startIndex = verticesCloud.size();
        for (int i = 0; i <= 100; ++i) {
            float angle = 2.0f * M_PI * i / 100;
            float radius = 30.0f;
            float x = -4 + radius * cos(angle);
            float y = -20 + radius * sin(angle);
            verticesCloud.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
        }
        for (int i = 1; i < 100; ++i) {
			indicesCloud.push_back(startIndex);
			indicesCloud.push_back(startIndex + i);
			indicesCloud.push_back(startIndex + i + 1);
        }
        int startIndex2 = verticesCloud.size();
        for (int i = 0; i <= 100; ++i) {
            float angle = 2.0f * M_PI * i / 100;
            float radius = 30.0f;
            float x = 33 + radius * cos(angle);
            float y = radius * sin(angle);
            verticesCloud.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
        }
        for (int i = 1; i < 100; ++i) {
            indicesCloud.push_back(startIndex2);
			indicesCloud.push_back(startIndex2 + i);
			indicesCloud.push_back(startIndex2 + i + 1);
        }
		int startIndex3 = verticesCloud.size();
        for (int i = 0; i <= 100; ++i) {
            float angle = 2.0f * M_PI * i / 100;
            float radius = 30.0f;
            float x = -33 + radius * cos(angle);
            float y = radius * sin(angle);
            verticesCloud.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
        }
        for (int i = 1; i < 100; ++i) {
            indicesCloud.push_back(startIndex3);
			indicesCloud.push_back(startIndex3 + i);
			indicesCloud.push_back(startIndex3 + i + 1);
        }

		meshes["cloud"] = new Mesh("cloud");
		meshes["cloud"]->InitFromData(verticesCloud, indicesCloud);
    }

    {
        vector<VertexFormat> verticesWireframe
        {
			VertexFormat(glm::vec3(-40, -5, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
			VertexFormat(glm::vec3(40, -5, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
			VertexFormat(glm::vec3(40, 5, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
			VertexFormat(glm::vec3(-40, 5, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
        };

		vector<unsigned int> indicesWireframe =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0
		};

		meshes["wireframe"] = new Mesh("wireframe");
		meshes["wireframe"]->SetDrawMode(GL_LINES);
		meshes["wireframe"]->InitFromData(verticesWireframe, indicesWireframe);
    }

    {
        vector<VertexFormat> verticesLife
        {
            VertexFormat(glm::vec3(-40, -5, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
			VertexFormat(glm::vec3(40, -5, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
			VertexFormat(glm::vec3(40, 5, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
			VertexFormat(glm::vec3(-40, 5, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
        };

		vector<unsigned int> indicesLife =
		{
			0, 1, 2, 
			2, 3, 0
		};

		meshes["life"] = new Mesh("life");
		meshes["life"]->InitFromData(verticesLife, indicesLife);
    }

    {
		vector<VertexFormat> verticesMilitaryBase
		{
			VertexFormat(glm::vec3(-0.1f, -0.2f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f)),
			VertexFormat(glm::vec3(0.5f, -0.2f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f)),
			VertexFormat(glm::vec3(0.5f, 0.2f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f)),
			VertexFormat(glm::vec3(-0.1f, 0.2f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f)),
		};
        
		vector<unsigned int> indicesMilitaryBase =
		{
			0, 1, 2,
			0, 2, 3
		};

		verticesMilitaryBase.push_back(VertexFormat(glm::vec3(0.6f, -0.2f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f)));
		verticesMilitaryBase.push_back(VertexFormat(glm::vec3(0.7f, -0.2f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f)));
		verticesMilitaryBase.push_back(VertexFormat(glm::vec3(0.7f, 0.4f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f)));
		verticesMilitaryBase.push_back(VertexFormat(glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f)));

		indicesMilitaryBase.push_back(4);
		indicesMilitaryBase.push_back(5);
		indicesMilitaryBase.push_back(6);
		indicesMilitaryBase.push_back(4);
		indicesMilitaryBase.push_back(6);
		indicesMilitaryBase.push_back(7);

		verticesMilitaryBase.push_back(VertexFormat(glm::vec3(0.55f, 0.4f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f)));
		verticesMilitaryBase.push_back(VertexFormat(glm::vec3(0.75f, 0.4f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f)));
		verticesMilitaryBase.push_back(VertexFormat(glm::vec3(0.75f, 0.45f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f)));
		verticesMilitaryBase.push_back(VertexFormat(glm::vec3(0.55f, 0.45f, 0.0f), glm::vec3(0.6f, 0.6f, 0.6f)));

		indicesMilitaryBase.push_back(8);
		indicesMilitaryBase.push_back(9);
		indicesMilitaryBase.push_back(10);
		indicesMilitaryBase.push_back(8);
		indicesMilitaryBase.push_back(10);
		indicesMilitaryBase.push_back(11);

		meshes["militaryBase"] = new Mesh("militaryBase");
		meshes["militaryBase"]->InitFromData(verticesMilitaryBase, indicesMilitaryBase);
    }

    {
        vector<VertexFormat> verticesPillar
        {
			VertexFormat(glm::vec3(0.8f, -0.2f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f)),
			VertexFormat(glm::vec3(0.8f, 0.3f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f)),
		};

		vector<unsigned int> indicesPillar =
		{
			0, 1
		};

		meshes["pillar"] = new Mesh("pillar");
		meshes["pillar"]->SetDrawMode(GL_LINES);
		meshes["pillar"]->InitFromData(verticesPillar, indicesPillar);
    }

    {
        vector<VertexFormat> verticesFlagRed
        {
            VertexFormat(glm::vec3(0.8f, 0.2f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(0.9f, 0.2f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexFormat(glm::vec3(0.9f, 0.25f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			VertexFormat(glm::vec3(0.8f, 0.25f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        };

		vector<unsigned int> indicesFlagRed =
		{
			0, 1, 2,
			2, 3, 0
		};

		meshes["flagred"] = new Mesh("flagred");
		meshes["flagred"]->InitFromData(verticesFlagRed, indicesFlagRed);
    }

    {
        vector<VertexFormat> verticesFlagBlue
        {
            VertexFormat(glm::vec3(0.8f, 0.2f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            VertexFormat(glm::vec3(0.9f, 0.2f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            VertexFormat(glm::vec3(0.9f, 0.25f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            VertexFormat(glm::vec3(0.8f, 0.25f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        };

        vector<unsigned int> indicesFlagBlue =
        {
            0, 1, 2,
            2, 3, 0
        };

        meshes["flagblue"] = new Mesh("flagblue");
		meshes["flagblue"]->InitFromData(verticesFlagBlue, indicesFlagBlue);
    }

    {
        vector<VertexFormat> verticesSmoke;
        vector<unsigned int> indicesSmoke;
		for (int i = 0; i <= 100; ++i) {
			float angle = 2.0f * M_PI * i / 100;
			float radius = 10.0f;
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			verticesSmoke.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
		}
        for (int i = 1; i < 100; ++i) {
            indicesSmoke.push_back(0);
            indicesSmoke.push_back(i);
            indicesSmoke.push_back(i + 1);
        }

		meshes["smoke"] = new Mesh("smoke");
		meshes["smoke"]->InitFromData(verticesSmoke, indicesSmoke);
    }

    {
        vector<VertexFormat> verticesSmoke2;
        vector<unsigned int> indicesSmoke2;
        for (int i = 0; i <= 100; ++i) {
            float angle = 2.0f * M_PI * i / 100;
            float radius = 10.0f;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
			verticesSmoke2.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0, 0, 0)));
        }
        for (int i = 1; i < 100; ++i) {
            indicesSmoke2.push_back(0);
            indicesSmoke2.push_back(i);
            indicesSmoke2.push_back(i + 1);
        }

        meshes["smoke2"] = new Mesh("smoke2");
        meshes["smoke2"]->InitFromData(verticesSmoke2, indicesSmoke2);
    }

    {
        vector<VertexFormat> verticesSmoke3;
        vector<unsigned int> indicesSmoke3;
        for (int i = 0; i <= 100; ++i) {
            float angle = 2.0f * M_PI * i / 100;
            float radius = 10.0f;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            verticesSmoke3.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(0.7f, 0, 0)));
        }
        for (int i = 1; i < 100; ++i) {
            indicesSmoke3.push_back(0);
            indicesSmoke3.push_back(i);
            indicesSmoke3.push_back(i + 1);
        }

        meshes["smoke3"] = new Mesh("smoke3");
        meshes["smoke3"]->InitFromData(verticesSmoke3, indicesSmoke3);
    }

    {
        vector<VertexFormat> verticesSmoke4;
        vector<unsigned int> indicesSmoke4;
        for (int i = 0; i <= 100; ++i) {
            float angle = 2.0f * M_PI * i / 100;
            float radius = 10.0f;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            verticesSmoke4.push_back(VertexFormat(glm::vec3(x, y, 0.0f), glm::vec3(255.0f / 255.0f, 165.0f / 255.0f, 0.0f / 255.0f)));
        }
        for (int i = 1; i < 100; ++i) {
            indicesSmoke4.push_back(0);
            indicesSmoke4.push_back(i);
            indicesSmoke4.push_back(i + 1);
        }

        meshes["smoke4"] = new Mesh("smoke4");
        meshes["smoke4"]->InitFromData(verticesSmoke4, indicesSmoke4);
    }
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
	//environment
	int length = vec.size();
    for (int index = 0; index < length - 1; index++) {
		float y1 = vec[index];
		float y2 = vec[index + 1];
        float e = 1.0f;
        if (index > 0 && vec[index] - vec[index - 1] > 1.5f) {
            if (vec[index] > vec[index - 1]) {
                vec[index] -= e;
                vec[index - 1] += e;
            }
            else {
                vec[index - 1] -= e;
                vec[index] += e;
            }
        }
        modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(index, y2);
		modelMatrix *= transform2D::Forfecare((y2 - y1) / 1);
		modelMatrix *= transform2D::Scale(1, max(y1, y2));
		modelMatrix *= transform2D::Translate(0, -1);
		RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
    cloudX += deltaTimeSeconds * 40;
    if (cloudX > 1150)
        cloudX = -250;
    modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(200 + cloudX, 500);
	modelMatrix *= transform2D::Scale(1.0f, 0.8f);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    cloudX2 += deltaTimeSeconds * 65;
    if (cloudX2 > 850)
        cloudX2 = -550;
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500 + cloudX2, 650);
    modelMatrix *= transform2D::Scale(1.0f, 0.8f);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    cloudX3 += deltaTimeSeconds * 40;
    if (cloudX3 > 550)
        cloudX3 = -850;
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(800 + cloudX3, 550);
    modelMatrix *= transform2D::Scale(1.0f, 0.8f);
    RenderMesh2D(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["sun"], shaders["VertexColor"], glm::mat3(1));


    //tank1
    if (hp1 > 0) {
        int index = static_cast<int>(translateX);
        if (index >= 0 && index < vec.size()) {
            translateY = vec[index];
            float Yv = vec[index + 1] - vec[index];
            angularStep = atan2(Yv, 1);
        }
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Scale(50, 50);
        modelMatrix *= transform2D::Translate(0, 0.17f);
        RenderMesh2D(meshes["tank"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateX, translateY);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Scale(50, 50);
        modelMatrix *= transform2D::Translate(0, 0.17f);
        modelMatrix *= transform2D::Translate(0, 0.4f);
        modelMatrix *= transform2D::Rotate(angularTurret);
        modelMatrix *= transform2D::Translate(0, -0.4f);
        RenderMesh2D(meshes["turret"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(translateX, translateY + 65);
        RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], modelMatrix);
		float scaleLife = hp1 / 3.0f;
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(translateX, translateY + 65);
        modelMatrix *= transform2D::Translate(13.2f * (hp1 - 3.0f), 0);
        modelMatrix *= transform2D::Scale(scaleLife, 1.0f);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
	}
	else {
		if (hp1 == 0 && smokelife1 > 0) {
			smokelife1 -= deltaTimeSeconds;
			modelMatrix = glm::mat3(1);
            smokeY1 += deltaTimeSeconds * 2;
			modelMatrix *= transform2D::Translate(translateX, translateY);
			modelMatrix *= transform2D::Translate(0, smokeY1);
			modelMatrix *= transform2D::Scale(1.0f, 1.0f);
			RenderMesh2D(meshes["smoke"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            smokeY1 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX, translateY);
            modelMatrix *= transform2D::Translate(0, smokeY1);
            modelMatrix *= transform2D::Translate(10, 10);
            modelMatrix *= transform2D::Scale(0.8f, 0.8f);
			RenderMesh2D(meshes["smoke2"], shaders["VertexColor"], modelMatrix);
			modelMatrix = glm::mat3(1);
			smokeY1 += deltaTimeSeconds * 2;
			modelMatrix *= transform2D::Translate(translateX, translateY);
			modelMatrix *= transform2D::Translate(0, smokeY1);
			modelMatrix *= transform2D::Translate(-8, 10);
			modelMatrix *= transform2D::Scale(0.6f, 0.6f);
			RenderMesh2D(meshes["smoke3"], shaders["VertexColor"], modelMatrix);
			modelMatrix = glm::mat3(1);
			smokeY1 += deltaTimeSeconds * 2;
			modelMatrix *= transform2D::Translate(translateX, translateY);
			modelMatrix *= transform2D::Translate(0, smokeY1);
			modelMatrix *= transform2D::Translate(0, 20);
			modelMatrix *= transform2D::Scale(0.4f, 0.4f);
			RenderMesh2D(meshes["smoke4"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            smokeY1 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX, translateY);
            modelMatrix *= transform2D::Translate(0, smokeY1);
            modelMatrix *= transform2D::Translate(-5, -20);
            modelMatrix *= transform2D::Scale(1.2f, 1.2f);
            RenderMesh2D(meshes["smoke2"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            smokeY1 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX, translateY);
            modelMatrix *= transform2D::Translate(0, smokeY1);
            modelMatrix *= transform2D::Translate(4, -20);
            modelMatrix *= transform2D::Scale(1.4f, 1.4f);
            RenderMesh2D(meshes["smoke4"], shaders["VertexColor"], modelMatrix);
		}
	}


    //tank2
    if (hp2 > 0) {
        int index2 = static_cast<int>(translateX2);
        if (index2 >= 0 && index2 < vec.size()) {
            translateY2 = vec[index2];
            float Yv = vec[index2 + 1] - vec[index2];
            angularStep2 = atan2(Yv, 1);
        }
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateX2, translateY2);
        modelMatrix *= transform2D::Rotate(angularStep2);
        modelMatrix *= transform2D::Scale(50, 50);
        modelMatrix *= transform2D::Translate(0, 0.17f);
        RenderMesh2D(meshes["tank2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateX2, translateY2);
        modelMatrix *= transform2D::Rotate(angularStep2);
        modelMatrix *= transform2D::Scale(50, 50);
        modelMatrix *= transform2D::Translate(0, 0.17f);
        modelMatrix *= transform2D::Translate(0, 0.4f);
        modelMatrix *= transform2D::Rotate(angularTurret2);
        modelMatrix *= transform2D::Translate(0, -0.4f);
        RenderMesh2D(meshes["turret2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateX2, translateY2 + 65);
        RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], modelMatrix);
        float scaleLife = hp2 / 3.0f;
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(translateX2, translateY2 + 65);
        modelMatrix *= transform2D::Translate(13.2f * (hp2 - 3.0f), 0);
        modelMatrix *= transform2D::Scale(scaleLife, 1.0f);
        RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
    }
    else {
        if (hp2 == 0 && smokelife2 > 0) {
            smokelife2 -= deltaTimeSeconds;
            modelMatrix = glm::mat3(1);
            smokeY2 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX2, translateY2);
            modelMatrix *= transform2D::Translate(0, smokeY2);
            modelMatrix *= transform2D::Scale(1.0f, 1.0f);
            RenderMesh2D(meshes["smoke"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            smokeY2 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX2, translateY2);
            modelMatrix *= transform2D::Translate(0, smokeY2);
            modelMatrix *= transform2D::Translate(10, 10);
            modelMatrix *= transform2D::Scale(0.8f, 0.8f);
            RenderMesh2D(meshes["smoke2"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            smokeY2 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX2, translateY2);
            modelMatrix *= transform2D::Translate(0, smokeY2);
            modelMatrix *= transform2D::Translate(-8, 10);
            modelMatrix *= transform2D::Scale(0.6f, 0.6f);
            RenderMesh2D(meshes["smoke3"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            smokeY2 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX2, translateY2);
            modelMatrix *= transform2D::Translate(0, smokeY2);
            modelMatrix *= transform2D::Translate(0, 20);
            modelMatrix *= transform2D::Scale(0.4f, 0.4f);
            RenderMesh2D(meshes["smoke4"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            smokeY2 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX2, translateY2);
            modelMatrix *= transform2D::Translate(0, smokeY2);
            modelMatrix *= transform2D::Translate(-5, -20);
            modelMatrix *= transform2D::Scale(1.2f, 1.2f);
            RenderMesh2D(meshes["smoke2"], shaders["VertexColor"], modelMatrix);
            modelMatrix = glm::mat3(1);
            smokeY2 += deltaTimeSeconds * 2;
            modelMatrix *= transform2D::Translate(translateX2, translateY2);
            modelMatrix *= transform2D::Translate(0, smokeY2);
            modelMatrix *= transform2D::Translate(4, -20);
            modelMatrix *= transform2D::Scale(1.4f, 1.4f);
            RenderMesh2D(meshes["smoke4"], shaders["VertexColor"], modelMatrix);
        }
    }


    for (int i = 0; i < 20; ++i) {
        if (bullets[i].ok) {
            bullets[i].velocity.y -= 300 * deltaTimeSeconds;
            bullets[i].position += bullets[i].velocity * deltaTimeSeconds;
            if (bullets[i].position.x < 0 || bullets[i].position.x > 1280 || bullets[i].position.y < 0 || bullets[i].position.y > 720) {
                bullets[i].ok = false;
            }
            else {
                if (hp1 > 0 && glm::length(bullets[i].position - glm::vec2(translateX, translateY)) < 25.0f) {
                    hp1--;
                    bullets[i].ok = false;
                }
                if (hp2 > 0 && glm::length(bullets[i].position - glm::vec2(translateX2, translateY2)) < 25.0f) {
                    hp2--;
                    bullets[i].ok = false;
                }
				int x = static_cast<int>(bullets[i].position.x);
                if (bullets[i].position.y - vec[x] < 0) {
                    int start = x - 20;
					int end = x + 20;
                    for (int j = start; j < end; j++) {
						float position = abs(j - x);
                        if (position < 20) {
                            float c = sqrt(400 - position * position);
							vec[j] -= max(0.0f, c);
                        }
                    }
                    bullets[i].ok = false;
                }
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(bullets[i].position.x, bullets[i].position.y);
				modelMatrix *= transform2D::Scale(40, 40);
                RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    RenderMesh2D(meshes["militaryBase"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(1130, 145) * transform2D::Scale(100, 100));
    RenderMesh2D(meshes["pillar"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(1130, 160) * transform2D::Scale(100, 100));
    RenderMesh2D(meshes["flagred"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(1130, 160) * transform2D::Scale(100, 100));
    RenderMesh2D(meshes["militaryBase"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(-54, 238) * transform2D::Scale(100, 100));
    RenderMesh2D(meshes["pillar"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(-55, 255) * transform2D::Scale(100, 100));
    RenderMesh2D(meshes["flagblue"], shaders["VertexColor"], glm::mat3(1) * transform2D::Translate(-55, 255) * transform2D::Scale(100, 100));
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    //1
	if (window->KeyHold(GLFW_KEY_A)) {
        if (-angularStep > 0)
            translateX -= deltaTime * 50;
		else
			translateX -= deltaTime * 100;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
        if (angularStep > 0)
            translateX += deltaTime * 50;
        else
            translateX += deltaTime * 100;
	}
    if (translateX < 0) {
        translateX = 0;
    }
    if (translateX >= 1279) {
        translateX = 1278;
    }

	if (window->KeyHold(GLFW_KEY_W)) {
		angularTurret += deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		angularTurret -= deltaTime;
	}
	float minTurretAngle = 0.0f;
	float maxTurretAngle = 3.0f;
    if (angularTurret < minTurretAngle) {
        angularTurret = minTurretAngle;
    }
    if (angularTurret > maxTurretAngle) {
        angularTurret = maxTurretAngle;
    }

    //2
	if (window->KeyHold(GLFW_KEY_LEFT)) {
		translateX2 -= deltaTime * 100;
	}
	if (window->KeyHold(GLFW_KEY_RIGHT)) {
		translateX2 += deltaTime * 100;
	}
	if (translateX2 < 0) {
		translateX2 = 0;
	}
	if (translateX2 >= 1279) {
		translateX2 = 1278;
	}
	float minTurretAngle2 = -3.0f;
	float maxTurretAngle2 = 0.0f;
	if (angularTurret2 < minTurretAngle2) {
		angularTurret2 = minTurretAngle2;
	}
	if (angularTurret2 > maxTurretAngle2) {
		angularTurret2 = maxTurretAngle2;
	}
	if (window->KeyHold(GLFW_KEY_UP)) {
		angularTurret2 -= deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_DOWN)) {
		angularTurret2 += deltaTime;
	}
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        for (int i = 0; i < 20; ++i) {
            if (!bullets[i].ok) {
                float angularStepRadians = angularStep;
                float angularTurretRadians = angularTurret;
				float x = translateX + 10 + 5.9f * cos(angularStepRadians + angularTurretRadians);
				float y = translateY + 30 + 5.9f * sin(angularStepRadians + angularTurretRadians);
                bullets[i].position = glm::vec2(x, y);
                bullets[i].velocity = glm::vec2(400 * cos(angularStepRadians + angularTurretRadians), 400 * sin(angularStepRadians + angularTurretRadians));
                bullets[i].ok = true;
                break;
            }
        }
    }

    if (key == GLFW_KEY_ENTER) {
        for (int i = 0; i < 20; ++i) {
            if (!bullets[i].ok) {
                float angularStepRadians2 = angularStep2;
                float angularTurretRadians2 = angularTurret2;
                float x = translateX2 + 5.9f * cos(angularStepRadians2 + angularTurretRadians2);
                float y = translateY2 + 35 + 5.9f * sin(angularStepRadians2 + angularTurretRadians2);
                bullets[i].position = glm::vec2(x, y);
                bullets[i].velocity = glm::vec2(-400 * cos(angularStepRadians2 + angularTurretRadians2), -400 * sin(angularStepRadians2 + angularTurretRadians2));
                bullets[i].ok = true;
                break;
            }
        }
    }

	if (key == GLFW_KEY_LEFT_SHIFT) {
        if (translateX < 150)
		    if (hp1 < 3) {
			    hp1++;
		    }
	}
	if (key == GLFW_KEY_RIGHT_SHIFT) {
		if (translateX2 > 1130)
		    if (hp2 < 3) {
			    hp2++;
		    }
	}
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
