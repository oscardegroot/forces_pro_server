#ifndef __SOLVER_SERVICE_NODE_H__
#define __SOLVER_SERVICE_NODE_H__

// To switch the solver type ctrl+h JackalForcesNLPsolver replace with
#include <JackalFORCESNLPsolver.h>
#include <JackalFORCESNLPsolver_memory.h>

#include <forces_pro_server/srv/call_forces_pro.hpp>

#define SOLVER_ADTOOL2FORCES JackalFORCESNLPsolver_adtool2forces
#define SOLVER_EXT_FUNC JackalFORCESNLPsolver_extfunc
#define SOLVER_EXTERNAL_MEM(x, y, z) JackalFORCESNLPsolver_external_mem(x, y, z)
#define SOLVER_GET_MEM_SIZE() JackalFORCESNLPsolver_get_mem_size()
#define SOLVER_SOLVE(x, y, z, a, b, c) JackalFORCESNLPsolver_solve(x, y, z, a, b, c)
extern "C"
{
    extern solver_int32_default JackalFORCESNLPsolver_adtool2forces(JackalFORCESNLPsolver_float *x,       /* primal vars                                         */
                                                                    JackalFORCESNLPsolver_float *y,       /* eq. constraint multiplers                           */
                                                                    JackalFORCESNLPsolver_float *l,       /* ineq. constraint multipliers                        */
                                                                    JackalFORCESNLPsolver_float *p,       /* parameters                                          */
                                                                    JackalFORCESNLPsolver_float *f,       /* objective function (scalar)                         */
                                                                    JackalFORCESNLPsolver_float *nabla_f, /* gradient of objective function                      */
                                                                    JackalFORCESNLPsolver_float *c,       /* dynamics                                            */
                                                                    JackalFORCESNLPsolver_float *nabla_c, /* Jacobian of the dynamics (column major)             */
                                                                    JackalFORCESNLPsolver_float *h,       /* inequality constraints                              */
                                                                    JackalFORCESNLPsolver_float *nabla_h, /* Jacobian of inequality constraints (column major)   */
                                                                    JackalFORCESNLPsolver_float *hess,    /* Hessian (column major)                              */
                                                                    solver_int32_default stage,           /* stage number (0 indexed)                            */
                                                                    solver_int32_default iteration,       /* iteration number of solver                          */
                                                                    solver_int32_default threadID /* Id of caller thread 								   */);
}

struct Solver
{

    // Memory used in solving
    char *solver_memory_;
    JackalFORCESNLPsolver_mem *solver_memory_handle_;

    // Parameters / info / output
    JackalFORCESNLPsolver_params forces_params_;
    JackalFORCESNLPsolver_output forces_output_;
    JackalFORCESNLPsolver_info forces_info_;

    Solver();
    virtual ~Solver();

    int Solve();

    void SolveServiceCallback(
        const std::shared_ptr<forces_pro_server::srv::CallForcesPro::Request> request,
        std::shared_ptr<forces_pro_server::srv::CallForcesPro::Response> response);
};

#endif