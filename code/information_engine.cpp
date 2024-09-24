#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;
const int runs=10;

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles
  double mean_work=0, mean_pot=0;//cumulants
  
  double gain=0;double SNR=0.72;
  
  while(SNR<100){
    while(gain<2.1){
      //initialize the many particles 
      for (int k=0;k<runs;k++) particle[k].Initialize(0,0); 
      //launch all the particles
      for (int k=0;k<runs;k++) particle[k].Launch();
      //Now comes the loop for each step
      for(double j=0;j<tmax;j+=dt){
	//evolve one step each particle:
	for (int k=0;k<runs;k++){
	  double previous_zero = particle[k].get_Potential_zero(); //store potential zero
	  double previous_Pos = particle[k].get_Pos(); //stores the position before the evolution
	  particle[k].CalculateForce();//calculates all forces
	  particle[k].ThermoEvolution(ran64);//evolves the system
	  if(abs(j - round(j)) < 0.0001) //apply the protocol every characteristic time
	    particle[k].Protocol(gain,ran64, previous_Pos, SNR);
	  particle[k].CalculateWork(previous_zero);
	}
      }
      //now you fill the array after the protocol is finished at a given gain and SNR
      for(int k=0;k<runs;k++){
	mean_work+=particle[k].get_Work();
	mean_pot+=particle[k].get_Potential_zero();
      }
      if(mean_work>0){
	cout<<SNR<<" "<<mean_pot*gravity/(runs*tmax)<<endl; //print the mean value metween the two gains that cross the x axis
	mean_work=0;//you have to reset this value even when satisfied the condition
	break;
      }   
      mean_work=0;//reset 
      mean_pot=0;
      gain+=0.01;//update the gain
    }
    gain=0;//you have to reset
    SNR*=1.25;//update the SNR
  }
  return 0;
}
  







