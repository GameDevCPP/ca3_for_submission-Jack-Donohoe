# The Depths
C++ CA3 for Advanced C++ Module.

## Links
- [Trello Project Management Board](https://trello.com/b/rprpagBQ/c-ca3-project-plan)
- [Video Demo](https://youtu.be/KjUSjlHCJYw)
- Game Design Document can be found in 'Game Design Document.docx'.

## Changes Added/Modified
### Player Physics Component
- Added handling for player animations. (Not working currently due to bug)
- Added tracking of number of gem pickups collected.

### Gem Component
- Added logic for detecting collisions and destroying the gem when the player collides with it.

### Level System
- Added loading of level files through JSON.
- Added loading of sprites for level from tileset image.
- Added loading of tilemap from level file.

### Scenes - Level 1, Level 2, Level 3
- Added creation of player and gem sprites.
- Added music to each level.
- Added loading of gem pickups from level tilemap.
- Added game view to follow player.
- Added player HUD to show number of gems collected. (Not working currently due to bug when loading between levels)
