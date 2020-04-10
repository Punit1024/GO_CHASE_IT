#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;
enum Dirc { None, Center, Left, Right, Reached };
bool reached;

float prev_lx;
float prev_az;
const float LINEAR_PROPOTIONAL = 0.1;
const float ANGULAR_PROPOTIONAL = 0.1;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    //ROS_INFO_STREAM("Transmitting move command");
    if(lin_x!= prev_lx || ang_z != prev_az){
    prev_lx = lin_x;
    prev_az = ang_z;
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // Call the safe_move service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service DriveToTarget");
    // ROS_INFO_STREAM("Transmitted");
     }

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    //ROS_INFO_STREAM("Image Encoding : " + img.encoding);
    //ROS_INFO_STREAM("Image Encoding : " + std::to_string(img.header));
    //ROS_INFO_STREAM("Image height : " + std::to_string(img.height));
    //ROS_INFO_STREAM("Image is_bigendian : " + std::to_string(img.is_bigendian));
    //ROS_INFO_STREAM("Image step : " + std::to_string(img.step));

    Dirc ball_location = None;



    //ROS_INFO_STREAM("in loop");
    float ball_distance = 0;
    float ball_direction = 0;
    int maxvalue = 0;
    int ppb = img.step/img.width;
    int white_pixel = 255*ppb;
    //ROS_INFO_STREAM("coloumn in each row" + std::to_string(ppb));
    //ROS_INFO_STREAM("center pixel color" + std::to_string(img.data[961200]) + ", "+ std::to_string(img.data[961201]) + ", " + std::to_string(img.data[961202]));
    int leftEdge = img.step;
    int rightEdge = 0;

   // ros::Time begin = ros::Time::now();
    for (int i = 0; i < img.step; i = i+ppb) {
        for (int j = 0; j < img.height; j++) {
       // ROS_INFO_STREAM("working...");

        int datatotal =0;
        for(int bt = 0;bt < ppb;bt++){
        datatotal = datatotal + img.data[(img.step*j) + i + bt ];
        if(maxvalue < img.data[(img.step*j) + i + bt ])
        maxvalue = img.data[(img.step*j) + i + bt];
        }


        if (datatotal == white_pixel) {
            //ROS_INFO_STREAM("ball is found");
            if(i < leftEdge)leftEdge = i;
            if(i > rightEdge)rightEdge = i;
        }

          /* if(i < img.step/3){
            //ROS_INFO_STREAM("ball is on Left");
            ball_location = Left;
            }
            else if(i > 0.66*img.step){
            //ROS_INFO_STREAM("ball is on Right");
            ball_location = Right;
            }
            else{
            //ROS_INFO_STREAM("Ball is in center");
            ball_location = Center;

            }
            break;*/

    }
    }
   //ROS_INFO_STREAM("max value : " + std::to_string(maxvalue));
    //ROS_INFO_STREAM("out loop");
   // ros::Time end = ros::Time::now();
    //ROS_INFO_STREAM("out loop" + std::to_string(begin.to_nsec()-end.to_nsec()));

     //ROS_INFO_STREAM("Left Edge : " + std::to_string(leftEdge) + " Right Edge : " + std::to_string(rightEdge));

    if(leftEdge != img.step&& rightEdge != 0){
            int ball_size = rightEdge - leftEdge;
            int ball_center = (leftEdge+rightEdge)/2;
            if(ball_size > 0.7*img.step){
	    
            ROS_INFO_STREAM_COND(!reached,"REACHED");
            ball_location = Reached;
	    reached = true;

            }/*
            else{
            ball_distance = ((float)1)/((float)ball_size);
            ball_direction =  (img.step/2) - ball_center;
            drive_robot(ball_distance*LINEAR_PROPOTIONAL,ball_direction*ANGULAR_PROPOTIONAL);




            }*/
            else if(ball_center < img.step*0.45){
            //ROS_INFO_STREAM("ball is on Left");
            ball_location = Left;
	    reached = false;
            }
            else if(ball_center > 0.55*img.step){
            //ROS_INFO_STREAM("ball is on Right");
            ball_location = Right;
	    reached = false;
            }
            else{
           // ROS_INFO_STREAM("Ball is in center");
            ball_location = Center;
	    reached = false;

            }
    }
    else{
    drive_robot(0,0);

            }


    //ROS_INFO_STREAM("image width : " + std::to_string(ball_location));
    switch(ball_location)
    {
    case None  :
     drive_robot(0,0);
    break;

    case Center:
    drive_robot(0.45,0);
    break;

    case Left:
    drive_robot(0.0,0.4);
    break;


    case Right :
    drive_robot(0.0,-0.4);
    break;

    case Reached :
    drive_robot(0,0);
    break;

    default:
    drive_robot(0,0);
    break;
    }
    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera


    // Wait 10 seconds for arm to settle
    //ros::Duration(1).sleep();

}

int main(int argc, char** argv)
{

    reached = false;	
    prev_lx = 0;
    prev_az = 0;
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/DriveToTarget");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 3, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
