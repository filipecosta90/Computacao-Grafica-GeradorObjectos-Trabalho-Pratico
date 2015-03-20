/*
 *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  * Copyright (C) a64391, a57816, a64377, a57779 
 *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  */

#include <string>
#include "model.hpp"

int main( int argc, char* argv[] ){
  Model model(argv[1]);
  if( strcmp ( argv[1] , "plano" ) == 0 ){
	  model.planoXZ_Yfixo(atof(argv[2]), 0, atof(argv[3]), atof ( argv[4]), atof (argv[5] ), 1 );
    model.save(argv[6]);
  }
  if( strcmp ( argv[1] , "paralelipipedo" ) == 0  ){
	  model.createParallelepipe(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]) );
    model.save(argv[8]);
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
