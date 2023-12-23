#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Text.hpp>
#include "engine.h"

class Level3Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  sf::View gameView;
  sf::Text gemText;
protected:
    std::shared_ptr<sf::SoundBuffer> _music;
    sf::Sound _sound;
};