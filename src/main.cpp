#include "main.h"





// Chassis constructor
Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-16, -17, -18}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{8, 9, 10}

  // IMU Port
  ,21

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,3.25

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,1.67
);


///
//Control Functions
///


//These functions are mainly here for future use, these can be exported to the auton file and used to make the auton code simpler.

//Having functions take in bools are made here so its eaiser to make toggles and change constant values, eg. Flywheel speed

//Flywheel bool function
void flywheelControl(bool value){
    pros::Motor flywheel(1);
    if(value == true){
      flywheel = -117;
    }else{
      flywheel = 0;}}

//Wings bool function
void wingsControl(bool value){
   pros::ADIDigitalOut wings('A');
   wings.set_value(value);}

//Lift bool function
void liftControl(bool value){
  pros::ADIDigitalOut lift('H');
  lift.set_value(value);}

//Drive bool function
void driveControl(bool value){
  if(value == false){
    chassis.arcade_standard(ez::SPLIT);
  }else{
    chassis.tank();}}


 //Runs initialization code. This occurs as soon as the program is started.
void initialize() {

  pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  // Configure your chassis controls
  chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0.1); // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({
    Auton("Regular", regular),
    Auton("Skills", skills)
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
}


//No idea what one would need this for but its here 
void disabled() {
  // . . .
}


//Same as initizalize but for competetions
void competition_initialize() {
    pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  // Configure your chassis controls
  chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0.1); // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!


  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({
    Auton("Regular", regular),
    Auton("Skills", skills)
  });
  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
}


void autonomous() {
  chassis.reset_pid_targets(); // Resets PID targets to 0
  chassis.reset_gyro(); // Reset gyro position to 0
  chassis.reset_drive_sensor(); // Reset drive sensors to 0
  chassis.set_drive_brake(MOTOR_BRAKE_BRAKE); // Set motors to hold.  This helps autonomous consistency.

  ez::as::auton_selector.print_selected_auton();
  ez::as::auton_selector.call_selected_auton(); // Calls selected auton from autonomous selector.
}


void opcontrol() {
  // This is preference to what you like to drive on.
  chassis.set_drive_brake(MOTOR_BRAKE_COAST);


  //These are toggle variables, toggling would make it easier for the driver to control the various componenets of the robot more efficenitly.
  static bool toggleFlywheel = {false};
  static bool toggleWings = {false};
  static bool toggleLift = {false};
  static bool toggleDrive = {false};

  while (true) { //Forever loop that checks looks for chnages in the controller state and translates those changes to motor or soleniod activations on the robot

    driveControl(toggleDrive); //Having Split drive as the default drive mode



    /*The following if statements have toggle logic. How this logic works is: The toggleComponent variable stores the current value, then once the controller button is pressed, 
    it checks what the previous value was and changes the current value to the opposite of the previous value was. Then stores the new value into the toggleComponent variable*/

    //Drive Mode Selection
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){ //Having toggle drive modes would mean that it would be eaiser to change between our drivers if they have different drive preferences
      driveControl(!toggleDrive);    
      toggleDrive = !toggleDrive;}

  
    // Flywheel 
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){  // This would make match loading eaiser as the mistake of accidently turning off the flywheel would not happen
    flywheelControl(!toggleFlywheel);    
    toggleFlywheel = !toggleFlywheel;}

    // Wings
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){ // Togglable wings help pushing the maximum number of balls into the net, this would avoid the mistake of accidently lowering the wings while pushing balls into the net
     wingsControl(!toggleWings);
     toggleWings = !toggleWings;}

    //Lift
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){ // This would make match loading eaiser as the mistake of accidently lowering the lift would not happen
     liftControl(!toggleLift);
     toggleLift = !toggleLift;};

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }


}





