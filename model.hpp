/*
 *  *  *  * Copyright (C) Bruno Gonçalves, Filipe Oliveira, José Sousa, Sérgio Caldas
 *  *  *  * Copyright (C) a64391, a57816, aa64377, a57779
 *  *  *  * Copyright (C) Computação Gráfica, Universidade do Minho, 2015
 *  *  *  */

#ifndef _MODEL_H_INCLUDED_
#define _MODEL_H_INCLUDED_


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

        TiXmlElement * points = new TiXmlElement( "objecto" );
        root->LinkEndChild( points );

        for ( it = pointsVector.begin(); it != pointsVector.end(); ++it)
        {
          std::cout << "entered iterator x:" << it->x << " y: "<< it->y << " z: "<< it->z << "\n";
          pElem = new TiXmlElement ("ponto");
          pElem->SetAttribute ("x", it->x);
          pElem->SetAttribute ("y", it->y);
          pElem->SetAttribute ("z", it->z);
          points->LinkEndChild( pElem );
        }
      }
      doc.SaveFile(pFilename);
    }

    void createPlan( float length, float width ){
      addPoint( Point ( length/2 , 0.0 , width/2 ) );
      addPoint( Point ( length/2 , 0.0 , -width/2 ) );
      addPoint( Point ( -length/2 , 0.0 , width/2 ) );
      addPoint( Point ( length/2 , 0.0 , width/2 ) );
      addPoint( Point ( -length/2 , 0.0 , -width/2 ) );
      addPoint( Point ( -length/2 , 0.0 , width/2 ) );
      std:: cout << "created plan\n";
    }
};

#endif /* _MODEL_H_INCLUDED_ */
