// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

//从右到左四个灰度传感器引脚号
#define G0 16 
#define G1 17
#define G2 4 
#define G3 18
#define G4 19 
#define GD 23

#define BLINKER_WIFI

#include <Blinker.h>

// 定义wifi信息
char auth[] = "your auth key";
char ssid[] = "your ssid";
char pswd[] = "your password";

// 定义标志位
int state = 0;

// 定义电机引脚
#define L1 25
#define L2 33
#define R1 14
#define R2 27

// 定义电机pwm通道
#define R1_CHANNEL 1
#define R2_CHANNEL 2
#define L1_CHANNEL 3
#define L2_CHANNEL 4

// 设置debug模式
#define DEBUG 1

// 定义电机速度
int16_t lspeedvalue = 0;
int16_t rspeedvalue = 0;
int16_t speedvalue = 0;

// 定义与 Blinker 链接的组件
BlinkerSlider Rpwm("rpwm");
BlinkerSlider Lpwm("lpwm");
BlinkerSlider Speed("speed");
BlinkerSlider State("state");
BlinkerSlider VoiceState("voice");
BlinkerText   Gray("GrayState");
BlinkerText   DebugMsg("debugmsg");

// 定义语音控制标志位
int voice_flag = 0;

// 定义各回调函数
void voice_callback(int32_t value){
  if (DEBUG){
    Serial.print("voice state:");
    Serial.println(value);
  }
  voice_flag = value;
}
void rpwm_callback(int32_t value) {
  if (DEBUG)
    Serial.println(value);
  rspeedvalue = value;
}
void lpwm_callback(int32_t value) {
  if (DEBUG)
    Serial.println(value);
  lspeedvalue = value;
}
void speed_callback(int32_t value) {
  if (DEBUG)
    Serial.println(value);
  speedvalue = value;
}
void state_callback(int32_t value) {
  if (DEBUG)
    Serial.println(value);
  state = value;
}

// Blinker 初始化
void Blinker_Init() {
  Blinker.begin(auth, ssid, pswd);
  Rpwm.attach(rpwm_callback);
  Lpwm.attach(lpwm_callback);
  Speed.attach(speed_callback);
  State.attach(state_callback);
  VoiceState.attach(voice_callback);
}


// 电机初始化
void Motor_Init()
{
  //  设置PWM
  ledcSetup(R1_CHANNEL, freq, 8);
  ledcSetup(L1_CHANNEL, freq, 8);
  ledcSetup(R2_CHANNEL, freq, 8);
  ledcSetup(L2_CHANNEL, freq, 8);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(R1, R1_CHANNEL);
  ledcAttachPin(R2, R2_CHANNEL);
  ledcAttachPin(L1, L1_CHANNEL);
  ledcAttachPin(L2, L2_CHANNEL);

  ledcWrite(R1_CHANNEL, 128);
  ledcWrite(R2_CHANNEL, 128);
  ledcWrite(L1_CHANNEL, 128);
  ledcWrite(L2_CHANNEL, 128);
  
}



// 设置速度的函数，速度范围为-255~255
void lspeed(int16_t speed)
{
  speed = speed >  255 ?  255 : speed;
  speed = speed < -255 ? -255 : speed;
  
  if(speed != 0) {
    ledcWrite(L1_CHANNEL, 128+speed/2);
    ledcWrite(L2_CHANNEL, 128-speed/2);
  } else {
    ledcWrite(L1_CHANNEL, 128);
    ledcWrite(L2_CHANNEL, 128);
  } 
  
}
void rspeed(int16_t speed)
{
  speed = speed > 255? 255: speed;
  speed = speed < -255 ? -255 : speed;
  
  if(speed != 0) {
    ledcWrite(R1_CHANNEL, 128+speed/2);
    ledcWrite(R2_CHANNEL, 128-speed/2);
  } else {
    ledcWrite(R1_CHANNEL, 128);
    ledcWrite(R2_CHANNEL, 128);
  } 
}

// 调用该函数以同时设置左右电机速度
void setspeed(){
  lspeed(lspeedvalue);
  rspeed(rspeedvalue);
}

// 灰度传感器初始化
void Gray_Init(){
  //16 17 5 18 19
  pinMode(G0, INPUT);
  pinMode(G1, INPUT);
  pinMode(G2, INPUT);
  pinMode(G3, INPUT);
  pinMode(G4, INPUT);
}

/************************主程序****************************/


void setup(){
  //串口初始化
  Serial.begin(115200);
  // 电机初始化
  Motor_Init();
  // Blinker 初始化
  BLINKER_DEBUG.stream(Serial);
  Blinker_Init();
  //灰度初始化
  Gray_Init();
  //光电初始化
  pinMode(GD, INPUT);
}
 

void loop(){
  // 每次循环必须调用
  Blinker.run();

  // 设置灰度数组
  static char g[5];
  // 将灰度读取结果用二维数组展示的变量
  static uint16_t gray_flag;
  //光电
  static char gd = 0;

  // 读光电
  gd = digitalRead(GD);

  // 读灰度
  g[0] = digitalRead(G0);
  g[1] = digitalRead(G1); 
  g[2] = digitalRead(G2); 
  g[3] = digitalRead(G3); 
  g[4] = digitalRead(G4);
  // 将灰度读取结果用二维数组展示
  gray_flag = digitalRead(G0)  
            + digitalRead(G1)*2 
            + digitalRead(G2)*4 
            + digitalRead(G3)*8 
            + digitalRead(G4)*16;
  
  

  if (state == 0) // 网络控制
  {
    if (DEBUG == 1) {
      Serial.println(voice_flag);
    }
    // 语音控制
    if (voice_flag){
      switch (voice_flag) {
        case 1: rspeed(130); lspeed(130); break;
        case 2: rspeed(-130); lspeed(-130); break;
        case 3: rspeed(130); lspeed(-130); break;
        case 4: rspeed(-130); lspeed(130); break;              
        case 5: rspeed(0); lspeed(0); break;
      }
      delay(400);
      voice_flag = 0;
      rspeed(0); lspeed(0);
    }
  }
  else if (state == 1) // 循迹避障
  {
    // error值，用百分比表示
    static int errorvalue;

    // 如果有障碍物
    if (gd == 0) {
      lspeedvalue = rspeedvalue = 0;
      Serial.println("gblocked!");
    } else {
      // 如果没有障碍物，开始循迹，errorvalue为偏差值，采用简单的差速控制
      Serial.println("tracking!");
      if (g[0] && g[1] && g[2] && g[3] && !g[4]) errorvalue = 95;
      if (g[0] && g[1] && g[2] && !g[3] && !g[4]) errorvalue = 50;
      if (g[0] && g[1] && !g[2] && !g[3] && g[4]) errorvalue = 20;
      if (g[0] && !g[1] && !g[2] && g[3] && g[4]) errorvalue = -20;
      if (!g[0] && !g[1] && g[2] && g[3] && g[4]) errorvalue = -50;
      if (!g[0] && g[1] && g[2] && g[3] && g[4]) errorvalue = -95;
      // 设置速度
      lspeedvalue = speedvalue - speedvalue / 100. * errorvalue ;
      rspeedvalue = speedvalue + speedvalue / 100. * errorvalue ;
    }
  }
  else if (state == 2) // 调试灰度
  {
    Serial.println(gray_flag, BIN);
  }
  else if (state == 3)
  {
    DebugMsg.print("1");
  }

  // 每次循环根据lspeedvalue和rspeedvalue设置速度
  setspeed();
}
