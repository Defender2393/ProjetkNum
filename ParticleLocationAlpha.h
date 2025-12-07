#include <iostream>
#include <cmath>
#include <vector>
#include <array>
using namespace std;

const double U_max = 7.06423;
const double diameter = 0.002;
double tau;
double Reynolds;
double U_cx = 0.0;
double U_cy = 0.0;
double U_cz = 0.0;
double U_px, U_py, U_pz;
double pos_x, pos_y, pos_z;
int Cell_ID;
const double g = 9.81;
const double rho_c = 1.199;
const double rho_p = 998.207;
const double zweidurchdrei = 2.0/3.0;
const double eta = 1.824878 * 1e-5;
const double deltaT = 0.5;
const double Zeitpunktmax = 11.0;
double U_p;
vector <array<double, 8>> temporaryContent;
array<double, 8> temporaryArray;
double temptransfer;

class Partikel_Eigenschaften{

public:
double Re_von_Partikel(double U_p, double diameter){
    Reynolds = (rho_c * U_p * diameter)/eta;
        return Reynolds;
}


vector<array<double, 8>> U_und_pos_von_Partikel(double U_px, double pos_x){
    double U_px0 = U_px;
    double pos_x0 = pos_x;
    
    double U_px1, U_py1, Widerstand, REp;
    double Zeitpunkt = 0.5;

    
    
    
    for(Zeitpunkt ; Zeitpunkt < Zeitpunktmax ; ){

        if(U_px0 <= U_max){
            
        }
        cout << "|U_p|: " << U_px0 << " Zum Zeitpunkt " << Zeitpunkt << endl;
    REp = Re_von_Partikel(U_px0, diameter);
        if(REp <= 0){             
            cout << "Fehler und/oder es wird durch null geteilt!" << endl; 
            break;
        }
        else if(REp <= 1000){
            Widerstand = (24.0/REp) * (1.0 + (1.0/6.0) * pow(REp, zweidurchdrei)); //Widerstand
        }else{
            Widerstand = 0.424;                                                    //Widerstand
        }

        tau = 4.0 / 3.0 * (rho_p * pow(diameter, 2)) / (eta * REp * Widerstand); //Relaxationszahl, aber der Kehrwert berechnet aus der Formelsammlung
    
        U_px1 = U_px0 + ((abs(U_cx - U_px0)/tau) + g * (1 - (rho_c / rho_p))) * deltaT / (1 + (deltaT / tau)); //Immernoch angenommen, dass U_c = 0 !
       if (U_px1 >= U_max){
        cout <<"Das Equilibrium und somit die theoretisch maximal erreichbare Geschwindigkeit ist erreicht!" << endl;
        U_px1 = U_max;
       }
        U_py = 0.0;
        U_pz = 0.0;                                                                                 //Und die Bewegung der Teilchen ist nur in x- und y Richtung berücksichtigt 
        U_px0 = U_px1;                                                                              //Trotzdem darf man die z-Koordinate nicht auslassen 
        
        cout << "U_px zum Zeitpunkt " << Zeitpunkt <<": " << U_px0 << endl; 
        cout << "U_py zum Zeitpunkt " << Zeitpunkt <<": " << U_py << endl;
temporaryArray[0] = U_px0;
temporaryArray[1] = U_py;
temporaryArray[2] = U_pz;
            
            double pos_y0 = 30.0;
            double pos_x1, pos_y1;
            pos_x1 = pos_x0 + U_px0 * deltaT;
            
            pos_z = 0.0;    //z-Komponente wird nicht berücksichtigt und bleibt daher konstant                                                                                 
            pos_x0 = pos_x1;
           
            cout << "pos_x zum Zeitpunkt " << Zeitpunkt <<": " << pos_x0<< endl; 
            cout << "pos_y zum Zeitpunkt " << Zeitpunkt <<": " << pos_y0 << endl;
temporaryArray[3] = diameter;
temporaryArray[4] = pos_x0;
temporaryArray[5] = pos_y0;
temporaryArray[6] = pos_z;

const double C_dist_x = 12; // Kantenlänge der Zelle in x-Richtung
const double C_dist_y = 20; // Kantenlänge der Zelle in y-Richtung
const int AnzC_x = 6; // Anzahl der Zellen in x-Richtung
const int AnzC_y = 3; // Anzahl der Zellen in y-Richtung

if(pos_x0 >= 72.0){
    cout << "Das Partikel hat die Auswertungsebene erreicht bzw. hat sie ueberschritten! Die Geschwindigkeit zum vorherigen Zeitpunkt betreagt: " << U_p <<" zum Zeitpunkt " << Zeitpunkt - deltaT << endl;
    int Hilfe_x = (pos_x1 / C_dist_x);
    int Hilfe_y = (pos_y1 / C_dist_y);
    Cell_ID = Hilfe_y * AnzC_x + Hilfe_x;
    cout << "Vor dem Erreichen der Auswertungsebene befand sich das Partikel in der Zelle mit der Host-ID: " << Cell_ID << endl;
temporaryArray[7] = Cell_ID;
    break;
}
    int Hilfe_x = (pos_x0 / C_dist_x);
    int Hilfe_y = (pos_y0 / C_dist_y);
    Cell_ID = Hilfe_y * AnzC_x + Hilfe_x;
temporaryArray[7] = Cell_ID;
if(Cell_ID > 17){
    cout << "Das Partikel hat den Auswertungsbereich verlassen!" << endl;
    break;
}

if(abs(fmod(pos_x0 , C_dist_x)) == 0.0 || abs(fmod(pos_y0, C_dist_y) == 0.0)){  
    cout << "Das Partikel befindet sich genau zwischen zwei Zellen!" << endl;
}
    cout << "Host_ID: " << Cell_ID << " zum Zeitpunkt " << Zeitpunkt <<  endl;
    
    Zeitpunkt += deltaT;
    temporaryContent.push_back(temporaryArray);
        
        }
   for (int i = 0; i < temporaryContent.size() ; i++) {
            for (int j = 0; j < 8; j++) {

                return  temporaryContent;

            }
        
        }
    
    }
    double returnup() {
    if (U_py==0){U_p=U_px;}
    return U_p;
}
};


int control(){ //Dient nur zur Kontrolle, kann bzw muss später entfernt werden
    cout << "Herzlich Willkommen zur Probeversion des Programms zur Berechnung von der Geschwindigkeit, Position und der Host-ID eines Partikels" << endl;
    cout << "Geben Sie eine Anfangsgeschwindigkeit in x-Richtung an: ";
    cin >> U_px;
    cout << "Geben Sie eine Anfangsgeschwindigkeit in y-Richtung an: ";
    cin >> U_py;   
    cout << "Nun wird noch jeweils die Anfangsposition des Partikels benoetigt" << endl;
    cout << "pos_x: ";
    cin >> pos_x;
    cout << "pos_y: ";
    cin >> pos_y;

Partikel_Eigenschaften Partikel1 ;                                   //                        
Partikel1.Re_von_Partikel(U_px, diameter);               //Zeile 129 - 131 nicht loeschen!
Partikel1.U_und_pos_von_Partikel(U_px, pos_x);                       //
    
    return 0;
}