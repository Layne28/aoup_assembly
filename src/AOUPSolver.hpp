//AOUPSolver is a child of the Solver class.
//It advances the dynamics of a set of AOUPs.

#ifndef AOUPSOLVER_HPP
#define AOUPSOLVER_HPP

#include "Solver.hpp"
#include <complex>
#include <string>
#include <sstream>

class AOUPSolver : Solver
{
public:

    double force_thresh = 200.0;

    /*** Methods ***/

    //constructor (needs to take System, unlike Solver)
    AOUPSolver(System &theSys, ParamDict &theParams, gsl_rng *&the_rg);

    //destructor
    ~AOUPSolver();

    //Take a step forward in time
    void update(System &theSys);
    void update_adaptive(System &theSys, double deet, int level);

    std::vector<arma::vec> get_thermal_forces(System &theSys, double deet);
    std::vector<arma::vec> get_active_forces(System &theSys, double deet);
};

#endif