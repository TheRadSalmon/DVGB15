#include <kaulab.h>

int FirstUse = 0;
int LineRead = 0;

int RightCount = 0;
int LeftCount = 0;

int UltraSensorDist = 0;
bool isAvoiding = false; 

TickType_t TickStart = 0;


void TaskSonicSensor() {

  UltraSensorDist = zRobotGetUltraSensor();

  if (!isAvoiding && UltraSensorDist <= 20) {
    isAvoiding = true;
    TickStart = xTaskGetTickCount();
  }

  if (isAvoiding) {

    TickType_t elapsed = xTaskGetTickCount() - TickStart;

    if (elapsed <= 20) {
      if (LeftCount < RightCount) {
        zRobotSetMotorSpeed(1, -120);
        zRobotSetMotorSpeed(2, -120);
      } else {
        zRobotSetMotorSpeed(1, 120);
        zRobotSetMotorSpeed(2, 120);
      }
    }

    else if (elapsed <= 165) {
      if (LeftCount < RightCount) {
        zRobotSetMotorSpeed(1, -50);
        zRobotSetMotorSpeed(2, 120);
      } else {
        zRobotSetMotorSpeed(1, -120);
        zRobotSetMotorSpeed(2, 50);
      }
    }

    else if(elapsed <= 200 && zRobotGetLineSensor() != 0){
      if (LeftCount < RightCount ) {
        zRobotSetMotorSpeed(1, -120);
        zRobotSetMotorSpeed(2, -120);
      } else {
        zRobotSetMotorSpeed(1, 120);
        zRobotSetMotorSpeed(2, 120);
      }
    }
    
    else {
      isAvoiding = false;
    }
  }
}

void TaskLineSensor() {

  if (isAvoiding) {
    return;
  }

  LineRead = zRobotGetLineSensor();

  if (LineRead == 0) {
    zRobotSetMotorSpeed(1, -120);
    zRobotSetMotorSpeed(2, 120);
  }

  else if (LineRead == 1) {
    zRobotSetMotorSpeed(1, -120);
    zRobotSetMotorSpeed(2, 90);
    FirstUse = 1;
  }

  else if (LineRead == 2) {
    zRobotSetMotorSpeed(1, -90);
    zRobotSetMotorSpeed(2, 120);
    FirstUse = 2;
  }

  else if (LineRead == 3) {
    if (FirstUse == 1) {
      zRobotSetMotorSpeed(1, -130);
      zRobotSetMotorSpeed(2, 50);
      RightCount++;
    } else if (FirstUse == 2) {
      zRobotSetMotorSpeed(1, -50);
      zRobotSetMotorSpeed(2, 130);
      LeftCount++;
    }
  }
}

void setup() {
  zInitialize();

  zScheduleTask(TaskSonicSensor, 4, 2);
  zScheduleTask(TaskLineSensor, 2, 2);


  zStart();
}

void loop() {
}
