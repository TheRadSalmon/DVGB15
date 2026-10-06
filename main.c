#include <kaulab.h>

int FirstUse;
int LineRead;

int RightCount = 0;
int LeftCount = 0;

int UltraSensorDist;
bool IsUltraSensor = false;
bool NeedGetBack = false;

//bool IsLineRead = false;


void TaskSonicSensor(){
  UltraSensorDist = zRobotGetUltraSensor();


  if(UltraSensorDist >= 0 && UltraSensorDist <= 400){
    IsUltraSensor = true;
  }




  if(IsUltraSensor == true){

    if(LeftCount > RightCount){

      zRobotSetMotorSpeed(1, -120);
      zRobotSetMotorSpeed(2, 90);

    }

    else if(LeftCount < RightCount){

      zRobotSetMotorSpeed(1, -90);
      zRobotSetMotorSpeed(2, 120);
    
    }

    IsUltraSensor = false;
    NeedGetBack = true;
  }



  if(NeedGetBack == true){
    
    if(LeftCount > RightCount){

      zRobotSetMotorSpeed(1, -90);
      zRobotSetMotorSpeed(2, 120);

    }

    else if(LeftCount < RightCount){

      zRobotSetMotorSpeed(1, -120);
      zRobotSetMotorSpeed(2, 90);
    
    }

    NeedGetBack = false;
  }
}




void TaskLineSensor(){

  LineRead = zRobotGetLineSensor();
  //IsLineRead = true; 

  if(LineRead == 0){
    zRobotSetMotorSpeed(1, -120);
    zRobotSetMotorSpeed(2, 120);
  
  }


  if(LineRead == 1){
    zRobotSetMotorSpeed(1, -120);
    zRobotSetMotorSpeed(2, 90);

    FirstUse = 1;
  }


  if(LineRead == 2){
    zRobotSetMotorSpeed(1, -90);
    zRobotSetMotorSpeed(2, 120);

    FirstUse = 2;
  }


  if(LineRead == 3){ 
    if(FirstUse == 1){
      zRobotSetMotorSpeed(1, -130);
      zRobotSetMotorSpeed(2, 50);
      RightCount =  RightCount + 1;
    
    }else if(FirstUse == 2){
      zRobotSetMotorSpeed(1, -50);
      zRobotSetMotorSpeed(2, 130);
      LeftCount =  LeftCount + 1;

    }
  }


}





void setup() {
  zInitialize();

  zScheduleTask(TaskLineSensor, 2, 1);
  zScheduleTask(TaskSonicSensor, 3, 2);

  zStart();
}

void loop() {
  // put your main code here, to run repeatedly:

}
