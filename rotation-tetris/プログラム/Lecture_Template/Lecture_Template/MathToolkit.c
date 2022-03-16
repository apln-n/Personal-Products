#include <math.h>

double min(const double a, const double b) {
  return a<b?a:b;
}
double max(const double a, const double b) {
  return a>b?a:b;
}
double absMin(const double a, const double absB) {
  return 0<=absB?min(a,absB):-min(a,fabs(absB));
}
double absMax(const double a, const double absB) {
  return 0<=absB?max(a,absB):-max(a,fabs(absB));
}
double minus(const double x) {
  return x<0.?x:-x;
}
double plus(const double x) {
  return x<0.?-x:x;
}

double dot2D(const double ax, const double ay, const double bx, const double by) {
  return ax*bx+ay*by;
}
double dot2DV(const double a[2], const double b[2]) {
  return a[0]*b[0]+a[1]*b[1];
}
double cross2D(const double ax, const double ay, const double bx, const double by) {
  return ax*by-ay*bx;
}
double cross2DV(const double a[2], const double b[2]) {
  return a[0]*b[1]-a[1]*b[0];
}
double abs2DV(const double a[2]) {
  return sqrt(dot2DV(a,a));
}

double abs2D(const double x, const double y) {
  /*  const double a[2] = {x, y}; */
  /*  double a[2] = {x, y}; */
  return sqrt(dot2D(x,y,x,y));
}

double distance2D(const double x1, const double y1, const double x2, const double y2) {
  const double dx = x2-x1;
  const double dy = y2-y1;
  /*  const double d[2] = {x2-x1, y2-y1}; */
  /*  return abs2DV(d); */
  return abs2D(dx,dy);
}
double distance2DV(const double a[2], const double b[2]) {
  return distance2D(a[0], a[1], b[0], b[1]);
}

int almostZero(const double x) {
  const double eps = 1.e-8;
  return (fabs(x) > eps) ? 0 : 1;
}
