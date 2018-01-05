#include <Servo.h>
#include <Stepper.h>

const bool ENABLE_LOGGING = true;
//=======================================
// Storage Variables
//=======================================
/*
String storageState[7] = {
  "FLAG_INCOMPLETE", "FLAG_INCOMPLETE", "FLAG_INCOMPLETE", "FLAG_INCOMPLETE", "FLAG_INCOMPLETE", "FLAG_INCOMPLETE", "FLAG_INCOMPLETE"
};

const int STORAGE_TOTAL_SLOTS = 9;
const int STORAGE_ARM_ROTATION[STORAGE_TOTAL_SLOTS] = {

};

const int STORAGE_ARM_SHOULDER_POSITION[2][STORAGE_TOTAL_SLOTS] = {
  {},
  {},
};
const int STORAGE_ARM_ELBOW_POSITION[2][STORAGE_TOTAL_SLOTS] = {
  {},
  {},
};
const int STORAGE_ARM_WRIST_POSITION[2][STORAGE_TOTAL_SLOTS] = {
  {},
  {},
};

const int ARM_STORAGE_ACCESS_DELAY = 200;

void storageTakeFlag(int position) {
  armSetShoulder(ARM_SHOULDER_INIT);
  armSetElbow(ARM_ELBOW_INIT);

  delay(ARM_STORAGE_ACCESS_DELAY);
  armSetShoulder(ARM_FLAG_SHOULDER_POSITION[0][position]);
  armSetElbow(ARM_FLAG_ELBOW_POSITION[0][position]);
  armSetWrist(ARM_FLAG_WRIST_POSITION[0][position]);
  armSetClaw(ARM_CLAW_OPEN);
  armPivot(STORAGE_ARM_ROTATION[position]);

  delay(ARM_STORAGE_ACCESS_DELAY);
  armSetClaw(ARM_CLAW_CLOSE);
  delay(ARM_STORAGE_ACCESS_DELAY);

  armSetShoulder(ARM_FLAG_SHOULDER_POSITION[1][position]);
  armSetElbow(ARM_FLAG_ELBOW_POSITION[1][position]);
  armSetWrist(ARM_FLAG_WRIST_POSITION[1][position]);

  delay(ARM_STORAGE_ACCESS_DELAY);
  armSetShoulder(ARM_SHOULDER_INIT);
  armSetElbow(ARM_ELBOW_INIT);
  armSetWrist(ARM_WRIST_INIT);
  armPivot(0);
}

void storageReturnFlag(int position) {
  armSetShoulder(ARM_SHOULDER_INIT);
  armSetElbow(ARM_ELBOW_INIT);

  delay(ARM_STORAGE_ACCESS_DELAY);
  armSetShoulder(ARM_FLAG_SHOULDER_POSITION[1][position]);
  armSetElbow(ARM_FLAG_ELBOW_POSITION[1][position]);
  armSetWrist(ARM_FLAG_WRIST_POSITION[1][position]);
  armPivot(STORAGE_ARM_ROTATION[position]);

  delay(ARM_STORAGE_ACCESS_DELAY);
  armSetShoulder(ARM_FLAG_SHOULDER_POSITION[0][position]);
  armSetElbow(ARM_FLAG_ELBOW_POSITION[0][position]);
  armSetWrist(ARM_FLAG_WRIST_POSITION[0][position]);
  delay(ARM_STORAGE_ACCESS_DELAY);

  armSetClaw(ARM_CLAW_OPEN);
  delay(ARM_STORAGE_ACCESS_DELAY);

  armSetShoulder(ARM_SHOULDER_INIT);
  armSetElbow(ARM_ELBOW_INIT);
  armSetWrist(ARM_WRIST_INIT);
}

int storageFirstSlot(String targetState) {
  for (int i = 0; i < STORAGE_TOTAL_SLOTS; i++) {
    if (storageState[i] == targetState) {
      return i;
    }
  }
  return 0;
}
*/
//=======================================
// Arm Variables
//=======================================
Stepper armPivot (200, 8, 9, 10, 11);
int armRotation = 0;
Servo armShoulder;
Servo armElbow;
Servo armWrist;
Servo armClaw;

const int ARM_SERVO_STEP = 5;
const int ARM_SHOULDER_MIN = 10;
const int ARM_SHOULDER_INIT = 90;
const int ARM_SHOULDER_MAX = 140;
const int ARM_ELBOW_MIN = 10;
const int ARM_ELBOW_INIT = 85;
const int ARM_ELBOW_MAX = 175;
const int ARM_WRIST_MIN = 0;
const int ARM_WRIST_INIT = 0;
const int ARM_WRIST_MAX = 180;
const int ARM_CLAW_OPEN = 60;
const int ARM_CLAW_CLOSE = 151;

void setupArm() {
  armPivot.setSpeed(50);
  armSetPivot(0);

  armShoulder.attach(16);
  armElbow.attach(17);
  armWrist.attach(18);
  armClaw.attach(19);

  armSetShoulder(ARM_SHOULDER_INIT);
  armSetElbow(ARM_ELBOW_INIT);
  armSetWrist(ARM_WRIST_INIT);
  armSetClaw(ARM_CLAW_OPEN);
}

void armSetPivot(int degrees) {
  armPivot.step(limitRange(degrees, 0, 360) - armRotation);
}

void armSetShoulder(int degrees) {
  armShoulder.write(limitRange(degrees, ARM_SHOULDER_MIN, ARM_SHOULDER_MAX));
}

void armSetElbow(int degrees) {
  armElbow.write(limitRange(degrees, ARM_ELBOW_MIN, ARM_ELBOW_MAX));
}

void armSetWrist(int degrees) {
  armWrist.write(limitRange(degrees, ARM_WRIST_MIN, ARM_WRIST_MAX));
}

void armSetClaw(int degrees) {
  armClaw.write(limitRange(degrees, ARM_CLAW_OPEN, ARM_CLAW_CLOSE));
}

void armFlipWrist() {
  if (armWrist.read() < (ARM_WRIST_MAX - ARM_WRIST_MIN) / 2){
    armWrist.write(ARM_WRIST_MAX);
  } else {
    armWrist.write(ARM_WRIST_MIN);
  }
}

//=======================================
// Track Variables
//=======================================
Servo trackRight;
Servo trackLeft;

const int TRACK_FORWARD = 80; // 50 lower bound
const int TRACK_STOP = 90;
const int TRACK_REVERSE = 120; // 150 upper bound

void setupTracks() {
  trackRight.attach(15);
  trackLeft.attach(14);

  trackRight.write(TRACK_STOP);
  trackLeft.write(TRACK_STOP);
}

void driveRight() {
  trackRight.write(TRACK_FORWARD);
  trackLeft.write(TRACK_REVERSE);
  delay(100);
  driveStop();
}

void driveLeft() {
  trackRight.write(TRACK_REVERSE);
  trackLeft.write(TRACK_FORWARD);
  delay(100);
  driveStop();
}

void driveStop() {
  trackRight.write(TRACK_STOP);
  trackLeft.write(TRACK_STOP);
}

void driveForward() {
  trackRight.write(TRACK_FORWARD);
  trackLeft.write(TRACK_FORWARD);
  delay(100);
  driveStop();
}

void driveReverse() {
  trackRight.write(TRACK_REVERSE);
  trackLeft.write(TRACK_REVERSE);
  delay(100);
  driveStop();
}

//=======================================
// Bridge Variables
//=======================================
Servo bridge;

const int BRIDGE_INITIAL = 180;
const int BRIDGE_DROP = 0;

void setupBridge(){
  bridge.attach(6);
  bridge.write(BRIDGE_INITIAL);
}

void dropBridge() {
  bridge.write(BRIDGE_DROP);
}

void setup()
{
  Serial.begin(9600);
  setupArm();
  setupTracks();
  setupBridge();
}

void loop()
{
  String commandPerformed = "";
  String command = readCommand();

  //=======================================
  // Arm Commands
  //=======================================
  if (command == "105") { // 'i'
    commandPerformed = "call: armUp";
    armSetShoulder(armShoulder.read() + ARM_SERVO_STEP);
    armSetElbow(armElbow.read() - ARM_SERVO_STEP);
  } else if (command == "107") { // 'k'
    commandPerformed = "call: armDown";
    armSetShoulder(armShoulder.read() - ARM_SERVO_STEP);
    armSetElbow(armElbow.read() + ARM_SERVO_STEP);
  } else if (command == "91") { // '['
    commandPerformed = "call: armPivotClockwise";
    armSetPivot(armRotation + 1);
  } else if (command == "93") { // ']'
    commandPerformed = "call: armPivotCounterClockwise";
    armSetPivot(armRotation - 1);
  } else if (command == "110") { // 'n'
    commandPerformed = "call: armShoulderUp";
    armSetShoulder(armShoulder.read() + ARM_SERVO_STEP);
  } else if (command == "109") { // 'm'
    commandPerformed = "call: armShoulderDown";
    armSetShoulder(armShoulder.read() - ARM_SERVO_STEP);
  } else if (command == "103") { // 'g'
    commandPerformed = "call: armElbowUp";
    armSetElbow(armElbow.read() + ARM_SERVO_STEP);
  } else if (command == "104") { // 'h'
    commandPerformed = "call: armElbowDown";
    armSetElbow(armElbow.read() - ARM_SERVO_STEP);
  } else if (command == "121") { // 'y'
    commandPerformed = "call: armWristClockwise";
    armSetWrist(armWrist.read() + ARM_SERVO_STEP);
  } else if (command == "117") { // 'u'
    commandPerformed = "call: armWristCounterClockwise";
    armSetWrist(armWrist.read() - ARM_SERVO_STEP);
  } else if (command == "116") { // 't'
    commandPerformed = "call: armFlipWrist";
    armSetWrist(armWrist.read() - ARM_SERVO_STEP);
  } else if (command == "98") { // 'b'
    commandPerformed = "call: armClawGrab";
    armSetClaw(armClaw.read() + ARM_SERVO_STEP);
  } else if (command == "118") { // 'v'
    commandPerformed = "call: armClawRelease";
    armSetClaw(armClaw.read() - ARM_SERVO_STEP);
  } else if (command == "111") { // 'o'
    commandPerformed = "call: armClawOpen";
    armSetClaw(ARM_CLAW_OPEN);
  } else if (command == "112") { // 'p'
    commandPerformed = "call: armClawClose";
    armSetClaw(ARM_CLAW_CLOSE);
  } else if (command == "114") { // 'r'
    commandPerformed = "call: armReset";
    armShoulder.write(ARM_SHOULDER_INIT);
    armElbow.write(ARM_ELBOW_INIT);
    armWrist.write(ARM_WRIST_INIT);
    armClaw.write(ARM_CLAW_OPEN);
  }
  //=======================================
  // Storage Commands
  //=======================================
  /*
  else if (command == "49") { // '1'
    commandPerformed = "call: storageTakeIncompleteFlag";
    int storageSlot = storageFirstSlot("FLAG_INCOMPLETE");
    storageTakeFlag(storageSlot);
    storageState[storageSlot] = "EMPTY";
  }
  else if (command == "50") { // '2'
    commandPerformed = "call: storageReturnCompleteFlag";
    int storageSlot = storageFirstSlot("EMPTY");
    storageReturnFlag(storageSlot);
    storageState[storageSlot] = "FLAG_COMPLETE";
  }
  else if (command == "51") { // '3'
    commandPerformed = "call: storageTakeCompleteFlag";
    int storageSlot = storageFirstSlot("FLAG_COMPLETE");
    storageTakeFlag(storageSlot);
    storageState[storageSlot] = "EMPTY";
  }
  */
  //=======================================
  // Drive Commands
  //=======================================
  else if (command == "119") { // 'w'
    commandPerformed = "call: driveForward";
    driveForward();
  } else if (command == "120") { // 'x'
    commandPerformed = "call: driveStop";
    driveStop();
  } else if (command == "115") { // 's'
    commandPerformed = "call: driveReverse";
    driveReverse();
  } else if (command == "100") { // 'd'
    commandPerformed = "call: driveRight";
    driveRight();
  } else if (command == "97") { // 'a'
    commandPerformed = "call: driveLeft";
    driveLeft();
  }
  //=======================================
  // Bridge Commands
  //=======================================
  else if (command == "47") { // '/'
    commandPerformed = "call: dropBridge";
    dropBridge();
  }
  //=======================================
  // Other Commands
  //=======================================
  else if (command == "99") { // 'c'
    commandPerformed = "call: printArmState";
    Serial.print("Pivot (deg) : ");
    Serial.println(armRotation);
    Serial.print("Shoulder (deg) : ");
    Serial.println(armShoulder.read());
    Serial.print("Elbow (deg) : ");
    Serial.println(armElbow.read());
    Serial.print("Wrist (deg): ");
    Serial.println(armWrist.read());
    Serial.print("Claw (deg) : ");
    Serial.println(armClaw.read());
  }

  if (ENABLE_LOGGING && command != "") {
    Serial.println(commandPerformed);
  }
}

String readCommand(){
  String command = "";
  while (Serial.available()) {
    command = Serial.read();
    delay(1);
  }

  if (command.length() > 0) {
    return command;
  }
}

int limitRange(int value, int lowerLimit, int upperLimit) {
  int limitedValue;

  limitedValue = (value > lowerLimit) ? value : lowerLimit;
  limitedValue = (value < upperLimit) ? value : upperLimit;

  return limitedValue;
}
