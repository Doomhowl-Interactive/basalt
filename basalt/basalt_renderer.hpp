#pragma once
#include "basalt_colors.hpp"

size_t GetNextTextureID();

void DrawDot(size_t id, int posX, int posY, Color tint);
void DrawLine(size_t id, int startX, int startY, int endX, int endY, Color tint);
void DrawRectangle(size_t id, int posX, int posY, int width, int height, Color tint);
void Blit(size_t id,
          size_t targetID,
          int posX,
          int posY,
          Color tint = WHITE,
          int srcX = -1,
          int srcY = -1,
          int srcWidth = -1,
          int srcHeight = -1);