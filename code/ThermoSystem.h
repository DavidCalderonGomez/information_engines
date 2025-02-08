#pragma once
#include <cmath>
#include "Random64.h"

//ts=1 mus, kspring=42pN/mum, gamma=3,36x10^-8m²kg/s, g=9,8m²/s, kBT=403,2x10^-23J, m=42 x10^-15kg
//We'll use the following new units: m'=9,8e-9m, s'=0,8e-3s, kg'=42e-15kg 

//sampling frequency is 40 times per relaxation time 
//---------------------------------------------------------------------------------------------------
const double samp = 40, evol=32; //sampling time and the number of evolutions each sampling time
const double Mass=0.8,kbT=640, gama=640/Mass, kspring=640, gravity = 640; //ts is the sampling time 
const double dt=1/(samp*evol); //that number dividing is found to reproduce the thermostat accurately
const double alpha = 1-exp(-gama*dt), alphap=alpha*(2-alpha);
//---------------------------------------------------------------------------------------------------
class Particle{
  double Pos,Vel,Vhalf, Dv, Work, Measurement, Fex, Pot, noise;
 public:		    
  double get_Pos(){ return Pos; }
  double get_Measurement(){return Measurement;}
  double get_Vel(){ return Vhalf; } //returns the velocity at half step times 
  double get_Pot(){return Pot;}
  void set_Pot(double value){Pot=value;}
  double get_Work(){return Work;}
  
  void Initialize(double Pos0, double Vel0);
  void Launch();//velocity half step before 
  void CalculateForce(); //Calculates forces. 
  void ThermoEvolution(Crandom &ran64);//evolves the system
  double Protocol(double gain, Crandom &ran64, double noise);//returns the difference between measurement and potential
  void CalculateWork(double previous_Pot);//calculates the input work 
};
//--------------------------------------------------------------------------------------------------
//Method implementation
void Particle::Initialize(double Pos0, double Vel0){
  Pos=Pos0,Vel=Vel0,Work=0,Pot=0,Fex=0;
}

void Particle::Launch(){ //for the fist step to begin half before
  Vhalf=Vel-dt*Fex/(2*Mass);
}
void Particle::CalculateForce(){
  Fex = -kspring*(Pos-Pot)-gravity*Mass;
}
void Particle::ThermoEvolution(Crandom &ran64){
  //one step of thermal evolution
  Vel=Vhalf+Fex/Mass*dt;
  Dv=-alpha*Vel+sqrt(alphap*kbT/Mass)*ran64.gauss(0,1);
  Pos=Pos+(Vel+Dv/2)*dt;
  Vhalf=Vel+Dv;
}

double Particle::Protocol(double gain, Crandom &ran64, double noise){ 
  Measurement = Pos+ran64.gauss(0,1)*noise; // not signal to noise ratio but the noise the characteristic scale is in fact 1
  double diff = Measurement-Pot;
  if(diff>0){ 
    return gain*diff;
  }
  return 0;
}
void Particle::CalculateWork(double previous_Pot){
  Work += kspring/2*(pow(Pos-Pot,2.0)-pow(Pos-previous_Pot,2.0));
}
