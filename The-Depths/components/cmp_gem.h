#ifndef THE_DEPTHS_CMP_GEM_H
#define THE_DEPTHS_CMP_GEM_H

#include "ecm.h"
#include "SFML/Audio/Sound.hpp"

class GemComponent : public Component {
public:
    GemComponent(Entity *p, std::shared_ptr<Entity> player);

    void update(double dt) override;
    void render() override ;
    GemComponent() = delete;
protected:
    std::shared_ptr<Entity> _player;
};

#endif //THE_DEPTHS_CMP_GEM_H
