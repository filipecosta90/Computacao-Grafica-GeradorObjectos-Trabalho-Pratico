/*
 *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  * Copyright (C) a64391, a57816, a64377, a57779 
 *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  */

#include <string>
#include "model.hpp"

int main( int argc, char* argv[] ){
  bool parametersOk = true;
  if (argc > 1){
    Model model(argv[1]);
    if (strcmp(argv[1], "plano") == 0){
      if (argc == 7){
        //plano comprimento largura camadasComprimento camadasLargura nomeficheiro
        model.planoXZ_Yfixo(atof(argv[2]), 0, atof(argv[3]), atof(argv[4]), atof(argv[5]), 1);
        model.save(argv[6]);
      }
      else{ parametersOk = false; }
    }
    if (strcmp(argv[1], "paralelipipedo") == 0){
      //paralelipipedo comprimento altura largura camadasComprimento camadasAltura camadasLargura nomeficheiro
      if (argc == 9){
        model.createParallelepipe(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]));
        model.save(argv[8]);
      }
      else { parametersOk = false; }
    }
    if (strcmp(argv[1], "cone") == 0){
      if (argc == 7){
        //cone raio altura fatias camadas nomeficheiro
        model.createCone(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
        model.save(argv[6]);
      }
      else { parametersOk = false; }
    }
    if (strcmp(argv[1], "esfera") == 0){
      if (argc == 6){
        //esfera raio fatias camadas nomeficheiro
        model.createSphere(atof(argv[2]), atof(argv[3]), atof(argv[4]));
		std::cout << "\ttesting normal vector size: " << model.testSizeNormalVector() << "\n";
		std::cout << "\ttesting texture vector size: " << model.testSizeTextureVector() << "\n";
        model.save(argv[5]);
      }
      else { parametersOk = false; }
    }
    if (strcmp(argv[1], "patch") == 0){
      if (argc == 5){
        //patch readFromFile DetailDegree saveToFile
        model.loadPatchFile( argv[2] );
        model.generatePointsFromPatch( atoi ( argv[3] ) );
        model.save(argv[4]);
      }
      else { parametersOk = false; }
    }
    else {
      //nao reconhece o sólido pedido
      parametersOk = false;
    }
  }
  else {
    parametersOk = false;
  }
  if (parametersOk){
    return EXIT_SUCCESS;
  }
  else{
    return EXIT_FAILURE;
  }
}

