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
public:


    GeleseneDatenC(string getfolder,string FolderNumberstr,bool DevMode) {
        Folder = getfolder;
        Dev=DevMode;
        Number = FolderNumberstr;
        char Character;
        for (int z= 0; z < 2 ; z++){
            if (z==0) {
                Character = 'C';
            }
    File.open(getfolder+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\"+Character);                                     //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
        File.imbue(std::locale::classic());
        if (!File.is_open()&&DevMode==true&&ErrorIsThere==false) {                                                     //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
       cout << "Error in opening file, have you checked weather the Ordner you want opened has the intended Name" << endl;
        ErrorIsThere = true;
    }
            if (DevMode==true) {
                cout << Character<< endl;
            }
            string token;
        File.ignore(1000,'>');
        File >> LineNumber[z];
            File.ignore(4,'(');
                for (int y = 0; y < LineNumber[z]; y++) {
                    File.ignore(4,'(');
                    if (Character=='C'){
                        if (DevMode){cout <<y;}
                        CContent.push_back(array<double, 3>{});
                        File >> CContent[y][0] >> CContent[y][1] >> CContent[y][2];
                    }
                    else if (Character=='U'){
                        if (DevMode){cout <<y;}
                        UContent.push_back(array<double, 3>{});
                        File >> UContent[y][0] >> UContent[y][1] >> UContent[y][2];
                    }
                }
            Character='U';
            File.close();
     }






    };
public:
    void PrintValue() {
        cout << " Ausgabe der daten aus " << Folder << "\\FallenderTropfen\\" << Number << "\\C" << endl;


                // Erste Datengruppe ausgeben
                for (int i = 0; i < LineNumber[0]; i++) {
                    for (int j = 0; j < 3; j++) {
                        cout << CContent[i][j] << " ";
                    }
                    cout << endl;
                }


        cout << "Ausgabe der daten aus " << Folder << "\\FallenderTropfen\\" << Number << "\\U" << endl;

                // Erste Datengruppe ausgeben
                for (int i = 0; i < LineNumber[1]; i++) {
                    for (int j = 0; j < 3; j++) {
                        cout << UContent[i][j] << " ";
                    }
                    cout << endl;
                }


    }


};


#endif //UNTITLED_GELESENEDATENC_H