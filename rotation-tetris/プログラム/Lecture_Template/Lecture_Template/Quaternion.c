#include "Quaternion.h"

#include <stdio.h>
#include <math.h>

void qmul(double r[4], const double p[4], const double q[4]) {
  r[0] = p[0]*q[0] - p[1]*q[1] - p[2]*q[2] - p[3]*q[3];
  r[1] = p[0]*q[1] + p[1]*q[0] + p[2]*q[3] - p[3]*q[2];
  r[2] = p[0]*q[2] - p[1]*q[3] + p[2]*q[0] + p[3]*q[1];
  r[3] = p[0]*q[3] + p[1]*q[2] - p[2]*q[1] + p[3]*q[0];
}

void qrot(double r[16], const double q[4]) {
  const double x2 = q[1]*q[1]*2.;
  const double y2 = q[2]*q[2]*2.;
  const double z2 = q[3]*q[3]*2.;
  const double xy = q[1]*q[2]*2.;
  const double yz = q[2]*q[3]*2.;
  const double zx = q[3]*q[1]*2.;
  const double xw = q[1]*q[0]*2.;
  const double yw = q[2]*q[0]*2.;
  const double zw = q[3]*q[0]*2.;

  r[0] = 1.-y2-z2;
  r[1] = xy+zw;
  r[2] = zx-yw;
  r[4] = xy-zw;
  r[5] = 1.-z2-x2;
  r[6] = yz+xw;
  r[8] = zx+yw;
  r[9] = yz-xw;
  r[10] = 1.-x2-y2;
  r[3] = r[7] = r[11] = r[12] = r[13] = r[14] = 0.;
  r[15] = 1.;
}

static int max(const int n, const double *q) {
  int i, max=0;
  for(i=1; i<n; ++i) {
    if(q[max]<q[i]) max=i;
  }
  return max;
}

/*** no testing ***/
void qirot(double q[4], const double r[16]) {
  const double x = sqrt(r[0]-r[5]-r[10]+1)/2.;
  const double y = sqrt(-r[0]+r[5]-r[10]+1)/2.;
  const double z = sqrt(-r[0]-r[5]+r[10]+1)/2.;
  const double w = sqrt(r[0]+r[5]+r[10]+1)/2.;
  q[0] = x; q[1] = y; q[2] = z; q[3] = w;
  switch(max(4,q)) {
  case 0:
    q[0]=x;
    q[1]=(r[1]+r[4])/(4.*x);
    q[2]=(r[8]+r[2])/(4.*x);
    q[3]=(r[6]-r[9])/(4.*x);
    break;
  case 1:
    q[0]=(r[1]+r[4])/(4.*y);
    q[1]=y;
    q[2]=(r[6]+r[9])/(4.*y);
    q[3]=(r[8]-r[2])/(4.*y);
    break;
  case 2:
    q[0]=(r[8]+r[2])/(4.*z);
    q[1]=(r[6]+r[9])/(4.*z);
    q[2]=z;
    q[3]=(r[1]-r[4])/(4.*z);
    break;
  case 3:
    q[0]=(r[6]-r[9])/(4.*w);
    q[1]=(r[8]-r[2])/(4.*w);
    q[2]=(r[1]-r[4])/(4.*w);
    q[3]=w;
    break;
  default:
    break;
  }
}

void toEular(double ypr[3], const double quat[4]) {
  double yaw, pitch, roll;
  double yaw_a = (quat[0]*quat[3]+quat[1]*quat[2])*2.;
  double yaw_b = 1.-(quat[2]*quat[2]-quat[3]*quat[3])*2.;
  double pitch_a = (quat[0]*quat[2]-quat[1]*quat[3])*2.;
  double roll_a = (quat[0]*quat[1]+quat[2]*quat[3])*2.;
  double roll_b = 1.-(quat[1]*quat[1]+quat[2]*quat[2])*2.;
  pitch_a = pitch_a>1.?1.:pitch_a;
  pitch_a = pitch_a<-1.?-1.:pitch_a;
  yaw = atan2(yaw_a,yaw_b);
  pitch = asin(pitch_a);
  roll = atan2(roll_a,roll_b);
  
  ypr[0]=yaw;
  ypr[1]=pitch;
  ypr[2]=roll;

  printf("yaw:%lf\n",yaw);
  printf("pitch:%lf\n",pitch);
  printf("roll:%lf\n",roll);
}
void toQuaternion(double quat[4], const double ypr[3]) {
  double cy = cos(ypr[0]/2.);
  double sy = sin(ypr[0]/2.);
  double cp = cos(ypr[1]/2.);
  double sp = sin(ypr[1]/2.);
  double cr = cos(ypr[2]/2.);
  double sr = sin(ypr[2]/2.);
  quat[0]=cy*cp*cr+sy*sp*sr;
  quat[1]=cy*cp*sr-sy*sp*cr;
  quat[2]=cy*sp*cr+sy*cp*sr;
  quat[3]=sy*cp*cr-cy*sp*sr;
}
