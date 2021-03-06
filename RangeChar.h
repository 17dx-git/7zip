#ifndef RANGECHAR_H
#define RANGECHAR_H

#include <string>
#include <vector>
#include "noerrors.h"
#include "common.h"

typedef  unsigned char TIndexInRange;

using std::string;
using std::vector;

class CRangeChar{
public:

  CRangeChar(const CRangeChar & obj);
  CRangeChar(string charRange_) ; 
  bool Inc() ;
  void ValueToFloor() ;
  char GetValue() ; 
  void SetValue(char nv) ; 
  void SetValueAsIndex(TIndexInRange nv);
  size_t GetRangeSize() ;
  CEventError eventError;
  string charRangeAsString;
protected:
  TIndexInRange value;  
  TIndexInRange minValue; // todo всегда ноль
  TIndexInRange maxValue;  //todo хранить по size charRange 
  vector<char> * charRange;   
  void ParseRangeChar(string& range);
  void AppendRange(char firstChar,char lastChar);
  
};

#endif // RANGECHAR_H