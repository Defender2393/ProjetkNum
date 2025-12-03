#include <iostream>
#include <cmath>
using namespace std;

const double diameter = 0.02;
double tau;
double Reynolds;
double U_px, U_py, U_pz;
double pos_x, pos_y, pos_z;
const double g = 9.81;
const double rho_c = 1.199;
const double rho_p = 998.207;
const double zweidurchdrei = 2.0/3.0;
const double eta = 1.824878 * 1e-5;
const double deltaT = 0.2;
const double Zeitpunktmax = 10.0;
double U_p;


void PartikelÜbergeben(){
}


void PartikelInjizieren(){

}





double Re(double rho_c, double U_p, double diameter, double eta){
    Reynolds = (rho_c * U_p * diameter)/eta;
        return Reynolds;
}


double U_und_pos(double U_px, double U_py, double pos_x, double pos_y){
    double U_px0 = U_px;
    double U_py0 = U_py;
    double U_px1, U_py1, Widerstand, REp;
    double Zeitpunkt = 0.0;

    for(Zeitpunkt ; Zeitpunkt < Zeitpunktmax ; ){

    REp = Re(rho_c, U_p, diameter, eta);
        if(REp <= 0){             
            cout << "Fehler und/oder es wird durch null geteilt!" << endl; //Hier kommt es immer zum Stehen 
            break;
        }
        else if(REp <= 1000){
            Widerstand = (24.0/REp) * (1.0 + (1.0/6.0) * pow(REp, zweidurchdrei)); //Widerstand
        }else{
            Widerstand = 0.424;                                                    //Widerstand
        }

        tau = 4.0 / 3.0 * (rho_p * pow(diameter, 2)) / (eta * REp * Widerstand); //Relaxationszahl, aber der Kehrwert berechnet aus der Formelsammlung
    
        U_px1 = U_px0 + ((-U_px0/tau) + g * (1 - (rho_c / rho_p))) * deltaT / (1 + (deltaT / tau)); //Immernoch angenommen, dass U_c = 0 !
        U_py1 = U_py0 + ((-U_py0/tau) + g * (1 - (rho_c / rho_p))) * deltaT / (1 + (deltaT / tau));
        U_pz = 0.0;                                                                                 //Und die Bewegung der Teilchen ist nur in x- und y Richtung berücksichtigt 
        U_px0 = U_px1;                                                                              //Trotzdem darf man die z-Koordinate nicht auslassen 
        U_py0 = U_py1;
        cout << "U_px zum Zeitpunkt " << Zeitpunkt <<": " << U_px0 << endl; 
        cout << "U_py zum Zeitpunkt " << Zeitpunkt <<": " << U_py0 << endl;
            
            double pos_x0 = pos_x;
            double pos_y0 = pos_y;
            double pos_x1, pos_y1;
            pos_x1 = pos_x0 + U_px0 * deltaT;
            pos_y1 = pos_y0 + U_py0 * deltaT;
            pos_z = 0.0;                                                                                    //Dasselbe gilt für die position in z-Richtung 
            pos_x0 = pos_x1;
            pos_y0 = pos_y1;
            cout << "pos_x zum Zeitpunkt " << Zeitpunkt <<": " << pos_x0<< endl; 
            cout << "pos_y zum Zeitpunkt " << Zeitpunkt <<": " << pos_y0 << endl;


// man kann ja auch einfach durch integer-division die jeweiligen koordinaten der Zellen ermitteln, so kann man sich die ewige Suche nach dem kleinsten element sparen
// maximaler Abstand zu einem Zellmittelpunkt in y- und x-Richtung = 0.1
// also int Hilfe_x = (pos_x0 / 0.1) und int Hilfe_y = (pos_y0 / 0.1)
// mit allgemeiner Formel für die Durchnummerierung mit Cell_ID = y * 25 + x umgewandelt zu Cell_ID = Hilfe_y * 25 + Hilfe_x
const double C_dist_xy = 0.1;
const int AnzC_x = 25; // Anzahl der Zellen in x-Richtung
const int AnzC_y = 65; // Anzahl der Zellen in y-Richtung

if(pos_x0 >= 5.0){
    cout << "Das Partikel hat die Auswertungsebene erreicht bzw. hat sie ueberschritten! Die Geschwindigkeit zum vorherigen Zeitpunkt betreagt: " << pos_x1 <<" zum Zeitpunkt " << Zeitpunkt - deltaT << endl;
    int Hilfe_x = (pos_x1 / C_dist_xy);
    int Hilfe_y = (pos_y1 / C_dist_xy);

    int Cell_ID = Hilfe_y * AnzC_x + Hilfe_x;
    cout << "Vor dem Erreichen der Auswertungsebene befand sich das Partikel in der Zelle mit der Host-ID: " << Cell_ID << endl;
    break;
}
int Hilfe_x = (pos_x0 / C_dist_xy);
int Hilfe_y = (pos_y0 / C_dist_xy);
int Cell_ID = Hilfe_y * AnzC_x + Hilfe_x;


if(abs(fmod(pos_x0 , C_dist_xy)) == 0 || abs(fmod(pos_y0, C_dist_xy) == 0.0)){ // Modulo-Operator nur für Integers!
    
    cout << "Das Partikel befindet sich genau zwischen zwei Zellen!" << endl;
}

cout << "Host_ID: " << Cell_ID << " zum Zeitpunkt " << Zeitpunkt <<  endl;
   
    }

}


int main(){
    cout << "Herzlich Willkommen zur Probeversion des Programms zur Berechnung von der Geschwindigkeit, Position und der Host-ID eines Partikels" << endl;
    cout << "Geben Sie eine Anfangsgeschwindigkeit in x-Richtung an: ";
    cin >> U_px;
    cout << "Geben Sie eine Anfangsgeschwindigkeit in y-Richtung an: ";
    cin >> U_py;
    U_p = sqrt(pow(U_px, 2) + pow(U_py, 2)); // Wichtig für Reynoldszahl
    cout << "|U| betraegt: " << U_p << endl;
    cout << "Nun wird noch jeweils die Anfangsposition des Partikels benoetigt" << endl;
    cout << "pos_x: ";
    cin >> pos_x;
    cout << "pos_y: ";
    cin >> pos_y;

    Re(rho_c, U_p, diameter, eta);
    U_und_pos(U_px, U_py, pos_x, pos_y);



    return 0;
}







     
















