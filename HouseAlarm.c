
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <wiringPi.h>
#include "ifttt.h"

/*btn 3,
  alarm 4,
  red 1,
  yellow 2.*/
int status;
extern int ifttt(char* , char* , char* , char* );
status = 1; /*=====inital status for handlingFunction=====*/


void alarmOff(void)
    {
        int buttonValue;
        buttonValue = digitalRead(3);
        while(buttonValue)
            {
            buttonValue = digitalRead(3);
            if(buttonValue == 0)
            {
            status = 2; /*=====status=====*/
            }
            digitalWrite (1, HIGH) ;
            digitalWrite (2, LOW) ;
            digitalWrite (4, LOW) ;
            }
  }

void alarmArming(void)
{
    int i;
    int buttonValue;
    double sec; /*  update time */
    double sec10;
    sec = (double) time(NULL); /*  current time */
    sec10 = sec + 10;
    buttonValue = digitalRead(3); /* saving  button status */
    digitalWrite (2, LOW) ;
    digitalWrite (4, LOW) ;
    while((sec <= sec10) && buttonValue)
    {
           buttonValue = digitalRead(3); /* checking button status */
           sec = (double) time(NULL);
           if(fmod(sec,2) == 0)
           {
                digitalWrite (1, HIGH);
           }
           else
           {
                digitalWrite (1, LOW) ;
           } 
    }
    digitalWrite (1, LOW) ;
    digitalWrite (2, HIGH) ;
    status = 3;
}

void alarmArmed(void)
{
    
    int buttonValue;
    int motionSignal;
    motionSignal = digitalRead(0);
    buttonValue = digitalRead(3);
    while( !motionSignal && buttonValue)
    {
        buttonValue = digitalRead(3);
        if(buttonValue == 0)
        {
            status = 1; 
            break;
        }
        motionSignal = digitalRead(0);
        if(motionSignal)
        {
	    printf("alarm armed\n");
            status = 4;
            break;
        }
        digitalWrite (1, LOW) ;
        digitalWrite (2, HIGH) ;
        digitalWrite (4, LOW) ;  
    }
}

void alarmTriggerd(void)
    {
        double seconds; /* var to  update time */
        double secondsafter10;
        int buttonValue;
        seconds = (double) time(NULL); /* current timing */
        secondsafter10 = seconds + 10;
        buttonValue = digitalRead(3); /* saving button status */
        while((seconds <= secondsafter10) && buttonValue)
        {
           buttonValue = digitalRead(3); /* checking button status */
           seconds = (double) time(NULL);
           if(fmod(seconds,2) == 0)
           {
              printf("checking AT\n"); 
                digitalWrite (1, HIGH) ;
                digitalWrite (2, HIGH) ;
           }
           else
           {
                digitalWrite (1, LOW) ;
                digitalWrite (2, LOW) ; 
           }
        }
        digitalWrite (1, LOW) ; /* turn LED are off */
        digitalWrite (2, LOW) ; /* turn LED are off */
        if(buttonValue == 0)
        {
            status = 1;
        }
        else
        {
            status = 5;
        }
   } 



void alarmSounding(void)
{
    /* IFFT event should be TRIGGERED*/
   /* ifttt("http://red.eecs.yorku.ca:8080/trigger/event/with/key/Ja3far", "alarm", "is", "sounding"); */
    ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/dxW-X1gOzkLeYN8RV6DhPQ", "Alarm", "is", "souding");
    int buttonValue;
    double seconds;
    buttonValue = digitalRead(3);
    while(buttonValue)
    {
        buttonValue = digitalRead(3);
        seconds = (double) time(NULL);
        if(fmod(seconds,2) == 0)
        {
            printf("checking AS\n"); 
            digitalWrite (1, HIGH) ;
            digitalWrite (2, HIGH) ;
            digitalWrite (4, HIGH) ;
        }
        else
        {
            digitalWrite (1, LOW) ;
            digitalWrite (2, LOW) ; 
            digitalWrite (4, LOW) ;
        }
        
    }
        status = 1;   
}


void handlingfunction(void)
{
    while(1)
    {
        if(status == 1)
        {
            alarmOff();
        }
        else if(status == 2)
        {
            alarmArming();
        }
        else if(status == 3)
        { 
           alarmArmed();
        }
        else if(status == 4)
        {
            alarmTriggerd();
        }
        else
        {
            alarmSounding();
        }
    }
    
}
int main()
{
    wiringPiSetup () ;
    pinMode(0, INPUT);
    pinMode (1, OUTPUT) ;
    pinMode (2, OUTPUT) ;
    pinMode (3, INPUT) ;
    pullUpDnControl(3, PUD_UP) ;
    pinMode (4, OUTPUT) ;
    handlingfunction();
    

    return 0;
}
