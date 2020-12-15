/*
 \file        left_turn.c
 \author    ${user}
 \date        ${date}
 \brief        Simple Hello World! for the Ev3
*/
#include <ev3.h>

int color, distance;
int blue = 17;
int red = 41;
int speed = 10;
int time = 100; // milliseconds

void ReadColor(){
	color = ReadSensor(IN_1);
}

void ReadDistance(){
	distance = ReadSensor(IN_3);
}

// Wander()
void TurnRight(){
	OnFwdReg(OUT_A, speed);
	OnRevReg(OUT_B, speed);
	Wait(time);
}

void TurnLeft(){
	OnRevReg(OUT_A, speed);
	OnFwdReg(OUT_B, speed);
	Wait(time*2);
}

void Forward(){
	OnFwdSync(OUT_AB, speed);
	Wait(time*3);
}

void Stop(){
	OnFwdSync(OUT_AB, 0);
}

void GoalFinding();
void WallFollowing();
void Clearing();

int main(void)
{
    // INFO This code template works only with recent versions of the API. If TermPrintln is not found,
    //      please update the API or use the "Hello World EV3 Project Old API" as template.
	InitEV3();
  	SetAllSensorMode(COL_REFLECT, NO_SEN, US_DIST_MM, NO_SEN);

    TermPrintln("Hello World!");
    ButtonWaitForPress(BUTTON_ID_ENTER);
    LcdClean();

    while(1){
    	ReadColor();
    	if(color < blue){
    		WallFollowing();
    		break;
    	}else{
    		OnFwdSync(OUT_AB, speed);
    	}
    }

    return 0;
}

void GoalFinding(){
	while(1){
		ReadDistance();
		if(distance < 600){
			ReadColor();
			if(color < blue){
				WallFollowing();
				return;
			}
			if(color >= red){
				Stop();
				PlaySound(SOUND_CLICK);
				Wait(1000);
				StopSound();
				Clearing();
				return;
			}
			Forward();
		}else{
			TurnRight();
		}
	}
}

void WallFollowing(){
	int i;
	while(1){
		ReadColor();
		if(color < blue){
			for( i=0; i<7; i++){
				TurnRight();
				ReadColor();
				// check distance
				LcdClean();
				ReadDistance();
				TermPrintln("Distance: %d, Color: %d", distance, color);
				if(distance < 600){
					OnRevSync(OUT_AB, speed);
					Wait(100);
					ReadColor();
					//if(color < blue) break;
					GoalFinding();
					return;
				}
			}
		}
		while(color >= blue){
			ReadColor();
			ReadDistance();
			if(color < blue){
				Stop();
				break;
			}
			TurnLeft();
			Forward();
			if(distance < 600){
				OnRevSync(OUT_AB, speed);
				Wait(100);
				ReadColor();
				if(color < blue) break;
				GoalFinding();
				return;
			}
		}
	}
}

void Clearing(){
	while(1){
		ReadColor();
		ReadDistance();
		if(distance < 300){
			break;
		}
		if(color < blue){
			WallFollowing();
			return;
		}
		TurnRight();
	}
	PlaySound(SOUND_DOUBLE_BEEP);
	OnFwdSync(OUT_AB, speed);
	Wait(7000);
	StopSound();
	Stop();
}

