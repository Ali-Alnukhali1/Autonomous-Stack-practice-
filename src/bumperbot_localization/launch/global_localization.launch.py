from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution


def generate_launch_description():

    #values that will be used later
    map_name = LaunchConfiguration("map_name")
    use_sim_time = LaunchConfiguration("use_sim_time")
    lifecycle_nodes = ["map_server"]

    #set arguments and their default value
    map_name_arg = DeclareLaunchArgument(
        "map_name",
        default_value="small_house"
    )

    use_sim_time_arg = DeclareLaunchArgument(
        "use_sim_time",
        default_value="true"
    )

    #construct the path to map settings 
    map_path = PathJoinSubstitution([
        get_package_share_directory("bumperbot_mapping"),
        "maps",
        map_name,
        "map.yaml"
    ])

    #start the navt_map_server node and set its settings
    nav2_map_server = Node(
        package="nav2_map_server",
        executable="map_server",
        name="map_server",
        output="screen",
        parameters=[
            {"yaml_filename": map_path},
            {"use_sim_time": use_sim_time}
        ],
    )

    #start the lifecycle manager and set its settings 
    nav2_lifecycle_manager = Node(
        package="nav2_lifecycle_manager",
        executable="lifecycle_manager",
        name="lifecycle_manager_localization",
        output="screen",
        parameters=[
            {"node_names": lifecycle_nodes},
            {"use_sim_time": use_sim_time},
            {"autostart": True}
        ],
    )

    #launch everything together
    return LaunchDescription([
        map_name_arg,
        use_sim_time_arg,
        nav2_map_server,  
        nav2_lifecycle_manager,
    ])

