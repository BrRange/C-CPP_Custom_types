#ifndef QUATERNIONH
#define QUATERNIONH

#include <math.h>

union Quaternion{
  float coord[4];
  struct{
    float r, x, y, z;
  };
};
typedef union Quaternion Quaternion;

union Point3{
  float coord[3];
  struct{
    float x, y, z;
  };
};
typedef union Point3 Point3;

Point3 point3Normal(float x, float y, float z){
  float scale = hypotf(x, hypotf(y, z));
  return (Point3){x / scale, y / scale, z / scale};
}

Quaternion quatRotation(float angle, Point3 normAxis){
  Quaternion quat;
  angle /= 2.f;
  sincosf(angle, &angle, &quat.r);
  quat.x = normAxis.x * angle;
  quat.y = normAxis.y * angle;
  quat.z = normAxis.z * angle;
  return quat;
}

void quatCompose(Quaternion *quat, Quaternion val){
  Quaternion refCopy = *quat;
  quat->r = refCopy.r * val.r - refCopy.x * val.x - refCopy.y * val.y - refCopy.z * val.z;
  quat->x = refCopy.r * val.x + refCopy.x * val.r + refCopy.y * val.z - refCopy.z * val.y;
  quat->y = refCopy.r * val.y - refCopy.x * val.z + refCopy.y * val.r + refCopy.z * val.x;
  quat->z = refCopy.r * val.z + refCopy.x * val.y - refCopy.y * val.x + refCopy.z * val.r;
}

Quaternion quatConjugate(Quaternion quat){
  quat.x *= -1.f;
  quat.y *= -1.f;
  quat.z *= -1.f;
  return quat;
}

Quaternion quatFromPoint3(Point3 point){
  return (Quaternion){0.f, point.x, point.y, point.z};
}

Point3 point3FromQuat(Quaternion quat){
  return (Point3){quat.x, quat.y, quat.z};
}

Point3 point3Rotate(Point3 point, Quaternion quat){
  Quaternion conjugate = quatConjugate(quat);
  quatCompose(&quat, quatFromPoint3(point));
  quatCompose(&quat, conjugate);
  return point3FromQuat(quat);
}

#endif