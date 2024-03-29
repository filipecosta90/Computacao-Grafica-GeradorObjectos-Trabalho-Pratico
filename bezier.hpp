//
//  bezier.hpp
//  testeGLEW
//
//  Created by Filipe Oliveira on 29/04/15.
//  Copyright (c) 2015 Filipe Oliveira. All rights reserved.
//

#ifndef testeGLEW_bezier_hpp
#define testeGLEW_bezier_hpp

#include <vector>
#include <string>
#include <math.h>
#include <iostream>
#include "point.hpp"


/**Computes factorial*/
int fact( int k){
  if(k==0 || k==1){
    return 1;
  }
  else{
    return k * fact(k-1);
  }
}

/**Computes Bernstain
 *parameter int i - the i-th index
 *parameter int n - the total number of points
 *parameter float t - the value of parameter t , between 0 and 1
 **/
float Berstain( int i, int n, float t){
  return fact(n) / (fact(i) * fact(n-i))* pow(t, i) * pow(1-t, n-i);
}

/**Computes a point's coordinates for a value of t
 *parameter float t - a value between 0 and 1
 *parameter std::vector<points> - an vector of [x,y,z] coodinates. The initial points
 **/
Point getBezierAtTime( float t, std::vector<Point> points){
  Point r ( 0.0f , 0.0f , 0.0f) ;
  int n = points.size()-1;
  for(int i=0; i <= n; i++){
    r.x += points[i].x * Berstain( i, n, t);
    r.y += points[i].y * Berstain( i, n, t);
    r.z += points[i].z * Berstain (i, n, t);
  }
  return r;
}

Point computePosition( std::vector< std::vector<Point> > ControlPoints, float u, float v) {
  Point result ( 0.0f, 0.0f , 0.0f );
  int outterOrder = ControlPoints.size() -1;
  for (int i = 0; i <= outterOrder ; i++) {
    int order = ControlPoints[i].size() - 1 ;
    for (int j = 0; j <= order ; j++) {
      float poly_i = Berstain( i, order , u );
      float poly_j = Berstain( j, order , v);
      result.x += poly_i * poly_j * ControlPoints[i][j].x;
      result.y += poly_i * poly_j * ControlPoints[i][j].y;
      result.z += poly_i * poly_j * ControlPoints[i][j].z;
    }
  }
  return result;
}

#endif
