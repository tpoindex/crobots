

/*****************************************************************************/
/*                                                                           */
/*  CROBOTS                                                                  */
/*                                                                           */
/*  (C) Copyright Tom Poindexter, 1985, all rights reserved.                 */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/* motion.c - routines to update robot & missiles positions */

#include "crobots.h"
#include <math.h>

/* define long absolute value function */
#define labs(l) ((long) l < 0L ? -l : l)


/* sine and cosine lookup table, times 100,000 */
/* to bypass floating point transcendentals, for speed */

long trig_tbl[91] = {
         0L,
      1745L,
      3489L,
      5233L,
      6975L,
      8715L,
     10452L,
     12186L,
     13917L,
     15643L,
     17364L,
     19080L,
     20791L,
     22495L,
     24192L,
     25881L,
     27563L,
     29237L,
     30901L,
     32556L,
     34202L,
     35836L,
     37460L,
     39073L,
     40673L,
     42261L,
     43837L,
     45399L,
     46947L,
     48480L,
     50000L,
     51503L,
     52991L,
     54463L,
     55919L,
     57357L,
     58778L,
     60181L,
     61566L,
     62932L,
     64278L,
     65605L,
     66913L,
     68199L,
     69465L,
     70710L,
     71933L,
     73135L,
     74314L,
     75470L,
     76604L,
     77714L,
     78801L,
     79863L,
     80901L,
     81915L,
     82903L,
     83867L,
     84804L,
     85716L,
     86602L,
     87461L,
     88294L,
     89100L,
     89879L,
     90630L,
     91354L,
     92050L,
     92718L,
     93358L,
     93969L,
     94551L,
     95105L,
     95630L,
     96126L,
     96592L,
     97029L,
     97437L,
     97814L,
     98162L,
     98480L,
     98768L,
     99026L,
     99254L,
     99452L,
     99619L,
     99756L,
     99862L,
     99939L,
     99984L,
    100000L
};


/* sin look up */

long lsin(deg)

int deg;
{
  deg = deg % 360;
  if (deg < 0)
    deg = 360 + deg;
  
  if (deg < 91)
    return (trig_tbl[deg]);

  if (deg < 181)
    return (trig_tbl[90-(deg-90)]);

  if (deg < 271)
    return (-(trig_tbl[deg-180]));

  if (deg < 361)
    return (-(trig_tbl[90-(deg-270)]));

  return (0L);   /* should be unreachable */
}


/* cos look up */

long lcos(deg)

int deg;
{
  deg = deg % 360;
  if (deg < 0)
    deg = 360 + deg;
  
  if (deg < 91)
    return (trig_tbl[90-deg]);

  if (deg < 181)
    return (-(trig_tbl[deg-90]));

  if (deg < 271)
    return (-(trig_tbl[90-(deg-180)]));

  if (deg < 361)
    return (trig_tbl[(deg-270)]);

  return (100000L);   /* should be unreachable */
}


/* the damage table */

struct {
  int dist;
  int dam;
} exp_dam[3] = {
  { DIRECT_RANGE, DIRECT_HIT },
  { NEAR_RANGE,   NEAR_HIT   },
  { FAR_RANGE,    FAR_HIT    }
};

/* move_robots - update the postion of all robots */
/*               parm 'displ' controls call to field display */

move_robots(displ)
int displ;
{
  register int i, n;
  long lsin(), lcos();

  for (i = 0; i < MAXROBOTS; i++) {
    if (robots[i].status == DEAD)
      continue;

    /* check for dead robots, and make sure they are dead */
    if (robots[i].damage >= 100) {
      robots[i].damage = 100;
      robots[i].status = DEAD;
      if (displ)
	robot_stat(i);
    }

    /* update cannon reloader */
    if (robots[i].reload > 0)
      robots[i].reload--;

    /* update speed, moderated by acceleration */
    if (robots[i].speed != robots[i].d_speed) {
      if (robots[i].speed > robots[i].d_speed) { /* slowing */
	robots[i].accel -= ACCEL;
	if (robots[i].accel < robots[i].d_speed) 
	  robots[i].speed = robots[i].accel = robots[i].d_speed;
	else 
	  robots[i].speed = robots[i].accel;
      } else { /* accelerating */
	robots[i].accel += ACCEL;
	if (robots[i].accel > robots[i].d_speed) 
	  robots[i].speed = robots[i].accel = robots[i].d_speed;
	else
  	  robots[i].speed = robots[i].accel;
      }
    }

    /* update heading; allow change below a certain speed*/
    if (robots[i].heading != robots[i].d_heading) {
      if (robots[i].speed <= TURN_SPEED) {
	robots[i].heading = robots[i].d_heading;
	robots[i].range = 0;
	robots[i].org_x = robots[i].x;
	robots[i].org_y = robots[i].y;
      } else 
	robots[i].d_speed = 0;
    }

    /* update distance traveled on this heading, x & y */
    if (robots[i].speed > 0) {
      robots[i].range += (robots[i].speed / CLICK) * ROBOT_SPEED;
      robots[i].x = (int) (robots[i].org_x + (int)
	    (lcos(robots[i].heading) * (long)(robots[i].range/CLICK) / 10000L));
      robots[i].y = (int) (robots[i].org_y + (int)
	    (lsin(robots[i].heading) * (long)(robots[i].range/CLICK) / 10000L));

      /* check for collision into another robot, less than 1 meter apart */
      for (n = 0; n < MAXROBOTS; n++) {
        if (robots[n].status == DEAD || i == n)
          continue;
	if ( abs(robots[i].x - robots[n].x) < CLICK &&
	     abs(robots[i].y - robots[n].y) < CLICK ) {
	  /* collision, damage moving robot... */
	  robots[i].speed = 0;
	  robots[i].d_speed = 0;
	  robots[i].damage += COLLISION;
	  /* ...and colliding robot */
	  robots[n].speed = 0;
	  robots[n].d_speed = 0;
	  robots[n].damage += COLLISION;
	}
      }

      /* check for collision into a wall */
      if (robots[i].x < 0) {
	robots[i].x = 0;
	robots[i].speed = 0;
	robots[i].d_speed = 0;
	robots[i].damage += COLLISION;
      } else {
	if (robots[i].x > MAX_X * CLICK) {
	  robots[i].x = (MAX_X * CLICK) - 1; 
	  robots[i].speed = 0;
	  robots[i].d_speed = 0;
	  robots[i].damage += COLLISION;
	}
      }
      if (robots[i].y < 0) {
	robots[i].y = 0;
	robots[i].speed = 0;
	robots[i].d_speed = 0;
	robots[i].damage += COLLISION;
      } else {
	if (robots[i].y > MAX_Y * CLICK) {
	  robots[i].y = (MAX_Y * CLICK) - 1; 
	  robots[i].speed = 0;
	  robots[i].d_speed = 0;
	  robots[i].damage += COLLISION;
	}
      }
    }
  }
}



/* move_miss - updates all missile positions */
/*             parm 'displ' control display */

move_miss(displ)
int displ;
{
  register int r, i;
  int n, j;
  int d, x, y;

  /* make sure dead robots are really dead */
  for (r = 0; r < MAXROBOTS; r++) {
    if (robots[r].damage >= 100) {
      robots[r].damage = 100;
      robots[r].status = DEAD;
      if (displ)
	robot_stat(r);
    }

    /* update flying missiles, even ones fired by dead robots before they died*/
    for (i = 0; i < MIS_ROBOT; i++) {

      if (missiles[r][i].stat == FLYING) {

	missiles[r][i].curr_dist += MIS_SPEED;	/* missiles fly at full speed */

	if (missiles[r][i].curr_dist > missiles[r][i].rang)
	  missiles[r][i].curr_dist = missiles[r][i].rang;

	missiles[r][i].cur_x = x = (int) (missiles[r][i].beg_x + (int)
	 (lcos(missiles[r][i].head) * (long)(missiles[r][i].curr_dist/CLICK) / 
								10000L));
	missiles[r][i].cur_y = y = (int) (missiles[r][i].beg_y + (int)
  	 (lsin(missiles[r][i].head) * (long)(missiles[r][i].curr_dist/CLICK) / 
								10000L));
		 
	/* check for missiles hitting walls */
	if (x < 0 ) {
	  missiles[r][i].stat = EXPLODING;
	  x = 1;
	}
	if (x >= MAX_X * CLICK) {
	  missiles[r][i].stat = EXPLODING;
	  x = (MAX_X * CLICK) -1;
	}
	if (y < 0 ) {
	  missiles[r][i].stat = EXPLODING;
	  y = 1;
	}
	if (y > MAX_Y * CLICK) {
	  missiles[r][i].stat = EXPLODING;
	  y = (MAX_Y * CLICK) -1;
	}

	/* check for missiles reaching target range */
	if (missiles[r][i].curr_dist == missiles[r][i].rang)
	  missiles[r][i].stat = EXPLODING;

	/* if missile has exploded, inflict damage on all nearby robots, */
	/* according to hit range */

	if (missiles[r][i].stat == EXPLODING) {
	  for (n = 0; n < MAXROBOTS; n++) {
	    if (robots[n].status == DEAD)
	      continue;
	    x = (robots[n].x - missiles[r][i].cur_x) / CLICK;
	    y = (robots[n].y - missiles[r][i].cur_y) / CLICK;
	    d = (int) sqrt(((double) x * (double) x)+((double) y * (double) y));

	    for (j = 0; j < 3; j++) {
	      if (d < exp_dam[j].dist) {
		robots[n].damage += exp_dam[j].dam;
		break;
	      }
	    }
	    /* kill any robots past 100% damage */
            if (robots[n].damage >= 100) {
	      robots[n].damage = 100;
	      robots[n].status = DEAD;
	      if (displ)
		robot_stat(n);
	    }
	  }
	}
      }
    }
  }
}

