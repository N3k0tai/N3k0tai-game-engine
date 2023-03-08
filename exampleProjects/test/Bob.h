class Bob : public GameObject
{
public:
	void init (int indexInScene)
	{
		physicsObject.x = 100;
		physicsObject.y = 100;
		cout << "hello!\n";
		sprite = new Sprite("cookie-monster.jpg", defaultShader);
	}

	void update (int indexInScene)
	{
		if (specialKeyPressed[GLUT_KEY_RIGHT])
			physicsObject.x += 100.0 * delta;

		if (specialKeyPressed[GLUT_KEY_LEFT])
			physicsObject.x -= 100.0 * delta;

		if (specialKeyPressed[GLUT_KEY_UP])
			physicsObject.y -= 100.0 * delta;

		if (specialKeyPressed[GLUT_KEY_DOWN])
			physicsObject.y += 100.0 * delta;
	}
};
