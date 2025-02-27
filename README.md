# Computational verification of the Generalized second law of thermodynamics with feedback control. 

Description of the project

### Random64.h

This is a library used to generate random numbers from a gaussian distributuion. It was made by J D Muñoz. 

### ThermoSystem.h 

This file has the variables and methods used to implemment the Impulsive Langevin Leap-Frog algorithm for systems without constraints described in the paper : Efficient Algorithms for Langevin and DPD dynamics by Goga et al [1]. the same that was used the in final undergraduate work by Julian David. [2] 

The goal is to simulate the system described in the paper Bayesian information engine that optimally exploits noisy measurements, by Saha et al.[3] It is the case of an information engine: a particle is lifted with no input power, just catching the favorable fluctuations. First we have the real units of the problem and the new used units. The new meter is the standard leght of the system, the new second is a relaxation time and the new mass is such that in the new units the mass is 0.8 . 

Important fact: the friction constant that is in units of kg/s happens to be 640 in the new units, althought the used constant in the algorithm has to have units of 1/s, so we divide by the mass. This is the used friction *rate* . 

The defined constants are then, the physical quintites re scaled. 

Very important fact: 

 how to define the time step? This was a question that i did not know the answer for a long time. It appeared to me somehow arbitrary. For that reason i decided to contact the writers of the paper, in fact i wrote an email to J Bechhoeffer to know how they performed the simulations. They used a matlab code and they solved the langevin dynamics using the analytical solution of the process. On the counterpart i am using a different method: brownian dynamics.  
 
 The answer was to test the algorithm: if it works it should reproduce the analytical results for the following two simple situations: Free diffusion and an Ohrnstein Uhlenbeck process. This is later explained in the specific programs that implement that situations. As a preview i can say that the correct steo time was found to be 1/32 times a sampling time. 

The i define the class particle, with the attributes and methods that make a single particle evolve following the overdamped langevin equation. 

Final Remarks: Launch calculates the velocity of a particle half step time before. This is explained in [1]. In the protocol the errors are defined by a noise as 1/nsr where snr is the signal to noise ratio the same way it was defined in [3].

From now on the system is measured at each sampling time and the action of the protocol is applied immediately. In the paper [3] there is some delay, but i decided not to introduce it since it implies memory effects and very complicated correlations when calculating prob dists. 

### /test-algorithm/free-diffusion-variance

 For the free diffusion i want to recalculate the diffusion constant D. The variance of the prob dist should grow linearly with time, the slope is two times the diffusion constant. When the time step is too wide the algorithm predicts a diffusion constant larger than the real one, in this sense i chose the time step when that calculated diffusion constant stabilizes , in fact, very near to the real predefined value. 5000 runs over a time of 1000 units. The linear fit is made using scipylinregress. 

 ### /test-algorithm/free-diffusion-trajectories

It is to graph all those trajectories :P 

### /test-algorithm/free-diffusion-evol. 

Here one can see, as in a heat map, how the delta distribution evolves into a gaussian as the time goes by. Plotting many histograms sequentially. 

 ### /test-algorithm/OU-process-std

 In this case the particles are initialized with a velocity different from zero. The standard deviation of the velocity distribution has to follow an exponential form that approaches to a constant value equal to the sqrt of kBT/m . This program uses a different ThermoSystem library called ThermoSystemTest, whose only difference is that the gamma constant is 0.25, very little on order to be able to see that exponential behavoir as it is also pointed out by Julian in [2]. 20000 runs over a 25 units of time. The goal is to recover the gamma constant and the kBT. The fit is made with scipy.optimize curve_fit. 

### /test-algorithm/OU-process-mean-velocity

Calculates the mean velocity of the Ornstein Uhlenbeck process, shows that it decays exponentially with the friction coefficient gamma. It is just another calculation of gamma

### /test-algorithm/OU-process-trajectories 

This is to graph all those trajectories for this OU process. 

### forward process

This is the case where the measurements are perfect. Protocol is applied every sampling time 1/40 untis of time. The file forward-process has data of time position and potential. total time 100 untis. In the corresponding notebook is the plot and in the legend the input work asociated with this realization. 

### forward-process-error

This is the same as before but now you have some gaussian error with 0 mean and variance 1 that is added to each measurement, scaled by a factor of 1/11. It can be understood that the input work is not zero anymore, in order to be that way one should reduce the gain to some optimal value that is calculated in forward-process-error-optimal.cpp. As a consecuence one expect the particle not to be lifted as fast as the ideal case. 

### forward-process-error-optimal

The program performs for a range of gains the calculation of the average work of 1000 runs for a total time of 100 units. The in the notebook we find the point at which a linear fit crosses zero, meaning that for that particular gain, the engine input work is zero and therefore is the optimal gain. The linear fit is again made with scipy linregress. 

### phase-transition 

Calculates the optimal gain for different values of noise and there is evidence of a phase transtion between a working regime, where the only way of zero input work is not to lift the particle , and a working regime where there is a non zero optimal gain. This is the same as exposed in [3]. The graph shows the inverse of the noise factor, knon as snr, in log scale on the x axis and the respective optimal gain in the y axis. The simulation runs 500 particles over a time of 100 units. It calculates optimal gains as a linear interpolation. This simulation has been so long, since it has to make the forward-process-error-optimal many times for different noise values. We have used 500 runs for the noise being multiplied by 1.1 and the gain resolution of 0.01. 

### output-power

This program calculates the output power, or in other words, the average gain of free energy divided over time, for the optimal gains calculated before in phase transition. It can be see that the output power reaches a max value at a 0.3 kBT/tau_r. In this case the phase transition also makes sense since for the non working regime the output power is simply zero. This results have to be compared with the experimental results of Saha et al [3]. 

### trajectories

This generates the data of position and measurements for many trajectories at a given noise and optimal gain. This is the data we need to calculate the mutual information. 

### mutual_info (python notebook)

This calculates the mutual information average value, using all the data generated by trajectories.cpp Goga
[2] Julian David 
[3] Saha

### mutual info knn 
Calculates the mutual info using the Kozachenko Leonenko entropy estimator. with the expressión i derived myself. 

