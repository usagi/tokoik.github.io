#include <math.h>
#include <GL/glut.h>

/* $BJ*BN$N?'(B */
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };

/* $B8w8;$N0LCV(B */
GLfloat pos[] = { 1.0, 2.0, 5.0, 1.0 };

/* $B;kE@$N0LCV(B */
GLdouble ex = 0.0, ey = 0.0, ez = 10.0;

/* $BL\I8E@$N0LCV(B */
GLdouble tx = 0.0, ty = 0.0, tz = 0.0;

/* $B2sE><4(B */
GLdouble ax = 0.0, ay = 1.0;

/* $B2sE>3Q(B */
GLdouble angle = 0.0;

/* $B%/%j%C%WLL(B */
GLdouble clip[] = { 0.0, 0.0, -1.0, 0.0 };

/* $BI=<(?^7A$N%G%#%9%W%l%$%j%9%HHV9f(B */
GLuint cube, dodeca;

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

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* $B%b%G%k%S%e!<JQ499TNs$N=i4|2=(B */
  glLoadIdentity();

  /* $B%/%j%C%WLL$rL\I8E@$KJ?9T0\F0(B */
  glTranslated(0.0, 0.0, -d);

  /* $B2sE>(B */
  glRotated(angle, ax, ay, 0.0);

  /* $B%/%j%C%WLL$r@_Dj(B */
  glClipPlane(GL_CLIP_PLANE0, clip);

  /* $B%b%G%k%S%e!<JQ499TNs$N=i4|2=(B */
  glLoadIdentity();

  /* $B;kE@$N0\F0(B */
  gluLookAt(ex, ey, ez, tx, ty, tz, 0.0, 1.0, 0.0);

  /* $B8w8;$N0LCV$r@_Dj(B */
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  /* $B30B&$N%/%j%C%W$5$l$k#1#2LLBN$rIA2h(B */
  glEnable(GL_CLIP_PLANE0);
  glCallList(dodeca);
  glDisable(GL_CLIP_PLANE0);

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

  /* $B%/%j%C%W$9$k$HN"B&$,8+$($k$N$G!"N"B&$K$b1"1FIU$1(B */
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
}

void scene(void)
{
  /* $BI=<(?^7A$r%G%#%9%W%l%$%j%9%H$KEPO?(B */
  int l = glGenLists(2);

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
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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