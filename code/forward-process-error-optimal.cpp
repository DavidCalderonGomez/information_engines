#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;
int total_time=100, sample=samp, evolve=evol;
int steps = sample*evolve*total_time; 
int runs = 5000;

int main(){

  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create a particle
  double noise=1.0/11.0; //noise ratio 
  double rise, Pot=0;
  double mean_work=0; //ensemble average 
  int thermal_steps=2*evol*samp; //2 relaxation times

  for (double gain = 1.56; gain <=1.58; gain+=0.001){

    for (int i = 0; i < runs; i++) {
      particle[i].Initialize(0,0); 
      particle[i].CalculateForce();
      particle[i].Launch();
    }
  for(int j=0;j<steps;j++){
    
    for (int i = 0; i < runs; i++){
      if(j<thermal_steps) continue; //thermalization for two relaxation times
      if (j%evolve==0){ //each sampling time the protocol is exeucted 
      //here's no delation time on the application of the protocol  
        Pot=particle[i].get_Pot();//stores potential before rising 
        rise = particle[i].Protocol(gain,ran64,noise);//measurement with some gaussian error
        particle[i].set_Pot(particle[i].get_Pot()+rise);//rises the trap immediately with no delation time
        particle[i].CalculateWork(Pot);//calculates the input work
      
        particle[i].CalculateForce();
        particle[i].ThermoEvolution(ran64);  
      }else{         
        particle[i].CalculateForce();
	      particle[i].ThermoEvolution(ran64);//evolves the system         
      }
    }
  }
   
    for(int i=0;i<runs;i++) {
      mean_work+= particle[i].get_Work();
    }
    cout<<gain<<"   "<<mean_work/runs<<endl;
    mean_work=0;
  }
 return 0;
}






