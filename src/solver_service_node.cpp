
#include <forces_pro_server/solver_service_node.h>

#include <rclcpp/rclcpp.hpp>

extern "C"
{
    SOLVER_EXT_FUNC ext_func = &JackalFORCESNLPsolver_adtool2forces;
}

// Solver
Solver::Solver()
{
    // Allocate memory
    solver_memory_ = (char *)malloc(SOLVER_GET_MEM_SIZE());
    solver_memory_handle_ = SOLVER_EXTERNAL_MEM(solver_memory_, 0, SOLVER_GET_MEM_SIZE());
}

Solver::~Solver()
{
    free(solver_memory_);
}

int Solver::Solve()
{
    int exit_code = SOLVER_SOLVE(&forces_params_, &forces_output_, &forces_info_, solver_memory_handle_, stdout, ext_func);
    return exit_code;
}

void Solver::SolveServiceCallback(
    const std::shared_ptr<forces_pro_server::srv::CallForcesPro::Request> request,
    std::shared_ptr<forces_pro_server::srv::CallForcesPro::Response> response)
{
    // Process the request (resize and add data)
    int param_size = 224;
    for (size_t i = 0; i < param_size; i++)
        forces_params_.x0[i] = request->x0.data[i];

    Solve();

    // Fill the response
    size_t j = 0;
    size_t num_var = 7;
    response->output.data.resize(num_var * 6);
    for (size_t i = 0; i < num_var; i++)
        response->output.data[j] = forces_output_.x01[i];
    j += num_var;
    for (size_t i = 0; i < num_var; i++)
        response->output.data[j] = forces_output_.x02[i];
    j += num_var;
    for (size_t i = 0; i < num_var; i++)
        response->output.data[j] = forces_output_.x03[i];
    j += num_var;
    for (size_t i = 0; i < num_var; i++)
        response->output.data[j] = forces_output_.x04[i];
    j += num_var;
    for (size_t i = 0; i < num_var; i++)
        response->output.data[j] = forces_output_.x05[i];
    j += num_var;
    for (size_t i = 0; i < num_var; i++)
        response->output.data[j] = forces_output_.x06[i];
    j += num_var;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto solver = std::make_shared<Solver>(); // The solver

    auto node = rclcpp::Node::make_shared("forces_pro_server");

    auto server = node->create_service<forces_pro_server::srv::CallForcesPro>(
        "call_forces_pro",
        std::bind(&Solver::SolveServiceCallback, solver, std::placeholders::_1, std::placeholders::_2));

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}