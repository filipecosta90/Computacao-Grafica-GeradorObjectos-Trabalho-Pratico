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
    std::vector<Point> normalVector;
    std::vector<Point> textureVector;

    std::vector<Point>::iterator it;
    std::string patchFileName;
    int numberPatches;
    std::vector< std::vector<int> > patchesVector;
    int numberControlPoints;
    std::vector<Point>bezierControlPoints;

    bool normalVectorDefined;
    bool textureVectorDefined;

    Model ( ) {
      numberPatches = 0;
      normalVectorDefined = false;
      textureVectorDefined = false;
    }

    Model ( std::string mName ){
      modelName = mName;
      numberPatches = 0;
      normalVectorDefined = false;
      textureVectorDefined = false;
    }

    void addPoint( Point p ){
      it = pointsVector.end();
      pointsVector.insert ( it , p );
    }

    void addNormalPoint(Point p){
      if (!normalVectorDefined){
        normalVectorDefined = true;
      }
      it = normalVector.end();
      normalVector.insert(it, p);
    }

    void addTexturePoint(Point p){
      if (!textureVectorDefined){
        textureVectorDefined = true;
      }
      it = textureVector.end();
      textureVector.insert(it, p);
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

    bool testSizeNormalVector(){
      if (normalVector.size() == pointsVector.size()){
        return true;
      }
      else
        return false;
    }
    bool testSizeTextureVector(){
      if (textureVector.size() == pointsVector.size()){
        return true;
      }
      else
        return false;
    }

    void generatePointsFromPatch ( int detailLevel ){
      std::vector< std::vector<int> >::iterator patchesVectorIterator;
      patchesVectorIterator=patchesVector.begin();

      for ( ; patchesVectorIterator != patchesVector.end(); ++patchesVectorIterator ){
        std::vector< std::vector <Point> > BezierLines;

        //first get the correct points for bezier lines
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
      TiXmlElement* pElem, *nElem, *tElem;
      TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
      doc.LinkEndChild( decl );
      bool testNormal, testTexture;
      testNormal = testTexture = false;

      TiXmlElement * root = new TiXmlElement(modelName.c_str());
      if (normalVectorDefined){
        testNormal = testSizeNormalVector();
        if ( testNormal == false ){
          std::cout << "\t something wen't wrong on the normalVector Size! not going to save normal points!\n";
        }
        else {
          root->SetAttribute("defNormal", true);
        }
      }
      if (textureVectorDefined){
        testTexture = testSizeTextureVector();
        if ( testTexture == false ){
          std::cout << "\t something wen't wrong on the textureVector Size! not going to save texture points!\n";
        }
        else {
          root->SetAttribute("defTextura", true);
        }
      }

      doc.LinkEndChild( root );
      // block: pontos

      for ( int pos = 0; pos < pointsVector.size() ; pos++)
      {
        pElem = new TiXmlElement ("ponto");
        pElem->SetDoubleAttribute ("x", pointsVector.at(pos).x);
        pElem->SetDoubleAttribute ("y", pointsVector.at(pos).y);
        pElem->SetDoubleAttribute ("z", pointsVector.at(pos).z);
        root->LinkEndChild( pElem );
        if (normalVectorDefined && testNormal ){
          nElem = new TiXmlElement("normal");
          nElem->SetDoubleAttribute("x", normalVector.at(pos).x);
          nElem->SetDoubleAttribute("y", normalVector.at(pos).y);
          nElem->SetDoubleAttribute("z", normalVector.at(pos).z);
          root->LinkEndChild(nElem);
        }
        if (textureVectorDefined && testTexture ){
          tElem = new TiXmlElement("textura");
          tElem->SetDoubleAttribute("x", textureVector.at(pos).x);
          tElem->SetDoubleAttribute("y", textureVector.at(pos).y);
          root->LinkEndChild(tElem);
        }
      }
      doc.SaveFile(pFilename);
    }


    void planoXZ_Yfixo(float comprimento, float altura, float largura, float fatiasComprimento, float fatiasLargura, int factor){

      float distanciaFatiasLargura = largura / fatiasLargura;
      float distanciaFatiasComprimento = comprimento / fatiasComprimento;

      float distanciaFatiasLarguraUnitario = 1.0f / fatiasLargura;
      float distanciaFatiasComprimentoUnitario = 1.0f / fatiasComprimento;


      for (float posicaoLargura = -(fatiasLargura / 2); posicaoLargura < fatiasLargura / 2; posicaoLargura++){
        for (float posicaoComprimento = -(fatiasComprimento / 2); posicaoComprimento < fatiasComprimento / 2; posicaoComprimento++){

          //ponto A
          addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
          if (factor == 1){
            addNormalPoint ( Point ( 0.0f , 1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
          }
          else if (factor == -1){
            addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
          }

          if (factor == 1){
            //ponto B
            posicaoLargura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 0.0f , 1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );

            //ponto C
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 0.0f , 1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
          }
          else if (factor == -1){
            //ponto C
            posicaoComprimento++;
            posicaoLargura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
            //ponto B
            posicaoComprimento--;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
            posicaoComprimento++;
          }

          posicaoLargura--;
          posicaoComprimento--;

          //ponto A
          addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
          if (factor == 1){
            addNormalPoint ( Point ( 0.0f , 1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
          }
          else if (factor == -1){
            addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
          }


          if (factor == 1){
            //ponto C
            posicaoLargura++;
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 0.0f , 1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );

            //ponto D
            posicaoLargura--;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 0.0f , 1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
            posicaoComprimento--;
          }
          else if (factor == -1){
            //ponto D
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
            //ponto C
            posicaoLargura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, altura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , 1.0f - posicaoLargura * distanciaFatiasLarguraUnitario ) );
            posicaoLargura--;
            posicaoComprimento--;
          }
        }
      }
    }

    void planoXY_Zfixo(float comprimento, float altura, float largura, float fatiasComprimento, float fatiasAltura, int factor){

      float distanciaFatiasAltura = altura / fatiasAltura;
      float distanciaFatiasComprimento = comprimento / fatiasComprimento;
      float distanciaFatiasAlturaUnitario = altura / fatiasAltura;
      float distanciaFatiasComprimentoUnitario = comprimento / fatiasComprimento;

      for (float posicaoAltura = -(fatiasAltura / 2); posicaoAltura < fatiasAltura / 2; posicaoAltura++){
        for (float posicaoComprimento = -(fatiasComprimento / 2); posicaoComprimento < fatiasComprimento / 2; posicaoComprimento++){
          //ponto A
          addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
          if (factor == 1){
            addNormalPoint ( Point ( 0.0f , 0.0f , 1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );
          }
          else if (factor == -1){
            addNormalPoint ( Point ( 0.0f , 0.0f , -1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );
          }

          if (factor == 1){
            //ponto B
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            addNormalPoint ( Point ( 0.0f , 0.0f , 1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );
            //ponto C
            posicaoComprimento--;
            posicaoAltura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            addNormalPoint ( Point ( 0.0f , 0.0f , 1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );
            posicaoAltura--;
          }
          else if (factor == -1){
            //ponto C

            posicaoAltura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            addNormalPoint ( Point ( 0.0f , 0.0f , -1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );
            //ponto B
            posicaoAltura--;
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            addNormalPoint ( Point ( 0.0f , 0.0f , -1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );
            posicaoComprimento--;
          }
          //ponto C
          posicaoAltura++;
          addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
          if (factor == 1){
            addNormalPoint ( Point ( 0.0f , 0.0f , 1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );
          }
          else if (factor == -1){
            addNormalPoint ( Point ( 0.0f , 0.0f , -1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );
          }

          if (factor == 1){
            //ponto B
            posicaoAltura--;
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            addNormalPoint ( Point ( 0.0f , 0.0f , 1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );

            //ponto D
            posicaoAltura++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            addNormalPoint ( Point ( 0.0f , 0.0f , 1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );

            posicaoComprimento--;
            posicaoAltura--;
          }
          else if (factor == -1){
            //ponto D
            posicaoComprimento++;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            addNormalPoint ( Point ( 0.0f , 0.0f , -1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );

            //ponto B
            posicaoAltura--;
            addPoint( Point ( distanciaFatiasComprimento * posicaoComprimento, distanciaFatiasAltura * posicaoAltura, largura));
            addNormalPoint ( Point ( 0.0f , 0.0f , -1.0f ) );
            addTexturePoint ( Point ( posicaoComprimento * distanciaFatiasComprimentoUnitario , posicaoAltura * distanciaFatiasAlturaUnitario ) );

            posicaoComprimento--;
          }
        }
      }
    }

    void planoYZ_Xfixo(float comprimento, float altura, float largura, float fatiasAltura, float fatiasLargura, int factor){

      float distanciaFatiasAltura = altura / fatiasAltura;
      float distanciaFatiasLargura = largura / fatiasLargura;

      float distanciaFatiasAlturaUnitario = altura / fatiasAltura;
      float distanciaFatiasLarguraUnitario = largura / fatiasLargura;

      for (float posicaoAltura = -(fatiasAltura / 2); posicaoAltura < fatiasAltura / 2; posicaoAltura++){
        for (float posicaoLargura = -(fatiasLargura / 2); posicaoLargura < fatiasLargura / 2; posicaoLargura++){
          //ponto A
          addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
          if (factor == 1){
            addNormalPoint ( Point ( 1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );
          }
          else if (factor == -1){
            addNormalPoint ( Point ( -1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );
          }

          if (factor == 1){
            //ponto C
            posicaoAltura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );

            //ponto B
            posicaoAltura--;
            posicaoLargura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );

            posicaoLargura--;
          }
          else if (factor == -1){
            //ponto B
            posicaoLargura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( -1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );

            //ponto C
            posicaoLargura--;
            posicaoAltura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( -1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );

            posicaoAltura--;
          }
          //ponto C
          posicaoAltura++;
          addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
          if (factor == 1){
            addNormalPoint ( Point ( 1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );
          }
          else if (factor == -1){
            addNormalPoint ( Point ( -1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );
          }

          if (factor == 1){
            //ponto D
            posicaoLargura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );

            //ponto B
            posicaoAltura--;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( 1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );
            posicaoLargura--;
          }
          else if (factor == -1){
            //ponto B
            posicaoAltura--;
            posicaoLargura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( -1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );

            //ponto D
            posicaoAltura++;
            addPoint( Point ( comprimento, distanciaFatiasAltura * posicaoAltura, distanciaFatiasLargura * posicaoLargura));
            addNormalPoint ( Point ( -1.0f , 0.0f , 0.0f ) );
            addTexturePoint ( Point ( posicaoLargura * distanciaFatiasLarguraUnitario ,  posicaoAltura * distanciaFatiasAlturaUnitario ) );
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
      float alpha = 360 / (fatias);
      float rad = 0.0;
      rad = ( i * alpha * M_PI ) / 180.0;
      return rad;
    }

    void desenhaTriangulosEntreCamadas(float raioOriginal , float raio, float raioCamadaCima, float altura, float alturaCamadaCima, float fatias, float camadas, int camadaNumero, int factor){
      float rad = 0.0;

      for (int k = 0; k < fatias; k++){
        //camadas do meio
        if (camadaNumero < (camadas - 1)){
          rad = getValorRad(raio, fatias, k);
          //PONTO D -- vamos guardar
          float radCeD = getValorRad(raio, fatias, k + 1);

          float Ux, Uy, Uz;
          float Vx, Vy, Vz;

          if (factor == -1){
            // vector U = point C - point A;
            Ux = raioCamadaCima * sinf(radCeD) - raio * sinf(rad);
            Uy = alturaCamadaCima - altura;
            Uz = raioCamadaCima * cosf(radCeD) - raio * cosf(rad);

            // vectorV = point C - point B;
            Vx = raioCamadaCima * sinf(radCeD) - raioCamadaCima * sinf(rad);
            Vy = alturaCamadaCima - alturaCamadaCima;
            Vz = raioCamadaCima * cosf(radCeD) - raioCamadaCima * cosf(rad);

          }
          else{
            // vector U = point D - point B;
            Ux = raio * sinf(radCeD) - raioCamadaCima * sinf(rad);
            Uy = altura - alturaCamadaCima;
            Uz = raio * cosf(radCeD) - raioCamadaCima * cosf(rad);

            // vectorV = point C - point B;
            Vx = raioCamadaCima * sinf(radCeD) - raioCamadaCima * sinf(rad);
            Vy = alturaCamadaCima - alturaCamadaCima;
            Vz = raioCamadaCima * cosf(radCeD) - raioCamadaCima * cosf(rad);
          }

          // normal vector
          float Nx, Ny, Nz;
          Nx = Uy * Vz - Uz *Vy;
          Ny = ( - ( Uz * Vx - Ux * Vz ) );
          Nz = Ux * Vy - Uy *Vx;

          // calculate the length of the vector
          float len = (float)(sqrt((Nx * Nx) + (Ny * Ny) + (Nz * Nz)));

          // avoid division by 0
          if (len == 0.0f)
            len = 1.0f;

          // reduce to unit size
          Nx /= len;
          Ny /= len;
          Nz /= len;

          Point normalPoint = Point(Nx, Ny, Nz);

          //texture points
          float aX, aY, bX, bY, cX, cY, dX, dY;

          float alturaRaio = altura / raioOriginal;
          float alturaRaioCima = alturaCamadaCima / raioOriginal;

          float yAD = (float) asinf(alturaRaio);
          yAD = yAD / M_PI_2;
          yAD = 0.5f + 0.5f * yAD;
          aY = dY = yAD;

          float yBC = (float)asinf(alturaRaioCima);
          yBC = yBC / M_PI_2;
          yBC = 0.5f + 0.5f * yBC;
          bY = cY = yBC;

          float xAB = rad * 180 / (360 * M_PI);
          aX = bX = xAB;

          float xCD = radCeD * 180 / (360 * M_PI);
          cX = dX = xCD;

          if (factor == 1){
            //Ponto A
            addPoint( Point (raio * sinf(rad), altura, raio * cosf(rad)));
            addNormalPoint( normalPoint );
            addTexturePoint(Point(aX, aY));
            //Ponto C
            addPoint( Point (raioCamadaCima * sinf(radCeD), alturaCamadaCima, raioCamadaCima * cosf(radCeD)));
            addNormalPoint( normalPoint );
            addTexturePoint(Point(cX, cY));
            //Ponto B
            addPoint( Point (raioCamadaCima  * sinf(rad), alturaCamadaCima, raioCamadaCima * cosf(rad)));
            addNormalPoint( normalPoint );
            addTexturePoint(Point(bX, bY));
          }
          else if (factor == -1){
            //Ponto A
            addPoint( Point (raio * sinf(rad), altura, raio * cosf(rad)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(aX, aY));

            //Ponto B
            addPoint( Point (raioCamadaCima  * sinf(rad), alturaCamadaCima, raioCamadaCima * cosf(rad)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(bX, bY));

            //Ponto C
            addPoint( Point (raioCamadaCima * sinf(radCeD), alturaCamadaCima, raioCamadaCima * cosf(radCeD)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(cX, cY));

          }
          if (factor == 1){
            //Ponto A
            addPoint( Point (raio * sinf(rad), altura, raio * cosf(rad)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(aX, aY));

            //Ponto D
            addPoint( Point (raio * sinf(radCeD), altura, raio * cosf(radCeD)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(dX, dY));

            //Ponto C
            addPoint( Point (raioCamadaCima * sinf(radCeD), alturaCamadaCima, raioCamadaCima * cosf(radCeD)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(cX, cY));

          }
          else if (factor == -1){
            //Ponto A
            addPoint( Point (raio * sinf(rad), altura, raio * cosf(rad)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(aX, aY));

            //Ponto C
            addPoint( Point (raioCamadaCima * sinf(radCeD), alturaCamadaCima, raioCamadaCima * cosf(radCeD)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(cX, cY));

            //Ponto D
            addPoint( Point (raio * sinf(radCeD), altura, raio * cosf(radCeD)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(dX, dY));

          }
        }
        //camada final
        if (camadaNumero == camadas - 1){
          rad = getValorRad(raio, fatias, k);
          float radCeD = getValorRad(raio, fatias, k + 1);

          //texture points
          float aX, aY, bX, bY, cX, cY, dX, dY;

          float alturaRaio = altura / raioOriginal;
          float alturaRaioCima = alturaCamadaCima / raioOriginal;

          float yAD = (float)asinf(alturaRaio);
          yAD = yAD / M_PI_2;
          yAD = 0.5f + 0.5f * yAD;
          aY = dY = yAD;

          float yBC = (float)asinf(alturaRaioCima);
          yBC = yBC / M_PI_2;
          yBC = 0.5f + 0.5f * yBC;
          bY = cY = yBC;

          float xAB = rad * 180 / (360 * M_PI);
          aX = bX = xAB;

          float xCD = radCeD * 180 / (360 * M_PI);
          cX = dX = xCD;

          //camada superior da esfera
          float Ux, Uy, Uz;
          float Vx, Vy, Vz;

          if (factor == -1){
            // vector U = point C - point A;
            Ux = raioCamadaCima * sinf(radCeD) - raio * sinf(rad);
            Uy = alturaCamadaCima - altura;
            Uz = raioCamadaCima * cosf(radCeD) - raio * cosf(rad);

            // vectorV = point C - point B;
            Vx = raioCamadaCima * sinf(radCeD) - raioCamadaCima * sinf(rad);
            Vy = alturaCamadaCima - alturaCamadaCima;
            Vz = raioCamadaCima * cosf(radCeD) - raioCamadaCima * cosf(rad);

          }
          else{
            // vector U = point D - point B;
            Ux = raio * sinf(radCeD) - raioCamadaCima * sinf(rad);
            Uy = altura - alturaCamadaCima;
            Uz = raio * cosf(radCeD) - raioCamadaCima * cosf(rad);

            // vectorV = point C - point B;
            Vx = raioCamadaCima * sinf(radCeD) - raioCamadaCima * sinf(rad);
            Vy = alturaCamadaCima - alturaCamadaCima;
            Vz = raioCamadaCima * cosf(radCeD) - raioCamadaCima * cosf(rad);
          }

          // normal vector
          float Nx, Ny, Nz;
          Nx = Uy * Vz - Uz *Vy;
          Ny = -(Uz * Vx - Ux * Vz);
          Nz = Ux * Vy - Uy *Vx;

          // calculate the length of the vector
          float len = (float)(sqrt((Nx * Nx) + (Ny * Ny) + (Nz * Nz)));

          // avoid division by 0
          if (len == 0.0f)
            len = 1.0f;

          // reduce to unit size
          Nx /= len;
          Ny /= len;
          Nz /= len;

          Point normalPoint = Point(Nx, Ny, Nz);

          if (factor == 1){
            //Ponto A
            addPoint( Point (raio * sinf(rad), altura, raio * cosf(rad)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(aX, aY));

            //Ponto D
            addPoint( Point (raio *  sinf(radCeD), altura, raio * cosf(radCeD)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(dX, dY));

            //Ponto C
            addPoint( Point (0.0, alturaCamadaCima, 0.0));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(cX, cY));
          }

          //camada inferior da esfera
          else if (factor == -1){
            //Ponto A
            addPoint( Point (raio * sinf(rad), altura, raio * cosf(rad)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(aX, aY));

            //Ponto C
            addPoint( Point (0.0, alturaCamadaCima, 0.0));
            addNormalPoint ( normalPoint );
            addTexturePoint(Point(cX, cY));

            //Ponto D
            addPoint( Point (raio *  sinf(radCeD), altura, raio * cosf(radCeD)));
            addNormalPoint(normalPoint);
            addTexturePoint(Point(dX, dY));

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
        addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );

        //Ponto B
        addPoint( Point (raio * -sinf(rad), altura, raio * cosf(rad)));
        addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );

        //Ponto C
        i++;
        rad = i * alpha * M_PI / 180.0;
        addPoint( Point (raio * -sinf(rad), altura, raio * cosf(rad)));
        addNormalPoint ( Point ( 0.0f , -1.0f , 0.0f ) );
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
        desenhaTriangulosEntreCamadas ( raio, raio - (j * ReducaoRaioPorAltura), raio - ((j + 1) * ReducaoRaioPorAltura), (j)* alturaPorCamadas, (j + 1)* alturaPorCamadas, fatias, camadas, j,1);
      }
    }

    void createSphere(float raio, float fatias, float camadas){

      float alturaPorCamadas = 2 * raio / (camadas);
      float raioAtual = raio;
      float raioAnterior;
      float alturaAtual;
      int camadasInt = (int) camadas;

      //desenha a parte superior da esfera
      for (int j = 0; j < camadasInt / 2; j++){
        raioAnterior = raioAtual;
        alturaAtual = (alturaPorCamadas*(j + 1));
        if (raio <= alturaAtual){
          raioAtual = 0;
        }
        else{
          raioAtual = sqrt((raio*raio) - (alturaAtual*alturaAtual));
        }
        desenhaTriangulosEntreCamadas( raio , raioAnterior, raioAtual, (j)* alturaPorCamadas, (j + 1)* alturaPorCamadas, fatias, camadas, j, 1);
      }
      //desenha parte inferior da esfera
      alturaPorCamadas = -alturaPorCamadas;
      raioAtual = raio;
      for (int j = 0; j < camadasInt / 2; j++){
        raioAnterior = raioAtual;
        alturaAtual = (alturaPorCamadas*(j + 1));
        raioAtual = sqrt((raio*raio) - (alturaAtual*alturaAtual));
        desenhaTriangulosEntreCamadas( raio , raioAnterior, raioAtual, (j)* alturaPorCamadas, (j + 1)* alturaPorCamadas, fatias, camadas, j, -1);
      }
    }
};

#endif /* _MODEL_H_INCLUDED_ */
