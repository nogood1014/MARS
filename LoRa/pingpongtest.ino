/*로라 아두이노 쉴드끼리 통신하기! 핑! 퐁! 티키타카!!*/
/*일반 입출력 데이터 핀을 RX, TX핀으로 동작할 수 있게 해주는 라이브러리*/
#include <SoftwareSerial.h>

/*로라 쉴드를 사용하려면 다음의 라이브러리를 추가하자!
https://github.com/codezoo-ltd/SNIPE */
#include "SNIPE.h"

#define PING  1 //보드 하나 PONG 보드에서는 이거 삭제!
#define PONG  2 //보드 다른 하나

#define CODE  PING    //컴파일 코드 구분을 위한 define, 각각의 보드 업로드 시 이 부분을 변경해야 함!

//USB to TTL을 이용해서 디버그 메시지를 보기 위한 tx, rx 통신 핀 지정 
#define TXpin 11
#define RXpin 10

//로라 아두이노 쉴드와 아두이노 보드가 서로 통신하기 위해 Serial 클래스를 ATSerial 이란 이름으로 선언
#define ATSerial Serial 

//로라 아두이노 쉴드끼리 통신 하는 암호 키 설정 <나만의 암호키 만들기!>
//16byte hex key
String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

//USB to TTL을 이용해서 Debug를 시리얼에 나타내 디버그 메시지를 보기 위해 선언
//1) SoftwareSerial을 DebugSerial이란 이름으로 2)TXpin 11번 핀, RXpin 10번 핀으로 설정/선언(객체 생성)
SoftwareSerial DebugSerial(RXpin,TXpin);

//SNIPE 객체 생성
SNIPE SNIPE(ATSerial);

void setup() {
  //직렬 통신의 속도 (전송 속도)를 설정
  //시리얼 포트를 열고, 로라 시리얼 전송 속도를 115200bps로 설정
  ATSerial.begin(115200); //115200: 로라 쉴드가 이 bps에서 동작함

  // put your setup code here, to run once:
  //
  while(ATSerial.read()>= 0) {}
  //시리얼을 안전하게 초기화 : 통신 초기에 쓰레기 값이 읽혀지는 것을 방지하기 위해 시리얼 버퍼의 수신된 데이터를 첫 번째 바이트만 시리얼 값이 읽혀지지않을때까지 무한대로 읽어와 초기화 함.
  while(!ATSerial); //읽혀지는 시리얼 값이 없을 때 까지

  //디버그 콘솔을 개방하고, 디버그 시리얼 전송 속도를 115200bps로 설정
  DebugSerial.begin(115200); 

  /* SNIPE LoRa Initialization */
  //로라 아두이노 쉴드 초기화
  if (!SNIPE.lora_init()) { //제대로 초기화되지 않으면 
    DebugSerial.println("SNIPE LoRa Initialization Fail!"); //정상동작이 되지 않는다는 문구 출력 => 이 문구 발생 시 보드 연결 문제이므로 다시 연결해 볼 것!
    while (1); //10회 재시도
  }

  /* SNIPE LoRa Set Appkey */
  //로라 앱 키 설정: 성공시 true, 실패 시 flase
  if (!SNIPE.lora_setAppKey(lora_app_key)) { //로라 앱키 설정에 실패 했을 경우
    DebugSerial.println("SNIPE LoRa app key value has not been changed");
  }
  
  /* SNIPE LoRa Set Frequency */
  //로라 주파수(=채널)을 1번으로 설정: 성공시 true, 실패 시 flase
  if (!SNIPE.lora_setFreq(LORA_CH_1)) { //로라 주파수 설정에 실패 했을 경우
    DebugSerial.println("SNIPE LoRa Frequency value has not been changed");
  }

  /* SNIPE LoRa Set Spreading Factor */
  //SF(확산계수) 설정: 성공시 true, 실패 시 flase
  if (!SNIPE.lora_setSf(LORA_SF_7)) {//로라 SF(spreading factor)값 설정에 실패 했을 경우
    DebugSerial.println("SNIPE LoRa Sf value has not been changed");
  }

  /* SNIPE LoRa Set Rx Timeout 
   * LORA_SF_12를 선택하면 
   * RX Timout은 5000보다 큰 값을 사용합니다 
  */
   /*모듈의 Rx Timeout을 설정*/
  if (!SNIPE.lora_setRxtout(5000)) { //수신 대기 시간 설정에 실패 했을 경우
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  

  //모든 설정(set)의 확인이 끝나면 loop문 gogo
  DebugSerial.println("SNIPE LoRa PingPong Test");
}

void loop() {

 /*PING코드*/
#if CODE == PING //데이터 송신 보드
        if (SNIPE.lora_send("PING")) //로라로 "PING"이라는 문자열을 송신! 영어 기준, 한번 통신 시 240글지 즉 240바이트까지 송신 가능! 송신 대기 시간 설정: lora_setTxout
        {
          DebugSerial.println("send success");
          /*모듈이 수신 한 데이터를 반환*/
          String ver = SNIPE.lora_recv(); //로라로 신호를 수신, 한번에 240바이트. 수신 대기 시간 설정: lora_setRxtout (기본 2초)
          DebugSerial.println(ver);

          if (ver == "PONG") //SNIPE.lora_recv()을 통해 수신한 데이터가 "PONG"일 경우 (정상적으로 데이터를 수신했을 경우)
          {
            DebugSerial.println("recv success"); //수신 성공했음을 시리얼에 표시
           /* RSSI 모듈을 가지고 옴*/
            DebugSerial.println(SNIPE.lora_getRssi()); //로라 아두이노 쉴드에 저장한 RSSI값(수신감도)과 
            DebugSerial.println(SNIPE.lora_getSnr()); //SNR값(신호세기)을 가져와 표시 <= 통신 상태를 점검하기 위해
          }
          else //수신에 실패 했을 경우
          {
            DebugSerial.println("recv fail"); //수신 실패했음을 시리얼에 표시
            delay(500);
          }
        }
       delay(1000);
 
/*PONG코드*/       
#elif CODE == PONG //데이터 수신용 보드
        String ver = SNIPE.lora_recv(); //PONG보드 이므로 데이터가 수신되었는지 확인
        delay(300);

        DebugSerial.println(ver);
        
        if (ver == "PING" )//SNIPE.lora_recv()을 통해 수신한 데이터가 "PING"일 경우 (정상적으로 데이터를 수신했을 경우)
        {
          DebugSerial.println("recv success"); //수신 성공했음을 시리얼에 표시
          DebugSerial.println(SNIPE.lora_getRssi());//로라 아두이노 쉴드에 저장한 RSSI값(수신감도)과 
          DebugSerial.println(SNIPE.lora_getSnr());//SNR값(신호세기)을 가져와 표시 <= 통신 상태를 점검하기 위해

          if(SNIPE.lora_send("PONG")) // "PONG" 이라는 문자열을 송신
          {
            DebugSerial.println("send success");//송신 성공했음을 시리얼에 표시
          }
          else
          {
            DebugSerial.println("send fail");//송신 실패 했음을 시리얼에 표시
            delay(500);
          }
        }
       delay(1000);
#endif
}
