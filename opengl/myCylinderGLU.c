#include <GL/glut.h>
#include <stdio.h>

/*
 * $B%(%i!<%O%s%I%i(B
 */

static void errCallBack(GLenum errCode)
{
  const GLubyte *estring = gluErrorString(errCode);
  fprintf(stderr, "Quadric Error: %s\n", estring);
  exit(1);
}

/*
 * $B1_Cl$rIA$/(B
 *   radius: $BH>7B(B
 *   height: $B9b$5(B
 *   sides: $BB&LL$N?t!J?t$,B?$$$[$I3j$i$+$K$J$k!K(B
 *
 */

void myCylinder(double radius, double height, int sides)
{
  /* quadric object $B$r0l$D@8@.$9$k(B */
  GLUquadricObj *quad = gluNewQuadric();

  /* $B@8@.$7$?%*%V%8%'%/%H$KBP$9$k%(%i!<%O%s%I%i$rEPO?$9$k(B */
  gluQuadricCallback(quad, GLU_ERROR, errCallBack);

  /* $BLL$NEI$jDY$7$r;XDj$9$k!J@~2h$G$O$J$/1"1F$r$D$1$?1_Cl$rIA$/!K(B*/
  gluQuadricDrawStyle(quad, GLU_FILL);

  /* $B%9%`!<%9%7%'!<%G%#%s%0$r9T$&$h$&@_Dj$9$k(B */
  gluQuadricNormals(quad, GLU_SMOOTH);

  /* $BB&LL$rIA$/!J(Bstacks = 1$B!K(B*/
  gluCylinder(quad, radius, radius, height, sides, 1);

  /* height $B$N9b$5$K>eLL$rIA$/(B */
  glPushMatrix();
  glTranslated(0.0, 0.0, height);
  gluDisk(quad, 0.0, radius, sides, 1);
  glPopMatrix();

  /* $B?^7A$rN"JV$7$FIA$/$h$&$K@_Dj$9$k(B */
  gluQuadricOrientation(quad, GLU_INSIDE);

  /* $B2<LL$rIA$/(B */
  gluDisk(quad, 0.0, radius, sides, 1);

  /* $B@8@.$7$?(B quadlic object $B$r:o=|$9$k(B */
  gluDeleteQuadric(quad);
}
