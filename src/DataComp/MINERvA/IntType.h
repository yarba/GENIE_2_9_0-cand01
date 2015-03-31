#ifndef Val_MINERvA_IntType_H
#define Val_MINERvA_IntType_H

#include <TObject.h>
class TRootIOCtor;

class IntType : public TObject
{

   public:
   
      // ctor & dtor
      IntType() : TObject(), fProjectilePDG(-1), fNucleonPDG(-1) {};
      IntType( int proj, int nuc ) : TObject(), fProjectilePDG(proj), fNucleonPDG(nuc) {};
      IntType( TRootIOCtor* ) : TObject(), fProjectilePDG(-1), fNucleonPDG(-1) {};
      ~IntType() {}; // no need to cleanup by hands
      
      IntType& operator=( const IntType& type );  
      bool     operator==( const IntType& other) const;
      
      void SetType( const int proj, const int nuc ) { fProjectilePDG=proj; fNucleonPDG=nuc; return; }
      bool SetType( const std::string& projname, const std::string& nucname );
      
      const int GetProjectilePDG() const { return fProjectilePDG; }
      const int GetTgtNucleonPDG() const { return fNucleonPDG; }
      const char* GetProjectileName() const; 
      const char* GetTgtNucleonName() const;
      
      bool CheckInteraction( int proj, int nuc ) { return ( fProjectilePDG==proj && fNucleonPDG==nuc ); }
   
   private:
   
      int fProjectilePDG;
      int fNucleonPDG;

ClassDef(IntType,2)

};

#endif
