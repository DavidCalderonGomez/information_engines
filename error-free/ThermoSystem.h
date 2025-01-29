#pragma once
#include <cmath>
#include "Random64.h"

//ts=1 mus, kspring=42pN/mum, gamma=3,36x10^-8m²kg/s, g=9,8m²/s, kBT=403,2x10^-23J, m=42 x10^-15kg
//We'll use the following new units: m'=9,8e-9m, s'=0,8e-3s, kg'=42e-15kg 
//---------------------------------------------------------------
const double samp_freq=41;
const double Mass=0.8,ts=1/samp_freq,kbT=640, gama=640, kspring=640, gravity = 640; //ts is the sampling time
const double dt=ts;
const double alpha = 1-exp(-gama*dt), alphap=alpha*(2-alpha);
//---------------------------------------------------------------------------------------------------
class Particle{
  double Pos,Vel,Vhalf, Dv, Work, Measurement, Fex, Pot;
 public:		    
  double get_Pos(){ return Pos; }
  double get_Vel(){ return Vhalf; }
  double get_Pot(){return Pot;}
  void set_Pot(double value){Pot=value;}
  double get_Work(){return Work;}
  
  void Initialize(double Pos0, double Vel0);
  void Launch();//First step in the Gunsteren Berendsen algorithm
  void ThermoEvolution(Crandom &ran64);//evolves the system
  double Protocol(double gain, Crandom &ran64);//returns the difference between measurement and potential
  void CalculateWork(double previous_Pot);
};
//--------------------------------------------------------------------------------------------------
//Method implementation
void Particle::Initialize(double Pos0, double Vel0){
  Pos=Pos0,Vel=Vel0,Work=0,Pot=0;
}

void Particle::Launch(){ //for the fist step to begin half before
  Vhalf=Vel-dt*Fex/(2*Mass);
}

void Particle::ThermoEvolution(Crandom &ran64){
  //one step of thermal evolution

  Fex = -kspring*(Pos-Pot)-gravity*Mass;

  Vel=Vhalf+Fex/Mass*dt;

  Dv=-alpha*Vel+sqrt(alphap*kbT/Mass)*ran64.gauss(0,1);

  Pos=Pos+(Vel+Dv/2)*dt;

  Vhalf=Vel+Dv;
}

double Particle::Protocol(double gain, Crandom &ran64){ 
  Measurement = Pos;
  double diff = Measurement-Pot;
  if(diff>0){ 
    return gain*diff;
  }
  return 0;
}
void Particle::CalculateWork(double previous_Pot){
  Work += kspring/2*(pow(Pos-Pot,2.0)-pow(Pos-previous_Pot,2.0));
}
