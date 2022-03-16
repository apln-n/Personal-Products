#pragma once

/*** The following are the drawing functions ***/
void drawLine2I(const int start1, const int start2,
	       const int end1, const int end2);
void drawLine2D(const double startX, const double startY,
		const double endX, const double endY);
void drawLine2DW(const double startX, const double startY,
		const double endX, const double endY,
		const double width);
void drawLine3D(const double startX, const double startY, const double startZ,
		const double endX, const double endY, const double endZ);
void drawLine2IV(const int start[2], const int end[2]);
void drawLine2IVW(const int start[2], const int end[2], const int width);
void drawLine2DV(const double start[2], const double end[2]);
void drawLine2DVW(const double start[2], const double end[2], const double width);
void SetLineWidth(const double w);
double GetLineWidth();
void drawCircle2D(const double centerX, const double centerY,
		  const double radius);
void drawCircle2DV(const double center[2], const double radius);

void drawTriangle2I(const int a0, const int a1,
		   const int b0, const int b1,
		   const int c0, const int c1);
void drawTriangle2IV(const int a[2], const int b[2], const int c[2]);
void drawTriangle2D(const double a0, const double a1,
		   const double b0, const double b1,
		   const double c0, const double c1);
void drawTriangle2DV(const double a[2], const double b[2], const double c[2]);
void drawSquare2I(const int a0, const int a1,
		  const int b0, const int b1,
		  const int c0, const int c1,
		  const int d0, const int d1);
void drawLines2D(const int num, const double *vecx, const double *vecy);

void rotateD(const double angle, const double x, const double y, const double z);
void translate2D(const double x, const double y);
void translate3D(const double x, const double y, const double z);
void pushMat();
void popMat();

void clearColor(const double color[4]);
void setColor(const double color[4]);
void setWindowTitle(const char *title);
double getFPS();
void BugResize(const int w, const int h);
void fixedWindowSize();
void fixedWindowAspect(const int w, const int h);
void OrthogonalProjection(const double left, const double right,
			  const double bottom, const double top,
			  const double near, const double far);
void PerspectiveProjection(const double left, const double right,
			   const double bottom, const double top,
			   const double near, const double far);

void activeRotation();
void DrawTeapot(const double scale);
void DrawWireTeapot(const double scale);

/*** definition of OpenGL ***/
typedef struct {
  int height;
  int width;
} WindowSize;

void GLUTCreateWindow(int argc, char *argv[], const WindowSize *window);
void GLUTStartLoop();
WindowSize getWindowInfo();

void drawRubberband();

