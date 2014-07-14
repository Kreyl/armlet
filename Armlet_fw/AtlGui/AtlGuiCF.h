#ifndef ATLGUICF_H_
#define ATLGUICF_H_

#define START_VOL_CONST 202

void CallMainToReason();
void CallReasonToMain();


//extern int buttonIsPressable(int, int);
//extern int buttonGetState(int, int);
//extern int buttonPress(int, int);
//press_mode 0- keRelease  2 - keLongPress 3 - keRepeat
//int buttonIsPressable(int, int);
//int buttonGetState(int , int );
//int buttonPress(int, int);
int buttonIsPressable(int, int);
int buttonGetState(int, int);
int buttonPress(int, int,int);
//sound buttons
int bSoundUpCheck(int screen_id, int button_id);
int bSoundDownCheck(int screen_id, int button_id);
int bSoundUpChange(int screen_id, int button_id ,int press_mode);
int bSoundDownChange(int screen_id, int button_id ,int press_mode);
//another music file
int bChangeMelodyCheck(int screen_id, int button_id);
int bChangeMelody(int screen_id, int button_id ,int press_mode);

//lock button
int bLockCheck(int screen_id, int button_id);
int bLockChange(int screen_id, int button_id ,int press_mode);
int bLockGetState(int screen_id, int button_id);
//reason button
int bReasonCheck(int screen_id, int button_id);
int bReasonChange(int screen_id, int button_id ,int press_mode);
int bReasonGetState(int screen_id, int button_id);

//вызывается, если резон закончился а картинка у человека на кране - перерисовывает кнопку
void CheckAndRedrawFinishedReasons();
//lock button
//int bLockCheck(int screen_id, int button_id);
//int bLockChange(int screen_id, int button_id);
//int bLockState(int screen_id, int button_id);
extern int current_volume_lvl;
#endif
