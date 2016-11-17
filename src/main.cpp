#include <iostream>
#include <vector>
#include <stdlib.h>
#include <GL/Mesh.h>
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

    //GL::Mesh rock("cube");


    try {
        cout << "Start" << endl;
        auto scene = Scene::GetScene(argc, argv, KeyboardEvents);

        Ground *ground = new Ground();
        scene->AddMesh(ground);
        cout << "Ground created" << endl;

        grass = new Grass();
        scene->AddMesh(grass);
        cout << "Grass created" << endl;


        glutMainLoop();

    } catch (string s) {
        cout << s << endl;
    }
}
