#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

/* $B%;%l%/%7%g%s%P%C%U%!$N%5%$%:(B */
#define SELECTIONS 100

/* $B%*%V%8%'%/%H$N?t(B */
#define NOBJECTS 5

/* $B%G%#%9%W%l%$%j%9%H$N<1JL;R(B */
GLuint objects;

/* $B%*%V%8%'%/%H$N?'(B */
static GLfloat color[][3] = {
  { 0.1, 0.1, 0.9 },
  { 0.1, 0.9, 0.1 },
  { 0.9, 0.1, 0.1 },
  { 0.1, 0.9, 0.9 },
  { 0.9, 0.1, 0.9 },
  { 0.9, 0.9, 0.1 },
};

/* $B%*%V%8%'%/%H$NG[CV(B */
#define ARRANGEWIDTH	4.0
#define ARRANGECENTER	(ARRANGEWIDTH / 2.0)
#define ARRANGESTEP	(ARRANGEWIDTH / (NOBJECTS - 1))
#define ARRANGEDEPTH	0.0
#define INITIALHEIGHT	0.0
#define INITIALVELOCITY	2.0
#define GRAVITY		-1.0
#define REFRESHRATE	0.01 /* 100Hz */
unsigned long touchtime[NOBJECTS];

/* $B8w8;$N0LCV(B */
GLfloat light0pos[] = { 4.0, 8.0, 6.0, 1.0 };

/* $B%+%l%s%H%U%l!<%`HV9f(B */
unsigned long cframe = 0;

void display(void)
{
  int i;

  /* $B;~4V$r7W$k$?$a$K%+%l%s%H%U%l!<%`HV9f$N%+%&%s%H$7$F$*$/(B */
  ++cframe;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(5.0, 4.0, 5.0, 0.4, 0.0, 0.0, 0.0, 1.0, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, light0pos);

  /* $B%7!<%s$NIA2h(B */
  for (i = 0; i < NOBJECTS; i++) {

    /* $B3F%*%V%8%'%/%H$N?bD>0LCV$r=i4|0LCV$K@_Dj$9$k(B */
    GLdouble h = INITIALHEIGHT;

    /* $B%/%j%C%/$5$l$?;~4V$,@_Dj$5$l$F$$$?$iF0$-=P$9(B */
    if (touchtime[i] > 0) {
      double t;

      /* $B7P2a%U%l!<%`?t$+$i;~4V$r7W;;!J$F$-$H!<!K(B*/
      t = (cframe - touchtime[i]) * REFRESHRATE;

      /* $B9b$5$N7W;;!JCf3X$+9b9;$NJ*M}$G=,$C$?$h$M!K(B*/
      h += (GRAVITY * t * 0.5 + INITIALVELOCITY) * t;

      /* $BCOLL$KMn2<$7$?$iF0$-$r;_$a$k$N$@(B */
      if (h < 0.0) {
	h = INITIALHEIGHT;
	touchtime[i] = 0;
      }
    }

    /* $B$H$j$"$($:?'$r@_Dj!%?'$O%;%l%/%7%g%s$K$O4X78$J$$$N$G!"(B
       $B%G%#%9%W%l%$%j%9%H$K$OF~$l$J$$$3$H$K$9$k(B */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color[i % 6]);

    /* $B%;%l%/%7%g%s$N$?$a$KF1$8%7!<%s$r$b$&0lEYIA2h$9$kI,MW(B
       $B$,$"$k$N$G!"%*%V%8%'%/%H$r%G%#%9%W%l%$%j%9%H$KF~$l$F(B
       $B$7$^$&!#$?$@$7!"%G%#%9%W%l%$%j%9%H$r:o=|$;$:$K;H$$2s(B
       $B$7$F$$$k$N$G!"%a%b%j%j!<%/$7$=$&$J5$$,$7$F$A$g$$$HIT(B
       $B0B(B */
    glNewList(objects + i, GL_COMPILE_AND_EXECUTE);

    /* $B?^7A$NIA2h(B */
    glPushMatrix();
    glTranslated((double)i * ARRANGESTEP - ARRANGECENTER, h, ARRANGEDEPTH);
    glutSolidCube(0.8);
    glPopMatrix();

    glEndList();

  }
  glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
  GLuint selection[SELECTIONS];  /* $B%;%l%/%7%g%s%P%C%U%!!!!!!!!!!!(B */
  GLint hits = 0;                /* $B%;%l%/%H$5$l$?%*%V%8%'%/%H$N?t(B */

  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      GLuint *ptr;
      GLint vp[4];
      int i;

      /* $B%;%l%/%7%g%s$K;H$&%P%C%U%!$N@_Dj!%$3$l$O%;%l%/%7%g(B
         $B%s%b!<%I0J30$N;~!J(BglRenderMode(GL_SELECT) $B$h$jA0!K(B
         $B$K<B9T$9$kI,MW$,$"$k!%%;%l%/%7%g%s%P%C%U%!$K$O!$F~(B
         $B$k$@$1$N%G!<%?$,5M$a9~$^$l$k(B */
      glSelectBuffer(SELECTIONS, selection);

      /* $B%l%s%@%j%s%0%b!<%I$r%;%l%/%7%g%s%b!<%I$K@ZBX$($k(B */
      glRenderMode(GL_SELECT);
      
      /* $B%;%l%/%7%g%s%P%C%U%!$r=i4|2=$9$k!%$3$l$O%;%l%/%7%g(B
         $B%s%b!<%I$K$J$C$F$J$$$HL5;k$5$l$k(B */
      glInitNames();

      /* $B%M!<%`%9%?%C%/$N@hF,$K2>$NL>A0$r5M$a$k!%%M!<%`%9%?%C(B
       * $B%/<+BN$OJ#?t$N%*%V%8%'%/%H$,A*Br$G$-$k$h$&$K%9%?%C(B
       * $B%/9=B$$K$J$C$F$$$k$,!$:#2s$O#18D$N%*%V%8%'%/%H$7$+(B
       * $BA*Br$7$J$$$N$G!$%M!<%`%9%?%C%/$N@hF,$NMWAG$@$1$r<h(B
       * $B$jBX$($J$,$iIA2h$9$l$P$h$$!%$=$3$G!$$"$i$+$8$a%M!<(B
       * $B%`%9%?%C%/$N@hF,$K2>$NL>A0(B (-1) $B$K5M$a$F$*$$$F!$$=(B
       * $B$3$r;H$$2s$9!%(B */
      glPushName(-1);

      /* $B%;%l%/%7%g%s$N=hM}$O;kE@:BI87O$G9T$&(B */
      glMatrixMode(GL_PROJECTION);

      /* $B8=:_$NF);kJQ49%^%H%j%/%9$rJ]B8$9$k(B */
      glPushMatrix();

      /* $BF);kJQ49%^%H%j%/%9$r=i4|2=$9$k(B */
      glLoadIdentity();

      /* $B8=:_$N%S%e!<%]!<%H@_Dj$rF@$k(B */
      glGetIntegerv(GL_VIEWPORT, vp);

      /* $BI=<(NN0h$,%^%&%9%]%$%s%?$N<~0O$@$1$K$J$k$h$&$KJQ49(B
         $B9TNs$r@_Dj$9$k!%%^%&%9$N:BI87O$O!$%9%/%j!<%s$N:BI8(B
         $B7O$KBP$7$F>e2<$,H?E>$7$F$$$k$N$G!$$=$l$rJd@5$9$k(B */
      gluPickMatrix(x, vp[3] - y - 1, 1, 1, vp);

      /* $BDL>o$NIA2h$HF1$8F);kJQ49%^%H%j%/%9$r@_Dj$9$k!%%&%#(B
         $B%s%I%&A4BN$r%S%e!<%]!<%H$K$7$F$$$k$N$G!$%"%9%Z%/%H(B
         $BHf$O(B vp[2] / vp[3] $B$GF@$i$l$k!%(B*/
      gluPerspective(30.0, (double)vp[2] / (double)vp[3], 1.0, 100.0);

      /* $B%b%G%k%S%e!<%^%H%j%/%9$K@ZBX$($k(B */
      glMatrixMode(GL_MODELVIEW);

      /* $B$3$3$G0lC6%b%G%k%S%e!<%^%H%j%/%9$K@ZBX$($F!$%S%e!<(B
       * $B%$%s%0JQ49$d%b%G%j%s%0JQ49$N@_Dj$r$9$k$N$@$,!$D>A0(B
       * $B$KIA2h$5$l$??^7A$KBP$7$F%;%l%/%7%g%s$r9T$J$&$J$i!$(B
       * $B$=$N;~$K;H$C$?%b%G%k%S%e!<%^%H%j%/%9$,$=$N$^$^;H$((B
       * $B$k!J$O$:!K!%$@$+$i:#$O0J2<$N=hM}(B (#if 0 $B!A(B #endif) 
       * $B$r>JN,$7$F$bBg>fIW!J$@$H;W$&!K(B*/

#if 0
      /* $B%b%G%k%S%e!<JQ499TNs$N=i4|2=(B */
      glLoadIdentity();

      /* $B;kE@$N@_Dj(B */
      gluLookAt(5.0, 4.0, 5.0, 0.4, 0.0, 0.0, 0.0, 1.0, 0.0);
#endif

      /* $B$b$&0lEY%7!<%s$rIA2h(B */
      for (i = 0; i < NOBJECTS; i++) {
	/* $B%M!<%`%9%?%C%/$N@hF,$K$3$l$+$iIA$/%*%V%8%'%/%H$N(B
           $BHV9f$r@_Dj$9$k(B */
	glLoadName(i);
	/* $B%*%V%8%'%/%H$rIA2h$9$k!J2hLL$K$OI=<($5$l$J$$!K(B*/
	glCallList(objects + i);
      }

      /* $B:F$SF);kJQ49%^%H%j%/%9$K@ZBX$($k(B */
      glMatrixMode(GL_PROJECTION);

      /* $BF);kJQ49%^%H%j%/%9$r85$KLa$9(B */
      glPopMatrix();

      /* $B%b%G%k%S%e!<%^%H%j%/%9$KLa$9(B */
      glMatrixMode(GL_MODELVIEW);

      /* $B%l%s%@%j%s%0%b!<%I$r85$KLa$9(B */
      hits = glRenderMode(GL_RENDER);

      /* $B%;%l%/%7%g%s%P%C%U%!$K$O$$$/$D$N%G!<%?$,F~$C$F$$$k(B
         $B$N$+$o$+$i$J$$$N$G!$$H$j$"$($:@hF,$N%]%$%s%?$r<h$j(B
         $B=P$7$F$*$/(B */
      ptr = selection;

      /* hits $B$K$O%;%l%/%7%g%s%P%C%U%!$K3JG<$5$l$?%G!<%?$N?t(B
         $B$,F~$k!%%G!<%?$,%;%l%/%7%g%s%P%C%U%!$KF~$j@Z$i$J$1(B
         $B$l$P(B hits < 0 $B$H$J$k(B */
      for (i = 0; i < hits; i++) {
	/* $B%;%l%/%7%g%s%P%C%U%!$N@hF,$NMWAG$O!"A*Br$5$l$?%*(B
           $B%V%8%'%/%H$N?t(B */
	unsigned int j, n = ptr[0];

	/* $BB3$/#2$D$NMWAG$O!"A*Br$5$l$?0LCV$KCV$1$k1|9T$-CM(B
           $B$N:G>.CM$H:GBgCM$rId9f$J$7@0?t$GI=$7$?$b$N(B */
	double near = (double)ptr[1] / (double)0x7fffffff;
	double far  = (double)ptr[2] / (double)0x7fffffff;

	/* $B%;%l%/%7%g%s%P%C%U%!$N#4$DL\$NMWAG!JE:;z!a#3!K$+(B
           $B$iA*Br$5$l$?%*%V%8%'%/%H$NHV9f$,F~$C$F$$$k(B */
	ptr += 3;
	for (j = 0; j < n; j++) {
	  /* $B%*%V%8%'%/%H$K%/%j%C%/$5$l$?;~4V$r@_Dj$9$k(B */
	  touchtime[*(ptr++)] = cframe;
	}
      }
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;
  default:
    break;
  }
}

void resize(int w, int h)
{
  /* $B%&%#%s%I%&A4BN$r%S%e!<%]!<%H$K$9$k(B */
  glViewport(0, 0, w, h);

  /* $BF);kJQ499TNs$r@_Dj$9$k(B */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

  /* $B%b%G%k%S%e!<JQ499TNs$r;XDj$7$F$*$/(B */
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    exit(0);  /* '\033' $B$O(B ESC $B$N(B ASCII $B%3!<%I(B */
  default:
    break;
  }
}

void init(void)
{
  int i;

  /* $B=i4|@_Dj(B */
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* $B3F%*%V%8%'%/%H$N%G%#%9%W%l%$%j%9%H$r:n$k(B */
  objects = glGenLists(NOBJECTS);
  if (objects <= 0) {
    fprintf(stderr, "Can't create so many objects: %d.\n", NOBJECTS);
    exit(1);
  }

  /* $B3F%*%V%8%'%/%H$r%/%j%C%/$7$?%U%l!<%`HV9f$r=i4|2=(B */
  for (i = 0; i < NOBJECTS; i++) {
    touchtime[i] = 0;
  }
}

void idle(void)
{
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
  return 0;
}
