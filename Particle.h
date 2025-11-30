//
// Created by bosch on 23.11.2025.
//

#ifndef UNTITLED_PARTICLE_H
#define UNTITLED_PARTICLE_H
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
    vector <array<double, 8>> kinematicContent;
    int kinematicLine;
    string Folder;
    string Number;

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
        cout << "Bestaetige die Ausgabe der" << Folder << "\\FallenderTropfen\\" << Number << "\\lagrangian\\kinematicCloud daten mit 1" << endl;
        int Devhelp = 0;
        cin >> Devhelp;
        if (Devhelp == 1) {
            // Kinematik-Daten ausgeben
            for (int i = 0; i < kinematicLine; i++) {
                for (int j = 0; j < 8; j++) {
                    float tempdata = kinematicContent[i][j];
                    cout << tempdata << endl;
                }
                cout << endl;
            }
        }
    }
};


#endif //UNTITLED_PARTICLE_H