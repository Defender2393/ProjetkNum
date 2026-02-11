#include <iostream>
#include <cmath>
#include <vector>
#include <array>
using namespace std;

//! Die Ausrufe-und Fragezeichen dienen zur Aenderung der Farbe der Comments, also nicht wundern

//!-------------------------------------


//!-------------------------------------

double U_px0, U_px1, U_py0, U_py1;


//!-------------------------------------

double pos_x0, pos_x1, pos_y0, pos_y1;

//!-------------------------------------

// Diese Variablen werden im Konstruktor optional aus Config.txt überschriebe
double g = 9.81;
double rho_c = 1.199;
double rho_p = 998.207;
const double zweidurchdrei = 2.0 / 3.0;
double eta = 1.824878 * 1e-5;
double dT = 0.02;
double tau;
double Reynolds, RE;
double Widerstand;
double U_p, U_c;
double temptransfer;
int CELL_ID;
int Raumrichtung;
bool Devmode;
vector<array<double, 8> > temporaryContent;
array<double, 8> temporaryArray;
//! 0,1,2 fuer Geschwindigkeiten / 3 fuer diameter / 4,5,6 fuer Positionen / 7 fuer Cell_ID
vector<double> timeContent;
vector<array<double, 3> > temporaryUValue;

// Klasse Partikel_Eigenschaften
// Enthält alle physikalischen Modelle für Partikel:
// Reynolds-Zahl
// Relaxationszeit (tau)
// Bewegungsgleichungen
//Zellzuordnung (Host-ID)
// Unterstützt Spray- und Tropfenfälle getrennt
class Partikel_Eigenschaften {
    // Konstruktor:
    // Liest physikalische Parameter aus Config.txt
    // Ermöglicht das Überschreiben von Standardwerten (Overwrite-Flag)
public:
    Partikel_Eigenschaften() {
        ifstream config(getfolder() + R"(\Config.txt)"); //öffnet config
        if (!config.is_open()) {
            cout << "Error in opening config file" << endl; //error wenn config nicht gefunen werden kann
        }
        config.ignore(100, '='); //geht bis zu 100 zeichen nach dem nächsten =
        config >> Devmode; //setzt die Variable DevMode
        config.ignore(100, '=');
        config.ignore(100, '=');
        config.ignore(100, '=');
        // Overwrite = 1 erlaubt das manuelle Setzen physikalischer Konstanten
        int Overwrite;
        config >> Overwrite;
        if (Overwrite) {
            config.ignore(100, '=');
            config >> g;
            config.ignore(100, '=');
            config >> rho_c;
            config.ignore(100, '=');
            config >> rho_p;
            config.ignore(100, '=');
            config >> eta;
            eta = eta * 1e-5;
        }

        if (Devmode) {
            cout << Devmode << endl; //wenn devmode=1 ist gibt es 1 aus
        }

        config.close();
    }

    string getfolder() {
        filesystem::path Filepath = filesystem::current_path();
        //Ordnerpath findet den ordnerpfad und setzt Ordnerpath als diesen
        string Ordnerstring = Filepath.string();
        //gibt Ordnerstring mit dem selben information wie ordnerpath als string an//gibt den string aus
        return Ordnerstring; //gibt als die Funktionswerk Ordnerstring aus
    }

public:
    double Betrag_von_U_von_Partikel(double U_px, double U_py) {
        U_p = sqrt(pow(U_px, 2) + pow(U_py, 2));
        return U_p;
    }

    double Betrag_von_U_von_Zelle(double U_cx, double U_cy) {
        U_c = sqrt(pow(U_cx, 2) + pow(U_cy, 2));
        return U_c;
    }

    // Berechnet die Reynolds-Zahl des Partikels basierend auf Relativgeschwindigkeit zwischen Partikel und Gas
    double Re_von_Partikel(double U_px, double U_py, double U_cx, double U_cy, double diameter) {
        U_px = U_px - U_cx;
        U_py = U_py - U_cy;
        Reynolds = (rho_c * abs(Betrag_von_U_von_Partikel(U_px, U_py)) * diameter) / eta;
        return Reynolds;
    }

    // Berechnet die Relaxationszeit tau des Partikels
    // Unterschiedliche Widerstandsmodelle je nach Reynolds-Zahl:
    // RE = 0        : Stokes-Bereich
    // RE <= 1000   : Übergangsbereich
    // RE > 1000    : Newton-Bereich
    double tau_von_Partikel(double RE, double rho_p, double diameter, double eta) {
        //? Relaxationszahl, aber der Kehrwert berechnet aus der Formelsammlung, außerdem wurde gleichung 8 in 11 eingesetzt, sodass sich Re rauskürzt

        if (RE == 0) {
            tau = (4.0 * rho_p * pow(diameter, 2)) / (72.0 * eta * (1 + (1.0 / 6.0) * pow(RE, 2.0 / 3.0)));
        } else if (RE <= 1000) {
            Widerstand = (24.0 / RE) * (1.0 + (1.0 / 6.0) * pow(RE, zweidurchdrei));
            tau = 4.0 / 3.0 * (rho_p * pow(diameter, 2)) / (eta * RE * Widerstand);
            //? Relaxationszahl, aber der Kehrwert berechnet aus der Formelsammlung
        } else if (RE > 1000) {
            Widerstand = 0.424;
            tau = 4.0 / 3.0 * (rho_p * pow(diameter, 2)) / (eta * RE * Widerstand);
        }
        return tau;
    }

    // Bestimmt die Host-Zelle des Partikels
    // Unterschiedliche Zellgeometrie für Spray und Tropfen:
    // größere Zellen beim Tropfen
    int Cell_ID_Tropfen(double pos_x1, double pos_y1) {
        const double C_dist_x = 12; //? Kantenlaenge der Zelle in x-Richtung
        const double C_dist_y = 20; //? Kantenlaenge der Zelle in y-Richtung
        const int AnzC_x = 6; //? Anzahl der Zellen in x-Richtung
        //? const int AnzC_y = 65;           //? Anzahl der Zellen in y-Richtung

        int Hilfe_x = (pos_x1 / C_dist_x);
        int Hilfe_y = (pos_y1 / C_dist_y);
        CELL_ID = Hilfe_y * AnzC_x + Hilfe_x;

        return CELL_ID;
    }

    // kleinere Zellen beim Spray
    int Cell_ID(double pos_x1, double pos_y1) {
        const double C_dist_x = 0.2; //? Kantenlaenge der Zelle in x-Richtung
        const double C_dist_y = 0.2; //? Kantenlaenge der Zelle in y-Richtung
        const int AnzC_x = 25; //? Anzahl der Zellen in x-Richtung
        //? const int AnzC_y = 65;           //? Anzahl der Zellen in y-Richtung

        int Hilfe_x = (pos_x1 / C_dist_x);
        int Hilfe_y = (pos_y1 / C_dist_y);
        CELL_ID = Hilfe_y * AnzC_x + Hilfe_x;

        return CELL_ID;
    }

    // Berechnet Geschwindigkeit und Position eines Spray Partikels für einen Zeitschritt mittels explizitem Integrationsschema
    array<double, 8> U_und_pos_von_Partikel(array<double, 8> data, double U_cx, double U_cy, int Timesteps) {
        // Sonderfall für Partikel die Auswertungsebene erreicht haben
        if (data[4] == 5) {
            temporaryArray = data;
            return temporaryArray;
        }

        double Zeitpunkt = 0.02 * Timesteps;
        double diameter = data[3];
        double U_px = data[0];
        double U_py = data[1];
        double pos_x = data[4];
        double pos_y = data[5];
        double U_pz = data[2];
        double pos_z = data[6];
        RE = Re_von_Partikel(U_px, U_py, U_cx, U_cy, diameter);
        //Bewegungsgleichungen mit Wiederstand, der Geschwingkeit und der Geschwindigkeit der Zellen
        U_px1 = U_px + ((((U_cx - U_px) / tau_von_Partikel(RE, rho_p, diameter, eta)) + (g * (1 - (rho_c / rho_p)))) * (
                            dT / (1 + (dT / tau_von_Partikel(RE, rho_p, diameter, eta)))));
        U_py1 = (U_py + (((U_cy - U_py) / tau_von_Partikel(RE, rho_p, diameter, eta))) * (
                     dT / (1 + (dT / tau_von_Partikel(RE, rho_p, diameter, eta)))));
        //? Keine Beschleunigung in y-Richtung
        temporaryArray[0] = U_px1;
        temporaryArray[1] = U_py1;
        temporaryArray[2] = U_pz;

        //!============================================================================================================================Berechnung der Position beginnt hier

        pos_x1 = pos_x + U_px * dT; //? Berechnung der Position in x
        pos_y1 = pos_y + U_py * dT; //? Berechnung der Position in y

        temporaryArray[3] = diameter;
        temporaryArray[4] = pos_x1;
        temporaryArray[5] = pos_y1;
        temporaryArray[6] = pos_z;

        //!============================================================================================================================Berechnung der Host-ID beginnt hier
        // Randbedingung: Auswertungsebene bei x = 5 m
        //Partikel wird gestoppt und genaue Position berechnet
        if (pos_x1 >= 5.0) {
            double pos_temp = pos_x1 - 5.0;
            double multiplier = pos_temp / U_px;
            pos_y1 = pos_y1 - multiplier * U_py;
            pos_x1 = 5.0;
            temporaryArray[4] = pos_x1;
            temporaryArray[5] = pos_y1;
            temporaryArray[7] = Cell_ID(pos_x1 - 0.1, pos_y1);

            temporaryArray[0] = 0.0; //? Geschwindigkeit bei dem Erreichen der Auswertungsebene wird null
            temporaryArray[1] = 0.0;

            U_py1=0;
            if (Devmode) {
                cout <<
                        "Das Partikel hat die Auswertungsebene erreicht bzw. hat sie ueberschritten! Die Geschwindigkeit zum vorherigen Zeitpunkt betreagt: "
                        << U_px << " zum Zeitpunkt " << Zeitpunkt << endl;
                cout << "Vor dem Erreichen der Auswertungsebene befand sich das Partikel in der Zelle mit der Host-ID: "
                        << Cell_ID(pos_x1-0.1, pos_y1) << endl;
            }
            U_px1=0;
            // WeitereRandbedingungen:
            // x <= 0   : Decke
            // y <= 0   : untere Wand
            // y >= 13  : obere Wand
        } else if (pos_x1 <= 0.0) {
            pos_x1 = 0.0;
            temporaryArray[4] = pos_x1;
            temporaryArray[5] = pos_y1;
            temporaryArray[0] = 0.0;
            temporaryArray[1] = 0.0;
            if (Devmode) {
                cout << "Partikel klebt an der Decke" << endl;
            }
            temporaryArray[7] = Cell_ID(pos_x1 + 0.1, pos_y1);
        } else if (pos_y1 <= 0.0) {
            pos_y1 = 0.0;
            temporaryArray[4] = pos_x1;
            temporaryArray[5] = pos_y1;
            temporaryArray[0] = 0.0;
            temporaryArray[1] = 0.0;
            if (Devmode) {
                cout << "Raum wird in negativer y-Richtung verlassen" << endl;
            }
            temporaryArray[7] = Cell_ID(pos_x1, pos_y1 + 0.1);
        } else if (pos_y1 >= 13.0) {
            pos_y1 = 13.0;
            temporaryArray[4] = pos_x1;
            temporaryArray[5] = pos_y1;
            temporaryArray[0] = 0.0;
            temporaryArray[1] = 0.0;
            if (Devmode) {
                cout << "Raum in positiver y-Richtung wird verlassen" << endl;
            }
            temporaryArray[7] = Cell_ID(pos_x1, pos_y1 - 0.1);
        } else {
            temporaryArray[7] = Cell_ID(pos_x1, pos_y1);
        }

        temporaryContent.push_back(temporaryArray);
        timeContent.push_back(Zeitpunkt);
        if (Devmode) {
            cout << "----------------------------------------------------------------------------------" << endl;
            cout << "U_px zum Zeitpunkt " << Zeitpunkt << ": " << U_px1 << endl;
            cout << "U_py zum Zeitpunkt " << Zeitpunkt << ": " << U_py1 << endl;
            cout << "U_pz zum Zeitpunkt " << Zeitpunkt << ": " << U_pz << endl;
            cout << "----------------------------------------------------------------------------------" << endl;
            cout << "pos_x zum Zeitpunkt " << Zeitpunkt << ": " << pos_x1 << endl;
            cout << "pos_y zum Zeitpunkt " << Zeitpunkt << ": " << pos_y1 << endl;
            cout << "pos_z zum Zeitpunkt " << Zeitpunkt << ": " << pos_z << endl;
            cout << "----------------------------------------------------------------------------------" << endl;
            cout << "Host_ID: " << temporaryArray[7] << " zum Zeitpunkt " << Zeitpunkt << endl;
            cout << "----------------------------------------------------------------------------------" << endl;
        }
        // Uebergabe der Partikeldaten
        return temporaryArray;
    }

    // Bewegungsgleichung für fallenden Tropfen
    // größeres Zeitintervall (dT = 0.5 s)
    // kein Strömungsfeld (U_c = 0)
    array<double, 8> U_und_pos_von_Tropfen(array<double, 8> data, int Timesteps) {
        if (data[4] == 72) {
            temporaryArray = data;
            return temporaryArray;
        }
        //setzen der physikalischen Variablen entweder aus dem Array oder als konstante Werte
        dT = 0.5;
        double Zeitpunkt = 0.5 * Timesteps;
        double diameter = data[3];
        double U_px = data[0];
        double U_py = data[1];
        double pos_x = data[4];
        double pos_y = data[5];
        double U_pz = data[2];
        double pos_z = data[6];
        double U_cx = 0;
        double U_cy = 0;
        RE = Re_von_Partikel(U_px, U_py, U_cx, U_cy, diameter);
        U_px1 = U_px + ((((U_cx - U_px) / tau_von_Partikel(RE, rho_p, diameter, eta)) + (g * (1 - (rho_c / rho_p)))) * (
                            dT / (1 + (dT / tau_von_Partikel(RE, rho_p, diameter, eta)))));


        temporaryArray[0] = U_px1;
        temporaryArray[1] = U_py1;
        temporaryArray[2] = U_pz;

        //!============================================================================================================================Berechnung der Position beginnt hier

        pos_x1 = pos_x + U_px * dT; //? Berechnung der Position in x
        pos_y1 = pos_y + U_py * dT; //? Berechnung der Position in y
        if (Devmode) {

        }
        temporaryArray[3] = diameter;
        temporaryArray[4] = pos_x1;
        temporaryArray[5] = pos_y1;
        temporaryArray[6] = pos_z;

        //!============================================================================================================================Berechnung der Host-ID beginnt hier
        //Sonderfall für die Auswertungsebene
        if (pos_x1 >= 72) {
            pos_x1 = 72;
            temporaryArray[4] = pos_x1;
            temporaryArray[7] = Cell_ID_Tropfen(pos_x1 - 0.1, pos_y1);

            U_py1 = 0;
            temporaryArray[0] = 0.0; //? Geschwindigkeit bei dem Erreichen der Auswertungsebene wird null
            temporaryArray[1] = 0.0;
            if (Devmode) {
                cout <<
                        "Das Partikel hat die Auswertungsebene erreicht bzw. hat sie ueberschritten! Die Geschwindigkeit zum vorherigen Zeitpunkt betreagt: "
                        << U_px << " zum Zeitpunkt " << Zeitpunkt << endl;
                cout << "Vor dem Erreichen der Auswertungsebene befand sich das Partikel in der Zelle mit der Host-ID: "
                        << Cell_ID_Tropfen(pos_x1-0.1, pos_y1) << endl;
            }
            U_px1 = 0;
        } else {
            temporaryArray[7] = Cell_ID_Tropfen(pos_x1, pos_y1);

            if (Devmode) {

            }
        }
        temporaryContent.push_back(temporaryArray);
        timeContent.push_back(Zeitpunkt);
        if (Devmode) {
            cout << "----------------------------------------------------------------------------------" << endl;
            cout << "U_px zum Zeitpunkt " << Zeitpunkt << ": " << U_px1 << endl;
            cout << "U_py zum Zeitpunkt " << Zeitpunkt << ": " << U_py1 << endl;
            cout << "U_pz zum Zeitpunkt " << Zeitpunkt << ": " << U_pz << endl;
            cout << "----------------------------------------------------------------------------------" << endl;
            cout << "pos_x zum Zeitpunkt " << Zeitpunkt << ": " << pos_x1 << endl;
            cout << "pos_y zum Zeitpunkt " << Zeitpunkt << ": " << pos_y1 << endl;
            cout << "pos_z zum Zeitpunkt " << Zeitpunkt << ": " << pos_z << endl;
            cout << "----------------------------------------------------------------------------------" << endl;
            cout << "Host_ID: " << temporaryArray[7] << " zum Zeitpunkt " << Zeitpunkt << endl;
            cout << "----------------------------------------------------------------------------------" << endl;
        }
        //? Uebergabe der Partikeldaten
        return temporaryArray;
    }

    //? Uebergabe der Zeitschritte ist inzwischen Veraltet.
    vector<double> GiveTime(vector<double> timeContent) {
        for (int i = 0; i < timeContent.size(); i++) {
            return timeContent;
        }
    }
};
