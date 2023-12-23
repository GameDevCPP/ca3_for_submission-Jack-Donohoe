#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include "engine.h"

class Level1Scene : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  sf::View gameView;
};
