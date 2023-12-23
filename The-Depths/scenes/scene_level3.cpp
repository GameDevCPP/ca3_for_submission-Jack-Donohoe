#include "scene_level3.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../depths_game.h"
#include "../components/cmp_bullet.h"
#include "../components/cmp_gem.h"
#include <LevelSystem_New.h>
#include <iostream>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level3Scene::Load() {
  cout << "Scene 3 Load" << endl;
  ls::loadJsonFile("../../res/levels/Level_3.json");
  ls::setBackgroundImage("res/img/background.jpg", Vector2f(0.f, 0.f), Vector2f(960.f, 640.f));

  _music = Resources::get<sf::SoundBuffer>("music.wav");
  _sound.setBuffer(*_music);
  _sound.play();
  _sound.setLoop(true);
  _sound.setVolume(15);

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

    // Generate gems for player to pickup
    {
        auto gems = ls::findTiles(ls::GEM);
        auto gemTexture = Resources::get<Texture>("gem.png");
        for (auto g: gems) {
            auto pos = ls::getTilePosition(g);
            pos += Vector2f(8.f, 8.f); //offset to center
            auto e = makeEntity();
            e->setPosition(pos);

            auto gem_sprite = e->addComponent<SpriteComponent>();
            gem_sprite->setTexture(gemTexture);
            gem_sprite->setTextureRect(IntRect(32, 0, 16, 16));

            e->addComponent<GemComponent>(player);
        }
    }

  gameView.setSize(300.f, 150.f);

//  auto font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
//  gemText.setFont(*font);
//  gemText.setCharacterSize(10);
//  gemText.setString("Gems: 0");

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
  gameView.setCenter(player->getPosition());

  const auto pp = player->getPosition();
  if (ls::getTileAt(pp) == ls::END) {
    Engine::ChangeScene((Scene*)&level1);
  } else if (!player->isAlive()) {
    Engine::ChangeScene((Scene*)&level3);
  }
  Engine::GetWindow().setView(gameView);

//  gemText.setString("Gems: " + to_string(player->get_components<PlayerPhysicsComponent>()[0]->getGemCount()));
//  gemText.setPosition(gameView.getCenter().x - gameView.getSize().x / 2, gameView.getCenter().y - gameView.getSize().y / 2 );

  Scene::Update(dt);
}

void Level3Scene::Render() {
  ls::render(Engine::GetWindow());
  Engine::GetWindow().draw(gemText);
  Scene::Render();
}
