#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

/* $B%;%l%/%7%g%s%P%C%U%!$N%5%$%:(B */
#define SELECTIONS 100

/* $B%*%V%8%'%/%H$N?t(B */
#define NOBJECTS 5

/* $B%G%#%9%W%l%$%j%9%H$N<1JL;R(B */
GLuint objects;

/* $B3F%*%V%8%'%/%H$N?'(B */
GLfloat *color[NOBJECTS];

/* $B?'%G!<%?(B */
GLfloat gray[] = { 0.7, 0.7, 0.7 };
GLfloat blue[] = { 0.1, 0.1, 0.9 };

void display(void)
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  /* $B0lC6%7!<%s$rIA2h$9$k(B */
  for (i = 0; i < NOBJECTS; i++) {
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color[i]);
    glCallList(objects + i);
  }

  glFlush();
}

void mouse(int button, int state, int x, int y)
{
  static GLuint selection[SELECTIONS];  /* $B%;%l%/%7%g%s%P%C%U%!!!!!!!!!!!(B */
  static GLint hits = 0;                /* $B%;%l%/%H$5$l$?%*%V%8%'%/%H$N?t(B */

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
      
      /* $B%;%l%/%7%g%s%P%C%U%!$N=i4|2=!$$3$l$O%;%l%/%7%g%s%b!<(B
         $B%I$K$J$C$F$J$$$HL5;k$5$l$k(B */
      glInitNames();

      /* $B%M!<%`%9%?%C%/$N@hF,$K2>$NL>A0$r5M$a$F$*$/!%%M!<%`(B
         $B%9%?%C%/<+BN$OJ#?t$N%*%V%8%'%/%H$,A*Br$G$-$k$h$&$K(B
         $B%9%?%C%/9=B$$K$J$C$F$$$k$,!$:#2s$O#18D$N%*%V%8%'%/(B
         $B%H$7$+A*Br$7$J$$$N$G!$%M!<%`%9%?%C%/$N@hF,$NMWAG$@(B
         $B$1$r<h$jBX$($J$,$iIA2h$9$l$P$h$$!%$=$3$G!$$"$i$+$8(B
         $B$a%M!<%`%9%?%C%/$N@hF,$K2>$NL>A0(B (-1) $B$K5M$a$F$*$$(B
         $B$F!$$=$3$r;H$$2s$9!%(B */
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
         $B%$%s%0JQ49$d%b%G%j%s%0JQ49$N@_Dj$r$9$k$N$@$,!$D>A0(B
         $B$KIA2h$5$l$??^7A$KBP$7$F%;%l%/%7%g%s$r9T$J$&$J$i!$(B
         $B$=$N;~$K;H$C$?%b%G%k%S%e!<%^%H%j%/%9$,$=$N$^$^;H$((B
         $B$k!J$O$:!K!%$@$+$i:#$O0J2<$N=hM}(B (#if 0 $B!A(B #endif) 
         $B$r>JN,$7$F$bBg>fIW!J$@$H;W$&!K(B*/

#if 0
      /* $B%b%G%k%S%e!<JQ499TNs$N=i4|2=(B */
      glLoadIdentity();

      /* $B;kE@$N@_Dj(B */
      gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
#endif

      /* $B$b$&0lEY%7!<%s$rIA2h$9$k(B */
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
	  color[*(ptr++)] = blue; /* $B%*%V%8%'%/%H$N?'$r@D$K$9$k(B */
	}
      }
    }
    else {
      GLuint *ptr = selection;
      int i;

      /* selection[] $B$NFbMF$O%^%&%9$N:8%\%?%s$r%/%j%C%/$7$?(B
         $B;~$K$7$+JQ99$5$l$J$$$O$:$@$+$i!$$^$@;D$C$F$$$k$O$:(B */
      for (i = 0; i < hits; i++) {
	unsigned int j, n = ptr[0];
	
	ptr += 3;
	for (j = 0; j < n; j++) {
	  color[*(ptr++)] = gray;
	}
      }
    }

    /* $B2hLL$r=q$-49$($F$_$k(B */
    glutPostRedisplay();
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

  /* $B3F%*%V%8%'%/%H$N?'$r=i4|2=(B */
  for (i = 0; i < NOBJECTS; i++) {
    color[i] = gray;
  }
}

void scene(void)
{
  const GLdouble width = 4.0;
  const GLdouble center = width / 2.0;
  const GLdouble step = width / (NOBJECTS - 1);
  int i;

  /* $B%;%l%/%7%g%s$NBP>]$K$J$k%*%V%8%'%/%H$r@8@.$7$F!$%G%#%9(B
     $B%W%l%$%j%9%H$r:n$C$F$*$/(B*/
  for (i = 0; i < NOBJECTS; i++) {

    /* $B?^7A$r%G%#%9%W%l%$%j%9%H$KEPO?(B */
    glNewList(objects + i, GL_COMPILE);

    /* $B5e$rIA$/(B */
    glPushMatrix();
    glTranslated((double)i * step - center, ((i & 1) * 2 - 1) * step, 0.0);
    glutSolidSphere(0.5, 10, 5);
    glPopMatrix();

    glEndList();
  }
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  init();
  scene();
  glutMainLoop();
  return 0;
}
