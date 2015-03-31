//____________________________________________________________________________
/*!

\class    genie::mc_vs_data::MBDataRelease

\brief    Class to hold labels of the MiniBooNE data releases         
          
\author   Chris Barry <cbarry \at hep.ph.liv.ac.uk>
          University of Liverpool
	  
	  Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          University of Liverpool & STFC Rutherford Appleton Laboratory    

\created  Jan 13, 2015

\cpright  Copyright (c) 2003-2015, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________


#ifndef _MINIBOONE_DATA_RELEASE_H_
#define _MINIBOONE_DATA_RELEASE_H_

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

namespace genie {
namespace mc_vs_data {

typedef enum EMBDataRelease {

   kMBUndefined    = -1, 
   kMBCCQENu2010,
   kMBCCQENuBar2013

} MBDataRelease_t; 

class MBDataRelease {
public:
   static const char * AsString(MBDataRelease_t d) {
     switch (d) { 
       case kMBUndefined:	return "Undefined";                 break;
       case kMBCCQENu2010:	return "miniboone_ccqenu2010";      break;
       case kMBCCQENuBar2013:	return "miniboone_ccqenubar2013";   break;
       default:                 break;
     }
     return "??";
   } 
   static const char * AsRooTeX(MBDataRelease_t d) {
     switch (d) { 
       case kMBUndefined:	return "Undefined";                               break;
       case kMBCCQENu2010:	return "MiniBooNE #nu_{#mu} CCQE (2010)";         break;
       case kMBCCQENuBar2013:	return "MiniBooNE #bar{#nu}_{#mu} CCQE (2013)";   break;
       default:                 break;
     }
     return "??";
   } 
   static MBDataRelease_t FromString(string d) {
     if(d == "ccqenu2010"    || d == "miniboone_ccqenu2010"   ) return kMBCCQENu2010;
     if(d == "ccqenubar2013" || d == "miniboone_ccqenubar2013") return kMBCCQENuBar2013;
     return kMBUndefined;
   } 
};

} // mc_vs_data namespace
} // genie namepace

#endif // _MINIBOONE_DATA_RELEASE_H_
