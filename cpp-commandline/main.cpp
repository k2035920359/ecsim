#include "../source/simulation.h"
#include <stdlib.h>

int main()
{
    // simulation object:
    Simulation* g_sim = new Simulation(std::cout);
    
    // setup CV experiment:
    g_sim->exper->setVertexPotentials({-1.0, 1.0});
    g_sim->exper->setConditioningTime(0.0);
    g_sim->exper->setConditioningPotential(0.0);
    g_sim->exper->setEquilibration(0.0);
    g_sim->exper->setInitialPotential(0.0);
    g_sim->exper->setFinalPotential(0.0);
    g_sim->exper->setVertexDelay(0.0);
    g_sim->exper->setScanRate(0.1);
    g_sim->exper->setNumCycles(1);
    
    // set up environment:
    g_sim->env->setTemperature(293.15); // [K]
    
    // OPTIONAL: set advanced settings:
    //g_sim->setGridSizing(_gamma, _minF, _maxF, _minlograte, _maxlograte);
    //g_sim->setPotentialSizing(_deltatheta);
    //g_sim->setDifferentialOrders(static_cast<size_t>(_numcurr), static_cast<size_t>(_numderiv));
    
    // define chemical species (name, initial concentration [mol/m3], diffusion coefficient [m2/s]):
    Species* spec_ox   = new Species("ox", 1.0, 1.0e-9);
    Species* spec_red  = new Species("red", 0.0, 1.0e-9);
    Species* spec_prod = new Species("product", 0.0, 1.0e-9);
    
    // add chemical species to the system:
    g_sim->sys->addSpecies(spec_ox);
    g_sim->sys->addSpecies(spec_red);
    g_sim->sys->addSpecies(spec_prod);
    // a pointer to the species can now be obtained by name:
    //Species* spec_red_new = g_sim->sys->getSpeciesByName("red");
    
    // add redox steps (oxidized species, reduced species,
    //                  number of electrons, standard potential [V],
    //                  ke [m/s], alpha [-], unused bool)):
    Redox* redox = new Redox(spec_ox, spec_red, 1, -0.5, 1.0, 0.5, false);
    redox->enabled = true;
    g_sim->sys->addRedox(redox);
    
    // add homogeneous reactions of the form A+B <-> C+D (A, B, C, D, k_f, k_b)
    Reaction* rxn = new Reaction(spec_red, nullptr, spec_prod, nullptr, // pass nullptr for "no species"
                                 10.0, 0.0); // forward, backward rate constant
    rxn->enabled = true;
    g_sim->sys->addReaction(rxn);

    // set up electrode (type and geometry):
    g_sim->el->setType(0); // 0=disk/1=square/2=rectangle/3=cylinder/4=sphere/5=hemisphere
    g_sim->el->setGeom1(1.0e-3); // main size (radius, width)
    g_sim->el->setGeom2(0.0); // (height of rectangle, length of cylinder)

    // run simulation:
    std::vector<double> current, potential;
    size_t sz = g_sim->run(current, potential);
    
    // voltammogram stored in current/potential vectors
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "V [V]:   I [uA]:" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << potential[i] << "     " << current[i]*1.0e6 << std::endl;
    }
    std::cout << ".....   ....." << std::endl;

    return 0;
}