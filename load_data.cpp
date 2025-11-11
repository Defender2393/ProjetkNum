#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
using namespace std;
bool DevMode;

string getfolder() {
    filesystem::path Filepath = filesystem::current_path();   //Ordnerpath findet den ordnerpfad und setzt Ordnerpath als diesen
    string Ordnerstring = Filepath.string();                  //gibt Ordnerstring mit dem selben information wie ordnerpath als string an//gibt den string aus
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

int main(const int FolderNumber, const char** COrU) {
    ifstream FileC;
    getConfig();
    string FolderNumberstr = to_string(FolderNumber);
    FileC.open(getfolder()+"\\Fallender_Tropfen"+FolderNumberstr+COrU[0]);
    if (!FileC.is_open()&&DevMode==true) {
        cout << "Error in opening file" << endl;
    }
    FileC.ignore(1000, '>');
    int LineNumber;
    FileC >> LineNumber;
    int LineContent[LineNumber][3];
    if (DevMode==true) {
        cout << LineNumber << endl;
    }
    for (int i = 0; i < LineNumber; i++) {
        FileC.ignore(4, '\n');
        FileC >> LineContent[i][0] >> LineContent[i][1] >> LineContent[i][2];
    }
    if (DevMode==true) {
        for (int i = 0; i < LineNumber; i++) {
            for (int j = 0; j < 3; j++) {
                cout << LineContent[i][j] << " ";
            }
            cout << endl;
        }
    }




    return LineNumber;
}