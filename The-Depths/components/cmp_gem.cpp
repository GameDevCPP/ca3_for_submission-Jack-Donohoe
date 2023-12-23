#include "cmp_sprite.h"
#include "cmp_player_physics.h"
#include "cmp_gem.h"
#include "system_resources.h"

void GemComponent::update(double dt) {
    auto playerRect = _player->get_components<SpriteComponent>()[0]->getSprite().getGlobalBounds();

    if (playerRect.contains(_parent->getPosition())) {
        _parent->setForDelete();
        _player->get_components<PlayerPhysicsComponent>()[0]->addGem();
    }
}

void GemComponent::render() {

}

GemComponent::GemComponent(Entity *p, std::shared_ptr<Entity> player) : Component(p), _player(std::move(player)) {

}
