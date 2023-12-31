#include "Zelda.h"
#include "GameScene/GameScene.h"

#include <GameEngine/System.h>
#include <GameEngine/SceneManager.h>
#include <GameEngine/Resources.h>

using namespace GameEngine;
using namespace GameEngine::Graphics;

void Zelda::Initialise() {
	System::Init();

	Resources::Get().SetAssetPath(
		"../../../../../../cs454/UnitTests/ZeldaII/assets/");
	Resources::Get().SetRootPath("../../../../../../cs454/UnitTests/ZeldaII/");
}

void Zelda::Load() {
	window = new Window("ZeldaII-Adventure Of Link", 100, 40, 1080, 720, Window::Shown | Window::Resizable);
	renderer = new Renderer(*window, -1, Renderer::Accelerated);

	this->game.SetFinished([&]() { return !window->IsOpen(); });
	this->game.AddQuit([&]() { window->Close(); }, false);

	this->game.AddInput(std::bind(&Zelda::Input, this), false);
	this->game.AddRender(std::bind(&Zelda::Render, this), false);
	this->game.AddAnim(std::bind(&Zelda::Anim, this), false);
	this->game.AddDestruct(std::bind(&Zelda::Destroy, this), false);
	this->game.AddCollisions(std::bind(&Zelda::Collision, this), false);
	this->game.AddCollisions(std::bind(&Zelda::AI, this), false);

	try {
		GameScene* scene = new GameScene(window, renderer);
		scene->SetPause([&](double t) { game.Pause(t); });
		scene->SetResume([&]() { game.Resume(); });
		SceneManager::GetSceneManager().Add(scene);
		SceneManager::GetSceneManager().LoadScene(0);
	} catch (std::exception& e) {
		// dont need to handle this
	}
}

void Zelda::Input() {
	SceneManager::GetSceneManager().GetCurrentScene()->Input();
}

void Zelda::Render() {
	SceneManager::GetSceneManager().GetCurrentScene()->Render();
}

void Zelda::Anim() {
	SceneManager::GetSceneManager().GetCurrentScene()->ProgressAnimations();
}

void Zelda::Destroy() {
	SceneManager::GetSceneManager().GetCurrentScene()->CommitDestructions();
}

void Zelda::Collision() {
	SceneManager::GetSceneManager().GetCurrentScene()->CollisionChecking();
}

void Zelda::AI() {
	SceneManager::GetSceneManager().GetCurrentScene()->AI();
}

void Zelda::Clear() {
	System::Destroy(window);
	System::Destroy(renderer);
	System::CleanUp();
}