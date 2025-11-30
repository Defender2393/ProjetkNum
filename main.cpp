//
// Created by bosch on 10.11.2025.
//
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include "GeleseneDatenC.h"
#include "Particle.h"
using namespace std;                                                                                                    //alle genutzen variablen werden hier deklariert
bool DevMode=false;
int LineNumber[4];
int kinematicLine;
char Character = 'C';
string FolderNumber;
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




int main() {
    // Lädt die Konfiguration aus einer externen Datei oder setzt Standardwerte
    getConfig();

    // Benutzeraufforderung: Nummer des gewünschten Ordners eingeben
    cout << "Bitte die Nummer des Gewuenschten Ordners in korrekter schreibweise angeben" << endl;
    cin >> FolderNumber; // Benutzer gibt Ordnernummer als String ein
    Particle Ptest(getfolder(), FolderNumber, DevMode);
    GeleseneDatenC Ftest(getfolder(), FolderNumber, DevMode);
    if (DevMode) {
        Ptest.PrintValue();
        Ftest.PrintValue();
    }
    // Fehlerbehandlung: Wenn ein Fehler aufgetreten ist
    // Rückgabewert des Programms
    return 27;
}
