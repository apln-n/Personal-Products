#pragma once

double min(const double a, const double b);
double max(const double a, const double b);
double absMin(const double a, const double absB);
double absMax(const double a, const double absB);
double minus(const double x);
double plus(const double x);
/* double distance(const double x1, const double y1, const double x2, const double y2); */

double dot2D(const double ax, const double ay, const double bx, const double by);
double dot2DV(const double a[2], const double b[2]);
double cross2D(const double ax, const double ay, const double bx, const double by);
double cross2DV(const double a[2], const double b[2]);
double abs2DV(const double a[2]);
double abs2D(const double x, const double y);
double distance2D(const double x1, const double y1, const double x2, const double y2);
double distance2DV(const double a[2], const double b[2]);

int almostZero(const double x);
