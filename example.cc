#include "xldl.h"
#include <exception>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>

using std::cout;
using std::endl;
using std::wcin;
using std::fixed;
using std::setprecision;

bool progress(float percent,float size,float total,float speed){
    if(percent*100 >= 99.99){
        return true;
    }else{
        cout << '\r' << fixed << setprecision(2)
            << "Progress: " << percent*100 << "%/100% "
            << "Size: " << size << "MB/" << total << "MB "
            << "Speed: " << speed << "MB/S";
        return false;
    }
}
int main(void){
    ThunderEngine engine;//This must be kept, otherwise the platform will be shutdown.
    engine.Init();
    try
    {
        std::wstring url,filename;

        InputData:{
            cout << "Enter your URL> " ;
            wcin >> url; 
            cout << endl;
            cout << "Enter your FileName> " ;
            wcin >> filename; 
            cout << endl;
            if(url == L"" || filename == L"") goto Complete;
        }


        DoDownLoad:{
            auto task = engine.CreateTask(url.data(), L"./", filename.data(), TRUE);
            task.Start();
            auto info = task.QueryEx();
            while(
                    !progress(
                        info.fPercent, 
                        info.nTotalDownload / float(1024 *1024), 
                        info.nTotalSize  / float(1024 *1024), 
                        info.nSpeed / float(1024 *1024)
                        )
                ){
                info = task.QueryEx();
                Sleep(200);
            }
        }
        
        Complete:{

        cout << endl << "Complete, press enter to exit!" << endl;
        
        wcin.get() && wcin.get();
        
        }
    }catch(XLException &e){
        
        cout << endl <<"Error: " << e;
    }
    return 0;
};