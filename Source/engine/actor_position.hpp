#pragma once

#include <cstdint>

#include "engine/point.hpp"
#include "engine/world_tile.hpp"

namespace devilution {

struct ActorPosition {
    Point tile;      // Tile position (grid coordinates)
    Point offset;    // Sub-tile offset in pixels
    Point velocity;  // Movement velocity

    void UpdatePosition() {
        offset += velocity;

        // Handle offset overflow/underflow
        while (offset.x >= TILE_WIDTH) {
            offset.x -= TILE_WIDTH;
            tile.x++;
        }
        while (offset.x < 0) {
            offset.x += TILE_WIDTH;
            tile.x--;
        }
        while (offset.y >= TILE_HEIGHT) {
            offset.y -= TILE_HEIGHT;
            tile.y++;
        }
        while (offset.y < 0) {
            offset.y += TILE_HEIGHT;
            tile.y--;
        }
    }
};

} // namespace devilution
