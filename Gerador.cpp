/*
 *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  * Copyright (C) aXXXXX, a57816, aXXXXX, a57779 
 *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  */

#include <string>
#include <iostream>
#include "model.hpp"

int main( int argc, char* argv[] ){
  Model model(argv[1]);
  if( strcmp ( argv[1] , "plano" ) == 0  ){
    model.createPlan( atof ( argv[2] ) , atof ( argv[3] ) );
    model.save(argv[4]);
  }
  if( strcmp ( argv[1] , "paralelipipedo" ) == 0  ){
    model.createParallelepipe ( atof ( argv[2] ) , atof ( argv[3] ) , atof ( argv[4] )  );
    model.save(argv[5]);
  }
  if( strcmp ( argv[1] , "cone" ) == 0  ){
    model.createCone ( atof ( argv[2] ) , atof ( argv[3] ) , atof ( argv[4] ) , atof ( argv[5] )  );
    model.save(argv[6]);
  }
  if( strcmp ( argv[1] , "esfera" ) == 0  ){
    model.createSphere ( atof ( argv[2] ) , atof ( argv[3] ) , atof ( argv[4] )  );
    model.save(argv[5]);
  }
  return 0;
}

