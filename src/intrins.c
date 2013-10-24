

/*****************************************************************************/
/*                                                                           */
/*  CROBOTS                                                                  */
/*                                                                           */
/*  (C) Copyright Tom Poindexter, 1985, all rights reserved.                 */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/* intrinsic routines - all must push a long onto the stack */
/* degree directions assumed: 0 = east; 90 = north; 180 = west; 270 = south */
/* 0,0 = lower left, max_x,max_y = upper right; i.e., quadrant 1 */

#include "crobots.h"
#include "math.h"

/* stack routines in cpu.c */
extern long push();
extern long pop();

/* radian to degrees conversion factor */
#define RAD_DEG 57.29578

/* define scale factor for trig functions */
#define SCALE 100000.0

/* resolution limit */
#define RES_LIMIT 10L


/* c_scan - radar scanning function - note degrees instead of radians */
/*          expects two agruments on stack, degree and resoultion */

long c_scan() 
{
  register int i;
  long degree;
  long res;
  float distance;
  long close_dist = 0L;
  float x, y;
  long d, dd, d1, d2;

  /* get degree of scan resolution, up to limit */
  res = pop();
  if (res < 0L)
    res = 0L;
  else
    if (res > RES_LIMIT)
      res = RES_LIMIT;

  /* get scan direction */
  degree = pop();
  if (degree < 0L)
    degree = -degree;
  if (degree >= 360L)
    degree %= 360L;
  

  cur_robot->scan = (int) degree;	/* record scan for display */

  /* check other robots for +/- resolution */
  for (i = 0; i < MAXROBOTS; i++) {
    if (cur_robot == &robots[i] || robots[i].status == DEAD)
      continue;  /* skip current or dead robots */
    /* find relative degree angle */
    x = (cur_robot->x / CLICK) - (robots[i].x / CLICK);
    y = (cur_robot->y / CLICK) - (robots[i].y / CLICK);
    if ((int)(x + 0.5) == 0)
      /* avoid division by zero */
      d = (robots[i].y > cur_robot->y) ? 90 : 270;
    else {
      if (robots[i].y < cur_robot->y) {
	if (robots[i].x > cur_robot->x)
	  d = 360.0 + (RAD_DEG * atan(y / x)); /* relative quadrant 4 */
	else 
	  d = 180.0 + (RAD_DEG * atan(y / x)); /* relative quadrant 3 */
      } else {
	if (robots[i].x > cur_robot->x)
	  d = RAD_DEG * atan(y / x);           /* relative quadrant 1 */
	else	   
	  d = 180.0 + (RAD_DEG * atan(y / x)); /* relative quadrant 2 */
      }
    }

    /* find out if robot tested is within the scan resolution */
    if (degree > res && degree < 360 - res) {
      dd = degree;
      d1 = d - res;
      d2 = d + res;
    } else {
      /* on 0 and 360 boundary, bring back into linear resolution */
      dd = degree + 180;
      d1 = 180 + d - res;
      d2 = 180 + d + res;
    }

    if (r_debug) 
      printf("\nscan: degree; %ld; bounds %ld, %ld; robot %d; deg %ld\n",
	     degree,d1,d2,i,d);
    
    if (dd >= d1 && dd <= d2) {
      /* this robot is within scan, so get his distance */
      distance = sqrt((x * x) + (y * y));
      /* only get the closest distance, when two or more robots are in scan */
      if (distance < close_dist || close_dist == 0L)
	close_dist = distance;
    }
  }

  push((long) close_dist);

}

/* c_cannon - fire a shot */
/*            expects two agruments on stack, degree distance */

long c_cannon() 
{
  long degree;
  long distance;
  register int i;
  register int r;

  r = cur_robot - &robots[0];

  distance = pop();
  if (distance > MIS_RANGE) 
    distance = MIS_RANGE;
  else
    if (distance < 0L) {
      push(1L);
      return;
    }
  degree = pop();
  if (degree < 0L)
    degree = -degree;
  if (degree >= 360L)
    degree %= 360L;

  if (r_debug)
    printf("\ncannon: degree %ld, distance %ld; reload %d\n",degree,distance,
	     cur_robot->reload);

  /* see if cannon is reloading */
  if (cur_robot->reload > 0) {
    /* cannot fire until reload cycle complete */
    if (r_debug)
      printf("reloading: %d\n",cur_robot->reload);
    push(0L);
    return;
  }

  /* fire cannon, if one of two missiles are available */
  for (i = 0; i < MIS_ROBOT; i++) {
    if (missiles[r][i].stat == AVAIL) {
      /* fire */
      if (r_debug)
	printf("cannon fired\n");
      cur_robot->reload = RELOAD;
      missiles[r][i].stat = FLYING;
      missiles[r][i].beg_x  = cur_robot->x;
      missiles[r][i].beg_y  = cur_robot->y;
      missiles[r][i].cur_x  = cur_robot->x;
      missiles[r][i].cur_y  = cur_robot->y;
      missiles[r][i].head = (int) degree;
      missiles[r][i].rang = (int) (distance * CLICK);
      missiles[r][i].curr_dist = 0;
      missiles[r][i].count = EXP_COUNT;
      push(1L);  
      return;
    }
  }

  push(0L);  

}


/* c_drive - start the propulsion system */
/*           expect two agruments, degrees & speed */

long c_drive() 
{
  long degree;
  long speed; 

  speed = pop();
  if (speed < 0L)
    speed = 0L;
  else
    if (speed > 100L)
      speed = 100L;
  degree = pop();
  if (degree < 0L)
    degree = -degree;
  if (degree >= 360L)
    degree %= 360L;

  if (r_debug)
    printf("\ndrive: degree %ld, speed %ld\n",degree,speed);

  /* update desired speed and heading */
  cur_robot->d_heading = (int) degree;
  cur_robot->d_speed = (int) speed;
  
  push(1L);
}


/* c_damage - report on damage sustained */

long c_damage()
{
  push((long) cur_robot->damage);
}


/* c_speed - report current speed */

long c_speed()
{
  push((long) cur_robot->speed);
}


/* c_loc_x - report current x location */

long c_loc_x()
{
  push((long) cur_robot->x / CLICK);
}


/* c_loc_y - report current y location */

long c_loc_y()
{
  int y;
  push((long) cur_robot->y / CLICK);
}


/* c_rand - return a random number between 0 and limit */
/*          expect one argument, limit */

long c_rand()
{
  int rand();
  int srand(); 	/* should be seeded elsewhere */
  long limit;

  limit = pop();
    
  if (limit <= 0L)
    push(0L);
  else
    push((long) ((long)(rand()) % limit));
}


/* c_sin - return sin(degrees) * SCALE */
/*         expect one agrument, degrees */

long c_sin()
{
  long degree;
  long lsin();

  degree = pop() % 360L;
  degree = (long) lsin(degree);

  push(degree);
}


/* c_cos - return cos(degrees) * SCALE */
/*         expect one agrument, degrees */

long c_cos()
{
  long degree;
  long lcos();

  degree = pop() % 360L;
  degree = (long) lcos(degree);

  push(degree);
}


/* c_tan - return tan(degrees) * SCALE */
/*         expect one agrument, degrees */

long c_tan()
{
  long degree;

  degree = pop() % 360L;
  degree = (long) (tan((double) degree / RAD_DEG) * SCALE);

  push(degree);
}


/* c_atan - return atan(x) */
/*          expect one agrument, ratio * SCALE */

long c_atan()
{
  long degree;
  long ratio;

  ratio = pop();
  degree = (long) (atan((double) ratio / SCALE) * RAD_DEG);

  push(degree);
}


/* c_sqrt - return sqrt(x) */
/*          expect one agrument, x */

long c_sqrt()
{
  long x;

  x = pop();

  /* ensure x is positive */
  if (x < 0L)
    x = -x;

  x = (long) (sqrt((double) x));

  push(x);
}


