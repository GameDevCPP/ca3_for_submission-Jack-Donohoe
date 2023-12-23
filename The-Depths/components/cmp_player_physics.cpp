#include "cmp_player_physics.h"
#include "system_physics.h"
#include "system_resources.h"
#include <LevelSystem_New.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

auto player_texture = Resources::get<Texture>("FinnSprite.png");

bool PlayerPhysicsComponent::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::update(double dt) {

  const auto pos = _parent->getPosition();

  //Teleport to start if we fall off map.
  if (pos.y > ls::getHeight() * ls::getTileSize()) {
    teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
  }

  if (Keyboard::isKeyPressed(Keyboard::Left) ||
      Keyboard::isKeyPressed(Keyboard::Right)) {
    // Moving Either Left or Right
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
      if (getVelocity().x < _maxVelocity.x)
        impulse({(float)(dt * _groundspeed), 0});
    } else {
      if (getVelocity().x > -_maxVelocity.x)
        impulse({-(float)(dt * _groundspeed), 0});
    }
  } else {
    // Dampen X axis movement
    dampen({0.9f, 1.0f});
  }

  // Handle Jump
  if (Keyboard::isKeyPressed(Keyboard::Up)) {
    _grounded = isGrounded();
    if (_grounded) {
      setVelocity(Vector2f(getVelocity().x, 0.f));
      teleport(Vector2f(pos.x, pos.y - 2.0f));
      impulse(Vector2f(0, -6.f));
    }
  }

  //Are we in air?
  if (!_grounded) {
    // Check to see if we have landed yet
    _grounded = isGrounded();
    // disable friction while jumping
    setFriction(0.f);
  } else {
    setFriction(0.1f);
  }

  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  // Flip sprite based on direction
  if (getVelocity().x > 0) {
      _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(1, 1);
  }else if (getVelocity().x < 0) {
      _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(-1, 1);
  }

  // Handle animations
  if(!_grounded){
      // Set to jump animation
      _parent->GetCompatibleComponent<AnimationComponent>()[0]->setAnimation(1, 1, player_texture, IntRect(Vector2i(480, 0), Vector2i(32, 32)));
  }
  else if(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right)) {
      // Set to run animation
      _parent->GetCompatibleComponent<AnimationComponent>()[0]->setAnimation(5, 0.2, player_texture, IntRect(Vector2i(120, 0), Vector2i(32, 32)));
  }else {
      // Set back to idle animation
      _parent->GetCompatibleComponent<AnimationComponent>()[0]->setAnimation(4, 0.2, player_texture, IntRect(Vector2i(0, 0), Vector2i(32, 32)));
  }

  PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size)
    : PhysicsComponent(p, true, size) {
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(200.f, 400.f);
  _groundspeed = 20.f;
  _grounded = true;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
}
