//
// Created by bosch on 10.11.2025.
//
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include "GeleseneDatenC.h"
using namespace std;
bool DevMode=false;
int LineNumber[4];
int kinematicLine;
char Character = 'C';
string FolderNumber;
vector<vector<float >> LineContent;
vector<array<float ,9 >> kinematicContent;


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


int GetData(const string FolderNumberstr) {
    ifstream FileC; //erstellt FileC als lesedatei
    for (int z= 0; z < 2 ; z++){
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\"+Character);      //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
    if (!FileC.is_open()&&DevMode==true) {                              //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
        cout << "Error in opening file" << endl;
    }
    FileC.ignore(1000, '>');            //geht bis nach dem symbol > in der Datei, hinter diesem befinden sich die Werte
    FileC >> LineNumber[0+3*z]; //holt sich die zahl die hier steht als Int
    FileC.ignore(1000, '(');            //array zum speichern der drei datenmengen
    if (DevMode){                         //wenn devmode true gibt menge der linien an
        cout << LineNumber[0] << endl;
    }
    for (int i = 0; i < LineNumber[0]; i++) {                  //wiederholt programm für dateimenge
        FileC.ignore(4, '(');                      //
        FileC >> LineContent[i][0+3*z] >> LineContent[i][1+3*z] >> LineContent[i][2+3*z];           // liest drei werte aus der datei aus
    }

    if (Character == 'C') {
        FileC.ignore(1000, '>');
        int LineNumber1;
        FileC >> LineNumber1;
        FileC.ignore(1, '(');
        for (int i = 0; i < LineNumber1; i++) {
            FileC.ignore(4, '(');
            FileC >> LineContent[i][6] >> LineContent[i][7] >> LineContent[i][8];
        }
        FileC.ignore(1000, '>');
        FileC >> LineNumber[2];
        FileC.ignore(1, '(');
        for (int j = 0; j < LineNumber1; j++) {
            FileC.ignore(4, '(');
            FileC >> LineContent[j][9] >> LineContent[j][10] >> LineContent[j][11];
        }
        Character='U';
        FileC.close();
    }
    }
    FileC.close();
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\lagrangian\\kinematicCloud\\"+Character);
    FileC.ignore(1000, '(');
    FileC.seekg(-4, ios::cur);
    FileC >> kinematicLine;
    FileC.ignore(10, '(');
    for (int i = 0; i < kinematicLine; i++) {
        FileC.ignore(10, '(');
        FileC >> kinematicContent[i][0] >> kinematicContent[i][1] >> kinematicContent[i][2];
    }
    FileC.close();
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\lagrangian\\kinematicCloud\\d");
    FileC.ignore(1000, '(');
    for (int i = 0; i < kinematicLine; i++) {
        FileC.ignore(10, '(');
        FileC >> kinematicContent[i][3] >> kinematicContent[i][4] >> kinematicContent[i][5];
    }
    FileC.close();
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\lagrangian\\kinematicCloud\\positions");
    FileC.ignore(1000, '(');
    for (int i = 0; i < kinematicLine; i++) {
        FileC.ignore(4, '(');
        FileC >> kinematicContent[i][6] >> kinematicContent[i][7] >> kinematicContent[i][8];
    }
    return LineNumber[0];
}
int main() {
        getConfig();
    string NameNumber;
    cout << "Bitte die Nummer des Gewünschten Ordners angeben" << endl;
    cin >> FolderNumber>> NameNumber;
    float kinematicContentTransfer[kinematicLine][9];
    for (int i = 0; i < kinematicLine; i++) {
        for (int j = 0; j < 9; j++) {
            kinematicContentTransfer[i][j]=kinematicContent[i][j];
        }
    }
    float LineContentTransfer[LineNumber[0]][11];
    for (int i = 0; i < LineNumber[0]; i++) {
        for (int j = 0; j < 11; j++) {
            LineContentTransfer[i][j]=LineContent[i][j];
        }
    }
    if (DevMode) {
        cout << "Bestätige mit 1" << endl;
        int Devhelp;
        cin >> Devhelp;
        if (Devhelp==1){
            for (int i = 0; i < kinematicLine; i++) {
                for (int j = 0; j < 9; j++) {
                    cout << kinematicContent[i][j] << " ";
                }
                cout << endl;
            }
        }
    }
       // GeleseneDatenC Classname(LineNumber, LineContentTransfer, kinematicLine, kinematicContentTransfer);

        return 0;
    }