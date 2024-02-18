#include "main.h"

// Chassis constructor
Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-9, 10, -20}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{-1, 2, 12}

  // IMU Port
  ,3

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
void flywheelControlFront(bool value){
    pros::Motor flywheel(14);
    if(value == true){
      flywheel = 127;
    }else{
      flywheel = 0;}}

void flywheelControlBack(bool value){
    pros::Motor flywheel(14);
    if(value == true){
      flywheel = -127;
    }else{
      flywheel = 0;}}


//Wings bool function
void wingsControl(bool value){
   pros::ADIDigitalOut wings('A');
   wings.set_value(value);}


//Drive bool function
void driveControl(bool value){
      chassis.arcade_standard(ez::SPLIT);
}

//Lift control
void hangControl(bool value){
    pros::Motor hang(6);
if(value == false){
  hang = 75;
}
else{
  hang = -75;
};
   }



 //Runs initialization code. This occurs as soon as the program is started.
void initialize() {

  pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  // Configure your chassis controls
    chassis.set_curve_default(7, 7); // Set curve
  chassis.set_active_brake(0.1); // Sets the active brake kP. We recommend 0.1.; // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({
    Auton("Win Point", winpoint),
    Auton("Regular", normal),
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
  chassis.set_curve_default(7, 7); // Set curve
  chassis.set_active_brake(0.1); // Sets the active brake kP. We recommend 0.1.
  default_constants(); // Set the drive to your own constants from autons.cpp!


  // Autonomous Selector using LLEMU
  ez::as::auton_selector.add_autons({
      Auton("Win Point", winpoint),
      Auton("Regular", normal),
     Auton("Skills", skills),

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
  pros::Motor intake(17);

  

  //These are toggle variables, toggling would make it easier for the driver to control the various componenets of the robot more efficenitly.
  static bool toggleFlywheelFront = {false};
  static bool toggleFlywheelBack = {false};
  static bool toggleWings = {false};
  static bool toggleDrive = {false};
  static bool toggleHang = {false};

  while (true) { //Forever loop that checks looks for chnages in the controller state and translates those changes to motor or soleniod activations on the robot

    driveControl(toggleDrive); //Having Split drive as the default drive mode


    /*The following if statements have toggle logic. How this logic works is: The toggleComponent variable stores the current value, then once the controller button is pressed, 
    it checks what the previous value was and changes the current value to the opposite of the previous value was. Then stores the new value into the toggleComponent variable*/

    //Drive Mode Selection
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)){ //Having toggle drive modes would mean that it would be eaiser to change between our drivers if they have different drive preferences
      driveControl(!toggleDrive);    
      toggleDrive = !toggleDrive;}

    // Flywheel
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){  // This would make match loading eaiser as the mistake of accidently turning off the flywheel would not happen
    flywheelControlFront(!toggleFlywheelFront);    
    toggleFlywheelFront = !toggleFlywheelFront;}

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){  // This would make match loading eaiser as the mistake of accidently turning off the flywheel would not happen
    flywheelControlBack(!toggleFlywheelBack);    
    toggleFlywheelBack = !toggleFlywheelBack;}

    // Wings
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){ // Togglable wings help pushing the maximum number of balls into the net, this would avoid the mistake of accidently lowering the wings while pushing balls into the net
     wingsControl(!toggleWings);
     toggleWings = !toggleWings;}
    

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){ // Togglable wings help pushing the maximum number of balls into the net, this would avoid the mistake of accidently lowering the wings while pushing balls into the net
     hangControl(!toggleHang);
     toggleHang = !toggleHang;}


//Intake
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
      intake = 127;
    }
    /**/
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
      intake = -127;
    }
  else {
      intake = 0;
    };

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }



  


}





