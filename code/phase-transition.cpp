#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;
int total_time=100, sample=samp, evolve=evol;
int steps = sample*evolve*total_time; 
int runs = 500;

int main(){

  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create a particle
  double optimal_gain=0, delta_gain=0.1; //noise ratio 
  double rise, Pot=0;
  double mean_work=0, mean_work_prev=0; //ensemble average 


for(double snr=0.1;snr<=100;snr*=1.5){
  for (double gain = 0; gain <=3; gain+=delta_gain){

    for (int i = 0; i < runs; i++) {
      particle[i].Initialize(0,0); 
      particle[i].CalculateForce();
      particle[i].Launch();
    }

    for(int j=1;j<steps;j++){
    
      for (int i = 0; i < runs; i++){
        if (j%evolve==0){ //each sampling time the protocol is exeucted 
        //here's no delation time on the application of the protocol  
          Pot=particle[i].get_Pot();//stores potential before rising 
          rise = particle[i].Protocol(gain,ran64,1.0/snr);//measurement with some gaussian error
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

    mean_work_prev=mean_work;
    mean_work=0;

    for(int i=0;i<runs;i++) {
      mean_work+= particle[i].get_Work();
    }

    if(mean_work > 0){
    //the optimal gain in this case is calculated as a linear interpolation
    // between two points the last negative and the first positive
      optimal_gain = gain-delta_gain-mean_work_prev*delta_gain/(mean_work-mean_work_prev);
      cout<<snr<<" "<<optimal_gain<<endl;
      break;
    }
  }
}
 return 0;
}