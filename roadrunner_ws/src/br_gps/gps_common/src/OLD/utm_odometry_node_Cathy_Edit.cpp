#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/NavSatStatus.h>
#include <sensor_msgs/NavSatFix.h>
#include <gps_common/conversions.h>
#include <nav_msgs/Odometry.h>
#include <sound_play/sound_play.h>
#include <std_msgs/String.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sensor_msgs/LaserScan.h>
#include <laser_sub/lasArray.h>

//ADDED
#include <cmvision/Blobs.h>
#include <sensor_msgs/Image.h>


using namespace gps_common;
using namespace std;

static ros::Publisher Drive_Motor; //publish to drive
static ros::Publisher Steer_Motor; //publish to steer
int MAX_POINTS; // Amount of points + start - set in SetGPSPoints()
int GPSCounter = 0; // Counter of point we are at in array
string PsocTopic = "drive_tx"; //Drive Topic
string PsocSteer = "steer_tx"; //Steer Topic
int prevx = 0; //Old GPS Points Latitude
int prevy = 0; //Old GPS Poitns Longitude
int Shift = 100000/2; // Shift points to integers
int CurrentX = 0; // Current Latitude
int CurrentY = 0; // Current Longitude

// Sleep for the sounds
void sleepok(int t, ros::NodeHandle &nh)
{
  if (nh.ok())
		sleep(t);
}

//GPS List Structure
struct GPSList 
{
	long double longitude;
	long double latitude;
};
// Vector for the GPS List Structure 
vector<GPSList> GPSPoints;
vector<GPSList> NEWESTgpsPoints; // IF a new gps point is given, go here first!

//Convert GPS Points into a large grid.
//Origin(0,0) our starting point.
//Every .0001 is a new grid point.
struct MassiveFuckingGrid
{
	int x;
	int y;
	long double originX;
	long double originY;
	bool visited;
};
//Vectors
vector<MassiveFuckingGrid> GPSGrid; //Holds the grid

//Declare Functions
std::pair< double, double> getHeading( double Currlatitude, double Currlongitude);
//Course Numbers:
//	1-Qualification
//	2-Practice
//	3-Basic
//	4-Advanced(Currently not programmed)
void SetGPSPoints(int course);
void GPS(const sensor_msgs::NavSatFixConstPtr& fix); // Main gps calculation function (callback)
void Laser(const laser_sub::lasArray& array); //laser callback
std::pair<int,int> ConvertToGrid(long double longitude, long double latitude); //converts to integer grid
void SpeedUp(); //Speeds up the golf cart so we do not stall the motor
void Turn(string TurnAngle); //Sets speed to turn at
void Camera(const cmvision::Blobs& blobs);

int main(int argc, char **argv)
{
	//Course Numbers:
	//	1-Qualification
	//	2-Practice
	//	3-Basic
	//	4-Advanced(Currently not programmed)
	//	5-Other Test Course
	SetGPSPoints(5);

	ros::init(argc,argv,"utm_odometry_node");
	ros::NodeHandle node;
	//advertise topics
	Drive_Motor = node.advertise<std_msgs::String>(PsocTopic,1);
	Steer_Motor = node.advertise<std_msgs::String>(PsocSteer,1);
	//subscribe topics
	ros::Subscriber fix_sub = node.subscribe("fix",10,GPS);
	ros::Subscriber Laser_Array = node.subscribe("array",10,Laser);
	ros::Subscriber camera_sub = node.subscribe("blobs",10,Camera);
	ros::spin();
	return 0;
}

//Takes in a latitude(x) and longitude(y) and turns them into gridpoints on an x y grid (integers only)
std::pair<int,int> ConvertToGrid(long double x, long double y)
{
	int CurrPointx;
	int CurrPointy;
	int Newx;
	int Newy;
	//origin
	if(x == 0 && y == 0)
	{
		GPSGrid.push_back(MassiveFuckingGrid());
		int starty = GPSPoints[MAX_POINTS-1].longitude*Shift;
		int startx = GPSPoints[MAX_POINTS-1].latitude *Shift;
		GPSGrid[0].originX = startx;
		GPSGrid[0].originY = starty;
		GPSGrid[0].x = 0;
		GPSGrid[0].y = 0;
		for(int i=0;i<MAX_POINTS;++i)
		{	
			GPSGrid.push_back(MassiveFuckingGrid());
			CurrPointy = GPSPoints[i].longitude*Shift;
			CurrPointx = GPSPoints[i].latitude*Shift;
			GPSGrid[i].x = GPSGrid[0].originX - CurrPointx;
			GPSGrid[i].y = GPSGrid[0].originY - CurrPointy;
			std::cout << "Point" << i << "_x: " << GPSGrid[i].x << " Point" << i << "_y: " << GPSGrid[i].y << std::endl;
		}
		return make_pair(0,0);
	}
	else
	{
		GPSGrid.push_back(MassiveFuckingGrid());
		Newx = GPSGrid[GPSGrid.size()].x = (GPSGrid[0].originX - (x*Shift));
		Newy = GPSGrid[GPSGrid.size()].y = (GPSGrid[0].originY - (y*Shift));
	}
	return make_pair(Newx,Newy);
}

void SetGPSPoints(int course)
{
	std::cout << "Setting Up Points for: ";

	if(course == 1)//Qualification Points
	{
		std::cout << "Qualification Course " << std::endl;
		//First GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[0].latitude = 42.67796895;
		GPSPoints[0].longitude = -83.19536504166666;

		//Second GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[1].latitude = 42.678212847222;
		GPSPoints[1].longitude = -83.19546280277778;
	
		//Set max Points
		MAX_POINTS = 3;
	}

	if(course == 2)//Practice Points
	{
		std::cout << "Practice Course " << std::endl;
		//First GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[0].latitude =42.678212847222;
		GPSPoints[0].longitude = -83.1950224194444;

		//Second GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[1].latitude = 42.67832571666667;
		GPSPoints[1].longitude = -83.19502294722223;
	
		//Third GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[2].latitude = 42.678305472222222;
		GPSPoints[2].longitude = -83.1948564861111;
	
		//Fourth GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[3].latitude = 42.678227622222222;
		GPSPoints[3].longitude = -83.1948651111111;
	
		//Set Max Points
		MAX_POINTS = 5;
	}

	if(course == 3)//Basic Points
	{
		std::cout << "Basic Course " << std::endl;
		//First GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[0].latitude = 42.6788439111111111;
		GPSPoints[0].longitude = -83.19496646388889;

		//Second GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[1].latitude = 42.678986258333333;
		GPSPoints[1].longitude = -83.1949690805555;
		
		//Set Max Points
		MAX_POINTS = 3;
	}

	if(course == 4)//Advanced Points
	{
		std::cout << "Advanced Course " << std::endl;
		//There are 7 points for future refererence
		
		//Set Max Points
		MAX_POINTS = 8;
	}
	
	if(course == 5)//randomly made up points from the tent
	{
		std::cout << "Tent Course " << std::endl;
		
		//Second GPS Point
		GPSPoints.push_back(GPSList());
		GPSPoints[0].latitude = 42.677964143;
		GPSPoints[0].longitude = -83.195377297;
	
		//Set Max Points
		MAX_POINTS = 2;
	}
}

void GPS(const sensor_msgs::NavSatFixConstPtr& fix)
{

	//Play Sound
	ros::NodeHandle nh;
	sound_play::SoundClient sc;

	//Information sent to drive and steer motor
	std_msgs::String GPSDriving;
	std_msgs::String GPSSteering;

	//Counter for initial data
	static int Count = 0;

	//The reading of Latitude and Longitude from /fix
	//Stored in variables to not have to make calls every time
 	long double Latitude = fix->latitude;
        long double Longitude = fix->longitude;
	int GridLat;
	int GridLong;

	//Set Up Slopes
	double slope_one = 0;
	double slope_two = 0;
	double turnangle = 0;
	//IF laser sees an obsticle in its path, set Latitude and Longitude to new points
	/*if(NEWESTgpsPoints.size() != 0)
	{
		Latitude = NEWESTgpsPoints[0].latitude;
		Longitude = NEWESTgpsPoints[0].longitude;
		NEWESTgpsPoints.pop_back();
	}*/

	std::pair<int,int> latlong;	
	latlong = ConvertToGrid(Latitude,Longitude);
	GridLat = latlong.first;
	GridLong = latlong.second;
	long double Turn_Angle;
	static int i = 0;
	int NewPointx = 0;
	int NewPointy = 0;
	int SlopeCalcy = 0;
	int SlopeCalcx = 0;
	//This will set the Start latitude and longitude.
	if(Count < 2)
	{
		//Set up finishing longitude and latitude
		Count++;
		GPSPoints[MAX_POINTS-1].latitude = Latitude;
		GPSPoints[MAX_POINTS-1].longitude = Longitude;
		if(Count == 2){
			GPSDriving.data = ("a\r");
			Drive_Motor.publish(GPSDriving);
			GPSSteering.data = "T 0 \r";
			Steer_Motor.publish(GPSSteering);
			SpeedUp();//Start Driving
			ConvertToGrid(0,0);
			//sc.playWave("/home/roadrunner/Music/Hands.wav");
			ros::Duration(2.5).sleep(); //sleep for 4 seconds
		}
		
	}
	else
	{ 	

	//////////////////
	// Logic of GPS //
	//////////////////

		if(((GridLat != GPSGrid[GPSCounter].x-1)|| (GridLat != GPSGrid[GPSCounter].x+1) || (GridLat != GPSGrid[GPSCounter].x)) && ((GridLong != GPSGrid[GPSCounter].y-1) || (GridLong != GPSGrid[GPSCounter].y+1) || (GridLong != GPSGrid[GPSCounter].y)))
		{

			if(i % 5 == 0)
			{
				// Slope I am at
				/*if(prevx - GridLat == 0)
				{
					slope_one = 0;
					cout << "Slope_One Is Zero" << std::endl;
				}
				else
					slope_one = (prevy-GridLong)/((prevx-GridLat));
				//Slope object is at
				if(prevx - GPSGrid[GPSCounter].x == 0)
				{
					slope_two = 0;
					cout <<"Slope_Two is Zero" << std::endl;
				}
				else
					slope_two = (prevy-GPSGrid[GPSCounter].y)/((prevx-GPSGrid[GPSCounter].x));
				*/
				slope_one = (prevy-GridLong)/((prevx-GridLat)+0.000000000000000001);
				slope_two = (prevy-GPSGrid[GPSCounter].y)/((prevx-GPSGrid[GPSCounter].x)+0.00000000000000000001);
				//Angle between slopes
			//	if(slope_one == 0 && slope_two == 0)
			//			turnangle = 0;
				//else
				turnangle = atan((slope_one-slope_two)/(1-slope_one*slope_two));
				turnangle = turnangle * 57.2957795;	
				cout<< "Turn Angle Is: " << turnangle << std::endl;	
				//Decide what direction we need to turn
				if((abs(GPSGrid[GPSCounter].y) - abs(GridLong)) > (abs(GPSGrid[GPSCounter].x) - abs(GridLat)))
				{
					if(GPSGrid[GPSCounter].x > GridLat)
						turnangle = (turnangle /*+ 90*/);
					else
						turnangle = (turnangle /*- 90*/);//*-1 ;//Just Switched
				} else {
					if(GPSGrid[GPSCounter].y > GridLong)
						turnangle = (turnangle /*- 90*/) ;
					else
						turnangle = (turnangle /*+ 90*/);//*-1;
				}
				//if the turn angle is any of these... its bullshit
				if(abs(turnangle) > 360 || abs(turnangle) == 135 || abs(turnangle) == 90 || (turnangle > 89.999 && turnangle < 89.99999999) || abs(turnangle) == 45 )
					turnangle = 0;
			/*	if( (turnangle > 1 || turnangle < -1))
				{	
					std::cout << "Turn Angle: " << turnangle << std::endl;
					while(turnangle > 29 || turnangle < -29)
					{
						if(turnangle > 0)
						{
							Turn(boost::to_string(29));
							turnangle = turnangle -29;
						}
						else
						{
							Turn(boost::to_string(-29));
							turnangle = turnangle + 29;
						}	
						ros::Duration(1.5).sleep(); //sleep for 1.5 seconds
					}
					Turn(boost::to_string(turnangle));
					ros::Duration(1.5).sleep(); //Sleep for 1 second per every 10 degrees on the turning motor
					Turn(boost::to_string(0));
					ros::Duration(1.5).sleep();
				}
				SpeedUp();
			*/
			}
			
			std::cout << "AT ( " << GridLat << ", " << GridLong << ") ";
			std::cout << "Going (" << GPSGrid[GPSCounter].x << ", " << GPSGrid[GPSCounter].y << ") ";
			std::cout << "Was At (" << prevx << ", " << prevy << ") " << std::endl;
		}
		i+= 1;
		
		if(GPSGrid[GPSCounter].x == GridLat && GPSGrid[GPSCounter].y == GridLong)
		{
			sc.playWave("/home/roadrunner/Music/BeepBeep.wav");
			GPSGrid[GPSCounter].visited = true;
			if(GPSGrid[MAX_POINTS-1].visited == true)
			{
				sc.playWave("/home/roadrunner/Music/Winning.wav");
				GPSDriving.data = "F 0\r"; //Stop Motor
				Drive_Motor.publish(GPSDriving);
				ros::Duration(1.0).sleep();	
				GPSDriving.data = "h on\r"; //Turn On Break
				Drive_Motor.publish(GPSDriving);
				exit(1); //Force Crash
			}
		}

		//If we just marked the point as visited...We must go to next point
		if(GPSGrid[GPSCounter].visited == true)	{GPSCounter++; i = 0;}
	}
	if(i % 5 == 0 && i != 0)
	{
		prevy = GridLong;
		prevx = GridLat;
	}	

	CurrentX = GridLat;
	CurrentY = GridLong;
	ros::Duration(0.5).sleep();
}

void Laser(const laser_sub::lasArray& array)
{
	int x = 0;
	int y = 0;
	int r = 0;
	std_msgs::String GPSDriving;
	std_msgs::String GPSSteering;
	double slope_one = 0;
	double slope_two = 0;
	for(int i = 0; i < array.laserArray.size();++i)
	{
		if(array.laserArray[i].centerX > 0 && array.laserArray[i].centerY > 0)
		{
			x = prevx + array.laserArray[i].centerX;
			y = prevy + array.laserArray[i].centerY;
			r = array.laserArray[i].radius;
			std::cout << "X: " << x << " Y: " << y << std::endl;
			//Is point in way?
			if(double(prevx) - (double)CurrentX == 0)
				slope_one = 0;
			else
    				slope_one = ((double)prevy - (double)CurrentX)/(((double)prevx - (double)CurrentY));
			if(double(prevx)-double(x) == 0)
				slope_two = 0;
			else
				slope_two = ((double)prevy - (double)y)/(((double)prevx - (double)x));
			double turnangle = atan((slope_one-slope_two)/(1-slope_one*slope_two));
			turnangle = turnangle *  57.2957795;
			std::cout << "Obsticle Turn Angle: " << turnangle << std::endl;
			if(turnangle > -15 && turnangle < 15)
			{
				std::cout << "I am turning at: " << turnangle << std::endl;
				Turn(boost::to_string(10));
				ros::Duration(0.5).sleep();
				Turn(boost::to_string(0));
				ros::Duration(0.5).sleep();
				SpeedUp();
			}
		}

	}

}

void SpeedUp()
{
	//Information sent to drive motor
	std_msgs::String GPSDriving;
	for(int i = 100; i < 120; ++i)
	{
		GPSDriving.data = "F " +  boost::to_string(i) + "\r";
		Drive_Motor.publish(GPSDriving);
		//ros::Duration(0.3).sleep(); //sleep for 3/10 of a second?
	}
}

void Turn(string TurnAngle)
{
	//Information sent to drive and steer motor
	std_msgs::String GPSDriving;
	std_msgs::String GPSSteering;

	if(TurnAngle == "180")
	{	
		//Set Turn Speed to zero
		GPSDriving.data = "F 0\r";
		Drive_Motor.publish(GPSDriving);
		ros::Duration(0.1);//Tiny sleep
		
		//Tell to go backwards
		for(int i = 90; i < 130; ++i)
		{
			GPSDriving.data = "B " +  boost::to_string(i) + "\r";
			Drive_Motor.publish(GPSDriving);
		}

	}
	else
	{
		//Set Turn Speed	
		GPSDriving.data = "F 100\r";
		Drive_Motor.publish(GPSDriving);
		ros::Duration(0.1);//Tiny sleep
	
		//Set Turn Angle
		GPSSteering.data = "T " + TurnAngle + "\r";
		Steer_Motor.publish(GPSSteering);
	}
	
}


//Added by cathy
void Camera(const cmvision::Blobs& blobs)
{
//	int x = blobs->x;
//	int y = blobs->y;

	std::cout << blobs.x[0] << std::endl;





/*	//may be useful for determining if blob is of a relevant size
	int area = blobs->area;
	
	
	if(x > 320)
	{
		//blob on the right
		cout << "blob on the right" << endl;
		//needs to turn right
	}
	else
	{
		//blob on left
		cout << "blob on left" << endl;
		//need to add turn left

*/
}


