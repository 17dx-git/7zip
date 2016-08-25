#include "GenPassword.h"
#include "ParseRangesInMask.h"


#include "GetOptions.h"

CAbstractGenPassword::CAbstractGenPassword(){
     codeError=ERROR_NONE;
}

int CAbstractGenPassword::LastError(){
  return codeError;
};

//�����������
CGenPassword::CGenPassword(){
  startLen=0;
  endLen=0;
  maxLen=0;
  stepOnLen=0; 
}

CGenPassword::CGenPassword(string& lenRange, string& range){
  ParseRangeLength (lenRange);
  if (LastError()!=ERROR_NONE){ return; }
  //������ ������ ������ endLen>0
  Init(startLen,range);
   
  numbPassword=0;
  CreatePassword(); //������� ��������� ������  
}


void CGenPassword::ReInit(int len){ 
  lenPassword=len;  
  //delete[] arr ;
  //arr = new char [lenPassword]; //�������� ������ ��� ������
  for(int i=0; i < lenPassword; i++){
    //arr[i]=minValue;
    chars[i]->ValueToFloor();
  }  
};

//������������������� ����������
// ::��� ������� ���������� �����������
void CGenPassword::Init(int len,string& range){
  if (maxLen==0 && startLen==0 && endLen==0){ 
       maxLen=len;
       startLen=len;
       endLen=len;
  } //�� �������???
  chars=new CRangeChar* [maxLen] ; // todo ��������� ��� � ��� ������ � �� ������ � ������
  chars[0]=new CRangeChar(range); 
  if (chars[0]->LastError() != ERROR_NONE) { 
     msgErr=chars[0]->msgErr ;
     codeError=chars[0]->LastError();
     return ;
  }
  for(int i=1; i < maxLen; i++){
     //chars[i] = new CRangeChar(chars[0]->GetPCharRange()) ;
     chars[i] = new CRangeChar(*chars[0]) ;
  } 
  Init(len);//startLen!! ������ ���� ,chars
}

// todo ����������
void CGenPassword::Init(int len){ //,CRangeChar** ranges//, string& range
  lenPassword=len; 
  if (maxLen==0 && startLen==0 && endLen==0){ 
       maxLen=len;
       startLen=len;
       endLen=len;
  } //�� �������???
  if (len==0) {
       msgErr="little length password";
       codeError=ERROR_LITTLE_LENGTH_PASSWORD;
       return ;
       //throw THROW_LITTLE_LENGTH_PASSWORD;
  }

  for(int i=0; i < lenPassword; i++){
    chars[i]->ValueToFloor();
  }  
};
    



int CGenPassword::StringToInt(string  s){
  int length=s.length();

  for (int i=0; i<length; i++){
    if (not isdigit(s[i]))
    {
       msgErr="no digital value";
       codeError=ERROR_OPTION_NO_DIGITAL_ARGUMENT;
       return 0;
    }
  }
  return atoi(s.c_str());
}

void CGenPassword::ParseRangeLength(string& range){
  startLen=0;
  endLen=0;
  stepOnLen=0;
  maxLen=0;
  std::size_t pos= range.find("-"); 
  if (pos == string::npos ){
    startLen= StringToInt(range);
    if(LastError() != ERROR_NONE) return;
    endLen= startLen;
  }
  else{
    if (pos == range.size()){
       msgErr="no correct value range length password";
       codeError=ERROR_RANGE_LENGTH_NOT_VALID;
       return ;
    }
    else{
        startLen= StringToInt(range.substr(0,pos));
        if(LastError() != ERROR_NONE) return;
        
        endLen= StringToInt(range.substr(pos+1));
        if(LastError() != ERROR_NONE) return;
        
        stepOnLen=(endLen-startLen > 0 ) ? 1: -1;        
    }     
  }  
  
  if (( startLen==0) || ( endLen==0)) {
       msgErr="little length password";
       codeError=ERROR_LITTLE_LENGTH_PASSWORD;
       return ;
  }
  maxLen=(startLen>endLen)? startLen:endLen;
  return ;
};
    


//����������
CGenPassword::~CGenPassword(){
  delete [] chars;//arr; //����������� ������
}

//������� ��������� ������
bool CGenPassword::Next(){
  if ( not Inc(lenPassword-1) ){
    if (endLen !=0 && lenPassword != endLen){   
     // endLen !=0 �� ������ ������������� �� ����� ����� -l    
      lenPassword+=stepOnLen;
      ReInit(lenPassword);
      CreatePassword(); //��������� ������� ��� �������� ��� �������� � ������ �������� ���������� ������
      return true;
    }
    else{
      return false;
    }  
  }
  CreatePassword();  
  return true;  
}

 void CGenPassword::CreatePassword(){
   password="";
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password+=chars[i]->GetValue();
   }
 }
 
 double CGenPassword::CountPasswords(){
   //if (endLen !=0){
       double count=0;
       int i = startLen-stepOnLen;
       do{
           i+=stepOnLen;
           double combination_count=1;
           for(int j=0;j<i;j++){
              combination_count*=chars[j]->GetRangeSize();
           }
           count+=  combination_count;//pow(charRange.size(),i);       
       }while (i != endLen);
       return  ceil(count);
   //}
    //return  ceil(pow(charRange.size(),lenPassword)); 
   
 }

//��������� ������
bool CGenPassword::Inc(int index){

  if ( not chars[index]->Inc()/*arr[index] == maxValue*/  ){
     // ���� ������������ ����� ������� � ������� ������
     // �� � ������� ������ ������ � ������ 
     chars[index]->ValueToFloor();     
     
     if (index ==0){
       //����� �������� ������� ���, ������ ����������� ������
       return false;
     } 
     else{
       //� � ���������� ������(� ������� �������) ��������� ������
       return Inc(index-1);
     }     
  }
  return true;
}

//////////////////////////////////////////////////////////////////
//////////////////////��������� ������ GenPassword //////////////
//////////////////////////////////////////////////////////////////

 CGenPasswordOnMask::CGenPasswordOnMask(string& mask,string& range){
   int lenMask=mask.length();
   int countPointInsert=0;
   for(int i = 0; i < lenMask; i++){
        if(mask[i] == '*'){
          posPointInsert.push_back(i);
          countPointInsert++;
        }
   }   
   Init(countPointInsert, range); // ������� � �������������� ������
   password=mask; 
   numbPassword=0;
   CreatePassword();   
 }
 
 void CGenPasswordOnMask::CreatePassword(){
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password[posPointInsert[i]]=chars[i]->GetValue();//charRange[arr[i]];
   }
 }
 


CGenPasswordOnMask::CGenPasswordOnMask(string& ext_mask){
   int lenMask=ext_mask.length();
   
   vector <string> ranges;
   CParseRangesInMask rangesInMask=CParseRangesInMask(ext_mask,&ranges, &posPointInsert);
   rangesInMask.Parse();
   if (rangesInMask.LastError()!=ERROR_NONE){
     codeError=rangesInMask.LastError();
     msgErr =rangesInMask.msgErr;    
     return ;
   }
   password=rangesInMask.newMask;
   if (password.empty()){
     msgErr="little length password, extended mask not valid";
     codeError=ERROR_EXT_MASK_NOT_VALID;
     return ;
     
   }
   int countPointInsert = ranges.size();
   chars=new CRangeChar* [countPointInsert] ; // todo ��������� ��� � ��� ������ � �� ������ � ������
   for(int i=0; i < countPointInsert; i++){
     chars[i] = new CRangeChar(ranges[i]) ;
     if (chars[i]->LastError() != ERROR_NONE) { 
         msgErr=chars[i]->msgErr ;
         codeError=chars[i]->LastError();
         return ;
      }
   } 
   Init(countPointInsert);
   numbPassword=0;
   CreatePassword();
}



 
 //////////////////////////////////////////////////////////////////
//////////////////////��������� ������ CAbstractGenPassword //////////////
//////////////////////////////////////////////////////////////////
 
 CGenPasswordFromDict::CGenPasswordFromDict(string dicPath_,int translitType_){
   dicPath= dicPath_;
   translitType=translitType_;
   
   trInit();
   CalcCountPassword();  
   if (countPasswords==0){
      msgErr="error dictonary empty";
      codeError=ERROR_DICTONARY_EMPTY;
      return;
   }
   numbPassword=0;
   queueToTranslit=not DO_TRANSLIT;
   fr.open( dicPath.c_str());//������ ��������� ���� ������  
   //todo fr.is_open?
   Next(); // todo ���������� ���������

 }
 
void  CGenPasswordFromDict::CalcCountPassword(){
   countPasswords=0;
   string tmp;
   
   fr.open( dicPath.c_str());
   
   while(!fr.eof()){
     std::getline(fr,tmp);
     if (not tmp.empty()){
        countPasswords++;
     }   
   }  
   fr.close();   
   //fr.seekg (0, fr.beg); //
   if (translitType==TYPE_TRANSLIT_BOTH){
      countPasswords*=2;
   }
   
}


void CGenPasswordFromDict::trInit(){
   for (int i=0 ;i<256;i++ ){
     tr[i]=0;
   }
   tr['z']='�';  tr['Z']='�';
   tr['x']='�';  tr['X']='�';
   tr['c']='�';  tr['C']='�';
   tr['v']='�';  tr['V']='�';
   tr['b']='�';  tr['B']='�';
   tr['n']='�';  tr['N']='�';
   tr['m']='�';  tr['M']='�';
   tr[',']='�';  tr['<']='�';
   tr['.']='�';  tr['>']='�';
   tr['/']='.';  tr['?']=',';
   tr['a']='�';  tr['A']='�';
   tr['s']='�';  tr['S']='�';
   tr['d']='�';  tr['D']='�';
   tr['f']='�';  tr['F']='�';
   tr['g']='�';  tr['G']='�';
   tr['h']='�';  tr['H']='�';
   tr['j']='�';  tr['J']='�';
   tr['k']='�';  tr['K']='�';
   tr['l']='�';  tr['L']='�';
   tr[';']='�';  tr[':']='�';
   tr['\'']='�'; tr['"']='�';
   tr['q']='�';  tr['Q']='�';
   tr['w']='�';  tr['W']='�';
   tr['e']='�';  tr['E']='�';
   tr['r']='�';  tr['R']='�';
   tr['t']='�';  tr['T']='�';
   tr['y']='�';  tr['Y']='�';
   tr['u']='�';  tr['U']='�';
   tr['i']='�';  tr['I']='�';
   tr['o']='�';  tr['O']='�';
   tr['p']='�';  tr['P']='�';
   tr['[']='�';  tr['{']='�';
   tr[']']='�';  tr['}']='�';
   tr['`']='�';  tr['~']='�';
                 tr['@']='"'; 
                 tr['#']='�';  
                 tr['$']=';';   
                 tr['^']=':';
                 tr['&']='?'; 
                 tr['|']='/'; 
 
   for (int i=0 ;i<256;i++ ){
     if (tr[i]==0){
        tr[i]=i;
        //���� �� ����
     }
     else { 
        tr[ tr[i]]=i;
        //�������� ������
     }
     
   }  
}

void CGenPasswordFromDict::TranslitPassword(){
   int lenPassword=password.size();
   for(int i=0 ; i<lenPassword; i++){
      password[i]= tr[static_cast<unsigned char> (password[i])] ; 
   } 
 }
//#include <iostream> 
bool  CGenPasswordFromDict::ReadNextPassword(){
  if (!fr.eof()){
         std::getline(fr,password);
         //if (fr.tellg()==-1) return false;;
         if (password.empty()){
           return ReadNextPassword();
         }
         numbPassword++;
         return true;
  } 
  return false;
 }
 
 bool  CGenPasswordFromDict::Next(){
   if (translitType==TYPE_TRANSLIT_BOTH ){ 
           bool result =true;   
           if (queueToTranslit==DO_TRANSLIT){                 
                 TranslitPassword();
                 numbPassword++;
           }      
           else{
                result = ReadNextPassword();
           }                 
           queueToTranslit= not queueToTranslit;
           //������������ �����������
           return result;
   }
   else if (translitType==TYPE_TRANSLIT_ONLY ){
      bool result = ReadNextPassword();
      TranslitPassword();          
      return result;      
   }
   
   else if (translitType==TYPE_TRANSLIT_NONE){
     return ReadNextPassword();
   }
 
   return false;
 }

 double CGenPasswordFromDict::CountPasswords(){

   return countPasswords;

 }
 
  CGenPasswordFromDict::~CGenPasswordFromDict(){
    fr.close(); 
  }