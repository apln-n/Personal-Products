#pragma once

void qmul(double r[4], const double p[4], const double q[4]);
void qrot(double r[16], const double q[4]);
void toEular(double ypr[3], const double quat[4]);
void toQuaternion(double quat[4], const double ypr[3]);
