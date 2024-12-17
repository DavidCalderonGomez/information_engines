#include <iostream>
#include <cmath>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;

int ttime = 100;
int steps = parameter*samp_freq*ttime; 
int runs = 100;

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles  
  double Pos=0, Pot=0, rise;
  double mean_work=0, mean_work_prev=0;
  double optimal_gain=0, delta_gain=0.05;

  //we'll use linear interpolation to calculate various optimal gains 

for(double SNR=0.5;SNR<=50;SNR*=1.25){
  for (double gain = 0; gain <= 2; gain+=delta_gain){ //iterate over different gains
    
    for(int k=0;k<runs;k++){
      particle[k].Initialize(0,0); 
      particle[k].CalculateForce();
      particle[k].Launch();
    } 

    for(int j=1;j<steps;j++){
        for(int k=0;k<runs;k++) {

        if(j%parameter==0){ 
            Pot = particle[k].get_Pot(); //store potential zero
            //Pos = particle[k].get_Pos(); //stores the position before the evolution        
            rise = particle[k].Protocol(gain,ran64,SNR);//calculate the one will be used in the next step
            particle[k].set_Pot(Pot+rise);//change the potential with the result from the previous step
            particle[k].CalculateWork(Pot);//calculates the work with the past potential
            }	        
        particle[k].CalculateForce();//calculates all forces
	      particle[k].ThermoEvolution(ran64);//evolves the system     
        }
    }

    mean_work_prev = mean_work;
    mean_work = 0;

    for(int k=0;k<runs;k++) {
      mean_work+= particle[k].get_Work();
    }

    if(mean_work > 0){
    //the optimal gain is calculated as a linear interpolation
        optimal_gain = gain-delta_gain-mean_work_prev*delta_gain/(mean_work-mean_work_prev);
        cout<<SNR<<" "<<optimal_gain<<endl;
        break;
    } //when you reach zero input work take a value as the optimal gain
    }
}
  return 0;
}