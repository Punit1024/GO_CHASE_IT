# ball_chaser
ROS package that processes image published by [my_robot](https://github.com/Punit1024/GO_CHASE_IT_my_robot) and publishes on topic  `\cmd_vel` which commands my_robot to move
contains to nodes
- process_image
- drive_bot

## How to Launch the simulation?


#### Clone the workspace in your prefered location
```sh
$ cd /home/workspace
$ git clone https://github.com/Punit1024/GO_CHASE_IT.git my_robot
```

#### Build the packages
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
$ rosdep install -i my_robot
```

#### Once the package has been built, you can launch the simulation environment using
```sh
$ roslaunch my_robot world.launch
```
 Inside another terminal launch ball_chaser nodes
```sh
$ cd /home/workspace/catkin_ws/
$ source devel/setup.bash
$ roslaunch ball_chaser ball_chaser.launch
```

#### How to command robot ?
There is a model of white ball, outside office building in Gazebo, you can drag the ball around and the robot will chase it, if it's in the range of camera vision


