#include "GLUT_Core.h"

#include "MathToolkit.h"
#include "Boolean.h"
#include "ColorSamples.h"
#include "Sleep.h"
#include "Quaternion.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "GL/freeglut.h"
#include <stdio.h>

#if defined(_WIN64) || defined(_WIN32)
#include "KillFuckingWinMacro.h"
#endif

void clearColor(const double color[4]) {
  glClearColor((GLfloat)color[0],(GLfloat)color[1],
	       (GLfloat)color[2],(GLfloat)color[3]);
}

void setColor(const double color[4]) {
  glColor4d((GLdouble)color[0],(GLdouble)color[1],
	    (GLdouble)color[2],(GLdouble)color[3]);
}

void setWindowTitle(const char *title) {
  glutSetWindowTitle(title);
}

static int lastMouseLeftDown[2];
static int currentMouse[2];
static int activeRubberband = 0;
static double diagonal = 0.;
static double lineMax = 0.;
/* for calculating the fps */
static double fps = 0.;
//static unsigned int frame = 0;
//static int baseTime = 0;
static double fixedFPS = 30;//60;//10;//30;
/* for rotation */
static int rotating = 0;
static double cq[4] = {1., 0., 0., 0.};
static double tq[4];
static double rt[16];

/*** called at once in the display callback for updating FPS ***/
void updateFPS() {
  static int old = 0; /* millisecond */
  const int current = glutGet(GLUT_ELAPSED_TIME); /* millisecond */
  fps=1000./(double)(current-old);
  old=current;
}

/*** Implementation of OpenGL Callback functions ***/
//const static WindowSize defaultWindow = {240, 360};
const static WindowSize defaultWindow = {360, 360};
static WindowSize currentWindow;
extern void callback_display(void);
extern void callback_resize(const int w, const int h);
extern void callback_mouse(const int button, const int state,
		  const int x, const int y);
extern void callback_motion(const int x, const int y);
extern void callback_keyboard(const unsigned char key, const int x, const int y);
extern void callback_idle(void);
extern void callback_initialization(void);
extern void callback_finalization(void);



void updateWindowInfo(WindowSize *win) {
  glFlush();
  win->width = glutGet(GLUT_WINDOW_X);
  win->height = glutGet(GLUT_WINDOW_Y);
}

void OrthogonalProjection(const double left, const double right,
			const double bottom, const double top,
			const double near, const double far) {
  WindowSize win = getWindowInfo();
  glViewport(0,0,win.width,win.height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(left,right,bottom,top,near,far);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void PerspectiveProjection(const double left, const double right,
			const double bottom, const double top,
			const double near, const double far) {
  WindowSize win = getWindowInfo();
  glViewport(0,0,win.width,win.height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(left,right,bottom,top,near,far);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void activeRotation() {
  rotating = 1;
}

void DrawTeapot(const double scale) {
  glutSolidTeapot(scale);
}
void DrawWireTeapot(const double scale) {
  glutWireTeapot(scale);
}

void GLUTDisplayCallbackFunction(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  if(rotating)
    glMultMatrixd(rt);
  
  callback_display();
  updateFPS();

  glutSwapBuffers();
}
void GLUTReshapeCallbackFunction(const int w, const int h) {
  callback_resize(w, h);
  glutPostRedisplay();
}
void BugResize(const int w, const int h) {
  glutReshapeWindow(w-1,h-1);
}
void fixedWindowSize() {
  glutReshapeWindow(currentWindow.width,currentWindow.height);
}
void fixedWindowAspect(const int w, const int h) {
  const double aspect = (double)currentWindow.width/(double)currentWindow.height;
  const double newAspect = (double)w/(double)h;
  const int diffW = currentWindow.width-w;
  const int diffH = currentWindow.height-h;
  const double margin = 0.1;
  int newWidth = 0, newHeight = 0;
  if(fabs(aspect-newAspect) > margin){
    if(abs(diffW) > abs(diffH)) {
      newWidth = w; newHeight = (int)(w/aspect);
    }else{
      newWidth = (int)(h*aspect); newHeight = h;
    }
    glutReshapeWindow(newWidth,newHeight);
    currentWindow.width = newWidth;
    currentWindow.height = newHeight;
  }
}
void GLUTMouseCallbackFunction(const int button, const int state,
			       const int x, const int y) {
  switch(button) {
  case GLUT_LEFT_BUTTON:
    if(state == GLUT_DOWN) {
      lastMouseLeftDown[0] = x;
      lastMouseLeftDown[1] = y;
      currentMouse[0] = x;
      currentMouse[1] = y;
      activeRubberband = 1;
    } else {
      activeRubberband = 0;
      /* for rotation by mouse */
      cq[0]=tq[0]; cq[1]=tq[1]; cq[2]=tq[2]; cq[3]=tq[3];
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;
  default:
    break;
  }
  callback_mouse(button, state, x, y);
}
void GLUTMotionCallbackFunction(const int x, const int y) {
  const double dx = (double)(x-lastMouseLeftDown[0])/(double)(currentWindow.width);
  const double dy = (double)(y-lastMouseLeftDown[1])/(double)(currentWindow.height);
  const double a = sqrt(dx*dx+dy*dy);
  currentMouse[0] = x;
  currentMouse[1] = y;
  callback_motion(x, y);
  if(a != 0.) {
    double ar = a*2.*M_PI*0.5;
    double as = sin(ar)/a;
    double dq[4] = {cos(ar), dy*as, dx*as, 0.};
    qmul(tq, dq, cq);
    //    double e[3];
    //    toEular(e,tq);
    //    toQuaternion(tq,e);
    qrot(rt, tq);
  }
}
void GLUTKeyboardCallbackFunction(const unsigned char key, const int x, const int y) {
  callback_keyboard(key, x, y);
}
void GLUTIdleCallbackFunction(void) {
  static double stime = 1.;
  callback_idle();

  //  if(fps>fixedFPS){
  //    stime=(fps-fixedFPS)/(fps*fixedFPS)*1000.;
  //    sleep_ms((int)stime);
  //  }
  stime+=(fps-fixedFPS)/(fps*fixedFPS)*1000.;
  stime=stime<1.?1.:stime;
  sleep_ms((int)stime);

  glutPostRedisplay();
}

void GLUTInitialization(int argc, char *argv[]) {
  atexit(callback_finalization);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  /* for rotation by mouse */
  qrot(rt, cq);
}

WindowSize getWindowInfo() {
  return currentWindow;
}

double getFPS() {
  return fps;
}

void GLUTCreateWindow(int argc, char *argv[], const WindowSize *window) {
  static bool initialized = false;
  const WindowSize *win = (window==NULL)? &defaultWindow : window;
  currentWindow = *win;
  diagonal = distance2D(0., 0., win->width, win->height);
  lineMax = min(win->width,win->height)*0.1;

  /* initializing at once */
  if(!initialized) {
    GLUTInitialization(argc,argv);
    initialized=true;
  }
  
  glutInitWindowSize(win->width, win->height);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(GLUTDisplayCallbackFunction);
  glutReshapeFunc(GLUTReshapeCallbackFunction);
  glutMouseFunc(GLUTMouseCallbackFunction);
  glutMotionFunc(GLUTMotionCallbackFunction);
  glutKeyboardFunc(GLUTKeyboardCallbackFunction);
  glutIdleFunc(GLUTIdleCallbackFunction);

  /*** for drawing texture ***/
  /*  glEnable(GL_BLEND);*/
  /*  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);*/
  /*  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
  /*  glBlendFunc(GL_SRC_ALPHA, GL_ONE);*/
  /*  glBlendFunc(GL_ONE, GL_ONE);*/
  /*
  glAlphaFunc(GL_GREATER, 0.);
  glEnable(GL_ALPHA_TEST);
  */

  callback_initialization();
}

void GLUTStartLoop() {
  glutMainLoop();
}

void drawLine2I(const int start1, const int start2, const int end1, const int end2) {
  glBegin(GL_LINES);
  glVertex2i((GLint)start1, (GLint)start2);
  glVertex2i((GLint)end1, (GLint)end2);
  glEnd();
}
void drawLine2D(const double startX, const double startY, const double endX, const double endY) {
  glBegin(GL_LINES);
  glVertex2d((GLdouble)startX, (GLdouble)startY);
  glVertex2d((GLdouble)endX, (GLdouble)endY);
  glEnd();
}
void drawLine2DW(const double startX, const double startY, const double endX, const double endY, const double width) {
  const double hw = width/2.;
  const double dy = endY-startY;
  const double dx = endX-startX;
  const double wx = (dy==0)?0.:sqrt(hw*hw*(1.+(dx*dx)/(dy*dy)));
  const double wy = (dx==0)?0.:sqrt(hw*hw*(1.+(dy*dy)/(dx*dx)));
  glBegin(GL_QUADS);
  glVertex2d(startX-wx,startY+wy);
  glVertex2d(startX+wx,startY-wy);
  glVertex2d(endX+wx,endY-wy);
  glVertex2d(endX-wx,endY+wy);
  glEnd();
}
void drawLine3D(const double startX, const double startY, const double startZ,
		const double endX, const double endY, const double endZ) {
  glBegin(GL_LINES);
  glVertex3d((GLdouble)startX, (GLdouble)startY, (GLdouble)startZ);
  glVertex3d((GLdouble)endX, (GLdouble)endY, (GLdouble)endZ);
  glEnd();
}
void drawLine2IV(const GLint start[2], const GLint end[2]) {
  drawLine2I(start[0], start[1], end[0], end[1]);
}
void drawLine2IVW(const int start[2], const int end[2], const int width) {
  const double dist = distance2D(start[0], start[1], end[0], end[1]);
  const double ratio = (double)width*0.5/dist;
  const int dx = (int)absMax(1., (double)(end[0]-start[0])*ratio);
  const int dy = (int)absMax(1., (double)(end[1]-start[1])*ratio);
  drawSquare2I(start[0]-dy,start[1]+dx,
	       start[0]+dy,start[1]-dx,
	       end[0]+dy,end[1]-dx,
	       end[0]-dy,end[1]+dx);
  //  glBegin(GL_QUADS);
  //  glVertex2i(start[0]-dy,start[1]+dx);
  //  glVertex2i(start[0]+dy,start[1]-dx);
  //  glVertex2i(end[0]+dy,end[1]-dx);
  //  glVertex2i(end[0]-dy,end[1]+dx);
  //  glEnd();
}
void drawLine2DV(const double start[2], const double end[2]) {
  drawLine2D(start[0], start[1], end[0], end[1]);
}
void drawLine2DVW(const double start[2], const double end[2], const double width) {
  drawLine2DW(start[0],start[1],end[0],end[1],width);
  /*
  const double dist = distance2D(start[0], start[1], end[0], end[1]);
  //  const double ratio = width*0.5/dist;
  //  const GLdouble dx = (GLint)absMax(1., (double)(end[0]-start[0])*ratio);
  //  const GLdouble dy = (GLint)absMax(1., (double)(end[1]-start[1])*ratio);
  const double slope = 1./((end[1]-start[1])/(end[0]-start[0]));
  const double dx = width*0.5/(1+slope);
  const double dy = dx*slope;
  printf("dist= %f\n",sqrt(dx*dx+dy*dy));
  glBegin(GL_QUADS);
  glVertex2i(start[0]-dy,start[1]+dx);
  glVertex2i(start[0]+dy,start[1]-dx);
  glVertex2i(end[0]+dy,end[1]-dx);
  glVertex2i(end[0]-dy,end[1]+dx);
  glEnd();
  */
}
void SetLineWidth(const double w) {
  glLineWidth((GLfloat)w);
}
double GetLineWidth() {
  double lw = 0.;
  glGetDoublev(GL_LINE_WIDTH, &lw);
  return lw;
}

void drawCircle2D(const double centerX, const double centerY,
		  const double radius) {
  //void drawCircle2DV(const double center[2], const double radius) {
  int i;
  /* double inter = 90./(double)radius; */
  /* int num = (int)(360/inter); */
  /*  int num = (int)(radius*1.);*/
  int num = currentWindow.width/50+3;
  double inter = 360./(double)num;
  /*
  float th1 = 0.;
  float th2 = 0.;
  GLint x1 = 0;
  GLint x2 = 0;
  GLint y1 = 0;
  GLint y2 = 0;
  */
  for(i=0; i<num; i++) {
    const double th1 = (double)(i)*inter/180.*M_PI;
    const double th2 = (double)(i+1)*inter/180.*M_PI;
    /* not nessecary to ceil in local coordinate */
    /* (in this case, coordinates are casted to Integer) */
    //    const GLint x1 = (GLint)ceil((float)radius*cos(th1));
    //    const GLint x2 = (GLint)ceil((float)radius*cos(th2));
    //    const GLint y1 = (GLint)ceil((float)radius*sin(th1));
    //    const GLint y2 = (GLint)ceil((float)radius*sin(th2));
    const double x1 = (double)radius*cos(th1);
    const double x2 = (double)radius*cos(th2);
    const double y1 = (double)radius*sin(th1);
    const double y2 = (double)radius*sin(th2);
    //    glBegin(GL_TRIANGLES);
    //    glVertex2d(center[0],center[1]);
    //    glVertex2d(x1+center[0],y1+center[1]);
    //    glVertex2d(x2+center[0],y2+center[1]);
    //    glEnd();
    drawTriangle2D(centerX,centerY,
		   x1+centerX,y1+centerY,
		   x2+centerX,y2+centerY);
  }
}
void drawCircle2DV(const double center[2], const double radius) {
  drawCircle2D(center[0], center[1], radius);
}

void drawTriangle2I(const int a0, const int a1,
		   const int b0, const int b1,
		   const int c0, const int c1) {
  glBegin(GL_TRIANGLES);
  glVertex2i(a0,a1);
  glVertex2i(b0,b1);
  glVertex2i(c0,c1);
  glEnd();
}
void drawTriangle2IV(const int a[2], const int b[2], const int c[2]) {
  glBegin(GL_TRIANGLES);
  glVertex2i(a[0],a[1]);
  glVertex2i(b[0],b[1]);
  glVertex2i(c[0],c[1]);
  glEnd();
}
void drawTriangle2D(const double a0, const double a1,
		    const double b0, const double b1,
		    const double c0, const double c1) {
  glBegin(GL_TRIANGLES);
  glVertex2d(a0,a1);
  glVertex2d(b0,b1);
  glVertex2d(c0,c1);
  glEnd();
}
void drawTriangle2DV(const double a[2], const double b[2], const double c[2]) {
  glBegin(GL_TRIANGLES);
  glVertex2d(a[0],a[1]);
  glVertex2d(b[0],b[1]);
  glVertex2d(c[0],c[1]);
  glEnd();
}
void drawSquare2I(const int a0, const int a1,
		  const int b0, const int b1,
		  const int c0, const int c1,
		  const int d0, const int d1) {
  glBegin(GL_QUADS);
  glVertex2i(a0, a1);
  glVertex2i(b0, b1);
  glVertex2i(c0, c1);
  glVertex2i(d0, d1);
  glEnd();
}
void drawLines2D(const int num, const double *vecX, const double *vecY) {
  int i;
  glBegin(GL_LINE_STRIP);
  for(i=0; i<num; ++i) {
    glVertex2d(vecX[i], vecY[i]);
  }
  glEnd();
}

void rotateD(const double angle, const double x, const double y, const double z) {
  glRotated(angle*180.0/M_PI,x,y,z);
}
void translate2D(const double x, const double y) {
  glTranslated(x,y,0);
}
void translate3D(const double x, const double y, const double z) {
  glTranslated(x,y,z);
}
void pushMat() {
  glPushMatrix(); 
}
void popMat() {
  glPopMatrix();
}

void drawRubberband() {
  double dist = 0.;
  if(activeRubberband) {
    glEnable(GL_LINE_SMOOTH);
    dist = distance2D(lastMouseLeftDown[0],lastMouseLeftDown[1],
		      currentMouse[0],currentMouse[1]);
    //    drawLineWIV(lastMouseLeftDown,currentMouse,logisticR(dist/diagonal,lineMax,2.));
    drawLine2IVW(lastMouseLeftDown,currentMouse,(int)lineMax);
  }
}

