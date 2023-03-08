// +----------------+
// | Graphics stuff |
// +----------------+

GLuint defaultShader;
int wWidth, wHeight;

class Sprite
{
public:
	GLuint texID;
	int width, height;
	GLuint shader;
	
	Sprite(const char* filename, GLuint shader_)
	{
		shader = shader_;

		unsigned char* imageData;
		int channels;
		bool hasAlpha;
		imageData = stbi_load(filename, &width, &height, &channels, 0);

		if (channels == 3)
			hasAlpha = false;
		else
			hasAlpha = true;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (hasAlpha)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(imageData);
	}

	~Sprite()
	{
		glDeleteTextures(1, &texID);
	}

	void render(double x, double y)
	{
		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		
		float vertices[] = {
		(float)x, (float)y + height,          0.0f, 1.0f,
		(float)x, (float)y,                   0.0f, 0.0f,
		(float)x + width, (float)y,           1.0f, 0.0f,
		(float)x + width, (float)y + height,  1.0f, 1.0f
		};
		
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
	
		glUseProgram(defaultShader);
		//glUniform4f(glGetUniformLocation(defaultShader, "color"), r, g, b, a);
		glUniform4f(glGetUniformLocation(defaultShader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
		glUniform2f(glGetUniformLocation(defaultShader, "wSize"), wWidth, wHeight);
		//glUniform4f(glGetUniformLocation(defaultShader, "camInfo"), camera.x, camera.y, camera.scaleX, camera.scaleY);
		glUniform4f(glGetUniformLocation(defaultShader, "camInfo"), 0.0f, 0.0f, 1.0f, 1.0f);
		//glUniform1i(glGetUniformLocation(defaultShader, "isScreenCoordinates"), currentScene[index].isScreenCoordinates);
		glUniform1i(glGetUniformLocation(defaultShader, "isScreenCoordinates"), 1);
		glDrawArrays(GL_QUADS, 0, 4);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
};

GLuint compileShaders(const char* vertFilename, const char* fragFilename)
{
	ifstream vertFile(vertFilename);
	string vertSource;
	getline(vertFile, vertSource, '\0');
	const char* vertSourceFinal = vertSource.c_str();
	vertFile.close();
	
	ifstream fragFile(fragFilename);
	string fragSource;
	getline(fragFile, fragSource, '\0');
	const char* fragSourceFinal = fragSource.c_str();
	fragFile.close();
	
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSourceFinal, NULL);
	glCompileShader(vertShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSourceFinal, NULL);
	glCompileShader(fragShader);

	GLuint finalShader = glCreateProgram();
	glAttachShader(finalShader, vertShader);
	glAttachShader(finalShader, fragShader);
	glLinkProgram(finalShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return finalShader;
}

// +---------------+
// | Physics stuff |
// +---------------+

struct PhysicsObject
{
	double x = 0;
	double y = 0;
};

// +------------------+
// | GameObject stuff |
// +------------------+

class GameObject
{
public:
	string name;
	Sprite* sprite;
	PhysicsObject physicsObject;
	
	GameObject(){}
	~GameObject()
	{
		delete sprite;
	}
	virtual void init(int indexInScene){}
	virtual void update(int indexInScene){}
};

vector <GameObject*> currentScene;

void loadScene(string sceneName);


double delta = 0.017;


// +-------------+
// | Input stuff |
// +-------------+


bool keyPressed[128];
map <int, bool> specialKeyPressed;
int mouseX, mouseY;

void keyPressFunc(unsigned char key, int x, int y)
{
	keyPressed[key] = true;
	mouseX = x;
	mouseY = y;
}

void specialKeyPressFunc(int key, int x, int y)
{
	specialKeyPressed[key] = true;
	mouseX = x;
	mouseY = y;
}

void keyUpFunc(unsigned char key, int x, int y)
{
	keyPressed[key] = false;
	mouseX = x;
	mouseY = y;
}

void specialKeyUpFunc(int key, int x, int y)
{
	specialKeyPressed[key] = false;
	mouseX = x;
	mouseY = y;
}


// +-----------------+
// | Game loop stuff |
// +-----------------+


void updateGame()
{
	auto start = chrono::high_resolution_clock::now();
	wWidth = glutGet(GLUT_WINDOW_WIDTH);
	wHeight = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, wWidth, wHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < currentScene.size(); i++)
		currentScene[i]->update(i);

	for (int i = 0; i < currentScene.size(); i++)
		if (currentScene[i]->sprite != nullptr)
			currentScene[i]->sprite->render(currentScene[i]->physicsObject.x, currentScene[i]->physicsObject.y);

	glFlush();
	glutPostRedisplay();

	auto finish = chrono::high_resolution_clock::now();
	delta = (double)(chrono::duration_cast<chrono::nanoseconds>(finish - start).count()) / 1000000000.0;
}


void initGame(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(W_TITLE);
	glutDisplayFunc(updateGame);
	glutKeyboardFunc(keyPressFunc);
	glutSpecialFunc(specialKeyPressFunc);
	glutKeyboardUpFunc(keyUpFunc);
	glutSpecialUpFunc(specialKeyUpFunc);

	glewInit();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	loadScene(START_SCENE);
	defaultShader = compileShaders("../../src/Engine/defaultShader.vert", "../../src/Engine/defaultShader.frag");
	fill(keyPressed, keyPressed + 128, false);
}
