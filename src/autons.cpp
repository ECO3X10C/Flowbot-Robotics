#include "main.h"



//The speeds the robot is moving when driving, turning and swinging. These were figured out from experimental testing
const int DRIVE_SPEED = 80; 
const int TURN_SPEED  = 50;
const int SWING_SPEED = 50;



///
// Constants
///

//PID Constant, these are still untuned, something that needs to change after the first league meeting on Jan 6 2024

void default_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 7, 0.005, 30, 12);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void one_mogo_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void two_mogo_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}


void modified_exit_condition() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}






///
//Auton Functions
///

//All this fucntion does it go forward put the preload into the net and then move back. This is a way to ensure we have a chance of winning auton in a regular match
void normal() {
  pros::Motor intake(17);
  pros::ADIDigitalOut wings('A');
  chassis.set_drive_pid(52,127);  //Go forward 25 inches
  chassis.wait_drive(); //Wait for the previous instruction to complete
  chassis.set_turn_pid(-90, TURN_SPEED);  //Go forward 25 inches
  chassis.wait_drive(); //Wait for the previous instruction to complete
  intake = -127;
  pros::delay(1000);
  chassis.set_drive_pid(10,80);  //Go forward 25 inches
  chassis.wait_drive();
  chassis.set_drive_pid(-10,80);  //Go forward 25 inches
  chassis.wait_drive();
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-4,127);  //Go forward 25 inches
  chassis.wait_drive();
  wings.set_value(true);
  pros::delay(1000);
  chassis.set_drive_pid(32,127);  //Go forward 25 inches
  chassis.wait_drive();
  chassis.set_drive_pid(-15,127);  //Go forward 25 inches
  chassis.wait_drive();
  chassis.set_drive_pid(15,127);  //Go forward 25 inches
  chassis.wait_drive();
  chassis.set_drive_pid(-15,127);  //Go forward 25 inches
  chassis.wait_drive();
  wings.set_value(false);

}

void winpoint(){
  pros::ADIDigitalOut wings('A');
  chassis.set_drive_pid(-20,50);
  chassis.wait_drive();
    pros::delay(1000);
  wings.set_value(true);
  pros::delay(1000);
  chassis.set_drive_pid(20,127,true);
  chassis.wait_drive();
  wings.set_value(false);
  chassis.set_turn_pid(-20,80);
  chassis.wait_drive();
    chassis.set_drive_pid(25.75,70,true);
  chassis.wait_drive();

}

// The lift and flywheel go up for the 1 min skils auton period, in that time matching loading happens

void skills(){

  /*In the future this would be replaced by the control functions in main.cpp as that would signifianctly make it more reliable as it is more central. 
  Also after the first league meet and once the PID values are tuned properly we can make the bot move and push balls into net*/
  pros::Motor flywheel(14);  //Set flywheel port
  flywheel = -127;
  pros::delay(50000); //set flywheel to ~92%, because when tested on the field this is the most efficent speed to get all the triballs over to the other side
  flywheel=0;
  chassis.set_drive_pid(-70,127,true);
}

