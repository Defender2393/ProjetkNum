#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
using namespace std;
bool DevMode;

string getfolder() {
    filesystem::path Ordnerpath = filesystem::current_path();   //Ordnerpath findet den ordnerpfad und setzt Ordnerpath als diesen
    string Ordnerstring = Ordnerpath.string();                  //gibt Ordnerstring mit dem selben information wie ordnerpath als string an
    cout << Ordnerstring << endl;                               //gibt den string aus
    return Ordnerstring;                                        //gibt als die Funktionswerk Ordnerstring aus
}
void getConfig() {
    ifstream config(getfolder()+"\\Config.txt");              //öffnet config
    if (!config.is_open()) {
        cout << "Error in opening config file" << endl;            //error wenn config nicht gefunen werden kann
    }
    config.ignore(100, '=');                            //geht bis zu 100 zeichen nach dem nächsten =
    config >> DevMode;
    if (DevMode) {
        cout << DevMode << endl;                                //wenn devmode=1 ist gibt es 1 aus
    }

    config.close();
}

int main() {
    ifstream FileC;                         //noch workinprogress
    getConfig();
    FileC.open(getfolder()+"");


    return 0;
}