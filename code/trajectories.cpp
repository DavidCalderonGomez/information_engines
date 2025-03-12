#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;
int total_time=50, sample=samp, evolve=evol;
int steps = sample*evolve*total_time; 
int runs = 2000;

int main(){

  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create a particle
  double gain=1.98284; //how much you rise the potential. 
  double rise;
  int thermal_steps=2*sample*evolve;

  for(int i=0; i< runs; i++){
    particle[i].Initialize(0,0); 
    particle[i].CalculateForce();
    particle[i].Launch();

    for (int j = 0; j < thermal_steps; j++){ //wait 2 relaxation times. 
      particle[i].CalculateForce();
      particle[i].ThermoEvolution(ran64);//evolves the system         
    }
  }

  for(int j=0;j<steps;j++){ 
	//evolve one step each particle:
    if (j%evolve==0){
      for (int i = 0; i < runs; i++){
       //each sampling time the protocol is exeucted 
        rise = particle[i].Protocol(gain,ran64,1.0/64.565);//measurement with some error
        particle[i].set_Pot(particle[i].get_Pot()+rise);//rises the trap immediately with no delation time
        particle[i].CalculateForce();
        particle[i].ThermoEvolution(ran64);  
        cout<<particle[i].get_Pos()<<" "<<particle[i].get_Measurement()<<" ";  
      }
      cout<<endl;
    }else{
      for (int i = 0; i < runs; i++){
        particle[i].CalculateForce();
	      particle[i].ThermoEvolution(ran64);//evolves the system         
      }
    }
  }
  return 0;     
}







