
/* sniper */
/* strategy: since a scan of the entire battlefield can be done in 90 */
/* degrees from a corner, sniper can scan the field quickly. */

/* external variables, that can be used by any function */
int corner;           /* current corner 0, 1, 2, or 2 */
int c1x, c1y;         /* corner 1 x and y */
int c2x, c2y;         /*   "    2 "  "  " */
int c3x, c3y;         /*   "    3 "  "  " */
int c4x, c4y;         /*   "    4 "  "  " */
int s1, s2, s3, s4;   /* starting scan position for corner 1 - 4 */
int sc;               /* current scan start */
int d;                /* last damage check */



/* main */
main()
{
  int closest;        /* check for targets in range */
  int range;          /* range to target */
  int dir;            /* scan direction */

  /* initialize the corner info */
  /* x and y location of a corner, and starting scan degree */
  c1x = 10;  c1y = 10;  s1 = 0;
  c2x = 10;  c2y = 990; s2 = 270;
  c3x = 990; c3y = 990; s3 = 180;
  c4x = 990; c4y = 10;  s4 = 90;
  closest = 9999;
  new_corner();       /* start at a random corner */
  d = damage();       /* get current damage */
  dir = sc;           /* starting scan direction */

  while (1) {         /* loop is executed forever */

    while (dir < sc + 90) {  /* scan through 90 degree range */
      range = scan(dir,1);   /* look at a direction */
      if (range <= 700 && range > 0) {
        while (range > 0) {    /* keep firing while in range */
          closest = range;     /* set closest flag */
          cannon(dir,range);   /* fire! */
          range = scan(dir,1); /* check target again */
          if (d + 15 > damage())  /* sustained several hits, */
            range = 0;            /* goto new corner */
        }
        dir -= 10;             /* back up scan, in case */
      }

      dir += 2;                /* increment scan */
      if (d != damage()) {     /* check for damage incurred */
        new_corner();          /* we're hit, move now */
        d = damage();
        dir = sc;
      }
    }

    if (closest == 9999) {       /* check for any targets in range */
      new_corner();             /* nothing, move to new corner */
      d = damage();
      dir = sc;
    } else                      /* targets in range, resume */
      dir = sc;
    closest = 9999;
  }

}  /* end of main */



/* new corner function to move to a different corner */
new_corner() {
  int x, y;
  int angle;
  int new;

  new = rand(4);           /* pick a random corner */
  if (new == corner)       /* but make it different than the */
    corner = (new + 1) % 4;/* current corner */
  else
    corner = new;
  if (corner == 0) {       /* set new x,y and scan start */
    x = c1x;
    y = c1y;
    sc = s1;
  }
  if (corner == 1) {
    x = c2x;
    y = c2y;
    sc = s2;
  }
  if (corner == 2) {
    x = c3x;
    y = c3y;
    sc = s3;
  }
  if (corner == 3) {
    x = c4x;
    y = c4y;
    sc = s4;
  }

  /* find the heading we need to get to the desired corner */
  angle = plot_course(x,y);

  /* start drive train, full speed */
  drive(angle,100);

  /* keep traveling until we are within 100 meters */
  /* speed is checked in case we run into wall, other robot */
  /* not terribly great, since were are doing nothing while moving */

  while (distance(loc_x(),loc_y(),x,y) > 100 && speed() > 0)
    ;

  /* cut speed, and creep the rest of the way */

  drive(angle,20);
  while (distance(loc_x(),loc_y(),x,y) > 10 && speed() > 0)
    ;

  /* stop drive, should coast in the rest of the way */
  drive(angle,0);
}  /* end of new_corner */

/* classical pythagorean distance formula */
distance(x1,y1,x2,y2)
int x1;
int y1;
int x2;
int y2;
{
  int x, y;

  x = x1 - x2;
  y = y1 - y2;
  d = sqrt((x*x) + (y*y));
  return(d);
}

/* plot course function, return degree heading to */
/* reach destination x, y; uses atan() trig function */
plot_course(xx,yy)
int xx, yy;
{
  int d;
  int x,y;
  int scale;
  int curx, cury;

  scale = 100000;  /* scale for trig functions */
  curx = loc_x();  /* get current location */
  cury = loc_y();
  x = curx - xx;
  y = cury - yy;

  /* atan only returns -90 to +90, so figure out how to use */
  /* the atan() value */

  if (x == 0) {      /* x is zero, we either move due north or south */
    if (yy > cury)
      d = 90;        /* north */
    else
      d = 270;       /* south */
  } else {
    if (yy < cury) {
      if (xx > curx)
        d = 360 + atan((scale * y) / x);  /* south-east, quadrant 4 */
      else
        d = 180 + atan((scale * y) / x);  /* south-west, quadrant 3 */
    } else {
      if (xx > curx)
        d = atan((scale * y) / x);        /* north-east, quadrant 1 */
      else
        d = 180 + atan((scale * y) / x);  /* north-west, quadrant 2 */
    }
  }
  return (d);
}

