#include "AOUParticle.hpp"

AOUParticle::AOUParticle(int dim){
    d = dim;
    //tau = 1.0;
    //D0 = 1.0;
    pos.zeros(dim);
    old_pos.zeros(dim);
    vel.zeros(dim);
    self_prop_vel.zeros(dim);
    id = AOUParticle::counter;
    AOUParticle::counter++;
}

AOUParticle::~AOUParticle() {}

arma::vec AOUParticle::get_self_prop_vel() {
    return self_prop_vel;
}