# GO_CHASE_IT_my_robot
The Gazebo world package a part of GO_CHASE_IT project - A ball chasing robot in Gazebo using ROS

## How to Launch the simulation?

#### Create a catkin_ws, feel free to skip if you already have one!
```sh
$ cd /home/workspace/
$ mkdir -p /home/workspace/catkin_ws/src/
$ cd catkin_ws/src/
$ catkin_init_workspace
$ cd ..
```

#### Clone the package in under my_robot package catkin_ws/src/
```sh
$ cd /home/workspace/catkin_ws/src/
$ git clone https://github.com/Punit1024/GO_CHASE_IT_my_robot.git my_robot
```

#### Build the `my_robot` package
```sh
$ cd /home/workspace/catkin_ws/ 
$ catkin_make
```

#### After building the package, source your environment
```sh
$ cd /home/workspace/catkin_ws/
$ source devel/setup.bash
```

#### Make sure to check and install any missing dependencies
```sh
$ rosdep install -i simple_arm
```

#### Once the `my_robot` package has been built, you can launch the simulation environment using
```sh
$ roslaunch my_robot world.launch
```

#### How to command robot ?
the skid steer gazebo plugin in the robot subscribses to following topic:
```sh
/cmd_vel
```

## How to view image stream from the camera?
Camera image stream is published to the following topic:
```
/rgb_camera/image_raw
```




