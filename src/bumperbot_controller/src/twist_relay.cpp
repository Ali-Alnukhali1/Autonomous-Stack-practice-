#include"rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"

class TwistRelayNode : public rclcpp::Node {
public:

    TwistRelayNode() : Node("twist_relay"){
        
        // --- CONTROLLER ---
        controller_sub_ = create_subscription<geometry_msgs::msg::Twist>(
            "/bumperbot_controller/cmd_vel_unstamped",
            10,
            std::bind(&TwistRelayNode::controller_twist_callback,this,std::placeholders::_1)
        );
        controller_pub_ = this->create_publisher<geometry_msgs::msg::TwistStamped>(
            "/bumperbot_controller/cmd_vel", 10);
        
        // --- JOYSTICK ---
        joy_sub_ = create_subscription<geometry_msgs::msg::TwistStamped>(
            "/input_joy/cmd_vel_unstamped",
            10,
            std::bind(&TwistRelayNode::joy_twist_callback,this,std::placeholders::_1)
        );
        joy_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(
            "/input_joy/cmd_vel", 10);

        // --- TELEOP KEYBOARD ---
        teleop_sub_ = create_subscription<geometry_msgs::msg::Twist>(
            "/key_vel",
            10,
            std::bind(&TwistRelayNode::teleop_twist_callback, this, std::placeholders::_1)
        );

    } 
private:

    void controller_twist_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
        geometry_msgs::msg::TwistStamped twist_stamped;
        twist_stamped.header.stamp = get_clock()->now();
        twist_stamped.twist = *msg;
        controller_pub_->publish(twist_stamped);
    }
    void joy_twist_callback(const geometry_msgs::msg::TwistStamped::SharedPtr msg)
    {
        geometry_msgs::msg::Twist twist;
        twist = msg->twist;
        joy_pub_->publish(twist);
    }

     void teleop_twist_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        geometry_msgs::msg::TwistStamped stamped_msg;
        stamped_msg.header.stamp = this->get_clock()->now();
        stamped_msg.header.frame_id = "base_link";
        stamped_msg.twist = *msg;

        // Publish to the same topic used by the controller
        controller_pub_->publish(stamped_msg);
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr controller_sub_;
    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr controller_pub_;

    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr joy_sub_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr joy_pub_;

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr teleop_sub_;

  
};

int main(int argc, char const *argv[])
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<TwistRelayNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
