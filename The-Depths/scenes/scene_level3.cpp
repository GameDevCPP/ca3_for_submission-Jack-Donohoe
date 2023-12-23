#include "scene_level3.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../depths_game.h"
#include "../components/cmp_bullet.h"
#include <LevelSystem_New.h>
#include <iostream>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level3Scene::Load() {
  cout << "Scene 3 Load" << endl;
  ls::loadJsonFile("../../res/levels/Level_3.json");

  // Create player
  {
    // *********************************
    player = makeEntity();
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto player_texture = Resources::get<Texture>("FinnSprite.png");
    auto player_sprite = player->addComponent<SpriteComponent>();

    player_sprite->setTexture(player_texture);
    player_sprite->getSprite().setOrigin(16.f,16.f);

    player->addComponent<PlayerPhysicsComponent>(Vector2f(32.f, 32.f));
    auto player_anim = player->addComponent<AnimationComponent>();
  }

  // Add physics colliders to level tiles.
  {
    // *********************************
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(16.f, 16.f); //offset to center
      auto e = makeEntity();
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(false, Vector2f(32.f, 32.f));
    }
    // *********************************
  }

  cout << " Scene 3 Load Done" << endl;
  setLoaded(true);
}

void Level3Scene::UnLoad() {
  cout << "Scene 3 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level3Scene::Update(const double& dt) {
  Scene::Update(dt);
  const auto pp = player->getPosition();
  if (ls::getTileAt(pp) == ls::END) {
    Engine::ChangeScene((Scene*)&level1);
  } else if (!player->isAlive()) {
    Engine::ChangeScene((Scene*)&level3);
  }
}

void Level3Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
