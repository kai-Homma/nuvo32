// https://dev.classmethod.jp/articles/esp32-aws-iot-pubsub-basic/
// https://qiita.com/rockguitar67/items/4f028500d520dbf14be1
// https://dev.classmethod.jp/articles/m5stack-core2-for-aws-arduino-mqtt/
// https://qiita.com/arakirai1128/items/e243564729552853f426
// https://qiita.com/emiki/items/5daa0e31b8a8fa7c680a
// https://wazalabo.com/aws-iot-core-1-aws-lambda.html

#include <Arduino.h>
#include <ArduinoJson.h>
#include <string.h>
// #include <SPIFFS.h>

#include <nsp_driver.h>
#include <nsp_PlaySample.h>

#include <wifi.h>
#include <WiFiClientSecure.h>

#include <PubSubClient.h>
#include <secrets.h> //AWS証明書とWi-Fi設定

// #define UART_BUF_SIZE   (520)	


// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "nuvo32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "nuvo32/sub"

WiFiClientSecure client;
PubSubClient mqttClient(client);
// const int mqtt_port = 1883; // ブローカーのポート

// UINT8 WriteBuffer[UART_BUF_SIZE];
// UINT8 ReadBuffer[UART_BUF_SIZE];
UINT32 ID;
UINT32 adr = 0;
const int inputPin = 22; // GPIO22を入力ピンに設定
int command = 0;

// put function declarations here:
// Wi-Fi接続
void connect_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
      delay(500);
  }
  Serial.println("Connected!");
}


// publish(送信)する

void publish() {
    DynamicJsonDocument json_document(200);
    char json_string[100];
    json_document["command"] = command;
    serializeJson(json_document, json_string);
    mqttClient.publish(AWS_IOT_PUBLISH_TOPIC, json_string);
}

// メッセージを受信した際にすること
void callback(char* topic, byte* payload, unsigned int length) {
    
    // Serial.print("Received![");
    // Serial.print(topic);
    // Serial.println("]!");
    //ちゃんとやるならシリアルで誤認識しないような処理が必要？
    if ((char)payload[2]=='i'){
      Serial.print((char)payload[6]);
      Serial.print(":");
    }else if ((char)payload[2]=='e' && (char)payload[3]=='n'){
      Serial.print("end:");
      // // バイナリデータを書き込み
      // File file = SPIFFS.open("/ffs.bin", FILE_WRITE);
      // if (!file) {
      //     Serial.println("Failed to open file for writing");
      //     return;
      // }
      // file.write(ISP_Buffer, adr);
      // file.close();

      Serial.print((char)payload[8]);
      Serial.println((char)payload[9]);
      UART_ISPUpdateAllResourceSample();
    }else{
      // ファイルにデータを追記
      // File file = SPIFFS.open("/ffs.bin", FILE_APPEND);
      // if (!file) {
      //     Serial.println("Failed to open file for appending");
      //     return;
      // }
      // // ファイルにデータを書き込む
      // if (file.write(payload, length) != length) {
      //     Serial.println("Failed to write to file");
      //     file.close();
      //     return;
      // }
      // file.close();
      memcpy(&ISP_Buffer[adr], payload, length);
      Serial.println(ISP_Buffer[adr],HEX);
      adr+=length;
    }
}

// AWS IoTへMQTT接続
void connectAWSIoT() {
  Serial.println("Connecting to AWS");
  client.setCACert(AWS_CERT_CA);
  client.setCertificate(AWS_CERT_CRT);
  client.setPrivateKey(AWS_CERT_PRIVATE);
  mqttClient.setServer(AWS_IOT_ENDPOINT, 8883);  
  mqttClient.setCallback(callback);

  while (!mqttClient.connected()) {
    if (mqttClient.connect("nuvo32")) {
        Serial.println("Connected!");
        mqttClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);  // サブスクリプションの設定
    } else {
        Serial.print("Failed. Error state=");
        Serial.print(mqttClient.state());
        // Wait 5 seconds before retrying
        delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(inputPin, INPUT_PULLUP); // GPIO22を内蔵プルアップ付きの入力モードに設定
  Serial.begin(115200); // ---> UART 0　シリアルコンソール
  Serial.printf("Starting………….\n");

  // SPIFFSの初期化
  // if (!SPIFFS.begin(true)) {
  //     Serial.println("SPIFFS Mount Failed");
  //     return;
  // }
  // SPIFFS.format();

  HOST_SYS_Init();
  N_READ_ID(&ID);
  Serial.println(ID);
  N_PLAY(1);
                                                                                                                       
  while (digitalRead(inputPin) == HIGH) {
    // ピンがHIGHの間、待つ
    delay(100); // 100ミリ秒待つ
  }

  connect_wifi();
  connectAWSIoT();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!mqttClient.connected()) {
    connectAWSIoT();
  }
  if (digitalRead(inputPin) == LOW) {
    // ピンがLOWで送信
    Serial.println("send!");
    delay(300);
    adr = 0;
    publish(); // 現在の状態をpublishする 
    if (command == 0){
      command = 1;
    }else{
      command = 0;
    }
  }

  mqttClient.loop(); // Subscribeする時は、必須
  delay(100);
}

// put function definitions here:
