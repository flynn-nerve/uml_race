#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

double range_l;
double range_r;

using namespace std;

void laserScanCallback(const sensor_msgs::LaserScan msg) {
  int size = (msg.ranges.size()) - 1;
  range_l = msg.ranges[size * 8 / 24];
  range_r = msg.ranges[size * 16 / 24];
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "race_controller");
  ros::NodeHandle nh;
  ros::Rate loop_rate(200);

  ros::Publisher pub =
      nh.advertise<geometry_msgs::Twist>("robot/cmd_vel", 1000);
  ros::Subscriber sub =
      nh.subscribe("/robot/base_scan", 1000, laserScanCallback);

  geometry_msgs::Twist cmd;
  cmd.linear.x = 5;
  cmd.angular.z = 0;

  double range_max = 0.5;
  double range_comp = range_max * 2.0;

  while (ros::ok()) {
    cmd.angular.z =
        (range_r < range_max || range_l < range_max)
            ? 0
            : (5.00 * (range_r / range_comp)) - (5.00 * (range_l / range_comp));

    pub.publish(cmd);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}