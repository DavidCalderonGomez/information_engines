#pragma once
#include <cmath>
#include "Random64.h"

//dt=1 mus, kspring=42pN/mum, gamma=3,36x10^-8m²kg/s, g=9,8m²/s, kBT=403,2x10^-23J, m=42 x10^-15kg
//We'll use the following new units: m'=9,8e-9m, s'=0,8e-3s, kg'=42e-15kg 
//---------------------------------------------------------------
const double samp_freq=41;
const int parameter=23; //parameter evolutions each sample time such that p
const double ts=1/samp_freq,kbT=640, gama=640, kspring=640, gravity = 640; //ts is the sampling time
const double dt=ts/parameter;
const double alpha = 1-exp(-gama*dt), alphap=alpha*(2-alpha);
//---------------------------------------------------------------------------------------------------
class Particle{
  double Pos,Vel, Vi, Vp, Dv, Work=0, Measurement=0 ;
  double Mass=0.8,Fex=0, Potential_zero = 0;//constant variables for each particle 
 public:		    
  double get_Pos(){ return Pos; }
  double get_Vel(){ return Vel; }
  double get_Potential_zero(){return Potential_zero;}
  void set_Potential_zero(double value){Potential_zero=value;}
  double get_Work(){return Work;}
  
  void Initialize(double xi, double vi);
  void Launch();//First step in the Gunsteren Berendsen algorithm
  void CalculateForce();//calculates all the forces acting on the particle
  void ThermoEvolution(Crandom &ran64);//evolves the system
  double Protocol(double gain, Crandom &ran64,double previous_Pos, double SNR);//returns the difference between measurement and potential
  void CalculateWork(double previous_Pot);
};
//--------------------------------------------------------------------------------------------------
//Method implementation
void Particle::Initialize(double xi,double vi){
  Pos=xi;Vi=vi; Vp=0; Dv=0;
}
void Particle::Launch(){
  Vel=Vi-dt*Fex/(2*Mass);
}
void Particle::CalculateForce(){ 
  Fex = -kspring*(Pos-Potential_zero)-gravity*Mass;
}
void Particle::ThermoEvolution(Crandom &ran64){
  //one step of thermal evolution
  Vp=Vel+Fex/Mass*dt;
  Dv=-alpha*Vel+sqrt(alphap*kbT/Mass)*ran64.gauss(0,1);
  Pos=Pos+(Vel+Dv/2)*dt;
  Vel=Vp+Dv;
}
double Particle::Protocol(double gain, Crandom &ran64,double previous_Pos,double SNR){ //introduce some gain that ideally could be 2
  Measurement = previous_Pos+(sqrt(kbT/kspring))/SNR*ran64.gauss(0,1); //This is a noisy measurement with SNR. 
  double diff =Measurement-Potential_zero;
  if(diff>0){ //if the measurement is over a the potential
    return gain*diff;// rise the potential
  }
  return 0;
}
void Particle::CalculateWork(double previous_Pot){
  Work += kspring/2*(pow(Pos-Potential_zero,2.0)-pow(Pos-previous_Pot,2.0));
} //Estimating the work with the measurement as it was done in the experiment 
