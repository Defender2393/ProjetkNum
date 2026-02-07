//
// Created by bosch on 10.11.2025.
//
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include "GeleseneDatenC.h"
#include "ParticleV-Edition.h"
using namespace std;                                                                                                    //alle genutzen variablen werden hier deklariert
int LineNumber[4];          // speichert relevante Zeilennummern aus Eingabedateien
int kinematicLine;          // Index der Zeile mit kinematischen Daten
bool DevMode=false;         // aktiviert Debug-Ausgaben
char Character = 'C';       // Kennzeichnung des Partikeltyps (z. B. 'C' = ...)
string FolderNumber;        // aktueller Zeitordner (z. B. "0.02")
bool ErrorIsThere = false;  // schwerwiegender Fehler aufgetreten
int IsTropfenOrSpray;       // 1 = Tropfen, 2 = Spray
bool MinorErrorIsThere = false; // nicht-kritischer Fehler
int Ptime;                  // Partikelzeitindex
int PrintRe;                // Anzahl der Re-Ausgaben
string getfolder() {
    // Ermittelt das aktuelle Arbeitsverzeichnis des Programms
    // Wird als Basis für alle Dateioperationen verwendet
    filesystem::path Filepath = filesystem::current_path();
    string Ordnerstring = Filepath.string();
    return Ordnerstring;
}
void getConfig() {
    //Liest Konfigurationswerte aus Config.txt aus
    ifstream config(getfolder()+"\\Config.txt");
    if (!config.is_open()) {
        cout << "Error in opening config file" << endl;      //error wenn config nicht gefunden werden kann
        ErrorIsThere = true;                                // Programm läuft trotzdem weiter
    }
    config.ignore(100, '=');                            //überspringt bis nach dem =
    config >> DevMode;                                           //liest DevMode Variable
    config.ignore(100, '=');
    config >> IsTropfenOrSpray;                                  //determiniert ob es sich um Tropfen oder Spray handelt
    config.ignore(100, '=');
    config >> PrintRe;
    if (DevMode) {
        cout << DevMode << endl;                                //wenn devmode=1 ist gibt es 1 aus
    }

    config.close();
}




int main() {
    // 1. Konfiguration laden
    getConfig();
        FolderNumber="0";   // Startzeitpunkt
    if (DevMode) {
        cout << "main is working"<<endl;
    }

    if (DevMode) {
    }

    if (IsTropfenOrSpray==2) {
        // Initiales Einlesen der Felddaten
        GeleseneDatenC FieldData(getfolder(), FolderNumber, DevMode,IsTropfenOrSpray);
        // Initialisierung der Partikel mit Startzustand
        Particle ParticleData(getfolder(), FolderNumber, DevMode, IsTropfenOrSpray,FieldData.Content(),PrintRe);
        // Zeitstempel für die einzelnen Simulationsordner (0 bis 2.8 in 0.02-Schritten)
        string TimeHelp[141]{"0","0.02","0.04","0.06","0.08","0.1","0.12","0.14","0.16","0.18","0.2","0.22","0.24","0.26","0.28","0.3","0.32","0.34","0.36","0.38","0.4","0.42","0.44","0.46","0.48","0.5","0.52","0.54","0.56","0.58","0.6","0.62","0.64","0.66","0.68","0.7","0.72","0.74","0.76","0.78","0.8","0.82","0.84","0.86","0.88","0.9","0.92","0.94","0.96","0.98","1","1.02","1.04","1.06","1.08","1.1","1.12","1.14","1.16","1.18","1.2","1.22","1.24","1.26","1.28","1.3","1.32","1.34","1.36","1.38","1.4","1.42","1.44","1.46","1.48","1.5","1.52","1.54","1.56","1.58","1.6","1.62","1.64","1.66","1.68","1.7","1.72","1.74","1.76","1.78","1.8","1.82","1.84","1.86","1.88","1.9","1.92","1.94","1.96","1.98","2","2.02","2.04","2.06","2.08","2.1","2.12","2.14","2.16","2.18","2.2","2.22","2.24","2.26","2.28","2.3","2.32","2.34","2.36","2.38","2.4","2.42","2.44","2.46","2.48","2.5","2.52","2.54","2.56","2.58","2.6","2.62","2.64","2.66","2.68","2.7","2.72","2.74","2.76","2.78","2.8"};
        // Zeitschrittweise Simulation
        for (int i=0; i<140; i++) {
            // Neue Felddaten für aktuellen Zeitschritt laden
            FolderNumber=TimeHelp[i];
         FieldData.ReadNewData(getfolder(), FolderNumber, DevMode);

            // Partikelzustand um einen Zeitschritt erhöhen
            ParticleData.increaseTime(FieldData.Content());
            if (DevMode) {
                ParticleData.PrintValue();
            }
        }
        // Letzter Zeitschritt und finale Auswertung
        FolderNumber=TimeHelp[140];
        FieldData.ReadNewData(getfolder(), FolderNumber, DevMode);
        ParticleData.ReFinal(FieldData.Content());
        ParticleData.Auswertung(getfolder());

    }
    if (IsTropfenOrSpray==1) {
        GeleseneDatenC FieldData(getfolder(), FolderNumber, DevMode,IsTropfenOrSpray);

        Particle ParticleData(getfolder(), FolderNumber, DevMode, IsTropfenOrSpray,FieldData.Content(),PrintRe);
        // Vereinfachte Simulation für Tropfen
        // Feste Anzahl an Iterationen ohne neue Felddaten
        for (int i=0; i<22; i++) {
          ParticleData.increaseTime(FieldData.Content());

        }
        ParticleData.ReFinal(FieldData.Content());

    }
    // Fehlerbehandlung: Wenn ein Fehler aufgetreten ist
    // Rückgabewert des Programms

    return 27;
}
