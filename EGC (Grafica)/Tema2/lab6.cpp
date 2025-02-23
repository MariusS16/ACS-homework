#include "lab_m1/Tema2/lab6.h"

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    glm::ivec2 resolution = window->GetResolution();

    camera = new gfxc::Camera();
    camera->SetPosition(glm::vec3(0, 2, 5));

    angularStep = 0;
    droneRotationAngle = 0;
    translateX = 0;
	translateY = 0;
	translateZ = 0;
    hp = 1;
    count = 0;
    ok = 1;
	ok2 = 1;
	hpDrone = 100;
    alertHp = 0;
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    {
        int nrTree = 100;
        float terrainW = 70.0f;
        float terrainH = 70.0f;
        float minDist = 2.0f;

        for (int i = 0; i < nrTree; ++i) {
            bool positionValid = false;
            glm::vec3 poz;

            while (!positionValid) {
                float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * terrainW - terrainW / 2;
                float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * terrainH - terrainH / 2;
                float y = 0.3f;
                poz = glm::vec3(x, y, z);

                positionValid = true;
                for (const auto& position : treePositions) {
                    if (glm::distance(poz, position) < minDist) {
                        positionValid = false;
                        break;
                    }
                }
            }

            treePositions.push_back(poz);
        }

        int nrCube = 10;
        terrainW = 40.0f;
        terrainH = 40.0f;
		for (int i = 0; i < nrCube; ++i) {
			bool positionValid = false;
			glm::vec3 poz;

			while (!positionValid) {
				float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * terrainW - terrainW / 2;
				float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * terrainH - terrainH / 2;
				float y = 0.3f;
				poz = glm::vec3(x, y, z);

				positionValid = true;
				for (const auto& position : treePositions) {
					if (glm::distance(poz, position) < minDist) {
						positionValid = false;
						break;
					}
				}
			}

			cubePositions.push_back(poz);
		}

        int nrDest = 10;
		for (int i = 0; i < nrDest; ++i) {
            bool positionValid = false;
            glm::vec3 poz;

            while (!positionValid) {
                float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * terrainW - terrainW / 2;
                float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * terrainH - terrainH / 2;
                float y = 0.3f;
                poz = glm::vec3(x, y, z);

                positionValid = true;
                for (const auto& position : treePositions) {
                    if (glm::distance(poz, position) < minDist) {
                        positionValid = false;
                        break;
                    }
                }
            }

            destPositions.push_back(poz);
        }

		int nrCloud = 80;
        terrainW = 150.0f;
        terrainH = 150.0f;

        for (int i = 0; i < nrCloud; ++i) {
            bool positionValid = false;
            glm::vec3 poz;

            while (!positionValid) {
                float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * terrainW - terrainW / 2;
                float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * terrainH - terrainH / 2;
                float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f + 15.0f;

                poz = glm::vec3(x, y, z);

                positionValid = true;
                for (const auto& position : cloudPositions) {
                    if (glm::distance(poz, position) < minDist) {
                        positionValid = false;
                        break;
                    }
                }
            }

            cloudPositions.push_back(poz);
        }
    
    }

    {
        vector<VertexFormat> verticesArrow
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(1.3, 0, 0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0))
        };

        vector<unsigned int> indicesArrow
		{
		0, 1, 2
		};

		meshes["arrow"] = new Mesh("arrow");
		meshes["arrow"]->InitFromData(verticesArrow, indicesArrow);
    }

    {
        vector<VertexFormat> verticesDrone2
        {
            // Long parallelepiped (main body)
            VertexFormat(glm::vec3(-1.0,  0.2, -0.2), glm::vec3(0.7, 0.7, 0.7)),
            VertexFormat(glm::vec3(-1.0, -0.2, -0.2), glm::vec3(0.7, 0.7, 0.7)),
            VertexFormat(glm::vec3(1.0,  0.2, -0.2), glm::vec3(0.7, 0.7, 0.7)),
            VertexFormat(glm::vec3(1.0, -0.2, -0.2), glm::vec3(0.7, 0.7, 0.7)),
            VertexFormat(glm::vec3(-1.0,  0.2,  0.2), glm::vec3(0.7, 0.7, 0.7)),
            VertexFormat(glm::vec3(-1.0, -0.2,  0.2), glm::vec3(0.7, 0.7, 0.7)),
            VertexFormat(glm::vec3(1.0,  0.2,  0.2), glm::vec3(0.7, 0.7, 0.7)),
            VertexFormat(glm::vec3(1.0, -0.2,  0.2), glm::vec3(0.7, 0.7, 0.7)),

            // Short parallelepiped (rear part)
            VertexFormat(glm::vec3(1.0,  0.1, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(1.0, -0.1, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(1.5,  0.1, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(1.5, -0.1, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(1.0,  0.1,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(1.0, -0.1,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(1.5,  0.1,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(1.5, -0.1,  0.1), glm::vec3(0.5, 0.5, 0.5)),

            // Propeller (circular part)
            VertexFormat(glm::vec3(1.5,  0.0,  0.2), glm::vec3(0.8, 0.8, 0.8)),
            VertexFormat(glm::vec3(1.7,  0.0,  0.0), glm::vec3(0.8, 0.8, 0.8)),
            VertexFormat(glm::vec3(1.5,  0.0, -0.2), glm::vec3(0.8, 0.8, 0.8)),
            VertexFormat(glm::vec3(1.3,  0.0,  0.0), glm::vec3(0.8, 0.8, 0.8)),
        };

        vector<unsigned int> indicesDrone2 =
        {
            // Long parallelepiped
            0, 1, 2,  1, 3, 2,
            4, 5, 6,  5, 7, 6,
            0, 1, 5,  0, 5, 4,
            2, 3, 7,  2, 7, 6,
            0, 4, 6,  0, 6, 2,
            1, 5, 7,  1, 7, 3,

            // Short parallelepiped
            8, 9, 10,  9, 11, 10,
            12, 13, 14,  13, 15, 14,
            8, 9, 13,  8, 13, 12,
            10, 11, 15,  10, 15, 14,
            8, 12, 14,  8, 14, 10,
            9, 13, 15,  9, 15, 11,

            // Propeller
            16, 17, 18,  16, 18, 19,
        };

        meshes["droneBody2"] = new Mesh("droneBody2");
        meshes["droneBody2"]->InitFromData(verticesDrone2, indicesDrone2);
    }


    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5,  0.5, -0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0.5,  0.5, -0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0.5, -0.5, -0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(-0.5,  0.5,  0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(-0.5, -0.5,  0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0.5,  0.5,  0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0.5, -0.5,  0.5), glm::vec3(1, 0, 0)),
        };

        vector<unsigned int> indices
        {
            0, 1, 2, 1, 3, 2,
            4, 5, 6, 5, 7, 6,
            0, 1, 5, 0, 5, 4,
            2, 3, 7, 2, 7, 6,
            0, 4, 6, 0, 6, 2,
            1, 5, 7, 1, 7, 3
        };

        meshes["cube"] = new Mesh("cube");
        meshes["cube"]->InitFromData(vertices, indices);
    }

    {
        vector<VertexFormat> verticesCircle;
        vector<unsigned int> indicesCircle;

        float radius = 1.5f; 
        int numSegments = 100; 

        for (int i = 0; i < numSegments; ++i) {
            float angle = 2.0f * M_PI * i / numSegments;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            verticesCircle.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1, 1, 0)));

            indicesCircle.push_back(i);
            indicesCircle.push_back((i + 1) % numSegments);
        }

        meshes["circle"] = new Mesh("circle");
        meshes["circle"]->InitFromData(verticesCircle, indicesCircle);
        meshes["circle"]->SetDrawMode(GL_LINES);
    }

    {
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        float terrainWidth = resolution.x / 10.0f;
        float terrainHeight = resolution.y / 10.0f;
        int rows = resolution.y / 20;
        int columns = resolution.x / 20;

        float cellWidth = terrainWidth / (columns - 1);
        float cellHeight = terrainHeight / (rows - 1);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                glm::vec3 position = glm::vec3(j * cellWidth - terrainWidth / 2, 0.0f, i * cellHeight - terrainHeight / 2);
                vertices.push_back(VertexFormat(position, glm::vec3(0, 1, 0), glm::vec3(0.3, 0.7, 0.3)));

                if (i < rows - 1 && j < columns - 1) {
                    unsigned int topLeft = i * columns + j;
                    unsigned int topRight = topLeft + 1;
                    unsigned int bottomLeft = topLeft + columns;
                    unsigned int bottomRight = bottomLeft + 1;

                    indices.push_back(topLeft);
                    indices.push_back(bottomLeft);
                    indices.push_back(topRight);

                    indices.push_back(topRight);
                    indices.push_back(bottomLeft);
                    indices.push_back(bottomRight);
                }
            }
        }

        meshes["terrain"] = new Mesh("terrain");
        meshes["terrain"]->InitFromData(vertices, indices);
    }

    {
        vector<VertexFormat> verticesDrone
        {
            VertexFormat(glm::vec3(-0.5,  0.1, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.5, -0.1, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.5,  0.1, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.5, -0.1, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.5,  0.1,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.5, -0.1,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.5,  0.1,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.5, -0.1,  0.1), glm::vec3(0.5, 0.5, 0.5)),

            VertexFormat(glm::vec3(-0.1,  0.5, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1, -0.5, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1,  0.5, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1, -0.5, -0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1,  0.5,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1, -0.5,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1,  0.5,  0.1), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1, -0.5,  0.1), glm::vec3(0.5, 0.5, 0.5)),

            VertexFormat(glm::vec3(0.5,  0.1, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.5, -0.1, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.7,  0.1, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.7, -0.1, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.5,  0.1,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.5, -0.1,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.7,  0.1,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.7, -0.1,  0.2), glm::vec3(0.5, 0.5, 0.5)),

			VertexFormat(glm::vec3(-0.5,  0.1, -0.2), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(-0.5, -0.1, -0.2), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(-0.7,  0.1, -0.2), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(-0.7, -0.1, -0.2), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(-0.5,  0.1,  0.2), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(-0.5, -0.1,  0.2), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(-0.7,  0.1,  0.2), glm::vec3(0.5, 0.5, 0.5)),
			VertexFormat(glm::vec3(-0.7, -0.1,  0.2), glm::vec3(0.5, 0.5, 0.5)),

            VertexFormat(glm::vec3(-0.1,  0.5, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1,  0.7, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1,  0.7, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1,  0.5, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1,  0.5,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1,  0.7,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1,  0.7,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1,  0.5,  0.2), glm::vec3(0.5, 0.5, 0.5)),

            VertexFormat(glm::vec3(-0.1, -0.7, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1, -0.5, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1, -0.5, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1, -0.7, -0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1, -0.7,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(-0.1, -0.5,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1, -0.5,  0.2), glm::vec3(0.5, 0.5, 0.5)),
            VertexFormat(glm::vec3(0.1, -0.7,  0.2), glm::vec3(0.5, 0.5, 0.5)),
        };

        vector<unsigned int> indicesDrone =
        {
            0, 1, 2,  1, 3, 2,
            4, 5, 6,  5, 7, 6,
            0, 1, 5,  0, 5, 4,
            2, 3, 7,  2, 7, 6,
            0, 4, 6,  0, 6, 2,
            1, 5, 7,  1, 7, 3,

            8, 9, 10,  9, 11, 10,
            12, 13, 14,  13, 15, 14,
            8, 9, 13,  8, 13, 12,
            10, 11, 15,  10, 15, 14,
            8, 12, 14,  8, 14, 10,
            9, 13, 15,  9, 15, 11,

			16, 17, 18,  17, 19, 18,
			20, 21, 22,  21, 23, 22,
			16, 17, 21,  16, 21, 20,
			18, 19, 23,  18, 23, 22,
			16, 20, 22,  16, 22, 18,
			17, 21, 23,  17, 23, 19,

			24, 25, 26,  25, 27, 26,
			28, 29, 30,  29, 31, 30,
			24, 25, 29,  24, 29, 28,
			26, 27, 31,  26, 31, 30,
			24, 28, 30,  24, 30, 26,
			25, 29, 31,  25, 31, 27,

			32, 33, 34,  33, 35, 34,
			36, 37, 38,  37, 39, 38,
			32, 33, 37,  32, 37, 36,
			34, 35, 39,  34, 39, 38,
			32, 36, 38,  32, 38, 34,
			33, 37, 39,  33, 39, 35,

			40, 41, 42,  41, 43, 42,
			44, 45, 46,  45, 47, 46,
			40, 41, 45,  40, 45, 44,
			42, 43, 47,  42, 47, 46,
			40, 44, 46,  40, 46, 42,
			41, 45, 47,  41, 47, 43,

        };

        meshes["droneBody"] = new Mesh("droneBody");
        meshes["droneBody"]->InitFromData(verticesDrone, indicesDrone);
    }


    {
        vector<VertexFormat> verticesHelix
        {
            VertexFormat(glm::vec3(-0.1, 0.0, -1.0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(0.1, 0.0, -1.0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(0.1, 0.0, 1.0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-0.1, 0.0, 1.0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-0.1, 0.2, -1.0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(0.1, 0.2, -1.0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(0.1, 0.2, 1.0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(-0.1, 0.2, 1.0), glm::vec3(0, 0, 0)),
        };

        vector<unsigned int> indicesHelix =
        {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            0, 1, 5, 0, 5, 4,
            1, 2, 6, 1, 6, 5,
            2, 3, 7, 2, 7, 6,
            3, 0, 4, 3, 4, 7
        };

        meshes["helix"] = new Mesh("helix");
        meshes["helix"]->InitFromData(verticesHelix, indicesHelix);
	}

    {
        vector<VertexFormat> verticesTree;
        vector<unsigned int> indicesTree;

        const int cylinderSegments = 20;
        float radius = 0.1f;
        float height = 0.4f;

        //cilindru
        glm::vec3 trunkColor = glm::vec3(0.545f, 0.271f, 0.075f);
		for (int i = 0; i < cylinderSegments; i++) {
			float theta1 = 2 * M_PI * i / cylinderSegments;
			float theta2 = 2 * M_PI * (i + 1) / cylinderSegments;

			verticesTree.push_back(VertexFormat(glm::vec3(radius * cos(theta1), 0.0f, radius * sin(theta1)), trunkColor));
			verticesTree.push_back(VertexFormat(glm::vec3(radius * cos(theta1), height, radius * sin(theta1)), trunkColor));
			verticesTree.push_back(VertexFormat(glm::vec3(radius * cos(theta2), height, radius * sin(theta2)), trunkColor));
			verticesTree.push_back(VertexFormat(glm::vec3(radius * cos(theta2), 0.0f, radius * sin(theta2)), trunkColor));

			indicesTree.push_back(4 * i);
			indicesTree.push_back(4 * i + 1);
			indicesTree.push_back(4 * i + 2);
			indicesTree.push_back(4 * i);
			indicesTree.push_back(4 * i + 2);
			indicesTree.push_back(4 * i + 3);
		}

        //conuri
        glm::vec3 foliageColor = glm::vec3(0.0f, 0.5f, 0.0f);
        float coneHeight = 0.8f;
        radius = 0.5f;

        int coneBaseIndex1 = verticesTree.size();
        for (int i = 0; i < cylinderSegments; i++) {
            float angle = 2.0f * M_PI * i / cylinderSegments;
            verticesTree.push_back(VertexFormat(glm::vec3(radius * cos(angle), height, radius * sin(angle)), foliageColor));
        }
        int coneTipIndex1 = verticesTree.size();
        verticesTree.push_back(VertexFormat(glm::vec3(0.0f, height + coneHeight, 0.0f), foliageColor));

        for (int i = 0; i < cylinderSegments; i++) {
            indicesTree.push_back(coneBaseIndex1 + i);
            indicesTree.push_back(coneBaseIndex1 + (i + 1) % cylinderSegments);
            indicesTree.push_back(coneTipIndex1);
        }

        radius = 0.45f;
        int coneBaseIndex2 = verticesTree.size();
        for (int i = 0; i < cylinderSegments; i++) {
            float angle = 2.0f * M_PI * i / cylinderSegments;
            verticesTree.push_back(VertexFormat(glm::vec3(radius * cos(angle), height + coneHeight - 0.3f, radius * sin(angle)), foliageColor));
        }
        int coneTipIndex2 = verticesTree.size();
        verticesTree.push_back(VertexFormat(glm::vec3(0.0f, height + 2 * coneHeight, 0.0f), foliageColor));

        for (int i = 0; i < cylinderSegments; i++) {
            indicesTree.push_back(coneBaseIndex2 + i);
            indicesTree.push_back(coneBaseIndex2 + (i + 1) % cylinderSegments);
            indicesTree.push_back(coneTipIndex2);
        }


		meshes["tree"] = new Mesh("tree");
		meshes["tree"]->InitFromData(verticesTree, indicesTree);
    }

    {
        vector<VertexFormat> verticesCloud;
        vector<unsigned int> indicesCloud;


        const int sphereSegments = 20; 
        const int sphereStacks = 20;   
        glm::vec3 cloudColor = glm::vec3(0.65f, 0.65f, 0.65f); 

        vector<glm::vec3> sphereCenters = {
            glm::vec3(-0.6f, 0.0f, 0.0f),
            glm::vec3(0.6f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, -0.3f, 0.0f),

            glm::vec3(-0.4f, 0.3f, 0.2f),
            glm::vec3(0.4f, -0.2f, -0.2f),
            glm::vec3(-0.2f, 0.4f, -0.3f),
            glm::vec3(0.2f, -0.4f, 0.3f)
        };

        float radius = 0.5f;
        int startIndex = 0;

        for (const auto& center : sphereCenters) {
            for (int stack = 0; stack <= sphereStacks; ++stack) {
                float phi = glm::pi<float>() * stack / sphereStacks;
                for (int segment = 0; segment <= sphereSegments; ++segment) {
                    float theta = 2.0f * glm::pi<float>() * segment / sphereSegments;

                    float x = radius * sin(phi) * cos(theta);
                    float y = radius * cos(phi);
                    float z = radius * sin(phi) * sin(theta);

                    verticesCloud.push_back(VertexFormat(center + glm::vec3(x, y, z), cloudColor));

                    if (stack < sphereStacks && segment < sphereSegments) {
                        int current = startIndex + stack * (sphereSegments + 1) + segment;
                        int next = current + 1;
                        int nextStack = current + (sphereSegments + 1);

                        indicesCloud.push_back(current);
                        indicesCloud.push_back(nextStack);
                        indicesCloud.push_back(nextStack + 1);

                        indicesCloud.push_back(current);
                        indicesCloud.push_back(nextStack + 1);
                        indicesCloud.push_back(next);
                    }
                }
            }

            startIndex = verticesCloud.size();
        }

        meshes["cloud"] = new Mesh("cloud");
        meshes["cloud"]->InitFromData(verticesCloud, indicesCloud);
    }

    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

}

bool Tema2::Collision(glm::vec3 dronePos, glm::vec3 objectPos, float droneRadius, float objectRadius, float objectHeight) {

    float horizontalDistance = sqrt(
        (objectPos.x - dronePos.x) * (objectPos.x - dronePos.x) +
        (objectPos.z - dronePos.z) * (objectPos.z - dronePos.z)
    );

    bool horizontalCollision = horizontalDistance < (droneRadius + objectRadius);

    bool verticalCollision = (dronePos.y - droneRadius) < (objectPos.y + objectHeight) &&
        (dronePos.y + droneRadius) > objectPos.y;

    return horizontalCollision && verticalCollision;
}

Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));  schimb 1 cu 3
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderScene(float deltaTimeSeconds) {
    float time = Engine::GetElapsedTime();

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        RenderSimpleMesh(meshes["terrain"], shaders["LabShader"], modelMatrix);

    }

    {
        if (hpDrone > 0) {
            float angularSpeed = 2.5f;
            angularStep += angularSpeed * deltaTimeSeconds;
            glm::mat4 baseMatrix = glm::mat4(1);
            baseMatrix = glm::translate(baseMatrix, glm::vec3(translateX, translateY, translateZ));
            baseMatrix = glm::rotate(baseMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0)); 
            baseMatrix = glm::rotate(baseMatrix, droneRotationAngle, glm::vec3(0, 1, 0));

            glm::mat4 modelMatrix = baseMatrix;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.6, 1.2, 0.0));
            modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
            RenderMesh(meshes["helix"], shaders["VertexColor"], modelMatrix);

            modelMatrix = baseMatrix;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.6, 1.2, 0.0));
            modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
            RenderMesh(meshes["helix"], shaders["VertexColor"], modelMatrix);

            modelMatrix = baseMatrix;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, 1.2, 0.6));
            modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
            RenderMesh(meshes["helix"], shaders["VertexColor"], modelMatrix);

            modelMatrix = baseMatrix;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, 1.2, -0.6));
            modelMatrix = glm::rotate(modelMatrix, angularStep, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
            RenderMesh(meshes["helix"], shaders["VertexColor"], modelMatrix);
        }
    }

    {
        /*RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(3, 0.25, 20));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(-3, 0.3, 12));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(-2, 0.1, 0.3));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(0, 0.15, 3));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(2, 0.35, -6));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(-8, 0.2, -5));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(-5, 0.25, 5));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(9, 0.3, -7));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(-15, 0.4, 6));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(-20, 0.2, 16));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(20, 0.3, 16));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(4, 0.2, -14));
        RenderMesh(meshes["tree"], shaders["VertexColor"], glm::vec3(-5, 0.3, -18));*/

        for (const auto& pos : treePositions) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, pos);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8, 1.8, 1.8));
            RenderSimpleMesh(meshes["tree"], shaders["VertexColor"], modelMatrix);
        }

    }

    {
        if (hpDrone > 0) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translateX, translateY + 1, translateZ));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0)); 
            modelMatrix = glm::rotate(modelMatrix, droneRotationAngle, glm::vec3(0, 1, 0)); 
            modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["droneBody"], shaders["VertexColor"], modelMatrix);
            if (hp == 0) {
                RenderMesh(meshes["cube"], shaders["VertexColor"], modelMatrix * glm::translate(glm::mat4(1), glm::vec3(0, 0, 0.4f)) * glm::scale(glm::mat4(1), glm::vec3(0.5, 0.5, 0.5)));
            }
        }
    }

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 2, 2));
    RenderSimpleMesh(meshes["droneBody2"], shaders["VertexColor"], modelMatrix);

    if (hp == 1) {
        if (!cubePositions.empty()) {
            RenderMesh(meshes["cube"], shaders["VertexColor"], cubePositions[count]);
        }

        if (Collision(glm::vec3(translateX, translateY + 1, translateZ), cubePositions[count], 0.5f, 0.5f, 0.5f)) {
            cubePositions.erase(cubePositions.begin() + count);
            hp = 0;
        }
    }
    else if (hp == 0) {
        if (!destPositions.empty()) {
            RenderMesh(meshes["circle"], shaders["VertexColor"], destPositions[count]);
        }

        if (Collision(glm::vec3(translateX, translateY + 1, translateZ), destPositions[count], 0.5f, 0.5f, 0.5f)) {
            destPositions.erase(destPositions.begin() + count);
            hp = 1;
            count++;
        }
    }

    if (count != previousCount) {
        
        std::cout << "\rAi livrat " << count << " pachete. Bravo!" << std::endl;
        previousCount = count;
    }

    if (count > 0 || (count == 0 && hp == 0)) {
        static auto startTime = chrono::steady_clock::now();
        static int seconds = 0;

        auto currentTime = chrono::steady_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();

        if (elapsedTime > seconds) {
            seconds = elapsedTime;
            cout << "\rCronometru: " << seconds << " secunde" << flush;
        }
    }

    if (alertHp == 0) {
        if (hpDrone < 50) {
            cout  << "\rAtentie! Drona incepe sa se defecteze!" << flush;
            alertHp = 1;
        }
    }

    {
        if (hp == 0) {
            glm::vec3 playerPosition = glm::vec3(translateX, translateY, translateZ);
            glm::vec3 destinationPosition = destPositions[count];
            glm::vec3 direction = glm::normalize(destinationPosition - playerPosition);
            float angle = atan2(direction.z, direction.x);

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, playerPosition);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.65, 0, -0.25));
            modelMatrix = glm::rotate(modelMatrix, -angle, glm::vec3(0, 1, 0));
            RenderMesh(meshes["arrow"], shaders["VertexNormal"], modelMatrix);
        }
    }

    {
		for (const auto& pos : cloudPositions) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, pos);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5, 1.5, 1.5));
			RenderSimpleMesh(meshes["cloud"], shaders["VertexColor"], modelMatrix);
		}
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    glm::vec3 drona = glm::vec3(translateX, translateY, translateZ);
    float droneRotationAngle2 = droneRotationAngle + RADIANS(180);
    if (ok == 1)
    {
        glm::vec3 cameraPos = drona - glm::vec3(sin(droneRotationAngle) * (-4.0f), -3.0f, cos(droneRotationAngle) * -4.0f);
        glm::vec3 cameraRot = glm::vec3(-RADIANS(20), droneRotationAngle, 0);
        GetSceneCamera()->SetPositionAndRotation(cameraPos, cameraRot);
    }
    else if (ok == 2)
    {
        glm::vec3 cameraPos = drona - glm::vec3(sin(droneRotationAngle2) * -0.35f, -1.5f, cos(droneRotationAngle2) * -0.35f);
        glm::vec3 cameraRot = glm::vec3(-RADIANS(15), droneRotationAngle2, 0);
        GetSceneCamera()->SetPositionAndRotation(cameraPos, cameraRot);
    }

	RenderScene(deltaTimeSeconds);


    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    glEnable(GL_SCISSOR_TEST);
    glScissor(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (ok2 == 1) {
        auto projection = GetSceneCamera();
        glm::vec3 dronePosition = glm::vec3(translateX, translateY, translateZ);
        glm::vec3 topViewPosition = dronePosition + glm::vec3(0, 15, -6);
        projection->SetPositionAndRotation(topViewPosition, glm::vec3(-90, 0, 0));
    }


    
    RenderScene(deltaTimeSeconds);
    glDisable(GL_SCISSOR_TEST);

}


void Tema2::FrameEnd()
{
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Get shader location for uniform mat4 "Model"
    int location1 = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Get shader location for uniform mat4 "View"
    int location2 = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(location2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Get shader location for uniform mat4 "Projection"
    int location3 = glGetUniformLocation(shader->program, "Projection");
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Get shader location for uniform float "Time"
    int location4 = glGetUniformLocation(shader->program, "Time");
    float time = Engine::GetElapsedTime();
    glUniform1f(location4, time);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{

    glm::vec3 DronePosition = glm::vec3(translateX, translateY, translateZ);
    // Add key press event
    if (window->KeyHold(GLFW_KEY_U)) {
        DronePosition.x += +1.5f * deltaTime;
    }

	if (window->KeyHold(GLFW_KEY_J)) {
        DronePosition.x -= 1.5f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_I)) {
        DronePosition.y += +1.5f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_Y)) {
		if (DronePosition.y <= -0.5f) {
            DronePosition.y = -0.5;
		}
		else {
            DronePosition.y -= 1.5f * deltaTime;
		}
	}

	if (window->KeyHold(GLFW_KEY_H)) {
        DronePosition.z += +1.5f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_K)) {
        DronePosition.z -= 1.5f * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_L)) {
		droneRotationAngle += 0.5f * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_G)) {
		droneRotationAngle -= 0.5f * deltaTime;
	}


    bool collisionDetected = false;
	for (const auto& pos : treePositions) {
		if (Collision(DronePosition, pos, 0.5f, 0.9f, 2.2f)) {
            collisionDetected = true;
            break;
		}
	}
    if (collisionDetected == true) {
        if (hpDrone > 0) {
            hpDrone--;
        }
    }
    if (!collisionDetected) {
        translateX = DronePosition.x;
        translateY = DronePosition.y;
        translateZ = DronePosition.z;
    }

	
    
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (window->KeyHold(GLFW_KEY_C)) {
        if (ok == 1) {
            ok = 2;
        }
        else if (ok == 2) {
            ok = 0;
		}
		else {
			ok = 1;
		}
    }
    if (window->KeyHold(GLFW_KEY_V)) {
        if (ok2 == 1) {
            ok2 = 0;
        }
        else {
            ok2 = 1;
        }
    }

    if (window->KeyHold(GLFW_KEY_X)) {
        hpDrone = 100;
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
