#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;

int ttime = 100;
int steps = parameter*samp_freq*ttime; 
int runs = 10;

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles  
  double Pos=0, Pot=0, rise;
  double mean_free=0;

  std::string data = "/home/david/information_engines/data/many_optimal_gain.dat";
  std::ifstream inputFile(data);
  double SNR, gain;
  
    while (inputFile >> SNR >> gain){

    for(int k=0;k<runs;k++){
      particle[k].Initialize(0,0); 
      particle[k].CalculateForce();
      particle[k].Launch();
    } 
    
    for(int j=1;j<steps;j++){
        for(int k=0;k<runs;k++) {//evolve one step each particle:
            if(j%parameter==0){ 
                Pot = particle[k].get_Pot(); //store potential zero
                //Pos = particle[k].get_Pos(); //stores the position before the evolution        
                rise = particle[k].Protocol(gain,ran64,SNR);//calculate the one will be used in the next step
                particle[k].set_Pot(Pot+rise);//change the potential with the result from the previous step
            }	        
            particle[k].CalculateForce();//calculates all forces
	        particle[k].ThermoEvolution(ran64);//evolves the system     
        }
    }

    for(int k=0;k<runs;k++){
        mean_free+=particle[k].get_Pot();
    }
    mean_free=mean_free*Mass*gravity/(runs*samp_freq*ttime); //average free energy
    cout<<SNR<<" "<<gain<<" "<<mean_free<<endl;

    mean_free=0; //reset the value. 
    }
  inputFile.close();
  return 0;
}
  