#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "Random64.h"
#include "ThermoSystem.h"

using std::cout, std::endl;

int total_time = 100, sample=samp, evolve=evol;
int steps = sample*evolve*total_time; 
int runs = 1000;

int main(){
  
  Crandom ran64(1);//create the generator of random numbers
  Particle particle[runs]; //create an array of particles  
  double rise;
  double mean_free=0;

  std::string data = "phase-transition-utf8.txt";
  std::ifstream inputFile(data);

  double snr, gain;
  
  while (inputFile >> snr >> gain){

    for (int i = 0; i < runs; i++) {
      particle[i].Initialize(0,0); 
      particle[i].CalculateForce();
      particle[i].Launch();
    }

  for(int j=1;j<steps;j++){
    
    for (int i = 0; i < runs; i++){
      if (j%evolve==0){ //each sampling time the protocol is exeucted 
      //here's no delation time on the application of the protocol  
        rise = particle[i].Protocol(gain,ran64,1.0/snr);//measurement with some gaussian error
        particle[i].set_Pot(particle[i].get_Pot()+rise);//rises the trap immediately with no delation time
        particle[i].CalculateForce();
        particle[i].ThermoEvolution(ran64);  
      }else{         
        particle[i].CalculateForce();
	      particle[i].ThermoEvolution(ran64);//evolves the system         
      }
    }
  }

    for(int k=0;k<runs;k++){
        mean_free+=particle[k].get_Pot();
    }
    mean_free=mean_free*Mass/(runs*total_time); //average output power
    cout<<snr<<" "<<mean_free<<endl;

    mean_free=0; //reset the value. 
    }
  inputFile.close();
  return 0;
}
  