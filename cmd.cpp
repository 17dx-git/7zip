//g++ GenPassword.cpp Arhive7zip.cpp GetOptions.cpp cmd.cpp -o cmd2
#include <iostream> //для  cout, cin
#include <cstdlib> // для exit
#include <ctime> //  для time
#include "GetOptions.h"
#include "Arhive7zip.h"
#include "GenPassword.h"

using std::cout;
using std::endl;
using std::cin;

const int SUCCESS=0;
const int NO_SUCCESS=-1;


void ExitProg(){
  cin.get(); 
  exit(0);
}

CGenPassword * GetObjGenPassword(CGetOptions * options){
    CGenPassword * genPassword;
    try {
        if (options->lengthPassword >0){
           genPassword=new CGenPassword(options->lengthPassword,options->range);
        }
        else {
           genPassword=new CGenPasswordOnMask(options->mask,options->range);
        }   
    }
    catch(int){
       cout<< genPassword->msgErr<<  endl;
	   ExitProg();
    }
    return genPassword;
}



int  main(int argc, char* argv[])
{   
    CGetOptions * options;
    try{
      options=new CGetOptions(argc,  argv); 
    }
    catch(...){
      ExitProg();
    }
    CArhive7zip arhive7zip(options->arhiveName,options->path7zip);
    CGenPassword * genPassword = GetObjGenPassword(options);
    
    
    time_t tStart, tEnd;
    time(&tStart); // получаем время начала работы программы

    if (not arhive7zip.FindPassword(*genPassword)){
       cout<< "password not found "<< endl;
    };
    time(&tEnd);  // получаем время конца работы программы
    double seconds = difftime(tEnd, tStart);
    cout<< "elapsed time: "<< seconds << " sec"<<endl;
    cin.get();  
	return 0;
}