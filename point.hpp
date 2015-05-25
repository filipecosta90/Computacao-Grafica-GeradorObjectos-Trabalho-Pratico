/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _POINT_H_INCLUDED_
#define _POINT_H_INCLUDED_

class Point {
  public:

    float x;
    float y;
    float z;

    Point ( float aX, float aY, float aZ ) {
      x = aX;
      y = aY;
      z = aZ;
    }

    Point (){
    }

    Point(float aX, float aY) {
      x = aX;
      y = aY;
      z = 0.0f;
    }
};

#endif /* _POINT_H_INCLUDED_ */

