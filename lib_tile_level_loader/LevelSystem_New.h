#pragma once

#include <SFML/Graphics.hpp>
#include <maths.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

#define ls LevelSystem

class LevelSystem {
public:
  static std::vector<sf::IntRect> tileImages;

  static void loadTilesetFile(const std::string& path, float tileSize);
  static void loadJsonFile(const std::string&);
  static void unload();
  static void render(sf::RenderWindow& window);

  typedef unsigned int Tile;

  enum TILES {
    EMPTY = 0,
    START = 70,
    END = 71,
    WALL = 1,
    GEM = 100,
  };

  static Tile getTile(sf::Vector2ul);

  static Tile getTileAt(sf::Vector2f);

  static bool isOnGrid(sf::Vector2f);

  static size_t getWidth();

  static size_t getHeight();

  static sf::Vector2f getTilePosition(sf::Vector2ul);

  static std::vector<sf::Vector2ul> findTiles(Tile);

  static sf::Color getColor(Tile t);

  static void setColor(Tile t, sf::Color c);

  static void setOffset(const sf::Vector2f& _offset);

  static const sf::Vector2f& getOffset();

  static float getTileSize();

  static void setBackgroundImage(const std::string &path, sf::Vector2f offset, sf::Vector2f scale);

protected:
  static std::unique_ptr<Tile[]> _tiles;
  static std::vector<int> _map;
  static size_t _width;
  static size_t _height;
  static sf::Vector2f _offset;

  static sf::Texture _tileset;

  static sf::Texture _backgroundTexture;
  static std::unique_ptr<sf::Sprite> _background;

  static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;

  static void buildSprites(bool optimise = true);

  static float _tileSize; // for rendering
  static std::map<Tile, sf::Color> _colours;

private:
  LevelSystem() = delete;

  ~LevelSystem() = delete;
};
