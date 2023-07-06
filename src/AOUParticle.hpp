//Defines an AOUP (has a self-propulsion velocity, p)

#ifndef AOUPARTICLE_HPP
#define AOUPARTICLE_HPP

#include "Particle.hpp"
#include <armadillo>

class AOUParticle : public Particle {
public:
    arma::vec self_prop_vel;
    //double tau; //self propulsion persistence time (Now a property of System, not sure if I will revert)
    //double D0; //diffusion constant of free particle (Now a property of System, not sure if I will revert)

    //constructor
    AOUParticle(int dim=2);

    //destructor
    ~AOUParticle();

    arma::vec get_self_prop_vel();
};

#endif