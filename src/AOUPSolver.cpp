#include "AOUPSolver.hpp"

AOUPSolver::AOUPSolver(System &theSys, ParamDict &theParams, gsl_rng *&the_rg) : Solver(theParams, the_rg) {}

AOUPSolver::~AOUPSolver() {}

void AOUPSolver::update(System &theSys)
{
    //Get conservative forces
    std::vector<arma::vec> potential_forces = theSys.get_forces();
    
    for (int i=0; i<theSys.N; i++) {
        theSys.particles[i].old_pos = theSys.particles[i].pos;
        arma::vec pos = theSys.particles[i].get_pos();
        for (int k=0; k<theSys.dim; k++) {
            double p = theSys.particles[i].self_prop_vel[k];
            if (fabs(potential_forces[i][k])>10000.0) std::cout << "Warning: large lj force: " << potential_forces[i][k] << std::endl;
            //Euler step
            double incr = potential_forces[i](k)/gamma*dt;
            if (theSys.D0>0){
                //Increment position according to self-propulsion velocity
                incr += p/gamma*dt; 
                //Then update the self-propulsion velocity according to OU process
                theSys.particles[i].self_prop_vel[k] += (-p/theSys.tau)*dt + sqrt(2*theSys.D0)/theSys.tau*gsl_ran_gaussian(rg, sqrt(dt));
            }
            if (theSys.kT>0) incr += sqrt(2*theSys.kT/gamma)*gsl_ran_gaussian(rg, sqrt(dt)); 
            pos(k) += incr; 
                   
            theSys.particles[i].pos[k] = pos(k);
            theSys.particles[i].vel[k] = incr/dt;
        }
    }
    theSys.apply_pbc();
    theSys.time++;
    if (theSys.do_neighbor_grid) theSys.update_neighborgrid();
}

void AOUPSolver::update_adaptive(System &theSys, double deet, int level){

    //Get conservative forces
    std::vector<arma::vec> potential_forces = theSys.get_forces();

    //Get active noise force on each particle
    std::vector<arma::vec> active_forces;
    active_forces = get_active_forces(theSys, deet);

    //Get thermal force on each particle
    std::vector<arma::vec> thermal_forces;
    thermal_forces = get_thermal_forces(theSys, deet);
    
    //Compute particle motion due to forces
    std::vector<arma::vec> incr(theSys.N);
    for (int i=0; i<theSys.N; i++) {
        arma::vec v(theSys.dim,arma::fill::zeros);
        incr[i] = v;
    }
    for(int i=0; i<theSys.N; i++){
        incr[i] = potential_forces[i]/gamma*dt 
                    + active_forces[i]
                    + thermal_forces[i];   
    }

    //Check whether the new position will result in a really large force
    for(int i=0; i<theSys.N; i++){
        theSys.particles[i].old_pos = theSys.particles[i].pos;
        theSys.particles[i].pos += incr[i];
    }
    theSys.apply_pbc();
    std::vector<arma::vec> new_forces = theSys.get_forces();
    double max_force = 0;
    for(int i=0; i<theSys.N; i++){
        for(int k=0; k<theSys.dim; k++){
            if(fabs(new_forces[i][k])>max_force) max_force = new_forces[i][k];
        }
    }
    //only decrease time step if force is above threshold
    //and timestep is not already tiny
    if(max_force > force_thresh && deet>1e-6){
        std::cout << "Force too high. Decreasing time step by a factor of 4 (now =" << deet/4 << ")." << std::endl;
        //revert to old position
        for(int i=0; i<theSys.N; i++){
            theSys.particles[i].pos = theSys.particles[i].old_pos;
        }
        for(int k=0; k<4; k++){
            update_adaptive(theSys, deet/4, level+1);
        }
    }
    else{
        for(int i=0; i<theSys.N; i++){
            theSys.particles[i].vel = incr[i]/deet;
            //Advance OU process in time
            for(int k=0; k<theSys.dim; k++){
                theSys.particles[i].self_prop_vel[k] += (-theSys.particles[i].self_prop_vel[k]/theSys.tau)*deet + sqrt(2*theSys.D0)/theSys.tau*gsl_ran_gaussian(rg, sqrt(deet));
            }
        }
        theSys.apply_pbc();
        if (theSys.do_neighbor_grid) theSys.update_neighborgrid();    
    }

    if(level==0) theSys.time++;

}

std::vector<arma::vec> AOUPSolver::get_thermal_forces(System &theSys, double deet){
    
    std::vector<arma::vec> thermal_forces(theSys.N);

    //Initialize to zero
    for (int i=0; i<theSys.N; i++) {
        arma::vec v(theSys.dim,arma::fill::zeros);
        thermal_forces[i] = v;
    }

    //if temperature is zero, don't bother doing calculation
    if (theSys.kT<1e-10) {
        //std::cout << "kT is zero." << std::endl;
        return thermal_forces;
    }

    for (int i=0; i<theSys.N; i++) {
        for (int k=0; k<theSys.dim; k++) {
            thermal_forces[i][k] = sqrt(2*theSys.kT/gamma)*gsl_ran_gaussian(rg, sqrt(deet)); 
        }
    }

    return thermal_forces;
}

std::vector<arma::vec> AOUPSolver::get_active_forces(System &theSys, double deet){
    
    std::vector<arma::vec> active_forces(theSys.N);

    //Initialize to zero
    for (int i=0; i<theSys.N; i++) {
        arma::vec v(theSys.dim,arma::fill::zeros);
        active_forces[i] = v;
    }

    //if OU diffusion constant is zero, don't bother doing calculation
    if (theSys.D0<1e-10) {
        return active_forces;
    }

    for (int i=0; i<theSys.N; i++) {
        for (int k=0; k<theSys.dim; k++) {
            active_forces[i][k] = theSys.particles[i].self_prop_vel[k]/gamma*deet;
        }
    }

    return active_forces;
}