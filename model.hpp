/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _MODEL_H_INCLUDED_
#define _MODEL_H_INCLUDED_

#define _USE_MATH_DEFINES

#include "point.hpp"

#include "tinyxml.cpp"
#include "tinyxml.h"
#include "tinyxmlerror.cpp"
#include "tinyxmlparser.cpp"
#include "tinystr.cpp"
#include "tinystr.h"

#include <vector>
#include <string>
#include <iostream>
#include <math.h>



class Model {
  public:
    std::string modelName;
    std::vector<Point> pointsVector;
    std::vector<Point>::iterator it;

    Model ( ) {
    }

    Model ( std::string mName ){
      modelName = mName;

    }

    void load()
    {
      TiXmlDocument doc( modelName.c_str() );
      if (!doc.LoadFile()) return;
      std::cout << modelName.c_str()<<"\n";
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
          std::cout << pX << "\t" << pY << "\t" << pZ << "\n";
          if ( pX && pY && pZ ){
            Point newPoint ( pX, pY, pZ );
            addPoint ( newPoint );
          }
        }
      }
    }

    void addPoint( Point p ){
      it = pointsVector.end();
      pointsVector.insert ( it , p );
    }

    void save( const char * pFilename )
    {
      std::cout << "going to save " << modelName << " in: " << pFilename << "\n";
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
          std::cout << "entered iterator x:" << it->x << " y: "<< it->y << " z: "<< it->z << "\n";
          pElem = new TiXmlElement ("ponto");
          pElem->SetDoubleAttribute ("x", it->x);
          pElem->SetDoubleAttribute ("y", it->y);
          pElem->SetDoubleAttribute ("z", it->z);
          root->LinkEndChild( pElem );
        }
      }
      doc.SaveFile(pFilename);
    }

    void createPlan( float length, float width ){
      addPoint( Point ( length/2 , 0.0 , width/2 ) );
      addPoint( Point ( length/2 , 0.0 , -width/2 ) );
      addPoint( Point ( -length/2 , 0.0 , -width/2 ) );
      addPoint( Point ( length/2 , 0.0 , width/2 ) );
      addPoint( Point ( -length/2 , 0.0 , -width/2 ) );
      addPoint( Point ( -length/2 , 0.0 , width/2 ) );
    }

    void createParallelepipe ( float comprimento, float largura , float altura ){

      //Base 1
      addPoint( Point ( comprimento / 2, -altura / 2, -largura / 2) );
      addPoint( Point (comprimento / 2, -altura / 2, largura / 2) );
      addPoint( Point (-comprimento / 2, -altura / 2, largura / 2) );

      //Base 2
      addPoint( Point (-comprimento / 2, -altura / 2, largura / 2) );
      addPoint( Point (-comprimento / 2, -altura / 2, -largura / 2) );
      addPoint( Point (comprimento / 2, -altura / 2, -largura / 2) );

      //Lado 1.1
      addPoint( Point (comprimento / 2, -altura / 2, largura / 2) );
      addPoint( Point (comprimento / 2, -altura / 2, -largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, largura / 2) );


      //Lado 1.2
      addPoint( Point (comprimento / 2, -altura / 2, -largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, -largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, largura / 2) );


      //Topo 1
      addPoint( Point (-comprimento / 2, altura / 2, largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, -largura / 2) );

      //Topo 2
      addPoint( Point (-comprimento / 2, altura / 2, -largura / 2) );
      addPoint( Point (-comprimento / 2, altura / 2, largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, -largura / 2) );

      //Lado 2.1
      addPoint( Point (-comprimento / 2, -altura / 2, largura / 2) );
      addPoint( Point (-comprimento / 2, altura / 2, largura / 2) );
      addPoint( Point (-comprimento / 2, -altura / 2, -largura / 2) );

      //Lado 2.2
      addPoint( Point (-comprimento / 2, -altura / 2, -largura / 2) );
      addPoint( Point (-comprimento / 2, altura / 2, largura / 2) );
      addPoint( Point (-comprimento / 2, altura / 2, -largura / 2) );

      //Frente 1
      addPoint( Point (-comprimento / 2, -altura / 2, largura / 2) );
      addPoint( Point (comprimento / 2, -altura / 2, largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, largura / 2) );

      //Frente 2
      addPoint( Point (-comprimento / 2, -altura / 2, largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, largura / 2) );
      addPoint( Point (-comprimento / 2, altura / 2, largura / 2) );

      //Tras 1
      addPoint( Point (comprimento / 2, -altura / 2, -largura / 2) );
      addPoint( Point (-comprimento / 2, -altura / 2, -largura / 2) );
      addPoint( Point (comprimento / 2, altura / 2, -largura / 2) );

      //Tras 2
      addPoint( Point (comprimento / 2, altura / 2, -largura / 2) );
      addPoint( Point (-comprimento / 2, -altura / 2, -largura / 2) );
      addPoint( Point (-comprimento / 2, altura / 2, -largura / 2) );

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
