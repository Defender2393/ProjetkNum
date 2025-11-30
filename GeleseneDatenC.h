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
    vector <array<int ,2>> LineNumber;
    vector<array<double, 12>> CContent;
    vector<array<double, 12>> UContent;
    string Folder;
    string Number;
public:


    GeleseneDatenC(string getfolder,string FolderNumberstr,bool DevMode) {
        Folder = getfolder;
        Dev=DevMode;
        Number = FolderNumberstr;
        char Character='C';
        for (int z= 0; z < 2 ; z++){
            int helper=0;
    File.open(getfolder+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\"+Character);                                     //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
        File.imbue(std::locale::classic());
        if (!File.is_open()&&DevMode==true&&ErrorIsThere==false) {                                                     //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
       cout << "Error in opening file, have you checked weather the Ordner you want opened has the intended Name" << endl;
        ErrorIsThere = true;
    }
            string token;
            while (!File.eof()){
        while (File >> token) {
            if (token == ">")
                {
                helper++;
                LineNumber.push_back(array<int, 2>{});
                break;
                }
        }
        File >> LineNumber[helper-1][z];
                for (int y = 0; y < LineNumber[helper-1][z]; y++) {
                    while (File >> token) {
                        if (token == "(")break;
                    }
                    if (Character=='C'){
                        if (CContent.size()<=LineNumber[helper-1][z])CContent.push_back(array<double, 12>{});
                        File >> CContent[y][3*helper-1] >> CContent[y][3*helper] >> CContent[y][3*helper+1];
                    }
                    else if (Character=='U'){
                        if (CContent.size()<=LineNumber[helper][z])CContent.push_back(array<double, 12>{});
                        File >> CContent[y][3*helper-1] >> CContent[y][3*helper] >> CContent[y][3*helper+1];
                    }
                }

     }
        Character='U';
            File.close();

        }



    };
public:
    void PrintValue() {
        int Devhelp;
        cout << "Bestatige die Ausgabe der daten aus " << Folder << "\\FallenderTropfen\\" << Number << "\\C mit 1" << endl;
        Devhelp = 0;
        cin >> Devhelp;
        if (Devhelp == 1) {
        for (int z = 0; z < LineNumber.size() ; z++){
            cout << "Das " << z+1 << ". Set an Daten " << endl;

                // Erste Datengruppe ausgeben
                for (int i = 0; i < LineNumber[z][0]; i++) {
                    for (int j = 0+3*z; j < 3+z*3; j++) {
                        cout << CContent[i][j] << " ";
                    }
                    cout << endl;
                }
            }
        }
        cout << "Bestatige die Ausgabe der daten aus " << Folder << "\\FallenderTropfen\\" << Number << "\\U mit 1" << endl;
        Devhelp = 0;
        cin >> Devhelp;
        if (Devhelp == 1) {
        for (int z = 0; z < LineNumber.size() ; z++){
            cout << "Das " << z+1 << ". Set an Daten " << endl;

                // Erste Datengruppe ausgeben
                for (int i = 0; i < LineNumber[z][1]; i++) {
                    for (int j = 0+3*z; j < 3+z*3; j++) {
                        cout << UContent[i][j] << " ";
                    }
                    cout << endl;
                }
            }
        }
    }


};


#endif //UNTITLED_GELESENEDATENC_H