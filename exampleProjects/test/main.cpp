#define GLEW_STATIC
#include <GL/glew.h>
#include "GL/freeglut.h"
#include "GL/gl.h"


#define STB_IMAGE_IMPLEMENTATION
#include "../../src/Engine/stb_image.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <map>
#include <functional>
using namespace std;

#include "Config.h"
#include "../../src/Engine/Engine.h"

// Game Objects
#include "Bob.h"


// Scenes
#include "scenes.h"


int main(int argc, char **argv)
{
	initGame(argc, argv);
	glutMainLoop();
	glDeleteProgram(defaultShader);
	return 0;
}
