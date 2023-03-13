#include <movaBackend.hpp>

using namespace Math;
using namespace VectorMath;

const vec2f PLAYER_SIZE = vec2f(1.f, 1.8f);
struct Player {
  vec2f pos, vel = 0;
  Player() = default;
  Player(vec2f pos) : pos(pos), m_Alive(true), m_Jumping(false), m_LastOnGrountTime(10.f) {}

  void update();
  void draw(MvWindow& window);
  inline bool isAlive() const { return m_Alive; }
  inline bool allInPlace() const { return s_GreenKey && s_YellowKey && s_RedKey && s_BlueKey; }

 private:
  static bool s_GreenKey, s_YellowKey, s_RedKey, s_BlueKey;
  float m_LastOnGrountTime;
  bool m_Jumping, m_Alive;

  bool collides(float dx, float dy);
};

struct Level {
  Level() = default;
  Level(uint32_t index);

  char getTile(int x, int y) const;
  void setTile(int x, int y, char tile);
  uint32_t getTransition(char portal) const;
  bool withIC() const { return s_WithIC; }

  inline char getTile(vec2i pos) const { return getTile(pos.x, pos.y); }
  inline void setTile(vec2i pos, char tile) { setTile(pos.x, pos.y, tile); }
  inline uint32_t width() const { return m_Width; }
  inline uint32_t height() const { return m_Height; }
  inline vec2i size() const { return vec2i(m_Width, m_Height); }

  void draw(MvWindow& window);

 private:
  static bool s_WithIC;
  std::string* level;
  std::string dialog = "";
  uint32_t m_Width, m_Height;

  void drawQuater(MvWindow& window, vec2i screen, vec2i pos, vec2i atlas, vec2i off, const char* accepted) const;
  void drawPatch(MvWindow& window, vec2i screen, vec2i pos, vec2i atlas, const char* accepted) const;
};

extern Player player;
extern Level level;
extern vec2i camera;
extern uint32_t tilesize;
extern uint32_t nextLevel;
