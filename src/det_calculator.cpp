#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "det_calculator/CalcDet.h"

ros::ServiceClient client;
ros::ServiceServer service;
ros::Publisher publisher;
ros::Subscriber subscriber;

bool calcDet(det_calculator::CalcDet::Request &req, det_calculator::CalcDet::Response &res);
void findDetParity(const std_msgs::Float32 det);

int main(int argc, char **argv)
{
  ros::init(argc,argv,"calc_det_server");
  ros::NodeHandle n;
  
  service = n.advertiseService("calc_det", calcDet);
  publisher = n.advertise<std_msgs::Float32>("/det_result", 1000);
  subscriber = n.subscribe("/det_result", 1000, findDetParity);
    
  ROS_INFO("READY TO CALCULATE YOUR DETERMINANT");
  ros::spin();
  return 0;
}


bool calcDet(det_calculator::CalcDet::Request &req, 
         det_calculator::CalcDet::Response &res)
{ 
  // Use Sarrus rule
  int part1 = req.matrix[0] * req.matrix[4] * req.matrix[8];  //a11*a22*a33
  int part2 = req.matrix[1] * req.matrix[5] * req.matrix[6];  //a12*a23*a31
  int part3 = req.matrix[2] * req.matrix[3] * req.matrix[7];  //a13*a21*a32
  
  int part4 = req.matrix[2] * req.matrix[4] * req.matrix[6];  //a13*a22*a31
  int part5 = req.matrix[0] * req.matrix[5] * req.matrix[7];  //a11*a23*a32
  int part6 = req.matrix[1] * req.matrix[3] * req.matrix[8];  //a12*a21*a33
  
  res.det = float(part1 + part2 + part3 - part4 - part5 - part6);
  
  // Send result to topic
  std_msgs::Float32 result;
  result.data = res.det;
  publisher.publish(result);
  return true;
}


void findDetParity(const std_msgs::Float32 det)
{
  if (int(det.data) % 2 == 0)
    ROS_INFO("even");
  else
    ROS_INFO("odd");
}

