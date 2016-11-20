#include <iostream>
#include <vector>
#include <stdlib.h>
#include <object.h>
#include <scene.h>

#include "Utility.h"
#include "grass.h"
#include "ground.h"



using namespace std;

GL::Camera camera;
Grass *grass;

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
    }
    return true;
}

int main(int argc, char **argv)
{
    putenv("MESA_GL_VERSION_OVERRIDE=3.3COMPAT");

    try {
        cout << "Start" << endl;
        auto scene = Scene::GetScene(argc, argv, KeyboardEvents);

        Ground *ground = new Ground();
        scene->AddMesh(ground);
        cout << "Ground created" << endl;

        vector<VM::vec3> rocks = {
                VM::vec3(.6, 0, .7),
                VM::vec3(.2, 0, .3),
                VM::vec3(.8, 0, .2)
        };
        vector<float> scales = {0.07, 0.02, 0.04};
        vector<VM::vec4> spots;


        for(int i = 0; i < rocks.size(); ++i){
            Object *rock = new Object("rock", "Texture/rock.jpg", rocks[i], scales[i]);
            scene->AddMesh(rock);

            auto v = rocks[i];
            auto radius = scales[i] * 2;
            spots.push_back(VM::vec4(v, radius));
        }

        grass = new Grass(spots);
        scene->AddMesh(grass);
        cout << "Grass created" << endl;


        glutMainLoop();

    } catch (string s) {
        cout << s << endl;
    }
}
