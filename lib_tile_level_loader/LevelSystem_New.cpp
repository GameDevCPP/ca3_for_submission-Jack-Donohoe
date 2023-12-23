#include "LevelSystem_New.h"
#include <fstream>
#include <json.hpp>

using namespace std;
using namespace sf;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colours{
    {WALL, Color::White}, {END, Color::Red}};

sf::Color LevelSystem::getColor(LevelSystem::Tile t) {
  auto it = _colours.find(t);
  if (it == _colours.end()) {
    _colours[t] = Color::Transparent;
  }
  return _colours[t];
}

void LevelSystem::setColor(LevelSystem::Tile t, sf::Color c) {
  _colours[t] = c;
}

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
std::vector<int> LevelSystem::_map;
size_t LevelSystem::_width;
size_t LevelSystem::_height;
sf::Texture LevelSystem::_tileset;
std::vector<sf::IntRect> LevelSystem::tileImages;

std::unique_ptr<sf::Sprite> LevelSystem::_background;
sf::Texture LevelSystem::_backgroundTexture;

float LevelSystem::_tileSize(100.f);
Vector2f LevelSystem::_offset(30.0f, 30.0f);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

void LevelSystem::loadTilesetFile(const std::string& path, float tileSize) {
    _tileSize = tileSize;
    size_t w = 0, h = 0;

    // Open tilemap image
    if (!_tileset.loadFromFile(path)) {
        throw string("Couldn't open tilemap image: ") + path;
    }

    // Get tilemap image size
    w = _tileset.getSize().x / _tileSize;
    h = _tileset.getSize().y / _tileSize;

    cout << "Tileset: " << path << " loaded. " << w << "x" << h << endl;

    // Create tilemap
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            IntRect tileBounds = IntRect(j * _tileSize, i * _tileSize, _tileSize, _tileSize);
            tileImages.push_back(tileBounds);
        }
    }
}

void LevelSystem::loadJsonFile(const std::string& path) {
    ifstream json(path);
    nlohmann::json file = nlohmann::json::parse(json);

    _tileSize = file.at("tilewidth").get<float>();

    _map = file.at("layers")[0].at("data").get<vector<int>>();

    _width = file.at("width").get<int>();
    _height = file.at("height").get<int>();

    _tiles = std::make_unique<Tile[]>(_width * _height);

    int wallTiles[] = {4,22,6,13,24,2,26,11,1,3,14};

    for (int i = 0; i < _map.size(); i++) {
        _tiles[i] = _map[i];
        for (int wallTile : wallTiles){
            if (_map[i] == wallTile){
                _tiles[i] = WALL;
            }
        }
    }

    cout << "Level " << path << " Loaded. " << _width << "x" << _height << std::endl;
    buildSprites();
}

void LevelSystem::buildSprites(bool optimise) {
  _sprites.clear();

  struct tp {
    sf::Vector2f p;
    sf::Vector2f s;
    sf::Color c;
  };
  vector<tp> tps;
  const auto tls = Vector2f(_tileSize, _tileSize);
  for (size_t y = 0; y < _height; ++y) {
    for (size_t x = 0; x < _width; ++x) {
      Tile t = getTile({x, y});
      if (t == EMPTY || t == START || t == GEM) {
        continue;
      }
      tps.push_back({getTilePosition({x, y}), tls, getColor(t)});
    }
  }

  for (auto& t : tps) {
    auto s = make_unique<sf::RectangleShape>();
    s->setPosition(t.p);
    s->setSize(t.s);

    int xVal = (int) t.p.x/t.s.x;
    int yVal = (int) t.p.y/t.s.y;
    int index = _map[(yVal * _width) + xVal] - 1;
    if (index < 0){
        //Negative number means not a tile
        continue;
    }

    if (index == 70){
        //End tiles are not walls, set to black squares to serve as doors
        s->setFillColor(Color::Black);
    } else {
        s->setTexture(&_tileset);
        s->setTextureRect(tileImages[index]);
    }

    _sprites.push_back(move(s));
  }

  cout << "Level with " << (_width * _height) << " Tiles, using: " << _sprites.size() << " Sprites\n";
}

void LevelSystem::setBackgroundImage(const std::string &path, sf::Vector2f offset, sf::Vector2f scale) {
    _backgroundTexture.loadFromFile(path);

    auto s = std::make_unique<sf::Sprite>();
    s->setTexture(_backgroundTexture);
    s->setPosition(offset);
    s->setScale(Vector2f{scale.x / _backgroundTexture.getSize().x, scale.y / _backgroundTexture.getSize().y});

    _background = std::move(s);
}

void LevelSystem::render(RenderWindow& window) {
  window.draw(*_background);
  for (auto& t : _sprites) {
    window.draw(*t);
  }
}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p) {
  if (p.x > _width || p.y > _height) {
    throw string("Tile out of range: ") + to_string(p.x) + "," +
        to_string(p.y) + ")";
  }
  return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
  return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type) {
  auto v = vector<sf::Vector2ul>();
  for (size_t i = 0; i < _width * _height; ++i) {
    if (_tiles[i] == type) {
      v.push_back({i % _width, i / _width});
    }
  }

  return v;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) {
  auto a = v - _offset;
  if (a.x < 0 || a.y < 0) {
    throw string("Tile out of range ");
  }
  return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) {
  auto a = v - _offset;
  if (a.x < 0 || a.y < 0) {
    return false;
  }
  auto p = Vector2ul((v - _offset) / (_tileSize));
  if (p.x > _width || p.y > _height) {
    return false;
  }
  return true;
}

void LevelSystem::setOffset(const Vector2f& _offset) {
  LevelSystem::_offset = _offset;
  buildSprites();
}

void LevelSystem::unload() {
  cout << "LevelSystem unloading\n";
  _sprites.clear();
  _tiles.reset();
  _width = 0;
  _height = 0;
  _offset = {0, 0};
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }
