/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _MODEL_H_INCLUDED_
#define _MODEL_H_INCLUDED_

#define _USE_MATH_DEFINES
#include "point.hpp"

#include "bezier.hpp"

#include "tinyxml.cpp"
#include "tinyxml.h"
#include "tinyxmlerror.cpp"
#include "tinyxmlparser.cpp"
#include "tinystr.cpp"
#include "tinystr.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>     /* atoi */

#include <vector>
#include <string>
#include <math.h>

class Model {
  public:
    std::string modelName;
    std::vector<Point> pointsVector;
    std::vector<Point>::iterator it;
    std::string patchFileName;
    int numberPatches;
    std::vector< std::vector<int> > patchesVector;
    int numberControlPoints;
    std::vector<Point>bezierControlPoints;

    Model ( ) {
      numberPatches = 0;
    }

    Model ( std::string mName ){
      modelName = mName;
      numberPatches = 0;
    }

    void addPoint( Point p ){
      it = pointsVector.end();
      pointsVector.insert ( it , p );
    }

    void loadPatchFile( std::string filename ){
      std::ifstream infile( filename );

      if( infile ){
        std::string s;
        getline( infile, s );
        numberPatches= atoi( s.c_str() );
      }

      int positionPatch = 1;
      while (infile && positionPatch <= numberPatches ){
        std::vector<int> patch;
        std::string s;
        if (!getline( infile, s )) break;
        std::istringstream ss( s );

        while (ss)
        {
          int record;
          std::string s;
          if (!getline( ss, s, ',' )) break;
          record = atoi ( s.c_str());
          patch.push_back( record );
        }
        patchesVector.push_back( patch );
        positionPatch++;
      }

      if( infile ){
        std::string s;
        getline( infile, s );
        numberControlPoints = atoi( s.c_str() );
      }

      int positionControlPoint = 1;
      while (infile && positionControlPoint <= numberControlPoints ){
        float pX, pY, pZ;
        std::string s;
        if (!getline( infile, s )) break;

        std::istringstream ss( s );

        getline( ss, s, ',' );
        pX = atof ( s.c_str());
        getline( ss, s, ',' );
        pY = atof ( s.c_str());
        getline( ss, s, ',' );
        pZ = atof ( s.c_str());
        Point newPoint ( pX , pY , pZ );
        bezierControlPoints.push_back( newPoint );
        positionPatch++;
      }
    }

    void generatePointsFromPatch ( int detailLevel ){
      std::vector< std::vector<int> >::iterator patchesVectorIterator;
      patchesVectorIterator=patchesVector.begin();

      for ( ; patchesVectorIterator != patchesVector.end(); ++patchesVectorIterator ){
        std::vector<std::vector <Point>> BezierLines;

        //first get the correct points for bezier lines
        int patchSize = patchesVectorIterator->size();
        for ( int lineNumber = 1; lineNumber <=4 ; lineNumber++ ){
          std::vector <Point> BezierLine;
          for ( int patchPosition = 4*(lineNumber-1) ; patchPosition < 4*lineNumber ; patchPosition++ ){
            int positionInControlPoints = (*patchesVectorIterator)[patchPosition];
            Point newPoint ( bezierControlPoints[ positionInControlPoints ]);
            BezierLine.push_back( newPoint );
          }
          BezierLines.push_back( BezierLine );
        }

        //then get the points of the line based on the detail
        Point pointA, pointB, pointC, pointD;

        for (int ru = 0; ru <= detailLevel-1; ru++) {
          float u = 1.0 * ru / (detailLevel-1);
          for (int rv = 0; rv <= detailLevel-1; rv++) {
            float v = 1.0 * rv / (detailLevel-1);
            pointA = computePosition( BezierLines , u, v);
            rv++;
            v = 1.0 * rv / (detailLevel-1);
            pointB = computePosition( BezierLines , u, v);
            ru++;
            u = 1.0 * ru / (detailLevel-1);
            pointC = computePosition( BezierLines , u, v);
            rv--;
            v = 1.0 * rv / (detailLevel-1);
            pointD = computePosition( BezierLines , u, v);
            ru--;
            u = 1.0 * ru / (detailLevel-1);
            addPoint( Point (pointA) );
            addPoint( Point (pointB) );
            addPoint( Point (pointC) );
            addPoint( Point (pointC) );
            addPoint( Point (pointD) );
            addPoint( Point (pointA) );
          }
        }
      }
    }

    void load(){
      TiXmlDocument doc( modelName.c_str() );
      if (!doc.LoadFile()) return;
      TiXmlHandle hDoc(&doc);
      TiXmlElement* pElem;
      TiXmlHandle hRoot(0);

      pElem=hDoc.FirstChildElement().Element();
      if (!pElem) return;
      hRoot=TiXmlHandle(pElem);
      // block: pontos
      {
        float pX, pY, pZ;
        pElem=hRoot.FirstChild( "ponto" ).Element();
        for( ; pElem; pElem=pElem->NextSiblingElement())
        {
          pElem->QueryFloatAttribute("x", &pX);
          pElem->QueryFloatAttribute("y", &pY);
          pElem->QueryFloatAttribute("z", &pZ);
          if ( pX && pY && pZ ){
            Point newPoint ( pX, pY, pZ );
            addPoint ( newPoint );
          }
        }
      }
    }



    void save( const char * pFilename )
    {
      TiXmlDocument doc;
      TiXmlElement* pElem;
      TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
      doc.LinkEndChild( decl );

      TiXmlElement * root = new TiXmlElement(modelName.c_str());  
      doc.LinkEndChild( root );
      // block: pontos
      {
        for ( it = pointsVector.begin(); it != pointsVector.end(); ++it)
        {
          pElem = new TiXmlElement ("ponto");
          pElem->SetDoubleAttribute ("x", it->x);
          pElem->SetDoubleAttribute ("y", it->y);
          pElem->SetDoubleAttribute ("z", it->z);
          root->LinkEndChild( pElem );
        }
      }
      doc.SaveFile(pFilename);
    }


    void planoXZ_Yfixo(float comprimento, float altura, float largura, float fatiasComprimento, float fatiasLargura, int factor){
      // pôr instruções de desenho aqui

      float distanciaFatiasLargura = largura / fatiasLargura;
      float distanciaFatiasComprimento = comprimento / fatiasComprimento;

      for (float posicaoLargura = -(fatiasLargura / 2); posicaoLargura < fatiasLargura / 2; posicaoLargura++){
        for (float posicaoComprimento = -(fatiasComprimento / 2); posicaoComprimento < fatiasComprimento / 2; posicaoComprimento++){
          //ponto A
          addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
          if (factor == 1){
            //ponto B
            posicaoLargura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            //ponto C
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
          }
          else if (factor == -1){
            //ponto C
            posicaoComprimento++;
            posicaoLargura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            //ponto B
            posicaoComprimento--;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            posicaoComprimento++;
          }

          posicaoLargura--;
          posicaoComprimento--;

          //ponto A
          addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
          if (factor == 1){
            //ponto C
            posicaoLargura++;
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            //ponto D
            posicaoLargura--;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            posicaoComprimento--;
          }
          else if (factor == -1){
            //ponto D
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            //ponto C
            posicaoLargura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            posicaoLargura--;
            posicaoComprimento--;
          }
        }
      }
    }

    void planoXY_Zfixo(float comprimento, float altura, float largura, float fatiasComprimento, float fatiasAltura, int factor){

      float distanciaFatiasAltura = altura / fatiasAltura;
      float distanciaFatiasComprimento = comprimento / fatiasComprimento;

      for (float posicaoAltura = -(fatiasAltura / 2); posicaoAltura < fatiasAltura / 2; posicaoAltura++){
        for (float posicaoComprimento = -(fatiasComprimento / 2); posicaoComprimento < fatiasComprimento / 2; posicaoComprimento++){
          //ponto A
          addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
          if (factor == 1){
            //ponto B
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            //ponto C
            posicaoComprimento--;
            posicaoAltura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            posicaoAltura--;
          }
          else if (factor == -1){
            //ponto C

            posicaoAltura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            //ponto B
            posicaoAltura--;
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            posicaoComprimento--;
          }
          //ponto C
          posicaoAltura++;
          addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
          if (factor == 1){
            //ponto B
            posicaoAltura--;
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            //ponto D
            posicaoAltura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            posicaoComprimento--;
            posicaoAltura--;
          }
          else if (factor == -1){
            //ponto D
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            //ponto B
            posicaoAltura--;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            posicaoComprimento--;
          }
        }
      }
    }

    void planoYZ_Xfixo(float comprimento, float altura, float largura, float fatiasAltura, float fatiasLargura, int factor){

      float distanciaFatiasAltura = altura / fatiasAltura;
      float distanciaFatiasLargura = largura / fatiasLargura;

      for (float posicaoAltura = -(fatiasAltura / 2); posicaoAltura < fatiasAltura / 2; posicaoAltura++){
        for (float posicaoLargura = -(fatiasLargura / 2); posicaoLargura < fatiasLargura / 2; posicaoLargura++){
          //ponto A
          addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
          if (factor == 1){
            //ponto C
            posicaoAltura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            //ponto B
            posicaoAltura--;
            posicaoLargura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            posicaoLargura--;
          }
          else if (factor == -1){
            //ponto B
            posicaoLargura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            //ponto C
            posicaoLargura--;
            posicaoAltura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            posicaoAltura--;
          }
          //ponto C
          posicaoAltura++;
          addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
          if (factor == 1){
            //ponto D
            posicaoLargura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            //ponto B
            posicaoAltura--;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            posicaoLargura--;
          }
          else if (factor == -1){
            //ponto B
            posicaoAltura--;
            posicaoLargura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            //ponto D
            posicaoAltura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            posicaoLargura--;
            posicaoAltura--;
          }
        }
      }
    }

    void createParallelepipe ( float comprimento, float altura, float largura, float fatiasComprimento, float fatiasAltura, float fatiasLargura ){

      //Topo
      planoXZ_Yfixo(comprimento, altura / 2, largura, fatiasComprimento, fatiasLargura, 1);

      //Base 
      planoXZ_Yfixo(comprimento, -altura / 2, largura, fatiasComprimento, fatiasLargura, -1);

      //Lado 1
      planoYZ_Xfixo(comprimento / 2, altura, largura, fatiasAltura, fatiasLargura, 1);

      //Lado 2
      planoYZ_Xfixo(-comprimento / 2, altura, largura, fatiasAltura, fatiasLargura, -1);

      //Frente
      planoXY_Zfixo(comprimento, altura, largura / 2, fatiasComprimento, fatiasAltura, 1);

      //Tras
      planoXY_Zfixo(comprimento, altura, -largura / 2, fatiasComprimento, fatiasAltura, -1);
    }

    float getValorRad(float raio, float fatias, int i){
      float alpha = 360 / fatias;
      float rad = 0.0;
      rad = i * alpha * M_PI / 180.0;
      return rad;
    }

    void desenhaTriangulosEntreCamadas(float raio, float raioCamadaCima, float altura, float alturaCamadaCima, float fatias, float camadas, int camadaNumero, int factor){
      float alturaPorCamadas = altura / camadas;
      float ReducaoRaioPorAltura = raio / camadas;
      float rad = 0.0;

      for (int k = 0; k < fatias; k++){
        //camadas do meio
        if (camadaNumero < (camadas - 1)){
          rad = getValorRad(raio, fatias, k);
          //PONTO D -- vamos guardar
          float radCeD = getValorRad(raio, fatias, k + 1);
          if (factor == 1){
            //Ponto A
            addPoint( Point (raio * sin(rad), altura, raio * cos(rad)));
            //Ponto C
            addPoint( Point (raioCamadaCima * sin(radCeD), alturaCamadaCima, raioCamadaCima * cos(radCeD)));
            //Ponto B
            addPoint( Point (raioCamadaCima  * sin(rad), alturaCamadaCima, raioCamadaCima * cos(rad)));
          }
          else if (factor == -1){
            //Ponto A
            addPoint( Point (raio * sin(rad), altura, raio * cos(rad)));
            //Ponto B
            addPoint( Point (raioCamadaCima  * sin(rad), alturaCamadaCima, raioCamadaCima * cos(rad)));
            //Ponto C
            addPoint( Point (raioCamadaCima * sin(radCeD), alturaCamadaCima, raioCamadaCima * cos(radCeD)));
          }
          if (factor == 1){
            //Ponto A
            addPoint( Point (raio * sin(rad), altura, raio * cos(rad)));
            //Ponto D
            addPoint( Point (raio * sin(radCeD), altura, raio * cos(radCeD)));
            //Ponto C
            addPoint( Point (raioCamadaCima * sin(radCeD), alturaCamadaCima, raioCamadaCima * cos(radCeD)));
          }
          else if (factor == -1){
            //Ponto A
            addPoint( Point (raio * sin(rad), altura, raio * cos(rad)));
            //Ponto C
            addPoint( Point (raioCamadaCima * sin(radCeD), alturaCamadaCima, raioCamadaCima * cos(radCeD)));
            //Ponto D
            addPoint( Point (raio * sin(radCeD), altura, raio * cos(radCeD)));
          }
        }
        //camada final
        if (camadaNumero == camadas - 1){
          rad = getValorRad(raio, fatias, k);
          //camada superior da esfera
          if (factor == 1){
            //Ponto A
            addPoint( Point (raio * sin(rad), altura, raio * cos(rad)));
            //Ponto D
            float radCeD = getValorRad(raio, fatias, k + 1);
            addPoint( Point (raio *  sin(radCeD), altura, raio * cos(radCeD)));
            //Ponto C
            addPoint( Point (0.0, alturaCamadaCima, 0.0));
          }
          //camada inferior da esfera
          else if (factor == -1){
            //Ponto A
            addPoint( Point (raio * sin(rad), altura, raio * cos(rad)));
            //Ponto C
            addPoint( Point (0.0, alturaCamadaCima, 0.0));
            //Ponto D
            float radCeD = getValorRad(raio, fatias, k + 1);
            addPoint( Point (raio *  sin(radCeD), altura, raio * cos(radCeD)));
          }
        }
      }
    }

    void desenhaBaseCircularCone(float raio, float altura, float fatias){

      // desenhar a base
      float alpha = 360 / fatias;
      float rad = 0.0;
      for (int i = 0; i < fatias; i++){
        rad = getValorRad(raio, fatias, i);
        //Ponto A
        addPoint( Point (0.0f, altura, 0.0f));
        //Ponto B
        addPoint( Point (raio * -sin(rad), altura, raio * cos(rad)));
        //Ponto C
        i++;
        rad = i * alpha * M_PI / 180.0;
        addPoint( Point (raio * -sin(rad), altura, raio * cos(rad)));
        i--;
      }
    }

    void createCone(float raio, float altura, float fatias, float camadas){
      float alturaPorCamadas = altura / camadas;
      float ReducaoRaioPorAltura = raio / camadas;

      //desenha a base
      desenhaBaseCircularCone(raio, 0, fatias);

      //desenha as camadas laterias
      for (int j = 0; j <= camadas; j++){
        desenhaTriangulosEntreCamadas(raio - (j * ReducaoRaioPorAltura), raio - ((j + 1) * ReducaoRaioPorAltura), (j)* alturaPorCamadas, (j + 1)* alturaPorCamadas, fatias, camadas, j,1);
      }
    }

    void createSphere(float raio, float fatias, float camadas){

      float alturaPorCamadas = 2 * raio / (camadas);
      float raioAtual = raio;
      float raioAnterior;
      float alturaAtual;
      int camadasInt = (int) camadas;

      //desenha a parte superior da esfera
      for (int j = 0; j <= camadasInt / 2; j++){
        raioAnterior = raioAtual;
        alturaAtual = (alturaPorCamadas*(j + 1));
        if (raio <= alturaAtual){
          raioAtual = 0;
        }
        else{
          raioAtual = sqrt((raio*raio) - (alturaAtual*alturaAtual));
        }
        desenhaTriangulosEntreCamadas(raioAnterior, raioAtual, (j)* alturaPorCamadas, (j + 1)* alturaPorCamadas, fatias, camadas, j, 1);
      }
      //desenha parte inferior da esfera
      alturaPorCamadas = -alturaPorCamadas;
      raioAtual = raio;
      for (int j = 0; j <= camadas / 2; j++){
        raioAnterior = raioAtual;
        alturaAtual = (alturaPorCamadas*(j + 1));
        raioAtual = sqrt((raio*raio) - (alturaAtual*alturaAtual));
        desenhaTriangulosEntreCamadas(raioAnterior, raioAtual, (j)* alturaPorCamadas, (j + 1)* alturaPorCamadas, fatias, camadas, j, -1);
      }
    }
};

#endif /* _MODEL_H_INCLUDED_ */
