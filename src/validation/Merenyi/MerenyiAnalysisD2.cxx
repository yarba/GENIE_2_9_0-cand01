//____________________________________________________________________________
/*
 Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

 Author: Pauli Kehayias (Tufts Univ.)

 For the class documentation see the corresponding header file.

 Important revisions after version 2.0.0 :
 @ Jan 13, 2009 - PK
   This file was added in v2.5.1

*/
//____________________________________________________________________________

#include <iostream>

#include "validation/Merenyi/MerenyiAnalysisD2.h"

using namespace genie;
using namespace genie::vld_merenyi;

//____________________________________________________________________________
void MerenyiAnalysisD2::classifyHadrons(MerenyiNuEvent& myEvent)
{
  int hadId;

  for(unsigned int i = 0; i < myEvent.getHadList().size(); i++) {

      // first element in the particle vector
      hadId = myEvent.getHadList()[i].GetPdgCode();

      switch (hadId) {
        case 11:        //e-
        case -11:       //e+
        case 22:        //gamma
        case 13:        //mu-
        case -13:       //mu+
          myEvent.addGam();     //events with these are rejected
          break;

        case 111:       //pi0
          myEvent.addPi0();
          break;

        case 211:       //pi+
          myEvent.addPiP();
          MerenyiAnalysis::findPiMom(myEvent.getHadList()[i]);
          break;

        case -211:      //pi-
          myEvent.addPiM();
          MerenyiAnalysis::findPiMom(myEvent.getHadList()[i]);
          break;

        case 2112:      //n
          myEvent.addN();
          break;

        case 2212:      //p
          myEvent.addP();
          break;

        case 130:       //k0 L
        case 310:       //k0 s
        case 3122:      //Lamda0
          myEvent.addStr0();
          break;

        case 321:       //k+
        case 3222:      //Sigma+
          myEvent.addStrP();
          break;

        case -321:      //k-
          myEvent.addStrM();
          break;

        default:
          cout << "Unknown hadron type " << hadId << " in event " << numEvents << endl;
        }
    }
}

//----------------------------------------------------------------------------

