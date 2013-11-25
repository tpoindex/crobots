/* rabbit */
/* rabbit runs around the field, randomly */
/* and never fires;  use as a target */


main()
{

  while(1) {
    go(rand(1000),rand(1000));  /* go somewhere in the field */
  }

}  /* end of main */



/* go - go to the point specified */

go (dest_x, dest_y)
int dest_x, dest_y;
{
  int course;

  course = plot_course(dest_x,dest_y);
  drive(course,25);
  while(distance(loc_x(),loc_y(),dest_x,dest_y) > 50)
    ;
  drive(course,0);
  while (speed() > 0)
    ;
}

/* distance forumula */

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

/* plot_course - figure out which heading to go */

plot_course(xx,yy)
int xx, yy;
{
  int d;
  int x,y;
  int scale;
  int curx, cury;

  scale = 100000;  /* scale for trig functions */

  curx = loc_x();
  cury = loc_y();
  x = curx - xx;
  y = cury - yy;

  if (x == 0) {
    if (yy > cury)
      d = 90;
    else
      d = 270;
  } else {
    if (yy < cury) {
      if (xx > curx)
        d = 360 + atan((scale * y) / x);
      else
        d = 180 + atan((scale * y) / x);
    } else {
      if (xx > curx)
        d = atan((scale * y) / x);
      else
        d = 180 + atan((scale * y) / x);
    }
  }
  return (d);
}

/* end of rabbit.r */
