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
#include <algorithm>
using namespace std;                                                                                                    //alle genutzen variablen werden hier deklariert
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
bool MinorErrorIsThere = false;

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
    config >> DevMode;                                           //setzt die Variable DevMode
    config.ignore(100, '=');                            //geht bis zu 100 zeichen nach dem nächsten =
    config >> PhysVar1;                                         //Setzt zwei physische Variablen auf werte
    config.ignore(100, '=');
    config >> PhysVar2;
    config.ignore(100, '=');
    config >> IsTropfenOrFoam;                                  //determiniert ob es sich um Tropfen oder Spray handelt
    if (DevMode) {
        cout << DevMode << endl;                                //wenn devmode=1 ist gibt es 1 aus
    }

    config.close();
}



int GetData(const string& FolderNumberstr) {
    ifstream FileC;                                                                                                     //erstellt FileC als lesedatei
    for (int z= 0; z < 2 ; z++){
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+"\\"+Character);                                     //öffnet den ordner Fallender Tropfen\ Nummer des Ordners + C oder U in diesem
        FileC.imbue(std::locale::classic());
        if (!FileC.is_open()&&DevMode==true&&ErrorIsThere==false) {                                                     //wenn die Datei nicht geöffntet werden kann und der DevMode an ist gibt es einen Error aus
       cout << "Error in opening file, have you checked weather the Ordner you want opened has the intended Name" << endl;
        ErrorIsThere = true;
    }
    FileC.ignore(1000, '>');                                                                                    //geht bis nach dem symbol > in der Datei, hinter diesem befinden sich die Werte
    FileC >> LineNumber[0+3*z];                                                                                         //holt sich die zahl die hier steht als Int, diese beschreibt die anzahl an datensätzen
    FileC.ignore(1000, '(');                                                                                    //array zum speichern der drei datenmengen
    if (DevMode&&!ErrorIsThere&&z==0){                                                                                     //wenn devmode true ist und kein Error existiert gibt menge der linien an
        cout << LineNumber[0] << endl;
    }
    for (int i = 0; i < LineNumber[0]; i++) {                                                                           //wiederholt programm für dateimenge
        FileC.ignore(4, '(');
        LineContent.push_back(help_Line);//                                                                             //der Vektor wird dynamisch vergrösert
        FileC >> LineContent[i][0+z*3] >> LineContent[i][1+z*3] >> LineContent[i][2+3*z];                                // liest drei werte aus der datei aus
    }

    if (Character == 'C') {                                                                                             //hier wird eine fallunterscheidung vorgenommen da C und U verschiedene Formate haben
        FileC.ignore(1000, '>');
        FileC >> LineNumber[1];                                                                                         //bei C gibt es noch weitere daten die auf dem normalen weg extrahiert werden
        FileC.ignore(1, '(');
        for (int i = 0; i < LineNumber[1]; i++) {
            FileC.ignore(4, '(');
            FileC >> LineContent[i][6] >> LineContent[i][7] >> LineContent[i][8];                                       //diese werden auf die anderen positionen des vektors geschrieben
        }
        FileC.ignore(1000, '>');                                                                               //wird zweimal gemacht da zwei weitere sets an daten existieren
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
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\)"+Character);       //öffnet die lagranian dateien und extrahier hier
    FileC.imbue(locale::classic());                                                                                 //locale::classic setzt die spracheinstellungen auf classic dass floats gelesen werden können
    string line;
    while (getline(FileC, line))                                                                                    //solange noch zeilen in datei liest diese
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
        MinorErrorIsThere = true;
    }

    FileC.ignore(numeric_limits<streamsize>::max(), '(');
    for (int i = 0; i < kinematicLine; i++) {
        FileC.ignore(numeric_limits<streamsize>::max(), '(');
        kinematicContent.push_back(help_kinematic);

        FileC >> kinematicContent[i][0] >> kinematicContent[i][1] >> kinematicContent[i][2];                            //liest wie gewohnt die daten aus
    }
    FileC.close();

    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\d)");                //öffnet das d file
    FileC.imbue(std::locale::classic());
    if (!FileC.is_open()) {                                                                                             //wenn kein file geöffnet wird gibt errorcode aus und ein minor error flag
        cout << "Error opening d file!" << endl;
        MinorErrorIsThere = true;
    }

    string token;                                                                                                       //deklariert einen string um so nach der richtigen pos zu suchen
    while (FileC >> token) {
        if (token == "(") break;                                                                                        //sucht bis es eine klammer auf findet
    }

    for (int i = 0; i < kinematicLine; ++i) {
        float value;                //
        while (!(FileC >> value)) {                                                                                     // überspringt nicht nummern
            FileC.clear();                                                                                              // cleared Fails
            FileC >> token;                                                                                             //schreibt FileC als string
        }
        kinematicContent[i][3] = value;
    }
    FileC.close();

    //laest positions auf dem ueblichen weg aus
    FileC.open(getfolder()+"\\Fallender_Tropfen\\"+FolderNumberstr+R"(\lagrangian\kinematicCloud\positions)");
    FileC.imbue(std::locale::classic());
    FileC.ignore(numeric_limits<streamsize>::max(), '(');                                                       //typische operation um auszulesen
    for (int i = 0; i < kinematicLine; i++) {
        FileC.ignore(numeric_limits<streamsize>::max(), '(');
        FileC >> kinematicContent[i][4] >> kinematicContent[i][5] >> kinematicContent[i][6];
        FileC.ignore(numeric_limits<streamsize>::max(), ')');                                                   //da hier hinter der klammer noch ein wert existiert wird dieser hier ausgelesen
        FileC >> kinematicContent[i][7];
    }
    return LineNumber[0];   //gibt die normale linenumber zurück
}
int main() {
    // Lädt die Konfiguration aus einer externen Datei oder setzt Standardwerte
    getConfig();

    // Benutzeraufforderung: Nummer des gewünschten Ordners eingeben
    cout << "Bitte die Nummer des Gewuenschten Ordners in korrekter schreibweise angeben" << endl;
    cin >> FolderNumber; // Benutzer gibt Ordnernummer als String ein

    // Liest die Daten aus dem angegebenen Ordner
    GetData(FolderNumber);

    // Wenn der Entwicklermodus aktiv ist, prüfe auf kleinere Fehler
    if (DevMode) {
        if (MinorErrorIsThere) {
            cout << "Minor error detected, read the error returns" << endl;
        }
    }

    // Wenn kein Fehler aufgetreten ist, weiter mit der Datenverarbeitung
    if (ErrorIsThere == false) {
        // Spezieller Fall: Wenn es sich um "Tropfen" handelt
        if (IsTropfenOrFoam == 1) {
            // Arrays zur Zwischenspeicherung der Daten erstellen
            float LineContentTransfer[18][12];
            float kinematicContentTransfer[1][8];

            // Kinematik-Daten vom Vektor auf ein Array übertragen
            for (int i = 0; i < kinematicLine; i++) {
                for (int j = 0; j < 8; j++) {
                    kinematicContentTransfer[i][j] = kinematicContent[i][j];
                }
            }

            // Andere Linieninhalte ebenfalls auf Array übertragen
            for (int i = 0; i < LineNumber[1]; i++) {
                for (int j = 0; j < 12; j++) {
                    LineContentTransfer[i][j] = LineContent[i][j];
                }
            }

            // Klasse instanziieren, die die gelesenen Daten speichert
            GeleseneDatenC Classname(LineNumber, LineContentTransfer, kinematicLine, kinematicContentTransfer);
        }

        // Entwicklermodus: Debug-Ausgabe zur Überprüfung der eingelesenen Daten
        if (DevMode) {
            cout << fixed;

            // Benutzer bestätigen lassen, dass die kinematischen Daten korrekt sind
            cout << "Bestaetige die Ausgabe der" << getfolder() << "\\FallenderTropfen\\" << FolderNumber << "\\lagrangian\\kinematicCloud daten mit 1" << endl;
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

            // Benutzer bestätigen lassen, dass die Linieninhalte korrekt sind
            cout << "Bestatige die Ausgabe der daten aus " << getfolder() << "\\FallenderTropfen\\" << FolderNumber << "\\C mit 1" << endl;
            Devhelp = 0;
            cin >> Devhelp;
            if (Devhelp == 1) {
                // Erste Datengruppe ausgeben
                for (int i = 0; i < LineNumber[0]; i++) {
                    for (int j = 0; j < 3; j++) {
                        cout << LineContent[i][j] << " ";
                    }
                    cout << endl;
                }

                // Zweite Datengruppe ausgeben
                cout << "Das zweite set an vektordaten" << endl;
                for (int i = 0; i < LineNumber[1]; i++) {
                    for (int j = 6; j < 9; j++) {
                        cout << LineContent[i][j] << " ";
                    }
                    cout << endl;
                }

                // Dritte Datengruppe ausgeben
                cout << "Das dritte set an vektordaten" << endl;
                for (int i = 0; i < LineNumber[2]; i++) {
                    for (int j = 9; j < 12; j++) {
                        cout << LineContent[i][j] << " ";
                    }
                    cout << endl;
                }

                // U-Daten ausgeben
                cout << "Bestatige die Ausgabe der daten aus " << getfolder() << "\\FallenderTropfen\\" << FolderNumber << "\\U mit 1" << endl;
                Devhelp = 0;
                cin >> Devhelp;
                if (Devhelp == 1) {
                    for (int i = 0; i < LineNumber[3]; i++) {
                        for (int j = 3; j < 6; j++) {
                            cout << LineContent[i][j] << " ";
                        }
                        cout << endl;
                    }
                }
            }
        }
    }
    // Fehlerbehandlung: Wenn ein Fehler aufgetreten ist
    else if (ErrorIsThere == true) {
        if (DevMode) {
            cout << "There was an Error opening a vital File, check the other Error messages for more Information" << endl;
        }
    }

    // Rückgabewert des Programms
    return 27;
}
