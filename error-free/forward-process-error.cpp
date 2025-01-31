#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;
int total_time=100, sample=samp, evolve=evol;
int steps = sample*evolve*total_time; 

int main(){

  Crandom ran64(1);//create the generator of random numbers
  Particle particle; //create a particle
  double gain=1.5, noise=0.1; //how much you rise the potential. 
  double rise, Pot=0;

    particle.Initialize(0,0); 
    particle.CalculateForce();
    particle.Launch();

  // feedback each sampling time with one step time delay
  //describe Experimental demonstration of information-to-energy conversion and validation of the generalized Jarzynski equality

  for(int j=1;j<steps;j++){ //begins in 1 for the initial dist to be canonical
	//evolve one step each particle:
  if (j%evolve==0){ //each sampling time the protocol is exeucted 
      //here's no delation time on the application of the protocol  
      Pot=particle.get_Pot();//stores potential before rising 
      rise = particle.Protocol(gain,ran64,noise);//measurement with some gaussian error
      particle.set_Pot(particle.get_Pot()+rise);//rises the trap immediately with no delation time
      particle.CalculateWork(Pot);//calculates the input work
      
      particle.CalculateForce();
      particle.ThermoEvolution(ran64);  
      cout<<j*dt<<"  "<<particle.get_Pos()<<" "<<particle.get_Pot()<<endl;  
  }else{         
      particle.CalculateForce();
	    particle.ThermoEvolution(ran64);//evolves the system         
      cout<<j*dt<<"  "<<particle.get_Pos()<<" "<<particle.get_Pot()<<endl;    
    }
  }
  cout<<particle.get_Work()<<endl; //the last line is the work 
  return 0;
}






