#include <GL/glut.h>
#include <math.h>
#define PI2 (3.14159265358979323846 * 2.0)

/*
 * $B1_Cl$rIA$/(B
 *   radius: $BH>7B(B
 *   height: $B9b$5(B
 *   sides: $BB&LL$N?t!J?t$,B?$$$[$I3j$i$+$K$J$k!K(B
 *
 */

void myCylinder(double radius, double height, int sides)
{
  double step = PI2 / (double)sides;
  int i;

  /* $B>eLL(B */
  glNormal3d(0.0, 1.0, 0.0);
  glBegin(GL_TRIANGLE_FAN);
  for (i = 0; i < sides; i++) {
    double t = step * (double)i;
    glVertex3d(radius * sin(t), height, radius * cos(t));
  }
  glEnd();

  /* $BDlLL(B */
  glNormal3d(0.0, -1.0, 0.0);
  glBegin(GL_TRIANGLE_FAN);
  for (i = sides; --i >= 0;) {
    double t = step * (double)i;
    glVertex3d(radius * sin(t), 0.0, radius * cos(t));
  }
  glEnd();

  /* $BB&LL(B */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= sides; i++) {
    double t = step * (double)i;
    double x = sin(t);
    double z = cos(t);

    glNormal3d(x, 0.0, z);
    glVertex3f(radius * x, height, radius * z);
    glVertex3f(radius * x, 0.0, radius * z);
  }
  glEnd();
}

