//
// Created by bosch on 18.11.2025.
//

#ifndef UNTITLED_GELESENEDATENC_H
#define UNTITLED_GELESENEDATENC_H
#include <vector>
#include <array>
#include <locale>
#include <fstream>
#include <iostream>
using namespace std;

class GeleseneDatenC {
    private:
    bool ErrorIsThere=false;
    bool Dev=false;
    ifstream File;
    int LineNumber[2];
    vector<array<double, 3>> CContent;
    vector<array<double, 3>> UContent;
    string Folder;
    string Number;
    string FolderName;
public:


    GeleseneDatenC(string getfolder,string FolderNumberstr,bool DevMode, int TropfenorSpray) {
        Folder = getfolder;
        if (TropfenorSpray==2) {
        FolderName = "Spray";
        }
        else {
        FolderName = "Fallender_Tropfen";
        }
        Dev=DevMode;
        Number = FolderNumberstr;
        char Character;
        for (int z= 0; z < 2 ; z++){
            if (z==0) {
                Character = 'C';
            }
    File.open(getfolder+"\\"+FolderName+"\\"+FolderNumberstr+"\\"+Character);                                     //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
        File.imbue(std::locale::classic());
        if (!File.is_open()&&DevMode==true&&ErrorIsThere==false) {                                                     //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
       cout << "Error in opening file, have you checked if the Ordner you want opened has the intended Name" << endl;
        ErrorIsThere = true;
    }
            //liest die Dateiinhalte aus indem zu entsprechender Stelle gesprungen wird, werte gelesen, wiederholt.
            //hat Fallunterscheidung für die Verschiedenen Ordner.
        File.ignore(1000,'>');
        File >> LineNumber[z];
            File.ignore(4,'(');
                for (int y = 0; y < LineNumber[z]; y++) {
                    File.ignore(4,'(');
                    if (Character=='C'){
                        CContent.push_back(array<double, 3>{});
                        File >> CContent[y][0] >> CContent[y][1] >> CContent[y][2];
                    }
                    else if (Character=='U'){
                        UContent.push_back(array<double, 3>{});
                        File >> UContent[y][0] >> UContent[y][1] >> UContent[y][2];
                    }
                }
            Character='U';
            File.close();
     }






    };
public:
    void ReadNewData(string getfolder, string FolderNumberstr, bool DevMode) {
        UContent.clear();
        char Character;
                Character = 'U';

            File.open(getfolder+"\\"+FolderName+"\\"+FolderNumberstr+"\\"+Character);                                     //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
            File.imbue(std::locale::classic());
            if (!File.is_open()&&DevMode==true&&ErrorIsThere==false) {                                                     //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
                cout << "Error in opening file, have you checked if the Ordner you want opened has the intended Name" << endl;
                ErrorIsThere = true;

            }
        //liest dateien im selben Vormat aus wie in der Initialisierung aber nur die U Ordner dateien da C gleich bleibt.
            File.ignore(1000,'>');
            File >> LineNumber[1];
            File.ignore(4,'(');
            for (int y = 0; y < LineNumber[1]; y++) {
                File.ignore(4,'(');
                    UContent.push_back(array<double, 3>{});
                    File >> UContent[y][0] >> UContent[y][1] >> UContent[y][2];
                }
            File.close();

    }
    vector<array<double,3>> Content() {
        return UContent;
    }
    void PrintValue() {
        //wurde genutzt um die U daten über die Konsole auszugeben
        //hat sich

        cout << "Ausgabe der daten aus " << Folder << +"\\"+FolderName+"\\" << Number << "\\U" << endl;

                // Erste Datengruppe ausgeben
                for (int i = 0; i < LineNumber[1]; i++) {
                    for (int j = 0; j < 3; j++) {
                        cout << UContent[i][j] << " ";
                    }
                    cout << endl;
                }


    }
    double CValue(int z, int y) {
        return CContent[z][y];
    }
    double UValue(int z, int y) {
        return UContent[z][y];
    }
};


#endif //UNTITLED_GELESENEDATENC_H