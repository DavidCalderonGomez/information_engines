#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;

int ttime = 100; // a hundred of my units of time
int steps = parameter*samp_freq*ttime;
int main(){

  Crandom ran64(1);//create the generator of random numbers
  Particle particle; //create a particle
  double gain=1.63;double SNR=11; 
  double Pos=0,Pot=0,rise;
  double t=0;

    particle.Initialize(0,0); 
    particle.CalculateForce();
    particle.Launch();
    
    for(int j=1;j<steps;j++){
	//evolve one step each particle:
        if(j%parameter==0){ 
            Pot = particle.get_Pot(); //store potential zero
            Pos = particle.get_Pos(); //stores the position before the evolution        
            rise = particle.Protocol(gain,ran64,SNR);//calculates rise of the potential
            particle.set_Pot(Pot+rise);//rises the trap
            
            t+=ts;
            cout<<t<<"  "<<Pos<<" "<<Pot<<endl;
        }	        
      particle.CalculateForce();//calculates all forces
	    particle.ThermoEvolution(ran64);//evolves the system     
    }
  return 0;
}
  





