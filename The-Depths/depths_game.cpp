#include "engine.h"
#include "depths_game.h"
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

short unsigned switchState;
int volume;
Vector2i resolution;

MenuScene menu;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;

int main() {
	resolution = Vector2i(1280, 720);
	volume = 20;
	Engine::Start(resolution.x, resolution.y, "The Depths", &menu);
}