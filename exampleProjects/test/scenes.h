void loadScene(string sceneName)
{
	if (sceneName == "Bob's scene")
	{
		for (auto ptr : currentScene)
			delete ptr;
		currentScene.clear();
		currentScene.push_back(new Bob());
		for (int i = 0; i < currentScene.size(); i++)
		currentScene[i]->init(i);
		return;
	}
}
