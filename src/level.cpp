#include "00Names.hpp"
#include <map>

#pragma region LevelData
std::string levels[] = {
    "                                              W"
    "                                               "
    "                            ###                "
    "                                               "
    "                                               "
    "                  ##    ##                     "
    "            ##                         G       "
    "  P                                            "
    " ####                                          "
    "                                               ",

    "                                              W"
    "                                               "
    "                                               "
    "                                               "
    "                                               "
    "     A           ####                          "
    "   P H                                         "
    "  #########                 ##   ##   ##   ##  "
    "                               G    Y    R     "
    "                                               ",

    "                                              W"
    "                                               "
    "                                               "
    "   P                                           "
    " G###                                          "
    "           ####                                "
    "                                             g "
    "                       ##       ####        ###"
    "               I                               "
    "               #                               ",

    "                                              W"
    "                                   A           "
    "                                   H           "
    "   P                 #####        ###          "
    " G###        #                                 "
    "                                               "
    "                                               "
    "                                               "
    "                                              y"
    "  #            #           #           #      #",

    "                                                                                                          W"
    "                B                                                                                          "
    "                                                                                                           "
    "                    ##      ###    ###    ##    ##     #     #####                                         "
    "                                                                       #                                   "
    "                                                                                                           "
    "                                                                           ###                             "
    "                                                                                                          r"
    "   P                                  i           ####             ###                        #           #"
    " G###             ####              ####                                                                   ",

    "                                              W"
    "                                               "
    "                                               "
    "    P                               O          "
    "  G###         ####              ##############"
    "                                               "
    "                                               "
    "                                               "
    "  b                                            "
    "  #                                            ",
};

// clang-format off
const uint32_t transitionMap[][4] = {
    {1, 0, 0, 0},
    {2, 3, 4, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 0, 0, 5},
    {4, 0, 0, 0},
};

std::string dialogMap[] = {
    "",
    "dialog/hello\ndialog/explanation1\ndialog/explanation2\ndialog/explanation3\ndialog/explanation4\ndialog/explanation5\ndialog/explanation6\n",
    "dialog/hello-IC\n",
    "dialog/dilemma\n",
    "dialog/coyoteTime\n",
    "",
};
// clang-format on
#pragma endregion LevelData
#pragma region Interface
bool Level::s_WithIC = false;

Level::Level(uint32_t index) {
  level = &levels[index];
  m_Width = level->find('W') + 1;
  m_Height = level->length() / m_Width;
  for (int x = 0; x < m_Width; x++) {
    for (int y = 0; y < m_Height; y++) {
      if (getTile(x, y) == 'P') player = Player(vec2f(x, y - PLAYER_SIZE.y));
    }
  }
}

char Level::getTile(int x, int y) const {
  if (!inRange<int>(x, 0, m_Width) || !inRange<int>(y, 0, m_Height)) return ' ';
  return level->at(x + y * m_Width);
}

void Level::setTile(int x, int y, char tile) {
  if (!inRange<int>(x, 0, m_Width) || !inRange<int>(y, 0, m_Height)) return;
  level->at(x + y * m_Width) = tile;
}

uint32_t Level::getTransition(char portal) const {
  if (portal == 'G') return transitionMap[nextLevel][0];
  else if (portal == 'Y') return transitionMap[nextLevel][1];
  else if (portal == 'R') return transitionMap[nextLevel][2];
  else if (portal == 'B') return transitionMap[nextLevel][3];
  return 0;
}
#pragma endregion Interface
#pragma region Draw
static MvImage tileset = MvImage("Assets/tileset.png");
static MvImage talk = MvImage("Assets/Talk.png");
static std::map<std::string, MvImage*> dialogs;

void Level::drawQuater(MvWindow& window, vec2i screen, vec2i pos, vec2i atlas, vec2i off, const char* accepted) const {
  vec2i delta = off * 2.f - 1.f;
  bool horizontal = strchr(accepted, getTile(pos + delta * vec2i(1, 0)));
  bool vertical = strchr(accepted, getTile(pos + delta * vec2i(0, 1)));
  bool diagonal = strchr(accepted, getTile(pos + delta));

  if (!horizontal) atlas.x += vertical ? 1 : 4;
  else if (!vertical) atlas.x += 3;
  else atlas.x += diagonal ? 0 : 2;

  window.drawImage(tileset, screen + off * tilesize / 2, tilesize / 2, atlas * 16 + off * 8, 8);
}

void Level::drawPatch(MvWindow& window, vec2i screen, vec2i pos, vec2i atlas, const char* accepted) const {
  drawQuater(window, screen, pos, atlas, vec2i(0, 0), accepted);
  drawQuater(window, screen, pos, atlas, vec2i(1, 0), accepted);
  drawQuater(window, screen, pos, atlas, vec2i(0, 1), accepted);
  drawQuater(window, screen, pos, atlas, vec2i(1, 1), accepted);
}

void Level::draw(MvWindow& window) {
  for (uint32_t x = camera.x / tilesize; x <= (camera.x + window.width()) / tilesize; x++) {
    for (uint32_t y = camera.y / tilesize; y <= (camera.y + window.height()) / tilesize; y++) {
      vec2i tile = vec2i(x, y), screen = tile * tilesize - camera;
      if (getTile(x, y) == 'G' && nextLevel == 5 && s_WithIC && player.allInPlace()) setTile(x, y, ' ');
      if (getTile(x, y) == '#') drawPatch(window, screen, tile, vec2i(0, 0), "#");
      else if (getTile(x, y) == 'G') window.drawImage(tileset, screen, tilesize, vec2i(0, 1) * 16, 16);
      else if (getTile(x, y) == 'Y') window.drawImage(tileset, screen, tilesize, vec2i(1, 1) * 16, 16);
      else if (getTile(x, y) == 'R') window.drawImage(tileset, screen, tilesize, vec2i(2, 1) * 16, 16);
      else if (getTile(x, y) == 'B') window.drawImage(tileset, screen, tilesize, vec2i(3, 1) * 16, 16);
      else if (getTile(x, y) == 'H') window.drawImage(tileset, screen, tilesize, vec2i(0, 3) * 16, 16);
      else if (getTile(x, y) == 'I') window.drawImage(tileset, screen, tilesize, vec2i(4, 1) * 16, 16);
      else if (getTile(x, y) == 'i' && s_WithIC) window.drawImage(tileset, screen, tilesize, vec2i(4, 1) * 16, 16);
      else if (getTile(x, y) == 'g' && s_WithIC) window.drawImage(tileset, screen, tilesize, vec2i(0, 2) * 16, 16);
      else if (getTile(x, y) == 'y' && s_WithIC) window.drawImage(tileset, screen, tilesize, vec2i(1, 2) * 16, 16);
      else if (getTile(x, y) == 'r' && s_WithIC) window.drawImage(tileset, screen, tilesize, vec2i(2, 2) * 16, 16);
      else if (getTile(x, y) == 'b' && s_WithIC) window.drawImage(tileset, screen, tilesize, vec2i(3, 2) * 16, 16);
      else if (getTile(x, y) == 'A') {
        static float timer = 1.5f;
        timer -= Mova::deltaTime();
        window.drawImage(tileset, screen, tilesize, vec2i(1 + (timer <= 0.1f), 3) * 16, 16);
        if (timer < 0.f) timer = rand() / (float)RAND_MAX * 3.f + 0.5f;
      } else if (getTile(x, y) == 'O') {
        if (s_WithIC) {
          setTile(x, y, 'I');
        } else {
          setTile(x, y, 'H');
          setTile(x, y - 1, 'A');
        }
      }
    }
  }
  if (nextLevel == 5) {
    if (s_WithIC && player.allInPlace()) dialogMap[nextLevel] = "dialog/youDidIt";
    else if(!s_WithIC) dialogMap[nextLevel] = "dialog/everythingHasConsequences";
  }
  char tileOnMouse = getTile((Mova::getMousePosition() + camera) / tilesize);
  if (strchr("AI", tileOnMouse) || tileOnMouse == 'i' && s_WithIC) {
    if (dialog.empty()) {
      window.drawImage(talk, Mova::getMousePosition() - vec2i(0, tilesize), talk.size() * tilesize / talk.height(), 0, talk.size());
    }
    if (Mova::isMouseButtonPressed(MvMouseRight)) {
      uint32_t start = dialog.empty() ? 0 : dialogMap[nextLevel].find(dialog) + dialog.size() + 1;
      if (start >= dialogMap[nextLevel].size()) dialog = "";
      else {
        uint32_t end = dialogMap[nextLevel].find('\n', start);
        dialog = dialogMap[nextLevel].substr(start, end - start);
      }
    }
  }
  if (!dialog.empty()) {
    if (!dialogs.count(dialog)) dialogs[dialog] = new MvImage("Assets/" + dialog + ".png");
    for (int x = 0; x < m_Width; x++) {
      for (int y = 0; y < m_Height; y++) {
        vec2i tile = vec2i(x, y), screen = tile * tilesize - camera;
        if (strchr("AIi", getTile(x, y))) {
          int height = (dialogs[dialog]->height() > 50 ? tilesize * 4 : tilesize);
          vec2i dialogSize = dialogs[dialog]->size() * height / dialogs[dialog]->height();
          screen += vec2i(tilesize, -height);

          window.drawImage(*dialogs[dialog], screen, dialogSize);
          if (dialog == "dialog/hello-IC") {
            s_WithIC = Mova::getMousePosition().x < screen.x + dialogSize.x / 2;
            if (s_WithIC) {
              window.drawLine(screen + vec2i(dialogSize.x * 48 / 148, height), screen + vec2i(dialogSize.x * 64 / 148, height), MvColor::green);
            } else {
              window.drawLine(screen + vec2i(dialogSize.x * 85 / 148, height), screen + vec2i(dialogSize.x * 97 / 148, height), MvColor::red);
            }
            if (Mova::isMouseButtonPressed(MvMouseLeft)) setTile(tile, ' '), dialog = "";
          }
        }
      }
    }
  }
}
#pragma endregion Draw
