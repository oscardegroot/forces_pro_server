This repository contains a small node to run a Forces Pro solver inside a ROS2 service. This service can be used to program an MPC in ROS2 python that Forces Pro does not support.

## Usage
To adapt the service node to your example:

- Put your solver in `solver/<your_solver>ForcesNLPsolver`. 
- In `CMakelists.txt` change `set(SYSTEM_TO_USE "Jackal")` to `set(SYSTEM_TO_USE "<your_solver>")`.
- In `include/solver_service_node.h` replace (ctrl+h) `my_solver` with `<your_solver>`, same in `src/solver_service_node.h`.

The example of how to call the service in Python is in `src/example_call.py`.

Note: Only the first 6 outputs are now loaded in the service (`solver_service_node.cpp`) because the forces outputs depend on the solver horizon. You can change it for your application.

## Testing
In one terminal
```
ros2 launch forces_pro_server launch_server.launch.xml
```

In a second terminal
```
python3 src/example_call.py
```

Zero outputs should be received (the test configures all inputs as zero).
