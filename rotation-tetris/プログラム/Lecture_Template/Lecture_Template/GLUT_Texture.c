#include "GLUT_Texture.h"

#include "GLUT_Core.h"
#include "ColorSamples.h"

#include "GL/freeglut.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libraries/stb_image.h"

unsigned int genTexture(const char *name) {
  int width = 0, height = 0;
  int comp = 0;
  GLuint id = 0;

  unsigned char *image = stbi_load(name, &width, &height, &comp, STBI_rgb_alpha);

  if(image == NULL) {
    fprintf(stderr, "Failed to load texture.\n");
    exit(1);
  }

  glGenTextures(1,&id);

  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  if(comp == 3) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  }else if(comp == 4) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(image);

  return id;
}

void delTexture(unsigned int id) {
  glDeleteTextures(1, &id);
}

void drawTexture2I(const unsigned int id,
		   const int x1, const int y1,
		   const int x2, const int y2,
		   const int x3, const int y3,
		   const int x4, const int y4) {
  setColor(white);
  glBindTexture(GL_TEXTURE_2D, id);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 1); glVertex2i(x1, y1);
  glTexCoord2f(1, 1); glVertex2i(x2, y2);
  glTexCoord2f(1, 0); glVertex2i(x3, y3);
  glTexCoord2f(0, 0); glVertex2i(x4, y4);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}
void drawTexture2D(const unsigned int id,
		   const double x1, const double y1,
		   const double x2, const double y2,
		   const double x3, const double y3,
		   const double x4, const double y4) {
  setColor(white);
  glBindTexture(GL_TEXTURE_2D, id);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  glBegin(GL_QUADS);
  glTexCoord2f(0, 1); glVertex2d(x1, y1);
  glTexCoord2f(1, 1); glVertex2d(x2, y2);
  glTexCoord2f(1, 0); glVertex2d(x3, y3);
  glTexCoord2f(0, 0); glVertex2d(x4, y4);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}
