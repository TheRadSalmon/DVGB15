#include <kaulab.h>

int FirstUse = 0;
int LineRead = 0;

int RightCount = 0;
int LeftCount = 0;

int UltraSensorDist = 0;
bool isAvoiding = false; // Förhindrar att TaskLineSensor stör under undvikning

TickType_t TickStart = 0;


void TaskSonicSensor() {

  UltraSensorDist = zRobotGetUltraSensor();

  // Starta manövern bara vid FLANKEN där hindret upptäcks (inte varje varv)
  if (!isAvoiding && UltraSensorDist <= 20) {
    isAvoiding = true;
    TickStart = xTaskGetTickCount();
  }

  if (isAvoiding) {

    TickType_t elapsed = xTaskGetTickCount() - TickStart;

    // Fas 1: sväng in mot insidan av banan (0-60 ticks)
    if (elapsed <= 60) {
      if (LeftCount < RightCount) {
        zRobotSetMotorSpeed(1, -120); // Sväng vänster
        zRobotSetMotorSpeed(2, -120);
      } else {
        zRobotSetMotorSpeed(1, 120);  // Sväng höger
        zRobotSetMotorSpeed(2, 120);
      }
    }
    // Fas 2: kör förbi hindret rakt/vridande (60-100 ticks)
    else if (elapsed <= 100) {
      zRobotSetMotorSpeed(1, -120);
      zRobotSetMotorSpeed(2, 120);
    }
    // Fas 3: sväng tillbaka mot linjen (100-160 ticks)
    else if (elapsed <= 160) {
      if (LeftCount > RightCount) {
        zRobotSetMotorSpeed(1, 120);  // Sväng höger
        zRobotSetMotorSpeed(2, 120);
      } else {
        zRobotSetMotorSpeed(1, -120); // Sväng vänster
        zRobotSetMotorSpeed(2, -120);
      }
    }
    // Manövern klar - lämna över till linjesensorn igen
    else {
      isAvoiding = false;
    }
  }
}

void TaskLineSensor() {

  // Låt sonic-tasken sköta styrningen helt under undvikningsmanövern
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

  //int val = zRobotGetUltraSensor();
  //Serial.println(val);


}
