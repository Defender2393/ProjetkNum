#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include "GeleseneDatenC.h"
using namespace std;

//! Die Ausrufe-und Fragezeichen dienen zur Aenderung der Farbe der Comments, also nicht wundern

//!-------------------------------------

double U_cx = 0.0;
double U_cy = 0.0;      //? Geschwindigkeiten der Zelle
double U_cz = 0.0;      //TODO kann nach der automatischen Uebergabe geloescht werden

//!-------------------------------------

double U_px, U_px0, U_px1, U_py0, U_py1; 
double U_py = 0.0;      //? Geschwindigkeiten vom Partikel
double U_pz = 0.0;      //TODO nur z bleibt null

//!-------------------------------------

double pos_x, pos_x0, pos_x1, pos_y0, pos_y1;
double pos_y = 30.0;    //? Positionen vom Partikel
double pos_z =  5.0;    //TODO nur z bleibt konstant

//!-------------------------------------

const double diameter = 0.02; //! in MS3 nicht mehr konstant, kann aber zur Kontrolle 0.02 vorerst bleiben
const double g = 9.81;
const double rho_c = 1.199;
const double rho_p = 998.207;
const double zweidurchdrei = 2.0/3.0;
const double eta = 1.824878*1e-5;
const double dT = 0.5;
double tau;
double Reynolds, RE;
double Widerstand;
double U_p, U_c;
double temptransfer;
int CELL_ID;
int Raumrichtung;
vector <array<double, 8>> temporaryContent; 
array<double, 8> temporaryArray; //! 0,1,2 fuer Geschwindigkeiten / 3 fuer diameter / 4,5,6 fuer Positionen / 7 fuer Cell_ID 
vector<double> timeContent;
vector<array<double, 3>> temporaryUValue;



class Partikel_Eigenschaften{

public:

double Betrag_von_U_von_Partikel(double U_px, double U_py){
    U_p = sqrt(pow(U_px, 2) + pow(U_py, 2)); 
            return U_p;
}

double Betrag_von_U_von_Zelle(double U_cx, double U_cy){
    U_c = sqrt(pow(U_cx, 2) + pow(U_cy, 2)); 
            return U_c;
}

//? ReynoldsZahl
double Re_von_Partikel(double U_px, double U_py, double U_cx, double U_cy, double diameter){ 
    Reynolds = (rho_c * abs(Betrag_von_U_von_Partikel(U_px, U_py) - Betrag_von_U_von_Zelle(U_cx, U_cy)) * diameter) / eta;
            return Reynolds;
}

//? Tau
double tau_von_Partikel(double RE, double rho_p, double diameter, double eta) { //? Relaxationszahl, aber der Kehrwert berechnet aus der Formelsammlung, außerdem wurde gleichung 8 in 11 eingesetzt, sodass sich Re rauskürzt
    
    if (RE == 0){
        tau = (4.0 * rho_p * pow(diameter, 2)) / (72.0 * eta * (1 + (1.0/6.0) * pow(RE, 2.0/3.0)));
    }
    else if (RE <= 1000){
        Widerstand = (24.0/RE) * (1.0 + (1.0/6.0) * pow(RE, zweidurchdrei));
        tau = 4.0 / 3.0 * (rho_p * pow(diameter, 2)) / (eta * RE * Widerstand); //? Relaxationszahl, aber der Kehrwert berechnet aus der Formelsammlung
    }
    else if(RE > 1000){
        Widerstand = 0.424;
        tau = 4.0 / 3.0 * (rho_p * pow(diameter, 2)) / (eta * RE * Widerstand);
    }
            return tau;

}

double WIND(int Cell_ID, double U_px1, double U_py1){
    for(Raumrichtung = 0; Raumrichtung < 2; Raumrichtung++){

        if(Raumrichtung == 0){ //? x-Richtung 
        
            U_cx = temporaryUValue[Cell_ID][0];

            U_px1 = U_px1 + U_cx;
                return U_px1;
        }
        else if(Raumrichtung == 1){ //? y-Rictung
        
            U_cy = temporaryUValue[Cell_ID][1];

            U_py1 = U_py1 + U_cy;
                return U_py1;
        }
    }
}

int Cell_ID(double pos_x1, double pos_y1){

        const double C_dist_x = 0.2;   //? Kantenlaenge der Zelle in x-Richtung
        const double C_dist_y = 0.2;   //? Kantenlaenge der Zelle in y-Richtung
        const int AnzC_x = 25;           //? Anzahl der Zellen in x-Richtung
    //? const int AnzC_y = 65;           //? Anzahl der Zellen in y-Richtung

        int Hilfe_x = (pos_x1 / C_dist_x);
        int Hilfe_y = (pos_y1 / C_dist_y);
        CELL_ID = Hilfe_y * AnzC_x + Hilfe_x;

    return CELL_ID;
}

//? Geschwindigkeiten und Positionen
vector<array<double, 8>> U_und_pos_von_Partikel(double U_px, double U_py, double U_pz, double pos_x, double pos_y, double pos_z){
    
        double Zeitpunkt = 0.0;

        RE = Re_von_Partikel(U_px, U_py, U_cx, U_cy, diameter);
        U_px1 = U_px + ((((U_cx - U_px) / tau_von_Partikel(RE, rho_p, diameter, eta)) + (g * (1 - (rho_c / rho_p)))) * (dT / (1 + (dT / tau_von_Partikel(RE, rho_p, diameter, eta)))));
        U_py1 = U_py +   ((U_cy - U_py) / tau_von_Partikel(RE, rho_p, diameter, eta)); //? Keine Beschleunigung in y-Richtung
        cout << "----------------------------------------------------------------------------------" << endl;                                                                                                                                                                     
        cout << "U_px zum Zeitpunkt " << Zeitpunkt <<": " << U_px0 << endl; 
        cout << "U_py zum Zeitpunkt " << Zeitpunkt <<": " << U_py1 << endl;
        cout << "U_pz zum Zeitpunkt " << Zeitpunkt <<": " << U_pz << endl;
        cout << "----------------------------------------------------------------------------------" <<endl;      
 
        temporaryArray[0] = U_px1;
        temporaryArray[1] = U_py1;
        temporaryArray[2] = U_pz;

//!============================================================================================================================Berechnung der Position beginnt hier         
               
        pos_x1 = pos_x + U_px * dT; //? Berechnung der Position in x
        pos_y1 = pos_y + U_py * dT; //? Berechnung der Position in y

        cout << "pos_x zum Zeitpunkt " << Zeitpunkt <<": " << pos_x1<< endl; 
        cout << "pos_y zum Zeitpunkt " << Zeitpunkt <<": " << pos_y1 << endl;
        cout << "pos_z zum Zeitpunkt " << Zeitpunkt <<": " << pos_z << endl;
        cout << "----------------------------------------------------------------------------------" << endl;
                                                   
        temporaryArray[3] = diameter;
        temporaryArray[4] = pos_x1;
        temporaryArray[5] = pos_y1;
        temporaryArray[6] = pos_z;

//!============================================================================================================================Berechnung der Host-ID beginnt hier
        
    if(pos_x1 >= 72.0){
        pos_x1 = 72.0;
        temporaryArray[4] = pos_x1;
        temporaryArray[5] = pos_y1;
        temporaryArray[0] = 0.0; //? Geschwindigkeit bei dem Erreichen der Auswertungsebene wird null
        temporaryArray[1] = 0.0;
        cout << "Das Partikel hat die Auswertungsebene erreicht bzw. hat sie ueberschritten! Die Geschwindigkeit zum vorherigen Zeitpunkt betreagt: " << U_px1 <<" zum Zeitpunkt " << Zeitpunkt - dT << endl;
        cout << "Vor dem Erreichen der Auswertungsebene befand sich das Partikel in der Zelle mit der Host-ID: " << Cell_ID(pos_x1, pos_y1) << endl;       
        temporaryArray[7] = Cell_ID(pos_x1 - 0.1, pos_y1);
    }
    else if(pos_x1 <= 0.0){
        pos_x1 = 0.0;
        temporaryArray[4] = pos_x1;
        temporaryArray[5] = pos_y1;
        temporaryArray[0] = 0.0; 
        temporaryArray[1] = 0.0;
cout<< "Partikel klebt an der Decke" << endl;
        temporaryArray[7] = Cell_ID(pos_x1 + 0.1, pos_y1);
    }
    else if(pos_y1 <= 0.0){
        pos_y1 = 0.0;
        temporaryArray[4] = pos_x1;
        temporaryArray[5] = pos_y1;
        temporaryArray[0] = 0.0; 
        temporaryArray[1] = 0.0;
        
cout << "Raum wird in negativer y-Richtung verlassen" << endl;
        temporaryArray[7] = Cell_ID(pos_x1, pos_y1 + 0.1);
    }
    else if(pos_y1 >= 13.0){
        pos_y1 = 13.0;
        temporaryArray[4] = pos_x1;
        temporaryArray[5] = pos_y1;
        temporaryArray[0] = 0.0; 
        temporaryArray[1] = 0.0;
cout << "Raum in positiver y-Richtung wird verlassen" << endl;
        temporaryArray[7] = Cell_ID(pos_x1, pos_y1 - 0.1);
    }
    else{        
        temporaryArray[7] = Cell_ID(pos_x1, pos_y1);
    }

        cout << "Host_ID: " << Cell_ID(pos_x1, pos_y1) << " zum Zeitpunkt " << Zeitpunkt <<  endl;
        cout << "----------------------------------------------------------------------------------" << endl;
    
        Zeitpunkt += dT;
        temporaryContent.push_back(temporaryArray);
        timeContent.push_back(Zeitpunkt);    
    
    //? Uebergabe der Partikeldaten
            return temporaryContent;
    } 

    //? Uebergabe der Zeitschritte
    vector<double> GiveTime(vector<double> timeContent){
        for(int i = 0 ; i < timeContent.size() ; i++){
            return timeContent;        
        }    
    }                    
};

//? Dient nur zur Kontrolle, kann bzw. muss spaeter entfernt werden
int control(){ 
    
    cout << "Herzlich Willkommen zur Probeversion des Programms zur Berechnung von der Geschwindigkeit, Position und der Host-ID eines Partikels" << endl;
    cout << "U_px: ";
    cin >> U_px; 
    cout << "U_py: ";
    cin >> U_py;
    cout << "pos_x: ";
    cin >> pos_x;
    cout << "pos_y: ";
    cin >> pos_y;

//! Partikel_Eigenschaften Partikel1;                                                                                   
//! Partikel1.Re_von_Partikel(U_px, U_cx, diameter);                                                
//! Partikel1.U_und_pos_von_Partikel(U_px, U_py, U_pz, pos_x, pos_y, pos_z);                        
    
    return 0;
}