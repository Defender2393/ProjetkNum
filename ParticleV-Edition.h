#ifndef UNTITLED_PARTICLE_H
#define UNTITLED_PARTICLE_H
#include "ParticleLocationAlpha.h"
#include <vector>
#include <array>
#include <locale>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class Particle {
    bool ErrorIsThere = false;
    bool Dev = false;
    ifstream File;
    vector <array<double, 8>> kinematicContent;//pos 0-2 is for U, pos 3 for d pos 4-7 for posisitions
    int kinematicLine;
    string Folder;
    string Number;
    int TimeSteps;
    double RePartikel;
    vector <array<double, 8>> temporaryContent;
    public:
    Particle(string getfolder, string FolderNumberstr,bool DevMode) {
        Folder = getfolder;
        Dev=DevMode;
        Number = FolderNumberstr;
        File.open(getfolder+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\U)");       //öffnet die lagranian dateien und extrahier hier
    File.imbue(locale::classic());                                                                                 //locale::classic setzt die spracheinstellungen auf classic dass floats gelesen werden können
    string line;
    while (getline(File, line))                                                                                    //solange noch zeilen in datei liest diese
    {
        stringstream ss(line);                                                                                          //erstellt einen stream an strings aus der zeile(einfacher zu nutzen)
        if (ss >> kinematicLine)                                                                                        //versucht das erste wort aus der zeile in kinematicline zu lesen, kinematicline akzeptiert nur zahlen
        {
            string remainder;
            ss >> remainder;                                                                                            //übergibt das nächste symbol dem rmainer

            if (remainder.empty())                                                                                      //wenn remainder empty endet die while schleife
                break;                                                                                                  //existiert da die zeile mit unserem wert nur ein token enthällt
        }
    }
    if (kinematicLine <= 0) {
        std::cout << "Error: Failed to find entry count in kinematicCloud. Last read line was:"                         //gibt einen error aus wenn es nicht funktioniert hat
                  << std::endl << "[" << line << "]" << std::endl;
        ErrorIsThere = true;
    }

    File.ignore(numeric_limits<streamsize>::max(), '(');
    for (int i = 0; i < kinematicLine; i++) {
        File.ignore(numeric_limits<streamsize>::max(), '(');
        kinematicContent.push_back(array<double, 8>());

        File >> kinematicContent[i][0] >> kinematicContent[i][1] >> kinematicContent[i][2];                            //liest wie gewohnt die daten aus
    }
    File.close();

    File.open(getfolder+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\d)");                //öffnet das d file
    File.imbue(std::locale::classic());
    if (!File.is_open()) {                                                                                             //wenn kein file geöffnet wird gibt errorcode aus und ein minor error flag
        cout << "Error opening d file!" << endl;
        ErrorIsThere = true;
    }

    string token;                                                                                                       //deklariert einen string um so nach der richtigen pos zu suchen
    while (File >> token) {
        if (token == "(") break;                                                                                        //sucht bis es eine klammer auf findet
    }

    for (int i = 0; i < kinematicLine; ++i) {
        float value;                //
        while (!(File >> value)) {                                                                                     // überspringt nicht nummern
            File.clear();                                                                                              // cleared Fails
            File >> token;                                                                                             //schreibt FileC als string
        }
        kinematicContent[i][3] = value;
    }
    File.close();

    //laest positions auf dem ueblichen weg aus
    File.open(getfolder+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\positions)");
    File.imbue(std::locale::classic());
    File.ignore(numeric_limits<streamsize>::max(), '(');                                                       //typische operation um auszulesen
    for (int i = 0; i < kinematicLine; i++) {
        File.ignore(numeric_limits<streamsize>::max(), '(');
        File >> kinematicContent[i][4] >> kinematicContent[i][5] >> kinematicContent[i][6];
        File.ignore(numeric_limits<streamsize>::max(), ')');                                                   //da hier hinter der klammer noch ein wert existiert wird dieser hier ausgelesen
        File >> kinematicContent[i][7];
    }
    };
public:
    void PrintValue() {
        cout << fixed;
        cout << "Ausgabe der" << Folder << "\\FallenderTropfen\\" << Number << "\\lagrangian\\kinematicCloud daten" << endl;
            // Kinematik-Daten ausgeben
            for (int i = 0; i < kinematicLine; i++) {
                for (int j = 0; j < 8; j++) {
                    float tempdata = kinematicContent[i][j];
                    cout << tempdata << endl;
                }
                cout << endl;
            }
    }
    int CreateFile(string getfolder,string FolderNumberStr,string fileName) {
        ofstream File;
        File.open(getfolder+"\\FallenderTropfen\\"+FolderNumberStr+R"(\lagrangian\kinematicCloud\)"+fileName);
        File <<  R"(/*--------------------------------*- C++ -*----------------------------------*\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  2412                                  |
|   \\  /    A nd           | Website:  www.openfoam.com                      |
|    \\/     M anipulation  |                                                 |
\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    arch        "LSB;label=32;scalar=64";
    class       vectorField;
    location    ")"<<FolderNumberStr<<R"(/lagrangian/kinematicCloud";
    object      )"<<fileName<<R"(;
}
        // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

)"<<kinematicLine<<R"(
()";
    for (int i = 0; i < kinematicLine; i++) {
        if (fileName == "positions"){
            File<<"("<< kinematicContent[i][4]<<" "<<kinematicContent[i][5]<<" "<<kinematicContent[i][6]<<")"<<kinematicContent[i][7];
        }
        else if (fileName == "d"){
            File<<"("<< kinematicContent[i][3]<<")";
        }
        else if (fileName == "U"){
            File<<"("<< kinematicContent[i][0]<<" "<<kinematicContent[i][1]<<" "<<kinematicContent[i][2]<<")";
    }
        else if (fileName == "Re") {
            File<< "("<<RePartikel<<")";
        }
    }
        File<<R"()

        // ************************************************************************* //
        )";
    File.close();
    }
    void increaseTime() {
        
        Partikel_Eigenschaften Partikel1;
        RePartikel = Partikel1.Re_von_Partikel();        //Argumente ergänzen

     
        for (int i = 0; i < kinematicLine; i++) {
            for (int j = 0; j < 8; j++) {
                kinematicContent[i][j] = Partikel1.U_und_pos_von_Partikel();
            }
        }
        TimeSteps = TimeSteps + 1;
        double Fldrhelp= 0.5*TimeSteps;
        string FldrNumber=std::to_string(Fldrhelp);
        //hier werdem die neuem dateipunkte hinnzugefügt werden#
        for (int i = 0; i < 4; i++) {
            string temp;
            if (i == 0) {
                temp = "U";
            }
            else if (i == 1) {
                temp = "d";
            }
            else if (i == 2) {
                temp="positions";
            }
            else if (i == 3) {
                temp = "Re";
            }
            CreateFile(Folder,FldrNumber,temp);
        }
    }
    int GiveTime() {
        return TimeSteps;
    }
};


#endif //UNTITLED_PARTICLE_H