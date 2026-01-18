#ifndef UNTITLED_PARTICLE_H
#define UNTITLED_PARTICLE_H
#include "ParticleLocationXY.h"
#include <vector>
#include <array>
#include <locale>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <filesystem>
using namespace std;

class Particle {
    bool ErrorIsThere = false;
    vector<array<double,8>> helper;
    bool Spray = false;
    bool Dev = false;
    ifstream File;
    vector <array<double, 8>> kinematicContent;//pos 0-2 is for U, pos 3 for d pos 4-7 for posisitions
    int kinematicLine;
    string Folder;
    string Number;
    int TimeSteps = 0;
    double RePartikel;
    string FolderName;
    vector <array<double, 8>> particletxt;
    vector <array<double, 8>> temporaryContent;
    string namesofFilesTrofpen[22]{"0.5","1","1.5","2","2.5","3","3.5","4","4.5","5","5.5","6","6.5","7","7.5","8","8.5","9","9.5","10","10.5","11"};
    string namesofFilesSpray[140]{"0.02","0.04","0.06","0.08","0.1","0.12","0.14","0.16","0.18","0.2","0.22","0.24","0.26","0.28","0.3","0.32","0.34","0.36","0.38","0.4","0.42","0.44","0.46","0.48","0.5","0.52","0.54","0.56","0.58","0.6","0.62","0.64","0.66","0.68","0.7","0.72","0.74","0.76","0.78","0.8","0.82","0.84","0.86","0.88","0.9","0.92","0.94","0.96","0.98","1","1.02","1.04","1.06","1.08","1.1","1.12","1.14","1.16","1.18","1.2","1.22","1.24","1.26","1.28","1.3","1.32","1.34","1.36","1.38","1.4","1.42","1.44","1.46","1.48","1.5","1.52","1.54","1.56","1.58","1.6","1.62","1.64","1.66","1.68","1.7","1.72","1.74","1.76","1.78","1.8","1.82","1.84","1.86","1.88","1.9","1.92","1.94","1.96","1.98","2","2.02","2.04","2.06","2.08","2.1","2.12","2.14","2.16","2.18","2.2","2.22","2.24","2.26","2.28","2.3","2.32","2.34","2.36","2.38","2.4","2.42","2.44","2.46","2.48","2.5","2.52","2.54","2.56","2.58","2.6","2.62","2.64","2.66","2.68","2.7","2.72","2.74","2.76","2.78","2.8"};
    public:
    Particle(string getfolder, string FolderNumberstr,bool DevMode,int SprayorTropfen) {
        Folder = getfolder;
        Dev=DevMode;
        Number = FolderNumberstr;
        if (SprayorTropfen==2) {
            FolderName="Spray";
        }
        else  {
            FolderName="Fallender_Tropfen";
        }
        File.open(getfolder+"\\"+FolderName+"\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\U)");       //öffnet die lagranian dateien und extrahier hier
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

    File.open(getfolder+"\\"+FolderName+"\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\d)");                //öffnet das d file
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
    File.open(getfolder+"\\"+FolderName+"\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\positions)");
    File.imbue(std::locale::classic());
    File.ignore(numeric_limits<streamsize>::max(), '(');                                                       //typische operation um auszulesen
    for (int i = 0; i < kinematicLine; i++) {
        File.ignore(numeric_limits<streamsize>::max(), '(');
        File >> kinematicContent[i][4] >> kinematicContent[i][5] >> kinematicContent[i][6];
        File.ignore(numeric_limits<streamsize>::max(), ')');                                                   //da hier hinter der klammer noch ein wert existiert wird dieser hier ausgelesen
        File >> kinematicContent[i][7];
    }
        if (SprayorTropfen==2) {
            Spray=true;
            File.open(getfolder+"\\Spray\\particles.txt");
            File.ignore(numeric_limits<streamsize>::max(), 'd');
            for (int i = 0; i < 50; i++) {
                particletxt.push_back(array<double, 8>());
                File >> particletxt[i][0] >> particletxt[i][1] >> particletxt[i][2]>>particletxt[i][3]>>particletxt[i][4]>>particletxt[i][5]>>particletxt[i][6]>>particletxt[i][7];
            }
        }
    };
public:
    void PrintValue() {
        cout << fixed;
        cout << "Ausgabe der" << Folder << FolderName<< Number << "\\lagrangian\\kinematicCloud daten" << endl;
            // Kinematik-Daten ausgeben
            for (int i = 0; i < kinematicLine; i++) {
                for (int j = 0; j < 8; j++) {
                    float tempdata = kinematicContent[i][j];
                    cout << tempdata << endl;
                }
                cout << endl;
            }
    }

    void CreateFile(string getfolder,string FolderNumberStr,string fileName) {
            // Build the full path

            string dirPath = getfolder +"\\"+FolderName+"\\" + FolderNumberStr + R"(\lagrangian\kinematicCloud\)";

            // Create all directories in the path if they don't exist
            ofstream File;


            File.open(dirPath + fileName);
        if (!File.is_open()) {
            filesystem::create_directories(dirPath);
            File.open(dirPath + fileName);
        }

            if (!File.is_open()) {
                cout << "Error opening file: " << dirPath + fileName << endl;
                return;
            }

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
(
)";
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
        File<<R"(
)

// ************************************************************************* //
        )";
    File.close();
        cout << fileName << endl;
    }

    void increaseTime( vector<array<double,3>> UCell) {
        //Argumente ergänzen
        Partikel_Eigenschaften Partikel1;
        for (int i = 0; i < kinematicLine; i++) {
            RePartikel = Partikel1.Re_von_Partikel(kinematicContent[i][4],kinematicContent[i][5],UCell[i][0],UCell[i][1],kinematicContent[i][3]);
            helper= Partikel1.U_und_pos_von_Partikel(kinematicContent[i][0],kinematicContent[i][1],kinematicContent[i][2],kinematicContent[i][4],kinematicContent[i][5],kinematicContent[i][6],kinematicContent[i][3]);

            for (int j = 0; j < 8; j++) {
                kinematicContent[i][j] = helper[i+TimeSteps][j];
            }
        }

        if (Spray) {
            setprecision(2);
            for (int i = 0; i < 50; i++) {
                if ((TimeSteps*0.02)==particletxt[i][0]) {
                    kinematicLine++;
                    kinematicContent.push_back(array<double, 8>());
                    kinematicContent[kinematicLine][0]=particletxt[i][4];
                    kinematicContent[kinematicLine][1]=particletxt[i][5];
                    kinematicContent[kinematicLine][2]=particletxt[i][6];
                    kinematicContent[kinematicLine][3]=particletxt[i][7];
                    kinematicContent[kinematicLine][4]=particletxt[i][1];
                    kinematicContent[kinematicLine][5]=particletxt[i][2];
                    kinematicContent[kinematicLine][6]=particletxt[i][3];
                    kinematicContent[kinematicLine][7]=Partikel1.Cell_ID(kinematicContent[kinematicLine][4],kinematicContent[kinematicLine][5]);
                }
            }

            TimeSteps = TimeSteps + 1;
            double Fldrhelp= 0.5*TimeSteps;
            string FldrNumber=to_string(Fldrhelp);
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
                if (Spray) {
                    CreateFile(Folder,namesofFilesSpray[TimeSteps-1],temp);

                }
                else {
                    CreateFile(Folder,namesofFilesTrofpen[TimeSteps-1],temp);
                }
            }
        }
    }
        int GiveTime() {
            return TimeSteps;
        }

};


#endif //UNTITLED_PARTICLE_H