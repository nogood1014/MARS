/*실제 로라 아두이노 쉴드를 설정하기 위한 소스코드 예제*/
#include <SoftwareSerial.h>
// USB to TTL을 이용해서 디버그 메시지를 보기 위해 선언
 

#include "SNIPE.h" 
//로라 아두이노 쉴드 라이브러리를 선언
// AT커맨드로 만들어진 로라 펌웨어를 아두이노 보드에서 동작시키기 위한 라이브러리 입니다. 

#define TXpin 11
#define RXpin 10
#define ATSerial Serial
// 로라 아두이노 쉴드와 아두이노 보드가 서로 통신하기 위해 Serial 클래스를 ATSerial 이름으로 선언
했습니다

String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  //16byte hex key
//로라 아두이노 쉴드끼리 통신 하는 암호키를 설정 

SoftwareSerial DebugSerial(RXpin,TXpin);
// SoftwareSerial은 DebugSerial 이름으로 TXpin 은 11번 핀, RXpin은 10번 핀으로 설정해서 선언 했습니다.

SNIPE SNIPE(ATSerial);
// AT커맨드로 만들어진 로라 펌웨어를 아두이노 보드에서 동작시키기 위해 

void setup() {
  ATSerial.begin(115200);
/*로라 Serial 속도 설정 : 로라 아두이노 쉴드 115200bps로 동작합니다. 
 따라서 아두이노 보드에서도 115200bps로 속도를 동일하게 설정해야 서로 통신할 수 있습니다.*/
 
  // put your setup code here, to run once:
  while(ATSerial.read()>= 0) {}
  while(!ATSerial);
/* Serial을 안전하게 초기화하는 코드
통신할때 초기에 쓰레기값이 읽혀지는 것을 방지하기 위해 Serial값이 읽혀지지 않을때까지 읽어서 초기화합니다. 
Serial에 이상이 있어서 클래스가 선언되지 않았을 경우 프로그램을 더이상 진행 시키지 않습니다.*/

  DebugSerial.begin(115200);
/*디버그 Serial 속도 설정: PC의 USB to TTL에 연결할 아두이노 보드의 Serial 속도를 115200bps로 설정했습니다. 
 PC의 테라텀 프로그램에서도 115200bps로 속도를 동일하게 설정해야 서로 통신할 수 있습니다.*/

  if (!SNIPE.lora_init()) {
    DebugSerial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }  
/*로라 아두이노 쉴드 초기화 : 아두이노 보드에서 로라 아두이노 쉴드로 초기화 명령을 보내서 로라쉴드가 정상동작 하는지 확인 합니다.
정상동작 하지 않을 경우 10회 재시도한 후 그래도 동작되지않으면 프로그램을 더이상 진행시키지 않습니다.*/
// PC 디버그 화면에서 “ SNIPE LoRa Initialization Fail! ” 메시지가 출력될 경우, 로라 아두이노 쉴드와
아두이노 보드를 분리 후 재결합 시켜보시기 바랍니다. 핀이 휘어져서 꼽혀있지 않거나 소켓에 덜 꼽
혀 있어서 제대로 연결되지 않을 경우, 해당 메시지가 출력되는 경우가 있습니다
  DebugSerial.println("SNIPE LoRa StartUP");
  /*성공시 SNIPE LoRa StartUP 출력*/
}

void loop() {
  DebugSerial.println("--- SNIPE LoRaPropertySetGet loop ---");

/*현재 설정된 로라 주파수를 String값으로 가져옵니다.*/
  DebugSerial.println(SNIPE.lora_getFreq());
  delay(500);
/*로라 주파수(채널)을 2번으로 설정합니다.
설정이 성공하면 true를 실패하면 false를 반환합니다.
설정 가능한 주파수 값은 922.1MHz입니다.*/
  DebugSerial.println(SNIPE.lora_setFreq(LORA_CH_2)==true?"TRUE":"FALSE");
  delay(500);
/*현재 설정된 로라 TX Power값을 String 값으로 가져옵니다
TX Power: 거리를 측정해 자신의 위치를 추정하는데 사용*/
  DebugSerial.println(SNIPE.lora_getTxp());
  delay(500);
/*로라 TX Power 값을 5dBm로 설정합니다. 성공 시 true 실패 시 false 반환*/
/*로라 TX Power는 0~14로 설정 가능 단 채널 1일 경우  국내 전파법의 의해 0~10까지만 설정할 수 있습니다.*/
  DebugSerial.println(SNIPE.lora_setTxp(5)==true?"TRUE":"FALSE"); //Tx Power (dBm)
  delay(500);
/*현재 설정된 로라 SF값을 String 값으로 가져 옵니다.*/
  DebugSerial.println(SNIPE.lora_getSf());
  delay(500);
/*로라 SF(Spreading Factor:확산 계수)값을 12로 설정합니다. 설정이 성공시 true 실패 시 false 반환*/
  DebugSerial.println(SNIPE.lora_setSf(LORA_SF_12)==true?"TRUE":"FALSE");
  delay(500);
/*현재 설정된 로라 RX TimeOut값을 String값으로 가져 옵니다*/
  DebugSerial.println(SNIPE.lora_getRxtout());
  delay(500);
/*!!로라 RX TimeOut값을 1000ms로 설정합니다.설정 성공시 true 실패시 false 반환
로라 RX TimeOut값은 0이상의 값을 설정해야 합니다. 설정한 시간동안 로라 아두이노 쉴드는 데이터가 수신되기를 기다립니다.*/
  DebugSerial.println(SNIPE.lora_setRxtout(1000)==true?"TRUE":"FALSE");
  delay(500);
/*현재 설징된 로라 TX TimeOut값을 String값으로 가져 옵니다*/
  DebugSerial.println(SNIPE.lora_getTxtout());
  delay(500);
/*로라 TX TimeOut값을 1000ms로 설정합니다.설정 성공시 true 실패시 false 반환
로라 TX TimeOut값은 0이상 4초 이하의 값을 설정해야합니다. 설정한 시간동안 로라 데이터를 송신합니다.*/
  DebugSerial.println(SNIPE.lora_setTxtout(1000)==true?"TRUE":"FALSE");
  delay(500);
/*(중요)로라 암호화/복호화 기능을 활성화 비활성화 할지 결정 합니다.
LORA_AES_OFF일 경우 비활성화, LORA_AES_ON일 경우 활성화
로라 아두이노 쉴드는 기본값이 암호화/복호화 기능 사용으로 설정되어 있으므로 LORA_AES_OFF로 설정 했을 경우에만 원래대로
기능을 설정하는 용도로 사용합니다.*/
  DebugSerial.println(SNIPE.lora_setAESMode(LORA_AES_OFF)==true?"TRUE":"FALSE");
  delay(500);
/*현재 설정된 로라 ApppKey값을 String값으로 가져옵니다.*/  
  DebugSerial.println(SNIPE.lora_getAppKey());
  delay(500);
/*lora_app_key로 선언된 String을 넣어서 AppKey를 변경합니다. 설정 성공시 true 실패시 false 반환
송신 로라노드와 수신 로라노드 모두 동일한 AppKey가 적용되어야 합니다. Key가 다른 경우 서로의 데이터를 읽을 수 없습니다.*/
  DebugSerial.println(SNIPE.lora_setAppKey(lora_app_key)==true?"TRUE":"FALSE");
  delay(500);  
/*아두이노 로라모듈 펌웨어 버전을 String값으로 가져옵니다.*/
  DebugSerial.println(SNIPE.lora_getVersion());
  delay(500);
}
