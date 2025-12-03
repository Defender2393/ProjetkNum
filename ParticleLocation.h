#include <iostream>
#include <cmath>
using namespace std;

double diameter, tau;
double Reynolds;
double pos_z, U_pz;
const double g = 9.81;
const double rho_c = 1.199;
const double rho_p = 998.207;
const double zweidurchdrei = 2.0/3.0;
const double eta = 1.824878 * 1e-5;
const double deltaT = 0.02;
const double Zeitpunktmax;


void PartikelÜbergeben(){
}


void PartikelInjizieren(){

}


double Re(double rho_c, double U_px, double diameter, double eta){
    Reynolds = (rho_c * U_px * diameter)/eta;
        return Reynolds;
}


void U_und_pos(){
double U_px0, U_px1, U_py0, U_py1, Widerstand, REp;
double Zeitpunkt = 0.02;


    REp = Re(rho_c, U_px0, diameter, eta);
        if(REp <= 0 || U_px0 == 0 || U_py0 == 0){             
            cout << "Fehler" << endl;
                
        }
        else if(REp <= 1000){
            Widerstand = (24.0/REp) * (1.0 + (1.0/6.0) * pow(REp, zweidurchdrei)); //Widerstand
        }else{
            Widerstand = 0.424;                                                    //Widerstand
        }

        tau = 4.0 / 3.0 * (rho_p * pow(diameter, 2)) / (eta * REp * Widerstand); //Relaxationszahl
    
        U_px1 = U_px0 + ((-U_px0/tau) + g * (1 - (rho_c / rho_p))) * deltaT / (1 + (deltaT / tau));
        U_py1 = U_py0 + ((-U_py0/tau) + g * (1 - (rho_c / rho_p))) * deltaT / (1 + (deltaT / tau));
        U_pz = 0.0;
        U_px0 = U_px1;
        U_py0 = U_py1;
        cout << "U_px zum Zeitpunkt " << Zeitpunkt <<": " << U_px0 << endl; 
        cout << "U_py zum Zeitpunkt " << Zeitpunkt <<": " << U_py0 << endl;
   Zeitpunkt += 0.02;


    cout << "U_px: " << U_px0 << endl;
    cout << "U_py: " << U_py0 << endl;
    
 // if(pos_x >= 5)!!!!
    double pos_x0, pos_x1, pos_y0, pos_y1;
    
    pos_x1 = pos_x0 + U_px0 * deltaT;
    pos_y1 = pos_y0 + U_py0 * deltaT;
    pos_z = 0.0;
    pos_x0 = pos_x1;
    pos_y0 = pos_y1;
    cout << "pos_x zum Zeitpunkt " << Zeitpunkt <<": " << pos_x0<< endl; 
    cout << "pos_y zum Zeitpunkt " << Zeitpunkt <<": " << pos_y0 << endl;

int Host_iD[1625];
int Grid_x[25];
int Grid_y[65];
int id = 0;





for(int y = 0 ; y < 65; y++){
    for(int x = 0; x < 25; x++){
       
       
        int Cell_ID = y * 25 + x;
        Host_iD[id] = Cell_ID;
        id++;
    }
}



}










     
















