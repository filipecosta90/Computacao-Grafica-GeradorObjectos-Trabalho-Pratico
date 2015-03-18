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
    std::cout << "vai criar plano\n" ;
    model.createPlan( atof ( argv[2] ) , atof ( argv[3] ) );
  }
  std::cout << argv[4] <<"\n";
  model.save(argv[4]);
  return 0;
}

