#include "GenPassword.h"
#include <sstream> // ��� stringstream
#include <cmath> // ��� pow
#include <cstdlib> //��� atoi
#include "GetOptions.h"

CAbstractGenPassword::CAbstractGenPassword(){
     codeError=ERROR_NONE;
}

int CAbstractGenPassword::LastError(){
  return codeError;
};

//�����������
CGenPassword::CGenPassword(){
  minLen=0;
  maxLen=0;
  stepOnLen=0; 
}

CGenPassword::CGenPassword(string& lenRange, string& range){
  ParseRangeLength (lenRange); 
  Init(minLen,range);
  numbPassword=0;
  CreatePassword(); //������� ��������� ������  
}


void CGenPassword::ReInit(int len){ 
  lenPassword=len;  
  delete[] arr ;
  arr = new char [lenPassword]; //�������� ������ ��� ������
  for(int i=0; i < lenPassword; i++){
    arr[i]=minValue;
  }  
};

//������������������� ����������
// ::��� ������� ���������� �����������
void CGenPassword::Init(int len, string& range){ 
  ParseRangeChar(range);
  lenPassword=len; 
  if (len==0) {
       msgErr="little length password";
       codeError=ERROR_LITTLE_LENGTH_PASSWORD;
       return ;
       //throw THROW_LITTLE_LENGTH_PASSWORD;
  }
  arr = new char [lenPassword]; //�������� ������ ��� ������
  for(int i=0; i < lenPassword; i++){
    arr[i]=minValue;
  }  
};
    
void CGenPassword::AppendRange(char firstChar,char lastChar){  
    char step = (firstChar<lastChar) ? 1 : -1;
    char ch=firstChar-step;
    do {
       ch+=step;
       charRange.push_back(ch);       
    }  while(ch-lastChar !=0);
}


int CGenPassword::StringToInt(string  s){
  int length=s.length();

  for (int i=0; i<length; i++){
    if (not isdigit(s[i]))
    {
       msgErr="no digital value by length password";
       codeError=ERROR_OPTION_NO_DIGITAL_ARGUMENT;
       return 0;
       //throw THROW_OPTION_NO_DIGITAL_ARGUMENT ;
    }
  }
  return atoi(s.c_str());
}

void CGenPassword::ParseRangeLength(string& range){
  minLen=0;
  maxLen=0;
  stepOnLen=0;
  std::size_t pos= range.find("-"); 
  if (pos == string::npos ){
    minLen= StringToInt(range);
    if(LastError() != ERROR_NONE) return;
    maxLen= minLen;
  }
  else{
    if (pos == range.size()){
       msgErr="no correct value range length password";
       codeError=ERROR_RANGE_LENGTH_NOT_VALID;
       return ;
       //throw THROW_RANGE_LENGTH_NOT_VALID ;
    }
    else{
        minLen= StringToInt(range.substr(0,pos));
        if(LastError() != ERROR_NONE) return;
        
        maxLen= StringToInt(range.substr(pos+1));
        if(LastError() != ERROR_NONE) return;
        
        stepOnLen=(maxLen-minLen > 0 ) ? 1: -1;        
    }     
  }  
  
  if (( minLen==0) && ( maxLen==0)) {
       msgErr="little length password";
       codeError=ERROR_LITTLE_LENGTH_PASSWORD;
       return ;
       //throw THROW_LITTLE_LENGTH_PASSWORD;
  }
  return ;
};
    
void CGenPassword::ParseRangeChar(string& range){
  std::stringstream ss(range);
  string item; 
  char firstChar;
  char lastChar;
  while (std::getline(ss, item, ',')){
    if(item.length()==3){
      if(item[1]=='-'){
          firstChar = item[0]; // � ������ ������� ������
          lastChar = item[2]; //����� �������� ���������   
          AppendRange (firstChar,lastChar);         
      }
    } 
    else if(item.length()==1)  {
       charRange.push_back(item[0]); 
    }
    
  }
  
  if (charRange.size()==0){
     msgErr= "range char not valid";
     codeError=ERROR_RANGE_CHAR_NOT_VALID;
     return ;
     //throw THROW_RANGE_CHAR_NOT_VALID;
  }
  else{
    minValue=0;
    maxValue=charRange.size()-1;
  }
};

//����������
CGenPassword::~CGenPassword(){
  delete [] arr; //����������� ������
}

//������� ��������� ������
bool CGenPassword::Next(){
  if ( not Inc(lenPassword-1) ){
    if (maxLen !=0 && lenPassword != maxLen){      
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
     password+=charRange[arr[i]];
   }
   //password.append(arr, lenPassword);
 }
 
 double CGenPassword::CountPasswords(){
   if (maxLen !=0){
       double count=0;
       int i = minLen-stepOnLen;
       do{
           i+=stepOnLen;
           count+=  pow(charRange.size(),i);       
       }while (i != maxLen);
       return  ceil(count);
   }
    return  ceil(pow(charRange.size(),lenPassword)); 
   
 }

//��������� ������
bool CGenPassword::Inc(int index){

  if ( arr[index] == maxValue  ){
     // ���� ������������ ����� ������� � ������� ������
     // �� � ������� ������ ������ � ������     
     arr[index] = minValue;
     
     if (index ==0){
       //����� �������� ������� ���, ������ ����������� ������
       return false;
     } 
     else{
       //� � ���������� ������(� ������� �������) ��������� ������
       return Inc(index-1);
     }     
  }
  // ���� �� ��� ��� ������� �� ���������,
  //  �� ����� ������ ��������� ������
  arr[index]++;
  return true;

}

//////////////////////////////////////////////////////////////////
//////////////////////��������� ������ GenPassword //////////////
//////////////////////////////////////////////////////////////////

 CGenPasswordOnMask::CGenPasswordOnMask(string mask,string& range){
   int lenMask=mask.length();
   int countStar=0;
   for(int i = 0; i < lenMask; i++){
        if(mask[i] == '*'){
          posStar.push_back(i);
          countStar++;
        }
   }   
   Init(countStar, range); // ������� � �������������� ������
   password=mask; 
   numbPassword=0;
   CreatePassword();   
 }
 
 void CGenPasswordOnMask::CreatePassword(){
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password[posStar[i]]=charRange[arr[i]];
   }
 }
 
 //////////////////////////////////////////////////////////////////
//////////////////////��������� ������ CAbstractGenPassword //////////////
//////////////////////////////////////////////////////////////////
 
 CGenPasswordFromDict::CGenPasswordFromDict(string dicPath_,int translitType_){
   dicPath= dicPath_;
   translitType=translitType_;
   
   trInit();
   CalcCountPassword();  
   
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