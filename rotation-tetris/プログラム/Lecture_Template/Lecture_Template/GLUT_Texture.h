#pragma once

unsigned int genTexture(const char *name);
void delTexture(unsigned int id);
void drawTexture2I(const unsigned id,
		   const int x1, const int y1,
		   const int x2, const int y2,
		   const int x3, const int y3,
		   const int x4, const int y4);
void drawTexture2D(const unsigned id,
		   const double x1, const double y1,
		   const double x2, const double y2,
		   const double x3, const double y3,
		   const double x4, const double y4);
