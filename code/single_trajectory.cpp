#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;

int steps = parameter*samp_freq*100; 

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle; //create an array of particles  
  double gain=1.5;double SNR=11;
  double up=0, previous_Pos=0,previous_Pot=0;  
  
    particle.Initialize(0,0); 
    particle.Launch();
    
    for(int j=1;j<steps;j++){
	//evolve one step each particle:
        if(j%parameter==0){ //apply the protocol every sampling time	
            previous_Pot = particle.get_Potential_zero(); //store potential zero
            previous_Pos = particle.get_Pos(); //stores the position before the evolution        
            up = particle.Protocol(gain,ran64, previous_Pos, SNR);
            particle.set_Potential_zero(previous_Pot+up);
        }	        
        particle.CalculateForce();//calculates all forces
	    particle.ThermoEvolution(ran64);//evolves the system     
    }
  return 0;
}
  





