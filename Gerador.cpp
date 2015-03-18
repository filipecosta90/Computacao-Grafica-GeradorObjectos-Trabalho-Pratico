#include <iostream>
#include <fstream>
#include <string>
#include "model.hpp"


void plano(float largura, float comprimento, char *file_name){

  FILE* filename;

  filename = fopen(file_name, "w");

  fprintf(filename, "%f %f %f\n", comprimento / 2, 0.0, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, 0.0, -largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, 0.0, largura / 2);

  fprintf(filename, "%f %f %f\n", comprimento / 2, 0.0, largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, 0.0, -largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, 0.0, largura / 2);

  fclose(filename);
}

void paralelipipedo(float largura, float comprimento, float altura, char *file_name){
  FILE *filename;

  filename = fopen(file_name, "w");

  //Base 1
  fprintf(filename, "%f %f %f\n", comprimento / 2, -altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, -altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, largura / 2);

  //Base 2 
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, -altura / 2, -largura / 2);

  //Lado 1.1
  fprintf(filename, "%f %f %f\n", comprimento / 2, -altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, -altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, largura / 2);

  //Lado 1.2
  fprintf(filename, "%f %f %f\n", comprimento / 2, -altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, largura / 2);

  //Topo 1
  fprintf(filename, "%f %f %f\n", -comprimento / 2, altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, -largura / 2);

  //Topo 2
  fprintf(filename, "%f %f %f\n", -comprimento / 2, altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, -largura / 2);

  //Lado 2.1
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, -largura / 2);

  //Lado 2.2
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, altura / 2, -largura / 2);

  //Frente 1
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, -altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, largura / 2);

  //Frente 2
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, altura / 2, largura / 2);

  //Tras 1
  fprintf(filename, "%f %f %f\n", comprimento / 2, -altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, -largura / 2);

  //Tras 2
  fprintf(filename, "%f %f %f\n", comprimento / 2, altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, -altura / 2, -largura / 2);
  fprintf(filename, "%f %f %f\n", -comprimento / 2, altura / 2, -largura / 2);

  fclose(filename);
}

int main(int argc, char *argv[]){
  /*switch (strcmp ( argv[0]){
    case "plano" : plano(atof(argv[1]), atof(argv[2]), argv[3]);
    break;
    case "paralelipipedo": paralelipipedo(atof(argv[1]), atof(argv[2]), atof(argv[3]), argv[4]);
    break;
    default:
    break;
    }*/

  plano(10.0, 10.0,"plano.3d");
  paralelipipedo(12,12,12,"paralelipipedo.3d");
}
