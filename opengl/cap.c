#include <math.h>
#include <GL/glut.h>

/* $BJ*BN$N?'(B */
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };
GLfloat yellow[] = { 0.8, 0.8, 0.2, 1.0 };

/* $B8w8;$N0LCV(B */
GLfloat pos[] = { 3.0, 2.0, 5.0, 1.0 };

/* $B;kE@$N0LCV(B */
GLdouble ex = 3.0, ey = 4.0, ez = 5.0;

/* $BL\I8E@$N0LCV(B */
GLdouble tx = 0.0, ty = 0.0, tz = 0.0;

/* $B2sE><4(B */
GLdouble ax = 0.0, ay = 1.0;

/* $B2sE>3Q(B */
GLdouble angle = 0.0;

/* $B%/%j%C%WLL(B */
GLdouble clip[] = { 0.0, 0.0, -1.0, 0.0 };

/* $BI=<(?^7A$N%G%#%9%W%l%$%j%9%HHV9f(B */
GLuint cube, dodeca, plane;

/* $B%^%&%90\F0NL$N%9%1!<%k(B */
double sx, sy;
#define SCALE 360.0

/* $B%I%i%C%03+;O0LCV(B */
int cx, cy;

/* $B%I%i%C%03+;O;~$N2sE>3Q(B */
double ca;

void display(void)
{
  double dx = tx - ex, dy = ty - ey, dz = tz - ez;
  double d = sqrt(dx * dx + dy * dy + dz * dz);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  /* $B%/%j%C%WLL$N@_Dj(B */
  glLoadIdentity();
  glTranslated(0.0, 0.0, -d);
  glRotated(angle, ax, ay, 0.0);
  glClipPlane(GL_CLIP_PLANE0, clip);

  /* $B%b%G%k%S%e!<JQ499TNs$N=i4|2=(B */
  glLoadIdentity();

  /* $B;kE@$N0\F0(B */
  gluLookAt(ex, ey, ez, tx, ty, tz, 0.0, 1.0, 0.0);

  /* $B8w8;$N0LCV$r@_Dj(B */
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  /* $B%9%F%s%7%k%P%C%U%!$N;HMQ3+;O(B */
  glEnable(GL_STENCIL_TEST);

  /* $B%9%F%s%7%k%P%C%U%!$X=q$-$3$`$h$&$K@_Dj(B */
  glStencilFunc(GL_ALWAYS, 0x0, 0xffffffff);

  /* $B30B&$N%/%j%C%W$5$l$k#1#2LLBN$rIA2h(B */
  glEnable(GL_CLIP_PLANE0);
  glCallList(dodeca);
  glDisable(GL_CLIP_PLANE0);

  /* $B%9%F%s%7%k$r;2>H$7$FIA2h$9$k$h$&$K@_Dj(B */
  glStencilFunc(GL_NOTEQUAL, 0x0, 0xffffffff);

  /* $B%-%c%C%W$r%/%j%C%WLL$HF1MM$K2sE>$7$FIA2h(B $B!A(B $B%/%j%C%WLL(B
   $B$N:BI8JQ49$N7k2L$r(B glGetFloatv(GL_MODELVIEW_MATRIX, ..) 
   $B$7$F!"$=$l$r(B glLoadMatrixf(..) $B$7$F$b$$$$$H;W$&$1$I!D(B */
  glPushMatrix();
  glLoadIdentity();
  glTranslated(0.0, 0.0, -d);
  glRotated(angle, ax, ay, 0.0);
  glCallList(plane);
  glPopMatrix();

  /* $B%9%F%s%7%k%P%C%U%!$N;HMQ=*N;(B */
  glDisable(GL_STENCIL_TEST);

  /* $BFbB&$N%/%j%C%W$5$l$J$$N)J}BN$rIA2h(B */
  glCallList(cube);

  glutSwapBuffers();
}

void resize(int w, int h)
{
  /* $B%^%&%9%]%$%s%?0LCV$N%&%#%s%I%&Fb$NAjBPE*0LCV$X$N49;;MQ(B */
  sx = 1.0 / (double)w;
  sy = 1.0 / (double)h;

  /* $B%&%#%s%I%&A4BN$r%S%e!<%]!<%H$K$9$k(B */
  glViewport(0, 0, w, h);

  /* $BF);kJQ499TNs$N;XDj(B */
  glMatrixMode(GL_PROJECTION);

  /* $BF);kJQ499TNs$N=i4|2=(B */
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

  /* $B%b%G%k%S%e!<JQ499TNs$N;XDj(B */
  glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* $B%"%K%a!<%7%g%s3+;O(B */
      glutIdleFunc(idle);
      /* $B%I%i%C%03+;OE@$r5-O?(B */
      cx = x;
      cy = y;
      /* $B%I%i%C%03+;O;~$N2sE>3Q$r5-O?(B */
      ca = angle;
      break;
    case GLUT_UP:
      /* $B%"%K%a!<%7%g%s=*N;(B */
      glutIdleFunc(0);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

void motion(int x, int y)
{
  double dx, dy, a;

  /* $B%^%&%9%]%$%s%?$N0LCV$N%I%i%C%03+;O0LCV$+$i$NJQ0L(B */
  dx = (x - cx) * sx;
  dy = (y - cy) * sy;

  /* $B%^%&%9%]%$%s%?$N0LCV$N%I%i%C%03+;O0LCV$+$i$N5wN%(B */
  a = sqrt(dx * dx + dy * dy);

  if (a != 0.0) {
    /* $B5wN%$r3QEY$K49;;$7$F%I%i%C%03+;O;~$N2sE>3Q$K2C;;(B */
    angle = fmod(ca + SCALE * a, 360.0);

    /* $B%^%&%9$NJQ0L$+$i2sE><4%Y%/%H%k$r5a$a$k(B */
    ax = dy / a;
    ay = dx / a;
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case '\033':
    /* ESC $B$r%?%$%W$7$?$i=*N;(B */
    exit(0);
  case 'x':
    ex += 0.5;
    break;
  case 'X':
    ex -= 0.5;
    break;
  case 'y':
    ey += 0.5;
    break;
  case 'Y':
    ey -= 0.5;
    break;
  case 'z':
    ez += 0.5;
    break;
  case 'Z':
    ez -= 0.5;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void init(void)
{
  /* $B=i4|@_Dj(B */
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  /* $B%9%F%s%7%k%P%C%U%!$K=q$-$3$`$?$S$K%9%F%s%7%k%P%C%U%!$N%S%C%H$rH?E>(B */
  glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);

  /* $B?^7A$,!VJD$8$?N)BN!W!J$+$D(B glCullFace() $B$r;H$o$J$$!K$J(B
     $B$i!"2hAG$N%9%F%s%7%k%P%C%U%!$X$N=q$-9~$_2s?t$O6v?t2s$K(B
     $B$J$k!#$H$3$m$,!"%/%j%C%W$5$l$FJ*BN$K7j$,3+$$$F$7$^$&$H!"(B
     $B$=$NItJ,!J$=$3$+$i$ON"B&$NLL$,8+$($k!K$KBP$9$k=q$-9~$_(B
     $B2s?t$O4q?t2s$K$J$k!#$7$?$,$C$F!"%9%F%s%7%k%P%C%U%!$N=q(B
     $B$-9~$_$,9T$o$l$kEY$K%9%F%s%7%k%P%C%U%!$N%S%C%H$rH?E>$9(B
     $B$l$P!"4q?t2s=q$-9~$_$,9T$o$l$?ItJ,!"$9$J$o$A7j$NItJ,$N(B
     $B2hAG$N%9%F%s%7%k%P%C%U%!$@$1CM$,%;%C%H$5$l$k$3$H$K$J$k!#(B*/

}

void scene(void)
{
  /* $BI=<(?^7A$r%G%#%9%W%l%$%j%9%H$KEPO?(B */
  int l = glGenLists(3);

  /* $BFbB&$NN)J}BN(B */
  cube = l;
  glNewList(cube, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  glutSolidCube(2.0);
  glEndList();

  /* $B30B&$N#1#2LLBN(B */
  dodeca = l + 1;
  glNewList(dodeca, GL_COMPILE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
  glutSolidDodecahedron();
  glEndList();

  /* $B%/%j%C%W$5$l$?ItJ,$N%-%c%C%W(B */
  plane = l + 2;
  glNewList(plane, GL_COMPILE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
  glNormal3d(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glVertex3d(2.0, 2.0, 0.0);
  glVertex3d(-2.0, 2.0, 0.0);
  glVertex3d(-2.0, -2.0, 0.0);
  glVertex3d(2.0, -2.0, 0.0);
  glEnd();
  glEndList();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  scene();
  glutMainLoop();
  return 0;
}
