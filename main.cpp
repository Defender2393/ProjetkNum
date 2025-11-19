//
// Created by bosch on 10.11.2025.
//
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include "GeleseneDatenC.h"
#include <locale>
using namespace std;
bool DevMode=false;
int LineNumber[4];
int kinematicLine;
char Character = 'C';
string FolderNumber;
array< float , 12> help_Line;
array<float , 8 > help_kinematic;
vector<array<float , 12>> LineContent;
vector<array<float ,8 >> kinematicContent;
bool ErrorIsThere = false;
float PhysVar1;
float PhysVar2;
int IsTropfenOrFoam;

string getfolder() {
    filesystem::path Filepath = filesystem::current_path();   //Ordnerpath findet den ordnerpfad und setzt Ordnerpath als diesen
    string Ordnerstring = Filepath.string();                  //gibt Ordnerstring mit dem selben information wie ordnerpath als string an//gibt den string aus
    return Ordnerstring;                                        //gibt als die Funktionswerk Ordnerstring aus
}
void getConfig() {
    ifstream config(getfolder()+"\\Config.txt");              //öffnet config
    if (!config.is_open()) {
        cout << "Error in opening config file" << endl;      //error wenn config nicht gefunen werden kann
        ErrorIsThere = true;
    }
    config.ignore(100, '=');                            //geht bis zu 100 zeichen nach dem nächsten =
    config >> DevMode;
    config.ignore(100, '=');                            //geht bis zu 100 zeichen nach dem nächsten =
    config >> PhysVar1;
    config.ignore(100, '=');
    config >> PhysVar2;
    config.ignore(100, '=');
    config >> IsTropfenOrFoam;
    if (DevMode) {
        cout << DevMode << endl;                                //wenn devmode=1 ist gibt es 1 aus
    }

    config.close();
}


int GetData(const string& FolderNumberstr) {
    ifstream FileC; //erstellt FileC als lesedatei
    for (int z= 0; z < 2 ; z++){
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\"+Character);      //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
        FileC.imbue(std::locale::classic());
        if (!FileC.is_open()&&DevMode==true&&ErrorIsThere==false) {                              //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
       cout << "Error in opening file, have you checked weather the Ordner you want opened has the intended Name" << endl;
        ErrorIsThere = true;
    }
    FileC.ignore(1000, '>');            //geht bis nach dem symbol > in der Datei, hinter diesem befinden sich die Werte
    FileC >> LineNumber[0+3*z]; //holt sich die zahl die hier steht als Int
    FileC.ignore(1000, '(');            //array zum speichern der drei datenmengen
    if (DevMode&&!ErrorIsThere&&z==0){                         //wenn devmode true ist und kein Error existiert gibt menge der linien an
        cout << LineNumber[0] << endl;
    }
    for (int i = 0; i < LineNumber[0]; i++) {                  //wiederholt programm für dateimenge
        FileC.ignore(4, '(');
        LineContent.push_back(help_Line);//
        FileC >> LineContent[i][0+z*3] >> LineContent[i][1+z*3] >> LineContent[i][2+3*z];           // liest drei werte aus der datei aus
    }

    if (Character == 'C') {
        FileC.ignore(1000, '>');
        FileC >> LineNumber[1];
        FileC.ignore(1, '(');
        for (int i = 0; i < LineNumber[1]; i++) {
            FileC.ignore(4, '(');
            FileC >> LineContent[i][6] >> LineContent[i][7] >> LineContent[i][8];
        }
        FileC.ignore(1000, '>');
        FileC >> LineNumber[2];
        FileC.ignore(1, '(');
        for (int j = 0; j < LineNumber[2]; j++) {
            FileC.ignore(4, '(');
            FileC >> LineContent[j][9] >> LineContent[j][10] >> LineContent[j][11];
        }
        Character='U';
        FileC.close();
    }
    }
    FileC.close();
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\)"+Character);
    FileC.imbue(std::locale::classic());
    std::string line;

    // Skip until we find a line that contains ONLY an integer
    while (std::getline(FileC, line))
    {
        std::stringstream ss(line);

        // Try to read an integer
        if (ss >> kinematicLine)
        {
            // Make sure the rest of the line is empty or whitespace
            std::string remainder;
            ss >> remainder;

            if (remainder.empty())
                break;   // We found the line with the count
        }
    }

    // If kinematicLine is still 0 or we failed to parse
    if (kinematicLine <= 0) {
        std::cout << "Failed to find entry count in kinematicCloud. Last read line was:"
                  << std::endl << "[" << line << "]" << std::endl;
        return -1;
    }

    FileC.ignore(numeric_limits<streamsize>::max(), '(');
    for (int i = 0; i < kinematicLine; i++) {
        FileC.ignore(numeric_limits<streamsize>::max(), '(');
        kinematicContent.push_back(help_kinematic);

        FileC >> kinematicContent[i][0] >> kinematicContent[i][1] >> kinematicContent[i][2];
    }
    FileC.close();
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\d)");
    FileC.imbue(std::locale::classic());
    FileC.ignore(numeric_limits<streamsize>::max(), '(');
    for (int i = 0; i < kinematicLine; i++) {
        FileC.ignore(numeric_limits<streamsize>::max(), '(');

       FileC >> kinematicContent[i][3];
    }
    FileC.close();
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\positions)");
    FileC.imbue(std::locale::classic());
    FileC.ignore(1000, '(');
    for (int i = 0; i < kinematicLine; i++) {
        FileC.ignore(numeric_limits<streamsize>::max(), '(');
        FileC >> kinematicContent[i][4] >> kinematicContent[i][5] >> kinematicContent[i][6];
        FileC.ignore(numeric_limits<streamsize>::max(), ')');
        FileC >> kinematicContent[i][7];
    }
    return LineNumber[0];
}
int main() {
        getConfig();
    cout << "Bitte die Nummer des Gewuenschten Ordners in korrekter schreibweise angeben" << endl;
    cin >> FolderNumber;
    GetData(FolderNumber);

    if (ErrorIsThere==false) {
        if (IsTropfenOrFoam==1) {
            float LineContentTransfer[18][12];
            float kinematicContentTransfer[1][8];
            for (int i = 0; i < kinematicLine; i++) {
                for (int j = 0; j < 8; j++) {
                    kinematicContentTransfer[i][j] = kinematicContent[i][j];
                }
            }
            for (int i = 0; i < LineNumber[1]; i++) {
                for (int j = 0; j < 12; j++) {
                    LineContentTransfer[i][j] = LineContent[i][j];
                }
            }
            GeleseneDatenC Classname(LineNumber, LineContentTransfer, kinematicLine, kinematicContentTransfer);
        }
        //else if (IsTropfenOrFoam==2) {
          //  float LineContentTransfer[1625][12];
            //float kinematicContentTransfer[1625][8];
       // }
        if (DevMode) {
            cout << fixed;
            cout << "Bestaetige die Ausgabe der" << getfolder() <<"\\FallenderTropfen\\"<< FolderNumber <<"\\lagrangian\\kinematicCloud daten mit 1" << endl;
            int Devhelp=0;
            cin >> Devhelp;
            if (Devhelp==1){
                for (int i = 0; i < kinematicLine; i++) {
                    for (int j = 0; j < 8; j++) {
                        float tempdata = kinematicContent[i][j];

                        cout << tempdata << endl;
                    }
                    cout << endl;
                }
            }
            cout << "Bestatige die Ausgabe der daten aus " << getfolder() <<"\\FallenderTropfen\\"<< FolderNumber <<"\\C mit 1" << endl;
            Devhelp=0;
            cin >> Devhelp;
            if (Devhelp==1) {
                for (int i = 0; i < LineNumber[0]; i++) {
                    for (int j = 0; j < 3; j++) {
                        cout<< LineContent[i][j] << " ";
                    }
                    cout << endl;
                }
                cout << "Das zweite set an vektordaten" << endl;
                for (int i = 0; i < LineNumber[1]; i++) {
                    for (int j = 6; j < 9; j++) {
                        cout<< LineContent[i][j] << " ";
                    }
                    cout << endl;
                }
                cout << "Das dritte set an vektordaten" << endl;
                for (int i = 0; i < LineNumber[2]; i++) {
                    for (int j = 9; j < 12; j++) {
                        cout<< LineContent[i][j] << " ";
                    }
                    cout << endl;
                }
                cout << "Bestatige die Ausgabe der daten aus " << getfolder() <<"\\FallenderTropfen\\"<< FolderNumber <<"\\U mit 1" << endl;
                Devhelp=0;
                cin >> Devhelp;
                if (Devhelp==1) {
                    for (int i = 0; i < LineNumber[3]; i++) {
                        for (int j = 3; j < 6; j++) {
                            cout<< LineContent[i][j] << " ";
                        }
                        cout << endl;
                    }
                }
            }
        }

    }
    else if (ErrorIsThere==true) {
        if (DevMode) {
            cout<< "There was an Error opening a vital File, check the other Error messages for more Information" << endl;
        }
    }
        return 27;
    }