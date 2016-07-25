#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#define DATA_SIZE 8000
#define DELAY 12                  /* 2��ΰ�ưʪ�Τε�Υ: 4�ʾ� */

static double de_sol[DATA_SIZE][3];    /* (��)��ƻ */

static double pl_p[DELAY][3];     /* ��ưʪ�Τΰ��� */
static double pl_v[DELAY][3];     /* ��ưʪ�Τοʤ����� */
static double pl_n[DELAY][3];     /* ��ưʪ�Τ�attractive������ */
static double pl_e[DELAY][3];     /* ��ưʪ�Τ�expanding������ */

static int repeat=1;              /* �ǥ��ȿ��(�����å�) */
static int sws=0;                 /* �����ΰ���(�����å�) */
static double vang=0.2;           /* �����γ��� */
static int zoom=8;                /* ����Ψ */
static int lane=1;                /* ��ƻ��ɽ��(�����å�) */
static int car=1;                 /* ��ưʪ�Τ�ɽ��(�����å�) */

void quit(void)
{
#ifdef STEREO
  system("/usr/gfx/setmon -n 60HZ");
#endif
  exit(0);
}

void normalize(u,v)
     double u[3],v[3];
{
  double r;
  int i;

  r = u[0]*u[0]+u[1]*u[1]+u[2]*u[2];
  for (i=0; i<3; i++) v[i] = u[i]/sqrt(r);
}

void ext_prod(v1,v2,nv)
     double v1[3],v2[3],nv[3];
{
  int i;

  for(i=0; i<3; i++) {
    nv[i] = v1[(i+1)%3]*v2[(i+2)%3] - v1[(i+2)%3]*v2[(i+1)%3];
  }
}

void solv_de(u,v)
     double u[3],v[3];
{
  double dt = 0.02;		/* ���ֳִ� */

  v[0] = u[0] + dt * (-u[1]-u[2]*20.0);
  v[1] = u[1] + dt * (u[0]+0.2*u[1]);
  v[2] = u[2] + dt * (0.01+u[2]*(u[0]-6.6));
}

void make_lane()
{
  double  v1[3], v2[3];
  int  h,i;

  v1[0]= 5.0;			/* ����� */
  v1[1]= 5.0;
  v1[2]= 0.0;

  /* ��ʬ�������η����֤��׻� */
  /* ����ζ��� */
  for(i=0; i<100; i++) {
    solv_de(v1,v2);
    for(h=0; h<3; h++) v1[h] = v2[h];
  }

  /* �ǡ�����׻�������¸ */
  for(i=0 ; i < DATA_SIZE ; i++) {
    solv_de(v1,v2);
    for(h=0; h<3; h++) de_sol[i][h] = v1[h] = v2[h];
  }
}

void move_car(void)
{
  int i,j;
  double u2[3],v1[3],v2[3];
  double w1[3],w2[3],w3[3],w4[3];

  for(i=0; i<3; i++) {
    for(j=DELAY-2; j>=0; j--) {
      pl_p[j+1][i] = pl_p[j][i];
      pl_v[j+1][i] = pl_v[j][i];
      pl_n[j+1][i] = pl_n[j][i];
      pl_e[j+1][i] = pl_e[j][i];
    }
    v1[i]=pl_p[0][i]+0.1*pl_e[0][i];
  }

  solv_de(pl_p[0],u2);
  solv_de(v1,v2);

  for(i=0; i<3; i++) {
    w1[i] = u2[i]-pl_p[0][i];
    w2[i] = v2[i]-u2[i];
  }

  ext_prod(w1,w2,w3);
  ext_prod(w3,w1,w4);

  normalize(w1,pl_v[0]);
  normalize(w3,pl_n[0]);
  normalize(w4,pl_e[0]);

  for(i=0; i<3; i++) pl_p[0][i]=u2[i];
}

void init_car(void)
{
  int i;

  pl_p[0][0]=4.0;
  pl_p[0][1]=4.0;
  pl_p[0][2]=0.0;
  pl_e[0][0]=1.0;
  pl_e[0][1]=0.0;
  pl_e[0][2]=0.0;
  pl_n[0][0]=0.0;
  pl_n[0][1]=1.0;
  pl_n[0][2]=0.0;
  pl_v[0][0]=0.0;
  pl_v[0][1]=0.0;
  pl_v[0][2]=1.0;

  for(i=0; i<200; i++) move_car();
}

void init_light(void)
{
  GLfloat light_diffuse[]  = {0.5, 0.5, 0.5, 1.0};
  GLfloat light_ambient[]  = {0.3, 0.3, 0.3, 1.0};
  GLfloat light_specular[] = {0.5, 0.5, 0.5, 1.0};

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
}

void set_light(void)
{
  GLfloat light_position0[] = {-20.0, 0.0, 10.0, 1.0};
  GLfloat light_position1[] = { 20.0, 0.0,-10.0, 1.0};
  /*  GLfloat light_position2[] = {  0.0, 0.0,  1.0, 1.0};   */

  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  /*  glLightfv(GL_LIGHT2, GL_POSITION, light_position2);   */

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  /*  glEnable(GL_LIGHT2);   */
}

void set_sight(double offset)
{
  int h;
  double u1[3], u2[3], u3[3], ue[3];

  if (sws == 1) {		/* ��ưʪ�Τμи��� */
    for(h=0; h<3; h++)
      ue[h] = pl_p[DELAY-1][h]
	- pl_v[DELAY-1][h]*(double)(zoom) * cos(vang)
	  + pl_n[DELAY-1][h]*(double)(zoom) * sin(vang)
	    + pl_e[DELAY-1][h]*offset;
    glLoadIdentity();
    gluLookAt(ue[0],ue[1],ue[2],
	      pl_p[DELAY-1][0], pl_p[DELAY-1][1], pl_p[DELAY-1][2],
	      pl_n[DELAY-1][0], pl_n[DELAY-1][1], pl_n[DELAY-1][2]);
  } else if (sws == 2) {	/* ��ưʪ�Τμ����� */
    for(h=0; h<3; h++)
      ue[h] = pl_p[0][h]
	+ pl_v[0][h]*(double)(zoom) * cos(vang)
	  + pl_n[0][h]*(double)(zoom) * sin(vang)
	    - pl_e[0][h]*offset;
    glLoadIdentity();
    gluLookAt(ue[0],ue[1],ue[2],
	      pl_p[0][0], pl_p[0][1], pl_p[0][2],
	      pl_n[0][0], pl_n[0][1], pl_n[0][2]);
  } else if (sws == 3) {	/* �濴���鳰 */
    glLoadIdentity();
    u1[0]=0.0; u1[1]=0.0; u1[2]=1.0;
    ext_prod (u1,pl_v[2],u2);
    normalize(u2,u3);
    for(h=0; h<3; h++)
      ue[h] = pl_p[2][h] + u3[h]*(double)(zoom) * cos(vang)
	+ u1[h]*(double)(zoom) * sin(vang)
	  + pl_v[2][h]*offset;
    gluLookAt(ue[0], ue[1], ue[2],
	      pl_p[2][0], pl_p[2][1], pl_p[2][2],
	      0.0, 0.0, 1.0);
  } else if (sws == 4) {	/* �������濴 */
    glLoadIdentity();
    u1[0]=0.0; u1[1]=0.0; u1[2]=1.0;
    ext_prod (pl_v[2],u1,u2);
    normalize(u2,u3);
    for(h=0; h<3; h++)
      ue[h] = pl_p[2][h] + u3[h]*(double)(zoom) * cos(vang)
	+ u1[h]*(double)(zoom) * sin(vang)
	  - pl_v[2][h]*offset;
    gluLookAt(ue[0], ue[1], ue[2],
	      pl_p[2][0], pl_p[2][1], pl_p[2][2],
	      0.0, 0.0, 1.0);
  } else {			/* �������� */
    glLoadIdentity();
    gluLookAt(offset, -2.0, 50.0, 0.0, -2.0, 0.0, 0.0, 1.0, 0.0);
  }
}

void display_lane(void)
{
  int i,j;
  GLdouble c1,c2,c3;
  double v1[3],v2[3],v3[3],v4[3];

  glBegin(GL_LINE_STRIP);   
  for(i=0; i<DATA_SIZE; i++) {
    c1 = de_sol[i][2] / 2.5 + 0.2;
    c2 = (de_sol[i][0]-de_sol[i][1]) / 40.0 + 0.5;
    c3 = 4.0 * (double)i * (DATA_SIZE - i) / DATA_SIZE / DATA_SIZE ;
    glColor3d (c2*c3, c1*c3, (1.0-c1)*c3);
    glVertex3dv(de_sol[i]);
  }
  glEnd();
}

void display_car_sub(cp,ce,cn,cv)
     double cp[3],ce[3],cn[3],cv[3];
{
  GLfloat mat_diffuse1[]   = {1.0, 0.7, 0.3, 1.0}; /* ���Το� */
  GLfloat mat_ambient1[]   = {0.8, 0.5, 0.2, 1.0};
  GLfloat mat_specular1[]  = {1.0, 0.7, 0.3, 1.0};
  GLfloat mat_diffuse2[]   = {0.3, 0.3, 0.3, 1.0}; /* ������ο� */
  GLfloat mat_ambient2[]   = {0.2, 0.2, 0.2, 1.0};
  GLfloat mat_specular2[]  = {0.3, 0.3, 0.3, 1.0};
  GLfloat mat_diffuse3[]   = {0.8, 0.8, 1.0, 1.0}; /* ���饹�ο� */
  GLfloat mat_ambient3[]   = {0.6, 0.6, 0.9, 1.0};
  GLfloat mat_specular3[]  = {0.8, 0.8, 1.0, 1.0};
  GLfloat mat_diffuse4[]   = {1.0, 1.0, 0.9, 1.0}; /* ���饤�Ȥο� */
  GLfloat mat_ambient4[]   = {0.9, 0.9, 0.8, 1.0};
  GLfloat mat_specular4[]  = {1.0, 1.0, 0.9, 1.0};
  GLfloat mat_diffuse5[]   = {1.0, 0.2, 0.2, 1.0}; /* ��饤�Ȥο� */
  GLfloat mat_ambient5[]   = {0.7, 0.0, 0.0, 1.0};
  GLfloat mat_specular5[]  = {1.0, 0.2, 0.2, 1.0};
  GLfloat mat_diffuse6[]   = {0.3, 0.3, 0.3, 1.0}; /* �����Ȥο� */
  GLfloat mat_ambient6[]   = {0.1, 0.1, 0.1, 1.0};
  GLfloat mat_specular6[]  = {0.3, 0.3, 0.3, 1.0};
  GLfloat mat_shininess[] = {5.0};

  int i;
  double box[8][3];
  double wheel[4][3];
  double glass[8][3];
  double light[4][3];
  double sheet[4][3];
  double norm[8][3];

  for(i=0; i<3; i++) {
    box[0][i] = cp[i]+0.15*ce[i]+0.5*cv[i]+0.12*cn[i];
    box[1][i] = cp[i]-0.15*ce[i]+0.5*cv[i]+0.12*cn[i];
    box[2][i] = cp[i]+0.15*ce[i]+0.5*cv[i];
    box[3][i] = cp[i]-0.15*ce[i]+0.5*cv[i];
    box[4][i] = cp[i]+0.15*ce[i]          +0.12*cn[i];
    box[5][i] = cp[i]-0.15*ce[i]          +0.12*cn[i];
    box[6][i] = cp[i]+0.15*ce[i];
    box[7][i] = cp[i]-0.15*ce[i];

    wheel[0][i] = cp[i]+0.13*ce[i]+0.4*cv[i]+0.02*cn[i];
    wheel[1][i] = cp[i]-0.13*ce[i]+0.4*cv[i]+0.02*cn[i];
    wheel[2][i] = cp[i]+0.13*ce[i]+0.1*cv[i]+0.02*cn[i];
    wheel[3][i] = cp[i]-0.13*ce[i]+0.1*cv[i]+0.02*cn[i];

    glass[0][i] = cp[i]+0.13*ce[i]+0.28*cv[i]+0.12*cn[i];
    glass[1][i] = cp[i]+0.11*ce[i]+0.28*cv[i]+0.18*cn[i];
    glass[2][i] = cp[i]+0.13*ce[i]+0.35*cv[i]+0.12*cn[i];
    glass[3][i] = cp[i]+0.11*ce[i]+0.3 *cv[i]+0.18*cn[i];
    glass[4][i] = cp[i]-0.13*ce[i]+0.35*cv[i]+0.12*cn[i];
    glass[5][i] = cp[i]-0.11*ce[i]+0.3 *cv[i]+0.18*cn[i];
    glass[6][i] = cp[i]-0.13*ce[i]+0.28*cv[i]+0.12*cn[i];
    glass[7][i] = cp[i]-0.11*ce[i]+0.28*cv[i]+0.18*cn[i];

    light[0][i] = cp[i]+0.08*ce[i]+0.48*cv[i]+0.06 *cn[i];
    light[1][i] = cp[i]-0.08*ce[i]+0.48*cv[i]+0.06 *cn[i];
    light[2][i] = cp[i]+0.11*ce[i]+0.01*cv[i]+0.075*cn[i];
    light[3][i] = cp[i]-0.11*ce[i]+0.01*cv[i]+0.075*cn[i];

    sheet[0][i] = cp[i]+0.12*ce[i]+0.35*cv[i]+0.121*cn[i];
    sheet[1][i] = cp[i]+0.12*ce[i]+0.07*cv[i]+0.121*cn[i];
    sheet[2][i] = cp[i]-0.12*ce[i]+0.07*cv[i]+0.121*cn[i];
    sheet[3][i] = cp[i]-0.12*ce[i]+0.35*cv[i]+0.121*cn[i];

    norm[0][i] =         0.5*ce[i]+0.7*cv[i]+0.5*cn[i];
    norm[1][i] =        -0.5*ce[i]+0.7*cv[i]+0.5*cn[i];
    norm[2][i] =         0.5*ce[i]+0.7*cv[i]-0.5*cn[i];
    norm[3][i] =        -0.5*ce[i]+0.7*cv[i]-0.5*cn[i];
    norm[4][i] =         0.5*ce[i]-0.7*cv[i]+0.5*cn[i];
    norm[5][i] =        -0.5*ce[i]-0.7*cv[i]+0.5*cn[i];
    norm[6][i] =         0.5*ce[i]-0.7*cv[i]-0.5*cn[i];
    norm[7][i] =        -0.5*ce[i]-0.7*cv[i]-0.5*cn[i];
  }

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  glBegin(GL_QUADS);
  glNormal3d(norm[0][0],norm[0][1],norm[0][2]);
  glVertex3dv(box[0]);
  glNormal3d(norm[1][0],norm[1][1],norm[1][2]);
  glVertex3dv(box[1]);
  glNormal3d(norm[3][0],norm[3][1],norm[3][2]);
  glVertex3dv(box[3]);
  glNormal3d(norm[2][0],norm[2][1],norm[2][2]);
  glVertex3dv(box[2]);

  glNormal3d(norm[6][0],norm[6][1],norm[6][2]);
  glVertex3dv(box[6]);
  glNormal3d(norm[7][0],norm[7][1],norm[7][2]);
  glVertex3dv(box[7]);
  glNormal3d(norm[5][0],norm[5][1],norm[5][2]);
  glVertex3dv(box[5]);
  glNormal3d(norm[4][0],norm[4][1],norm[4][2]);
  glVertex3dv(box[4]);

  glNormal3d(norm[0][0],norm[0][1],norm[0][2]);
  glVertex3dv(box[0]);
  glNormal3d(norm[2][0],norm[2][1],norm[2][2]);
  glVertex3dv(box[2]);
  glNormal3d(norm[6][0],norm[6][1],norm[6][2]);
  glVertex3dv(box[6]);
  glNormal3d(norm[4][0],norm[4][1],norm[4][2]);
  glVertex3dv(box[4]);

  glNormal3d(norm[5][0],norm[5][1],norm[5][2]);
  glVertex3dv(box[5]);
  glNormal3d(norm[7][0],norm[7][1],norm[7][2]);
  glVertex3dv(box[7]);
  glNormal3d(norm[3][0],norm[3][1],norm[3][2]);
  glVertex3dv(box[3]);
  glNormal3d(norm[1][0],norm[1][1],norm[1][2]);
  glVertex3dv(box[1]);

  glNormal3d(norm[0][0],norm[0][1],norm[0][2]);
  glVertex3dv(box[0]);
  glNormal3d(norm[4][0],norm[4][1],norm[4][2]);
  glVertex3dv(box[4]);
  glNormal3d(norm[5][0],norm[5][1],norm[5][2]);
  glVertex3dv(box[5]);
  glNormal3d(norm[1][0],norm[1][1],norm[1][2]);
  glVertex3dv(box[1]);

  glNormal3d(norm[3][0],norm[3][1],norm[3][2]);
  glVertex3dv(box[3]);
  glNormal3d(norm[7][0],norm[7][1],norm[7][2]);
  glVertex3dv(box[7]);
  glNormal3d(norm[6][0],norm[6][1],norm[6][2]);
  glVertex3dv(box[6]);
  glNormal3d(norm[2][0],norm[2][1],norm[2][2]);
  glVertex3dv(box[2]);
  glEnd();

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse2);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient2);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  for(i=0; i<4; i++) {
    glPushMatrix();
    glTranslated(wheel[i][0],wheel[i][1],wheel[i][2]);
    glutSolidSphere(0.05,8,8);
    glPopMatrix();
  }

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse3);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient3);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular3);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  glBegin(GL_QUAD_STRIP);
  glNormal3d(ce[0],ce[1],ce[2]);
  glVertex3dv(glass[0]);
  glVertex3dv(glass[1]);
  glNormal3d(0.8*cv[0]+0.6*cn[0],0.8*cv[1]+0.6*cn[0],0.8*cv[2]+0.6*cn[0]);
  glVertex3dv(glass[2]);
  glVertex3dv(glass[3]);
  glVertex3dv(glass[4]);
  glVertex3dv(glass[5]);
  glNormal3d(-ce[0],-ce[1],-ce[2]);
  glVertex3dv(glass[6]);
  glVertex3dv(glass[7]);
  glEnd();

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse4);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient4);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular4);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  for(i=0; i<2; i++) {
    glPushMatrix();
    glTranslated(light[i][0],light[i][1],light[i][2]);
    glutSolidSphere(0.04,10,10);
    glPopMatrix();
  }

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse5);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient5);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular5);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  for(i=2; i<4; i++) {
    glPushMatrix();
    glTranslated(light[i][0],light[i][1],light[i][2]);
    glutSolidSphere(0.03,10,10);
    glPopMatrix();
  }

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse6);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient6);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular6);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  glBegin(GL_QUADS);
  glNormal3d(cn[0], cn[1], cn[2]);
  glVertex3dv(sheet[0]);
  glVertex3dv(sheet[1]);
  glVertex3dv(sheet[2]);
  glVertex3dv(sheet[3]);
  glEnd();
}

void display_car(void)
{
  int i;
  double cp2[3];

  for(i=0; i<3; i++) {
    cp2[i] = pl_p[DELAY-2][i] - 0.5 * pl_v[DELAY-2][i];
  }

  display_car_sub(pl_p[0],pl_e[0],pl_n[0],pl_v[0]);
  display_car_sub(cp2,pl_e[DELAY-2],pl_n[DELAY-2],pl_v[DELAY-2]);
}

void display_sun(void)
{
  GLfloat mat_diffuse[]   = {1.0, 0.5, 0.2, 1.0};
  GLfloat mat_specular[]  = {0.5, 0.2, 0.1, 1.0};
  GLfloat mat_ambient[]   = {0.5, 0.2, 0.1, 1.0};
  GLfloat mat_shininess[] = {5.0};

  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glutSolidSphere(0.4,10,10);
}

void idle(void)
{
  move_car();
  glutPostRedisplay();
}

void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  init_light();

  make_lane();

  init_car();
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

#ifdef STEREO
  set_sight(0.2);
  glViewport(0, 0, 1280, 492);
  glNewList(1, GL_COMPILE_AND_EXECUTE);
#else
  set_sight(0.0);
#endif
  set_light();

  if (lane == 1) display_lane();

  glEnable(GL_LIGHTING);
  if (car == 1) display_car();
  display_sun();
  glDisable(GL_LIGHTING);
#ifdef STEREO
  glEndList();
  set_sight(-0.2);
  glViewport(0, 532, 1280, 492);
  glCallList(1);
#endif

  glutSwapBuffers();
}


void reshape(int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (30.0, (GLfloat)w/(GLfloat)h, 1.0, 200.0);
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 27:			/* �ץ�����ཪλ */
  case 'q':
    quit();
    break;
  case 's':			/* ���˥᡼���������� */
    glutIdleFunc(NULL);
    break;
  case ' ':			/* (��߻���)1���޿ʤ�� */
    idle();
    break;
  case 'g':			/* ���˥᡼�����Ƴ� */
    glutIdleFunc(idle);
    break;
  case 'V':			/* �������ѹ������å� */
    sws--; if(sws == -1) sws=4;
    glutPostRedisplay();
    break;
  case 'v':			/* �������ѹ������å� */
    sws++; if(sws == 5) sws=0;
    glutPostRedisplay();
    break;
  case 'u':			/* �����γ��٤�夲�� */
    if (sws > 0) vang += 0.2;
    if (vang > 1.0) vang = 1.0;
    glutPostRedisplay();
    break;
  case 'd':			/* �����γ��٤򲼤��� */
    if (sws > 0) vang -= 0.2;
    if (vang < -1.0) vang = -1.0;
    glutPostRedisplay();
    break;
  case '+':			/* �����।�� */
    if (sws > 0) zoom = zoom/2;
    if (zoom < 2) zoom = 2;
    glutPostRedisplay();
    break;
  case '-':			/* �����ॢ���� */
    if (sws > 0) zoom = zoom*2;
    if (zoom > 64) zoom = 64;
    glutPostRedisplay();
    break;
  case 'l':			/* ��ƻ��ɽ�������å� */
    lane = 1-lane;
    glutPostRedisplay();
    break;
  case 'c':			/* ��ưʪ�Τ�ɽ�������å� */
    car = 1-car;
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

int main(int argc, char *argv[])
{
#ifdef STEREO
  system("/usr/gfx/setmon -n STR_RECT");
#endif

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(480,360);
  glutInitWindowPosition(100,100);

  glutCreateWindow("Rossler Attracter");

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);
#ifdef STEREO
  glutFullScreen();
#endif
  glutMainLoop();

  return 0;
}