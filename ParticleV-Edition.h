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
    // Klasse Particle
    // Verwaltet den vollständigen Zustand und die zeitliche Entwicklung
    // von Partikeln (Spray oder fallender Tropfen) in einer OpenFOAM-Simulation.
    // - liest kinematicCloud-Daten
    // - berechnet Re-Zahlen
    // - schreibt OpenFOAM-konforme Ausgabedateien
    //Fehlerstatus
    bool ErrorIsThere = false;
    //hilfscontainer
    vector<array<double, 8> > helper;
    vector<array<double, 8> > temporaryContent;
    //Modusflags
    bool Spray = false; // true = Spray, false = fallender Tropfen
    bool Dev = false; // Debug-Ausgaben aktivieren
    bool RePrint; // Re-Zahl in Dateien ausgeben
    //Datei Handling
    ifstream File;
    //Zeitverwaltung
    int TimeHelper = 1;
    int TimeSteps = 0;
    //Partikelzustand
    vector<array<double, 8> > kinematicContent;
    //pos 0-2 is for U(x,y,z), pos 3 for d pos 4-6 for posisitions(x,y,z) 7 ist die Cell ID
    //Anzahl Partikel
    int kinematicLine;
    //Pfade und Namen für schreiben der datein
    string Folder;
    string Number;
    string FolderName;
    //Re-Zahlen
    vector<double> RePartikel;
    //Spray initialdaten
    vector<array<double, 8> > particletxt;
    //Vordefinierter Zeitstempel für Tropfen
    string namesofFilesTrofpen[23]{
        "0","0.5", "1", "1.5", "2", "2.5", "3", "3.5", "4", "4.5", "5", "5.5", "6", "6.5", "7", "7.5", "8", "8.5", "9",
        "9.5", "10", "10.5", "11"
    };
    //Vordefinierter Zeitstempel für Spray
    string namesofFilesSpray[141]{
        "0", "0.02", "0.04", "0.06", "0.08", "0.1", "0.12", "0.14", "0.16", "0.18", "0.2", "0.22", "0.24", "0.26",
        "0.28", "0.3", "0.32", "0.34", "0.36", "0.38", "0.4", "0.42", "0.44", "0.46", "0.48", "0.5", "0.52", "0.54",
        "0.56", "0.58", "0.6", "0.62", "0.64", "0.66", "0.68", "0.7", "0.72", "0.74", "0.76", "0.78", "0.8", "0.82",
        "0.84", "0.86", "0.88", "0.9", "0.92", "0.94", "0.96", "0.98", "1", "1.02", "1.04", "1.06", "1.08", "1.1",
        "1.12", "1.14", "1.16", "1.18", "1.2", "1.22", "1.24", "1.26", "1.28", "1.3", "1.32", "1.34", "1.36", "1.38",
        "1.4", "1.42", "1.44", "1.46", "1.48", "1.5", "1.52", "1.54", "1.56", "1.58", "1.6", "1.62", "1.64", "1.66",
        "1.68", "1.7", "1.72", "1.74", "1.76", "1.78", "1.8", "1.82", "1.84", "1.86", "1.88", "1.9", "1.92", "1.94",
        "1.96", "1.98", "2", "2.02", "2.04", "2.06", "2.08", "2.1", "2.12", "2.14", "2.16", "2.18", "2.2", "2.22",
        "2.24", "2.26", "2.28", "2.3", "2.32", "2.34", "2.36", "2.38", "2.4", "2.42", "2.44", "2.46", "2.48", "2.5",
        "2.52", "2.54", "2.56", "2.58", "2.6", "2.62", "2.64", "2.66", "2.68", "2.7", "2.72", "2.74", "2.76", "2.78",
        "2.8"
    };

public:
    // Konstruktor:
    // liest alle initialen Partikeldaten aus OpenFOAM-Dateien
    // erkennt Spray- oder Tropfenmodus
    // initialisiert Re-Zahlen
    Particle(string getfolder, string FolderNumberstr, bool DevMode, int SprayorTropfen,
             vector<array<double, 3> > UCell, int PrintRe) {
        Folder = getfolder;
        RePrint = PrintRe;
        Dev = DevMode;
        Number = FolderNumberstr;
        //Moduswahl
        if (SprayorTropfen == 2) {
            FolderName = "Spray";
        } else {
            FolderName = "Fallender_Tropfen";
        }
        //U-Datei öffnen und lesen, auslesen der Partikelzahl
        // Ermittelt die Anzahl der Partikel aus der Kopfzeile der U-Datei
        File.open(getfolder + R"(\)" + FolderName + R"(\)" + FolderNumberstr + R"(\lagrangian\kinematicCloud\U)");
        //öffnet die lagranian dateien und extrahier hier
        File.imbue(locale::classic());
        //locale::classic setzt die spracheinstellungen auf classic dass doubles gelesen werden können
        string line;
        while (getline(File, line)) //solange noch zeilen in datei liest diese
        {
            stringstream ss(line); //erstellt einen stream an strings aus der zeile(einfacher zu nutzen)
            if (ss >> kinematicLine)
            //versucht das erste wort aus der zeile in kinematicline zu lesen, kinematicline akzeptiert nur zahlen
            {
                string remainder;
                ss >> remainder; //übergibt das nächste symbol dem rmainer

                if (remainder.empty()) //wenn remainder empty endet die while schleife
                    break; //existiert da die zeile mit unserem wert nur ein token enthällt
            }
        }
        if (kinematicLine <= 0) {
            std::cout << "Error: Failed to find entry count in kinematicCloud. Last read line was:"
                    //gibt einen error aus wenn es nicht funktioniert hat
                    << std::endl << "[" << line << "]" << std::endl;
            ErrorIsThere = true;
        }

        File.ignore(numeric_limits<streamsize>::max(), '(');
        // Springt zum Beginn des Datenblocks '(' und liest alle Partikelgeschwindigkeiten
        for (int i = 0; i < kinematicLine; i++) {
            File.ignore(numeric_limits<streamsize>::max(), '(');
            kinematicContent.push_back(array<double, 8>());
            RePartikel.push_back(0);
            File >> kinematicContent[i][0] >> kinematicContent[i][1] >> kinematicContent[i][2];
            //liest wie gewohnt die daten aus
        }
        File.close();
        //d-Datei lesen
        File.open(getfolder + R"(\)" + FolderName + R"(\)" + FolderNumberstr + R"(\lagrangian\kinematicCloud\d)");
        //öffnet das d file
        File.imbue(std::locale::classic());
        if (!File.is_open()) {
            //wenn kein file geöffnet wird gibt errorcode aus und ein minor error flag
            cout << "Error opening d file!" << endl;
            ErrorIsThere = true;
        }
        //alternativer Weg um nach den Zeichen hinter klammern zu suchen
        string token; //deklariert einen string um so nach der richtigen pos zu suchen
        while (File >> token) {
            if (token == "(") break; //sucht bis es eine klammer auf findet
        }

        for (int i = 0; i < kinematicLine; ++i) {
            float value; //
            while (!(File >> value)) {
                // überspringt nicht nummern
                File.clear(); // cleared Fails
                File >> token; //schreibt FileC als string
            }
            kinematicContent[i][3] = value;
        }
        File.close();

        //liest positions auf dem ueblichen weg aus
        File.open(getfolder + R"(\)" + FolderName + R"(\)" + FolderNumberstr + R"(\lagrangian\kinematicCloud\positions)");
        File.imbue(std::locale::classic());
        File.ignore(numeric_limits<streamsize>::max(), '('); //typische operation um auszulesen
        for (int i = 0; i < kinematicLine; i++) {
            File.ignore(numeric_limits<streamsize>::max(), '(');
            File >> kinematicContent[i][4] >> kinematicContent[i][5] >> kinematicContent[i][6];
            File.ignore(numeric_limits<streamsize>::max(), ')');
            //da hier hinter der klammer noch ein wert existiert wird dieser hier ausgelesen
            File >> kinematicContent[i][7];
        }
        File.close();
        // particles.txt enthält Injektionsdaten für neue Spray-Partikel:
        // Zeit, Position, Geschwindigkeit und Durchmesser
        if (SprayorTropfen == 2) {
            Spray = true;
            File.open(getfolder + R"(\Spray\particles.txt)");
            File.ignore(numeric_limits<streamsize>::max(), 'd');
            File.imbue(std::locale::classic());
            if (!File.is_open()) {
                cout << "Error opening particles.txt file!" << endl;
                ErrorIsThere = true;
            }
            for (int i = 0; i < 50; i++) {
                // Vorgabe 50 neue Partikel aus particles.txt
                particletxt.push_back(array<double, 8>());
                File >> particletxt[i][0] >> particletxt[i][1] >> particletxt[i][2] >> particletxt[i][3] >> particletxt[
                    i][4] >> particletxt[i][5] >> particletxt[i][6] >> particletxt[i][7];
            }
        }
        //Initiale Re berechnung bei Spray
        if (Spray) {
            Partikel_Eigenschaften Partikelmath;
            for (int i = 0; i < kinematicLine; i++) {
                RePartikel[i] = Partikelmath.Re_von_Partikel(kinematicContent[i][0], kinematicContent[i][1],
                                                             UCell[kinematicContent[i][7]][0],
                                                             UCell[kinematicContent[i][7]][1], kinematicContent[i][3]);
            }
            if (RePrint) {
                CreateFile(Folder, namesofFilesSpray[0], "Re");
            }
        }
    };

public:
    //Debug ausgabe
    void PrintValue() {
        cout << "Ausgabe der" << Folder << FolderName << Number << R"(\lagrangian\kinematicCloud daten)" << endl;
        // Kinematik-Daten ausgeben
        for (int i = 0; i < kinematicLine; i++) {
            for (int j = 0; j < 8; j++) {
                double tempdata = kinematicContent[i][j];
                cout << tempdata << "  ";
            }
            cout << endl;
        }
    }

    //erstellt openfoam kompatible Datei inkl. header struktur etc.
    void CreateFile(string getfolder, string FolderNumberStr, string fileName) {
        // Baut die datei wenn sie nicht existiert

        string dirPath = getfolder + R"(\)" + FolderName + R"(\)" + FolderNumberStr + R"(\lagrangian\kinematicCloud\)";

        // Erstellt die ordner wenn sie nicht existieren
        ofstream File;


        File.open(dirPath + fileName, std::ios_base::binary | std::ios_base::out);
        if (!File.is_open()) {
            filesystem::create_directories(dirPath);
            File.open(dirPath + fileName, std::ios_base::binary | std::ios_base::out);
        }

        if (!File.is_open()) {
            cout << "Error opening file: " << dirPath + fileName << endl;
            return;
        }

        //Open Foam header mit entsprechenden variablen
        File << R"(/*--------------------------------*- C++ -*----------------------------------*\
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
    class       )";
        if (fileName == "positions") {
            File << "Cloud<basicKinematicParcel>;";
        } else if (fileName == "d") {
            File << "scalarField;";
        } else if (fileName == "U") {
            File << "vectorField;";
        } else if (fileName == "Re") {
            File << "scalarField;";
        }

        File << R"(
    location    ")" << FolderNumberStr << R"(/lagrangian/kinematicCloud";
    object      )" << fileName << R"(;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

)";
        //sonderfall da wir RE einen Zeitschritt später berechnen passiert es, dass KinematicLine schon auf 10 Partikel mehr gesetzt wurde
        //da diese aus der TXT geladen werden
        if ((FolderNumberStr == "0.18" || FolderNumberStr == "0.38" || FolderNumberStr == "0.58" || FolderNumberStr ==
             "0.78" || FolderNumberStr == "0.98") && fileName == "Re") {
            File << kinematicLine - 10;
        } else {
            File << kinematicLine;
        }
        File << R"(
(
)";
        //ausgabe der dateien im entsprechenden format
        for (int i = 0; i < kinematicLine; i++) {
            if (fileName == "positions") {
                File << "(" << kinematicContent[i][4] << " " << kinematicContent[i][5] << " " << kinematicContent[i][6]
                        << ") " << kinematicContent[i][7] << endl;
            } else if (fileName == "d") {
                File << kinematicContent[i][3] << endl;
            } else if (fileName == "U") {
                File << "(" << kinematicContent[i][0] << " " << kinematicContent[i][1] << " " << kinematicContent[i][2]
                        << ")" << endl;
            }
        }

        if (fileName == "Re") {
            if (FolderNumberStr == "0.18" || FolderNumberStr == "0.38" || FolderNumberStr == "0.58" || FolderNumberStr
                == "0.78" || FolderNumberStr == "0.98") {
                for (int i = 0; i < kinematicLine - 10; i++) {
                    File << RePartikel[i] << endl;
                }
            } else {
                for (int i = 0; i < kinematicLine; i++) {
                    File << RePartikel[i] << endl;
                }
            }
        }
        File << R"()


// ************************************************************************* //
        )";
        File.close();
        if (Devmode) {
            cout << fileName << endl;
        }
    }

    // Führt einen Zeitschritt der Simulation aus:
    //  aktualisiert Geschwindigkeit & Position
    //  berechnet Re-Zahlen
    //  schreibt OpenFOAM-Ausgabedateien
    void increaseTime(vector<array<double, 3> > UCell) {
        TimeSteps = TimeSteps + 1;
        Partikel_Eigenschaften Partikel1;
        //partikeldaten und re neu berechnen
        for (int i = 0; i < kinematicLine; i++) {
            //Fallunterscheidung zwischen Spray und Tropfen um unterschiedleiche Funktionen zu rufen
            if (Spray) {
                if (kinematicContent[i][4] == 5 && Spray) {
                    // Partikel hat Zielbereich erreicht → Re = 0
                    RePartikel[i] = 0;
                } else {
                    RePartikel[i] = Partikel1.Re_von_Partikel(kinematicContent[i][0], kinematicContent[i][1],
                                                              UCell[kinematicContent[i][7]][0],
                                                              UCell[kinematicContent[i][7]][1], kinematicContent[i][3]);
                }
                kinematicContent[i] = Partikel1.U_und_pos_von_Partikel(kinematicContent[i],
                                                                       UCell[kinematicContent[i][7]][0],
                                                                       UCell[kinematicContent[i][7]][1], TimeSteps);
            } else {
                RePartikel[i] = Partikel1.Re_von_Partikel(kinematicContent[i][0], kinematicContent[i][1], 0, 0,
                                                          kinematicContent[i][3]);
                kinematicContent[i] = Partikel1.U_und_pos_von_Tropfen(kinematicContent[i], TimeSteps);
            }
        }
        //hinzufügen der entsprechenden daten wenn Spray existiert
        if (Spray) {
            for (int i = 0; i < 50; i++) {
                if ((TimeSteps * 0.02) == particletxt[i][0]) {
                    // Fügt neue Spray-Partikel hinzu, wenn Injektionszeit erreicht ist
                    RePartikel.push_back(0);
                    kinematicContent.push_back(array<double, 8>());
                    kinematicContent[kinematicLine][0] = particletxt[i][4];
                    kinematicContent[kinematicLine][1] = particletxt[i][5];
                    kinematicContent[kinematicLine][2] = particletxt[i][6];
                    kinematicContent[kinematicLine][3] = particletxt[i][7];
                    kinematicContent[kinematicLine][4] = particletxt[i][1];
                    kinematicContent[kinematicLine][5] = particletxt[i][2];
                    kinematicContent[kinematicLine][6] = particletxt[i][3];
                    kinematicContent[kinematicLine][7] = Partikel1.Cell_ID(
                        kinematicContent[kinematicLine][4], kinematicContent[kinematicLine][5]);
                    kinematicLine++;
                }
            }
        }
        // Erzeugt für jeden Zeitschritt die entsprechenden OpenFOAM-Dateien: U, d, positions und optional Re
        for (int i = 0; i < 4; i++) {
            string temp;
            if (i == 0) {
                temp = "U";
            } else if (i == 1) {
                temp = "d";
            } else if (i == 2) {
                temp = "positions";
            } else if (i == 3) {
                temp = "Re";
            }
            if (Spray) {
                if (temp == "Re" && RePrint) {
                    CreateFile(Folder, namesofFilesSpray[TimeHelper - 1], temp);
                } else if (temp != "Re") {
                    CreateFile(Folder, namesofFilesSpray[TimeHelper], temp);
                }
            } else {
                if (temp == "Re" && RePrint) {
                    CreateFile(Folder, namesofFilesTrofpen[TimeHelper - 1], temp);
                } else if (temp != "Re") {
                    CreateFile(Folder, namesofFilesTrofpen[TimeHelper], temp);
                }
            }
        }
        TimeHelper++;
    }

    //gibt simulationszeitschritte zurücl
    int GiveTime() {
        return TimeSteps;
    }

    //finale Re berechnung da man alle daten von schritt 2.8 braucht
    void ReFinal(vector<array<double, 3> > UCell) {
        Partikel_Eigenschaften Partikel1;
        for (int i = 0; i < kinematicLine; i++) {
            //Fallunterscheidung abhängig von den Werten
            if (kinematicContent[i][4] == 5 && Spray) {
                RePartikel[i] = 0;
            } else if (Spray) {
                RePartikel[i] = Partikel1.Re_von_Partikel(kinematicContent[i][0], kinematicContent[i][1],
                                                          UCell[kinematicContent[i][7]][0],
                                                          UCell[kinematicContent[i][7]][1], kinematicContent[i][3]);
            } else if (kinematicContent[i][4] == 72 && !Spray) {
                RePartikel[i] = 0;
            }
        }
        //Printed nur Wenn Spray=Wahr unterschiedliche Parameter für die Unterschiedlichen Funktionen
        if (RePrint && Spray) {
            CreateFile(Folder, "2.8", "Re");
        } else if (RePrint) {
            CreateFile(Folder, "11", "Re");
        }
    }

    //gibt auswertungsdaten als txt zurück(optional aber hilfreich für aufgaben)
    //schreibt sie zur vereinfachten auswertung in txt dokument
    //998;... = Dichte von Wasser
    void Auswertung(string getfolder) {
        array<double, 26> Auswertung;
        for (int i = 0; i < kinematicLine; i++) {
            if (kinematicContent[i][4] == 5) {
                //Berechnet das Volumen der entsprechenden Teile
                Auswertung[kinematicContent[i][5] * 2] =
                        Auswertung[kinematicContent[i][5] / 0.5] + (
                            pow((kinematicContent[i][3] / 2), 3) * numbers::pi * (4.0 / 3.0)) * 998.207;
            }
        }
        //schreibt die Daten als txt Ordner
        ofstream File;
        File.open(getfolder + R"(\Spray)" + R"(\Auswertung.txt)");
        if (!File.is_open()) {
            cout << "Unable to open Auswertung.txt";
        }
        for (int i = 0; i < 26; i++) {
            File << "Der " << i << ". Wert ist " << Auswertung[i] << endl;
        }
    }
};


#endif //UNTITLED_PARTICLE_H
