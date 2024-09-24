#pragma once
#include <cmath>
#include "Random64.h"

//dt=1 mus, kspring=42pN/mum, gamma=3,36x10^-8m²kg/s, g=9,8m²/s, kBT=403,2x10^-23J, m=42 x10^-15kg
//We'll use the following new units: m'=9,8e-9m, s'=0,8e-3s, kg'=42e-15kg 
//---------------------------------------------------------------
const double dt=0.00125,ts=0.025, tmax=10,kbT=640, gama=640, gravity = 640; //ts is the sampling time
const double alpha = 1-exp(-gama*dt), alphap=alpha*(2-alpha);
const int steps=20;
//---------------------------------------------------------------------------------------------------
class Particle{
  double Pos,Vel, Vi, Vp, Dv, Work=0, Measurement=0 ;
  double Mass=0.8,Fex=0, kspring=640, Potential_zero = 0;//constant variables for each particle 
 public:		    
  double get_Pos(){ return Pos; }
  double get_Vel(){ return Vel; }
  double get_Potential_zero(){return Potential_zero;}
  void set_Potential_zero(double value){Potential_zero=value;}
  double get_Work(){return Work;}
  double get_Measurement(){return Measurement;}
  
  void Initialize(double xi, double vi);
  void Launch();//First step in the Gunsteren Berendsen algorithm
  void CalculateForce();//calculates all the forces acting on the particle
  void ThermoEvolution(Crandom &ran64);//evolves the system
  double Protocol(double gain, Crandom &ran64, double SNR);//returns the difference between measurement and potential
  void CalculateWork(double pot3);
};
//--------------------------------------------------------------------------------------------------
//Method implementation
void Particle::Initialize(double xi,double vi){
  Pos=xi;Vi=vi; Vp=0; Dv=0;Work=0;Fex=0;Potential_zero=0;
}
void Particle::Launch(){
  Vel=Vi-dt*Fex/(2*Mass);
}
void Particle::CalculateForce(){ 
  Fex = -kspring*(Pos-Potential_zero)-gravity*Mass; // the restoring force
}
void Particle::ThermoEvolution(Crandom &ran64){//pass by reference the ran64 element
  //one step of thermal evolution
  Vp=Vel+Fex/Mass*dt;
  Dv=-alpha*Vel+sqrt(alphap*kbT/Mass)*ran64.gauss(0,1);
   //update the variables
  Pos=Pos+(Vel+Dv/2)*dt;
  Vel=Vp+Dv;
}
double Particle::Protocol(double gain, Crandom &ran64, double SNR){ //introduce some gain that ideally could be 2
  Measurement = Pos+(sqrt(kbT/kspring))/SNR*ran64.gauss(0,1); //This is a noisy measurement with SNR. 
  double diff =Measurement-Potential_zero;
  if(diff>0){ //if the measurement is over a the potential
    return Potential_zero+gain*diff;// rise the potential
  }
  return Potential_zero;
}
void Particle::CalculateWork(double pot3){
  Work += kspring/2*(pow(Pos-Potential_zero,2.0)-pow(Measurement-pot3,2.0));
} //Estimating the work with the measurement as it was done in the experiment 
