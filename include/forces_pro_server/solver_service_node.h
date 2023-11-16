#ifndef __SOLVER_SERVICE_NODE_H__
#define __SOLVER_SERVICE_NODE_H__

// To switch the solver type ctrl+h my_solver replace with
#include <my_solver.h>
#include <my_solver_memory.h>

#include <forces_pro_server/srv/call_forces_pro.hpp>

#define SOLVER_ADTOOL2FORCES my_solver_adtool2forces
#define SOLVER_EXT_FUNC my_solver_extfunc
#define SOLVER_EXTERNAL_MEM(x, y, z) my_solver_external_mem(x, y, z)
#define SOLVER_GET_MEM_SIZE() my_solver_get_mem_size()
#define SOLVER_SOLVE(x, y, z, a, b, c) my_solver_solve(x, y, z, a, b, c)
extern "C"
{
    extern solver_int32_default my_solver_adtool2forces(my_solver_float *x,       /* primal vars                                         */
                                                                    my_solver_float *y,       /* eq. constraint multiplers                           */
                                                                    my_solver_float *l,       /* ineq. constraint multipliers                        */
                                                                    my_solver_float *p,       /* parameters                                          */
                                                                    my_solver_float *f,       /* objective function (scalar)                         */
                                                                    my_solver_float *nabla_f, /* gradient of objective function                      */
                                                                    my_solver_float *c,       /* dynamics                                            */
                                                                    my_solver_float *nabla_c, /* Jacobian of the dynamics (column major)             */
                                                                    my_solver_float *h,       /* inequality constraints                              */
                                                                    my_solver_float *nabla_h, /* Jacobian of inequality constraints (column major)   */
                                                                    my_solver_float *hess,    /* Hessian (column major)                              */
                                                                    solver_int32_default stage,           /* stage number (0 indexed)                            */
                                                                    solver_int32_default iteration,       /* iteration number of solver                          */
                                                                    solver_int32_default threadID /* Id of caller thread 								   */);
}

struct Solver
{

    // Memory used in solving
    char *solver_memory_;
    my_solver_mem *solver_memory_handle_;

    // Parameters / info / output
    my_solver_params forces_params_;
    my_solver_output forces_output_;
    my_solver_info forces_info_;

    Solver();
    virtual ~Solver();

    int Solve();

    void SolveServiceCallback(
        const std::shared_ptr<forces_pro_server::srv::CallForcesPro::Request> request,
        std::shared_ptr<forces_pro_server::srv::CallForcesPro::Response> response);
};

#endif