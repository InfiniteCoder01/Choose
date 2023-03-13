#include "00Names.hpp"

static MvImage playerTexture = MvImage("Assets/Player.png");
bool Player::s_GreenKey = false;
bool Player::s_YellowKey = false;
bool Player::s_RedKey = false;
bool Player::s_BlueKey = false;

void Player::update() {
  if (pos.y + PLAYER_SIZE.y >= level.height()) m_Alive = false;
  float speed = 7.f, gravity = 29.4f;

  if (m_LastOnGrountTime < 0.4f && Mova::isKeyHeld(MvKey::Space) && !m_Jumping) vel.y = -10.f, m_Jumping = true;
  if (m_Jumping && Mova::isKeyHeld(MvKey::Space)) speed *= 1.5f, gravity *= 0.5f;
  if (m_Jumping && Mova::isKeyReleased(MvKey::Space)) vel.y *= 0.6f;

  float targetVelocity = (Mova::isKeyHeld(MvKey::D) - Mova::isKeyHeld(MvKey::A)) * speed;
  vel.x += (targetVelocity - vel.x) * 10.f * Mova::deltaTime();
  vel.y += gravity * Mova::deltaTime();

  m_LastOnGrountTime += Mova::deltaTime();

  // Integration
  pos.x += vel.x * Mova::deltaTime();
  if (collides(sign(vel.x), 0)) {
    while (collides(sign(vel.x), 0)) pos.x -= sign(vel.x) / (float)tilesize;
    vel.x = 0;
  }

  pos.y += vel.y * Mova::deltaTime();
  if (collides(0, sign(vel.y))) {
    while (collides(0, sign(vel.y))) pos.y -= sign(vel.y) / (float)tilesize;
    if (vel.y > 0) m_LastOnGrountTime = 0, m_Jumping = false;
    vel.y = 0;
  }
}

bool Player::collides(float dx, float dy) {
  if (!m_Alive) return false;
  if (!inRange<float>(pos.x, 0, level.width() - PLAYER_SIZE.x) || pos.y < 0) return true;
  for (uint32_t x = pos.x; x <= pos.x + PLAYER_SIZE.x; x++) {
    for (uint32_t y = pos.y; y <= pos.y + PLAYER_SIZE.y; y++) {
      if (strchr("#", level.getTile(x, y))) return true;
      if (strchr("GYRB", level.getTile(x, y))) {
        nextLevel = level.getTransition(level.getTile(x, y));
        m_Alive = false;
        return true;
      } else if (strchr("gyrb", level.getTile(x, y)) && level.withIC()) {
        if(level.getTile(x, y) == 'g') s_GreenKey = true;
        else if(level.getTile(x, y) == 'y') s_YellowKey = true;
        else if(level.getTile(x, y) == 'r') s_RedKey = true;
        else if(level.getTile(x, y) == 'b') s_BlueKey = true;
        level.setTile(x, y, ' ');
        return true;
      }
    }
  }
  return false;
}

void Player::draw(MvWindow& window) {
  camera = pos * tilesize + PLAYER_SIZE * tilesize / 2 - window.size() / 2;
  camera = VectorMath::max(camera, vec2i(0));
  camera = VectorMath::min(camera, level.size() * tilesize - window.size());
  static float timer = 0;
  timer += Mova::deltaTime() * 13;
  if (timer >= 8) timer = 0;
  window.drawImage(playerTexture, (pos - vec2f(9 / 16.f, 0) * (vel.x < 0)) * tilesize - camera, vec2i(24 * (vel.x >= 0 ? 1 : -1.f), 29) * tilesize * PLAYER_SIZE.y / 29, (abs(vel.x) < 0.3 ? vec2i(0) : vec2i(24, 0) * (int)(timer + 1)), vec2i(24, 29));
}
