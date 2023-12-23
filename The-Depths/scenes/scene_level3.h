#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include "engine.h"

class Level3Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  sf::Sprite level_background;

  sf::View gameView;
};