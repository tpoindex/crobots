/* counter */
/* scan in a counter-clockwise direction (increasing degrees) */
/* moves when hit */

main()
{
  int angle, range;
  int res;
  register int d;
  long i;

  res = 1;
  d = damage();
  angle = rand(360);
  while(1) {
    while ((range = scan(angle,res)) > 0) {
      if (range > 700) { /* out of range, head toward it */
        drive(angle,50);
        i = 1;
        while (i++ < 50) /* use a counter to limit move time */
          ;
        drive (angle,0);
        if (d != damage()) {
          d = damage();
          run();
        }
        angle -= 3;
      } else {
        cannon(angle,range);
        while (cannon(angle,range) == 0)
          ;
        if (d != damage()) {
          d = damage();
          run();
        }
        angle -=15;
      }
    }
    if (d != damage()) {
      d = damage();
      run();
    }
    angle += res;
    angle %= 360;
  }
}


int last_dir;

/* run moves around the center of the field */

run()
{
  int x, y;
  int i;

  x = loc_x();
  y = loc_y();

  if (last_dir == 0) {
    if (y > 512) {
      last_dir = 1;
      drive(270,100);
      while (y -100 < loc_y() && i++ < 100)
        ;
      drive(270,0);
    } else {
      last_dir = 1;
      drive(90,100);
      while (y +100 > loc_y() && i++ < 100)
        ;
      drive(90,0);
    }
  } else {
    if (x > 512) {
      last_dir = 0;
      drive(180,100);
      while (x -100 < loc_x() && i++ < 100)
        ;
      drive(180,0);
    } else {
      last_dir = 0;
      drive(0,100);
      while (x +100 > loc_x() && i++ < 100)
        ;
      drive(0,0);
    }
  }
}

/* end of counter.r */
