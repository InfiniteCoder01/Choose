#include "00Names.hpp"

Player player;
Level level;
vec2i camera;
uint32_t tilesize;
uint32_t nextLevel;

int main() {
  MvWindow window("Choose");
  tilesize = 50;
  nextLevel = 0;
  while (window.isOpen()) {
    level = Level(nextLevel);

    while (player.isAlive()) {
      tilesize = Math::ceil(window.height() / (float)level.height());
      player.update();

      if (nextLevel == 5 && level.withIC() && player.allInPlace()) window.clear(MvColor(135, 206, 235));
      else window.clear(MvColor(21, 34, 56));
      level.draw(window);
      player.draw(window);
      Mova::nextFrame();
    }
  }
  return 0;
}
