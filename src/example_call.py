
from forces_pro_server.srv import CallForcesPro
import rclpy
from rclpy.node import Node

# An example of how to call the service
class MinimalClientAsync(Node):

    def __init__(self):
        super().__init__('example_call_node')
        
        self.cli = self.create_client(CallForcesPro, 'call_forces_pro') # Creates the client in ROS2
        
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')
        
        # Sets up the request
        self.req = CallForcesPro.Request()

    def send_request(self):
       
        # We add 0 data for params, xinit and x0 for illustration
        self.req.x0.data = [0.] * 224 # insert here your sizes
        self.req.xinit.data = [0.] * 7 # insert here your sizes
        self.req.params.data = [0.] *5088 # insert here your sizes
        
        # Call the service
        self.future = self.cli.call_async(self.req)
        rclpy.spin_until_future_complete(self, self.future)
        return self.future.result()


def main():
    rclpy.init()

    minimal_client = MinimalClientAsync()
    response = minimal_client.send_request()
    
    # Here we print the result
    for i in range(len(response.output.data)):
        minimal_client.get_logger().info('Output %d: %f' % (i, response.output.data[i]))

    minimal_client.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()