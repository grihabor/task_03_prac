#include <iostream>
#include <vector>
#include <stdlib.h>

#include "Utility.h"
#include "object.h"
#include "scene.h"
#include "grass.h"
#include "ground.h"
#include "skybox.h"
#include "serializer.h"


using namespace std;

GL::Camera camera;
Grass *grass;

const char* JSON_FILENAME = "save.json";
const char* BINARY_FILENAME = "save.bin";

bool KeyboardEvents(GL::Camera &camera, unsigned char key) {
    if (key == 27) {
        return false;
    } else if (key == 'w') {
        camera.goForward();
    } else if (key == 's') {
        camera.goBack();
    } else if (key == 'a') {
        camera.goLeft();
    } else if (key == 'd') {
        camera.goRight();
    } else if (key == 'i') {
        grass->WindSwitch();
    } else if (key == 'v') {
        JSONSerializerReader reader(JSON_FILENAME);
        reader.Open();
        float t;
        reader.process(t);
        grass->SetWindDirection(t);
    } else if (key == 'b') {
        JSONSerializerWriter writer(JSON_FILENAME);
        writer.Open();
        float t = grass->GetWindDirection();
        writer.process(t);
    } else if (key == 'n') {

    } else if (key == 'm') {

    }
    return true;
}

int main(int argc, char **argv)
{
    putenv("MESA_GL_VERSION_OVERRIDE=3.3COMPAT");

    try {
        cout << "Start" << endl;
        auto scene = Scene::GetScene(argc, argv, KeyboardEvents);


        // rocks characteristics
        vector<VM::vec3> rocks = {
                VM::vec3(.6, 0, .7),
                VM::vec3(.2, 0, .3),
                VM::vec3(.8, 0, .2)
        };
        vector<float> scales = {0.07, 0.02, 0.04};
        vector<VM::vec4> spots;

        // spots for rocks
        for(int i = 0; i < rocks.size(); ++i) {
            auto v = rocks[i];
            auto radius = scales[i] * 2;
            spots.push_back(VM::vec4(v, radius));
        }

        // grass
        grass = new Grass(spots);
        scene->AddMesh(grass);
        cout << "Grass created" << endl;

        // ground
        Ground *ground = new Ground();
        scene->AddMesh(ground);
        cout << "Ground created" << endl;

        // skybox
        Skybox *skybox = new Skybox();
        scene->AddMesh(skybox, true);

        // tree 1
        VM::vec3 position(.2, 0, .55);
        float size = 0.05;
        Object *tree = new Object("tree", "Texture/tree.jpg", position, size);
        scene->AddMesh(tree);
        Object *leaves = new Object("leaves", "Texture/leaf.jpg", position, size);
        scene->AddMesh(leaves);

        // tree 2
        position = VM::vec3(.85, 0, .4);
        size = 0.03;
        tree = new Object("tree", "Texture/tree.jpg", position, size);
        scene->AddMesh(tree);
        leaves = new Object("leaves", "Texture/leaf.jpg", position, size);
        scene->AddMesh(leaves);

        // rocks
        for(int i = 0; i < rocks.size(); ++i) {
            Object *rock = new Object("rock", "Texture/rock.jpg",
                                      rocks[i], scales[i]);
            scene->AddMesh(rock);
            cout << "Rock " << i << " created" << endl;
        }

        glutMainLoop();

    } catch (string s) {
        cout << s << endl;
    }
}
