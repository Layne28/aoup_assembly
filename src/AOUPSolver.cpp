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
