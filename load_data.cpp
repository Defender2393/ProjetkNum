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
    ifstream FileC;                                         //erstellt FileC als lesedatei
    getConfig();                                            //holt sich die Config usw
    string FolderNumberstr = to_string(FolderNumber);       //gibt FolderNumber als string an
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\"+COrU[0]);      //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
    if (!FileC.is_open()&&DevMode==true) {                              //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
        cout << "Error in opening file" << endl;
    }
    FileC.ignore(1000, '>');            //geht bis nach dem symbol > in der Datei, hinter diesem befinden sich die Werte
    int LineNumber;
    FileC >> LineNumber;                        //holt sich die zahl die hier steht als Int
    int LineContent[LineNumber][3];             //array zum speichern der drei datenmengen
    if (DevMode==true){                         //wenn devmode true gibt menge der linien an
        cout << LineNumber << endl;
    }
    for (int i = 0; i < LineNumber; i++) {                  //wiederholt programm für dateimenge
        FileC.ignore(4, '\n');                      //überspringt nicht zahleneinheiten
        FileC >> LineContent[i][0] >> LineContent[i][1] >> LineContent[i][2];           // liest drei werte aus der datei aus
    }
    if (DevMode==true) {
        for (int i = 0; i < LineNumber; i++) {          //spuckt die werte wieder aus wemnn DevMode==true
            for (int j = 0; j < 3; j++) {
                cout << LineContent[i][j] << " ";
            }
            cout << endl;
        }
    }




    return LineNumber;
}