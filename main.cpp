//
// Created by bosch on 10.11.2025.
//
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
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

int GetData(const int FolderNumber, const char COrU) {
    ifstream FileC;                                         //erstellt FileC als lesedatei
    string FolderNumberstr = to_string(FolderNumber);       //gibt FolderNumber als string an
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\"+COrU);      //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
    if (!FileC.is_open()&&DevMode==true) {                              //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
        cout << "Error in opening file" << endl;
    }
    FileC.ignore(1000, '>');            //geht bis nach dem symbol > in der Datei, hinter diesem befinden sich die Werte
    int LineNumber;
    FileC >> LineNumber; //holt sich die zahl die hier steht als Int
    FileC.ignore(1000, '(');
    int LineContent[LineNumber][3];             //array zum speichern der drei datenmengen
    if (DevMode==true){                         //wenn devmode true gibt menge der linien an
        cout << LineNumber << endl;
    }
    for (int i = 0; i < LineNumber; i++) {                  //wiederholt programm für dateimenge
        FileC.ignore(4, '(');                      //
        FileC >> LineContent[i][0] >> LineContent[i][1] >> LineContent[i][2];           // liest drei werte aus der datei aus
    }

    if (COrU == 'C') {
        FileC.ignore(1000, '>');
        int LineNumber1;
        FileC >> LineNumber1;
        float LineContent1[LineNumber1][3];
        FileC.ignore(1, '(');
        for (int i = 0; i < LineNumber1; i++) {
            FileC.ignore(4, '(');
            FileC >> LineContent1[i][0] >> LineContent1[i][1] >> LineContent1[i][2];
            FileC.ignore(1000, '>');
            int LineNumber2;
            FileC >> LineNumber2;
            float LineContent2[LineNumber2][3];
            FileC.ignore(1, '(');
            for (int j = 0; j < LineNumber1; j++) {
                FileC.ignore(4, '(');
                FileC >> LineContent2[i][0] >> LineContent2[i][1] >> LineContent2[i][2];
        }
    }
    else if (COrU == 'U') {
        FileC.ignore(1000, 'uniform');
        int LineNumber3[3];
        FileC.ignore(1, '(');
        FileC >> LineNumber3[0] >> LineNumber3[1] >> LineNumber3[2];
    }
    else {
        cout << "Error COrU is neither C nor U" << endl;
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
int main() {
        getConfig();


        return 0;
    }