#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;
int total_time=100, samp=41;// a hundred of my units of time
int steps = samp*total_time; 
int main(){

  Crandom ran64(1);//create the generator of random numbers
  Particle particle; //create a particle
  double gain=2; //how much you rise the potential. 
  double rise;

    particle.Initialize(0,0); 
    particle.Launch();
    
  // feedback each relaxation time with one step time delay
  //describe Experimental demonstration of information-to-energy conversion and validation of the generalized Jarzynski equality
  //each feedback cycle transition between two equilibirum states. 

  for(int j=1;j<steps-1;j++){ //you put -1 to ensure final state is at equilibrium. 
	//evolve one step each particle:
  if (j%samp==0){ //each relaxation time the protocol is executed.      
      rise = particle.Protocol(gain,ran64);//calculates rise of the potential
      particle.ThermoEvolution(ran64);
      cout<<j/samp_freq<<"  "<<particle.get_Pos()<<" "<<particle.get_Pot()<<endl;  
      particle.set_Pot(particle.get_Pot()+rise);//rises the trap with a time step ddelay.
  }else{         
	    particle.ThermoEvolution(ran64);//evolves the system   
      cout<<j/samp_freq<<"  "<<particle.get_Pos()<<" "<<particle.get_Pot()<<endl;    
    }
  }
  return 0;
}






