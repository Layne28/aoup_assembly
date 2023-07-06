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

    /*** Methods ***/

    //constructor (needs to take System, unlike Solver)
    AOUPSolver(System &theSys, ParamDict &theParams, gsl_rng *&the_rg);

    //destructor
    ~AOUPSolver();

    //Take a step forward in time
    void update(System &theSys);

};

#endif