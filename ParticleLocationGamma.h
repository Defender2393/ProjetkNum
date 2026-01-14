#include <iostream>
#include <cmath>
#include <vector>
#include <array>
using namespace std;


//-------------------------------------
double U_cx = 0.0;
double U_cy = 0.0;  //Geschwindigkeiten der Zelle
double U_cz = 0.0;
//-------------------------------------
double U_px, U_px0, U_px1; 
double U_py = 0.0;  //Geschwindigkeiten vom Partikel
double U_pz = 0.0;
//-------------------------------------
double pos_x, pos_x0, pos_x1;
double pos_y = 30.0;    //Positionen vom Partikel
double pos_z =  5.0;
//-------------------------------------
const double U_max = 7.06423;
const double diameter = 0.002;
const double g = 9.81;
const double rho_c = 1.199;
const double rho_p = 998.207;
const double zweidurchdrei = 2.0/3.0;
const double eta = 1.824878*1e-5;
const double dT = 0.5;
const double Zeitpunktmax = 11.0;
double tau;
double Reynolds, RE;
double Widerstand;
int Cell_ID;
double U_p;
vector <array<double, 8>> temporaryContent;
array<double, 8> temporaryArray;
vector <double> timeContent;
double temptransfer;

class Partikel_Eigenschaften{

public:

double Re_von_Partikel(double U_px, double U_cx, double diameter){
    Reynolds = (rho_c * abs(U_px - U_cx) * diameter) / eta;
        return Reynolds;
}
    /*
    double Widerstand_von_Partikel(double RE){
    
    if(RE <= 0){             
        cout << "Fehler, weil Reynoldszahl kleiner/gleich null" << endl; 
            return 0;    
    }
    else if(RE <= 1000){
        Widerstand = (24.0/RE) * (1.0 + (1.0/6.0) * pow(RE, zweidurchdrei));
            return Widerstand;
    }
    else if(RE > 1000){
        Widerstand = 0.424;   
            return Widerstand;
        }
    }
    */

double tau_von_Partikel(double RE, double rho_p, double diameter, double eta) {
    //Relaxationszahl, aber der Kehrwert berechnet aus der Formelsammlung, außerdem wurde gleichung 8 in 11 eingesetzt, sodass sich Re rauskürzt

    if (RE==0){
        tau =  (4.0 * rho_p * pow(diameter, 2)) / (72.0 * eta * (1 + (1.0/6.0) * pow(RE, 2.0/3.0)));

    }
    else if (RE<=1000){
        Widerstand = (24.0/RE) * (1.0 + (1.0/6.0) * pow(RE, zweidurchdrei));
        tau = 4.0 / 3.0 * (rho_p * pow(diameter, 2)) / (eta * RE * Widerstand); //Relaxationszahl, aber der Kehrwert berechnet aus der Formelsammlung

        }
    return tau;
}



vector<array<double, 8>> U_und_pos_von_Partikel(array<double, 8> temporaryArray){
    
    //Hier erfolgt eine Ueberschreibung!
    //double U_px0 = U_px;
    
    //Hier erfolgt eine Ueberschreibung!
    //double pos_x0 = pos_x;
    
    double Zeitpunkt = 0.0;

    /*
    if(U_px0 == 0){
    cout << "Korrektur! " << endl;
    
    //Hier erfolgt zwar auch eine Ueberschreibung, aber nur einmal(hoffentlich)
    U_px0 = U_px0 + g * dT;
    
    cout << "Das Partikel wird auf " << U_px0 << " beschleunigt!" << endl;
    }
    */
    


        
    RE = Re_von_Partikel(U_px, 0, diameter);
    U_px1 = U_px + ((((U_cx - U_px) / tau_von_Partikel(RE, rho_p, diameter, eta)) + (g * (1 - (rho_c / rho_p)))) * (dT / (1 + (dT / tau_von_Partikel(RE, rho_p, diameter, eta)))));
   //     cout << "----------------------------------------------------------------------------------" << endl;
     //   cout << "U_px zum Zeitpunkt " << Zeitpunkt <<": " << U_px0 << endl;
//        cout << "U_py zum Zeitpunkt " << Zeitpunkt <<": " << U_py << endl;
  //      cout << "U_pz zum Zeitpunkt " << Zeitpunkt <<": " << U_pz << endl;
    //    cout << "----------------------------------------------------------------------------------" <<endl;
        
        //Hier erfolgt eine Ueberschreibung!
        //U_px0 = U_px1;
 
        temporaryArray[0] = U_px1;
        temporaryArray[1] = U_py;
        temporaryArray[2] = U_pz;
//============================================================================================================================Berechnung der Position beginnt hier         
               
        pos_x1 = pos_x+ U_px * dT;
        //pos_x1 = 0.5 * g * pow(dT, 2) + U_px * Zeitpunkt + pos_x;
        
  //      cout << "pos_x zum Zeitpunkt " << Zeitpunkt <<": " << pos_x1<< endl;
    //    cout << "pos_y zum Zeitpunkt " << Zeitpunkt <<": " << pos_y << endl;
      //  cout << "pos_z zum Zeitpunkt " << Zeitpunkt <<": " << pos_z << endl;
        //cout << "----------------------------------------------------------------------------------" << endl;
        
        //Hier erfolgt eine Ueberschreibung!
        //pos_x0 = pos_x1;                                 
                   
        temporaryArray[3] = diameter;
        temporaryArray[4] = pos_x1;
        temporaryArray[5] = pos_y;
        temporaryArray[6] = pos_z;
//============================================================================================================================Berechnung der Host-ID beginnt hier
        const double C_dist_x = 12.0; // Kantenlänge der Zelle in x-Richtung
        const double C_dist_y = 20.0; // Kantenlänge der Zelle in y-Richtung
        const int AnzC_x = 6; // Anzahl der Zellen in x-Richtung
        //const int AnzC_y = 3; // Anzahl der Zellen in y-Richtung

    if(pos_x1 >= 72.0){
        pos_x1 = 72.0;
        temporaryArray[4] = pos_x1;
   //     cout << "Das Partikel hat die Auswertungsebene erreicht bzw. hat sie ueberschritten! Die Geschwindigkeit zum vorherigen Zeitpunkt betreagt: " << U_px1 <<" zum Zeitpunkt " << Zeitpunkt - dT << endl;
        int Hilfe_x = (pos_x1 / C_dist_x);
        int Hilfe_y = (pos_y / C_dist_y);
        Cell_ID = Hilfe_y * AnzC_x + Hilfe_x-1;
    //    cout << "Vor dem Erreichen der Auswertungsebene befand sich das Partikel in der Zelle mit der Host-ID: " << Cell_ID << endl;
        temporaryArray[7] = Cell_ID;
        temporaryArray[0] = 0;
    }
    else {
        int Hilfe_x = (pos_x1 / C_dist_x);
        int Hilfe_y = (pos_y / C_dist_y);
        Cell_ID = Hilfe_y * AnzC_x + Hilfe_x;

        temporaryArray[7] = Cell_ID;
    }



   //     cout << "Host_ID: " << Cell_ID << " zum Zeitpunkt " << Zeitpunkt <<  endl;
     //   cout << "----------------------------------------------------------------------------------" << endl;
    
        Zeitpunkt += dT;
        temporaryContent.push_back(temporaryArray);
        timeContent.push_back(Zeitpunkt);    
    
    //Übergabe der Partikeldaten

                return  temporaryContent;


    } 

    //Übergabe der Zeitschritte
    vector<double> GiveTime(vector<double> timeContent){

        for(int i = 0 ; i < timeContent.size() ; i++){

                return timeContent;
        }
    }                    
};


int control(){ //Dient nur zur Kontrolle, kann bzw muss später entfernt werden
    cout << "Herzlich Willkommen zur Probeversion des Programms zur Berechnung von der Geschwindigkeit, Position und der Host-ID eines Partikels" << endl;
    cout << "U_px: ";
    cin >> U_px; 
    cout << "pos_x: ";
    cin >> pos_x;

//Partikel_Eigenschaften Partikel1 ;                                                                                   
//Partikel1.Re_von_Partikel(U_px, U_cx, diameter);                                                
//Partikel1.U_und_pos_von_Partikel(U_px, U_py, U_pz, pos_x, pos_y, pos_z);                        
    
    return 0;
}