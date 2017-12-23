#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

const char* ssid = "*******";//路由器ssid
const char* password = "*******";//路由器密码
const char* mqtt_server_ip = "********";//服务器的地址

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;//存放时间的变量
char msg[200];//存放要发的数据
String load;
int var = 0;
//全局变量区域上界

//全局变量区域下界


//自动连WIFI接入网络
void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}


//等待，直到连接上服务器
void reconnect() {
  while (!client.connected()) {//如果没有连接上
    int randnum = random(0, 999);
    if (client.connect("DEMO" + randnum)) { //接入时的用户名，尽量取一个很不常用的用户名
      client.subscribe("inTopic");//接收外来的数据时的intopic
    }
    else {
      Serial.print("failed, rc=");//连接失败
      Serial.print(client.state());//重新连接
      Serial.println(" try again in 5 seconds");//延时5秒后重新连接
      delay(5000);
    }
  }
}


//用于接收服务器接收的数据
void callback(char* topic, byte* payload, unsigned int length) {
  load = "";
  for (int i = 0; i < length; i++) {
    load += (char)payload[i]; //串口打印出接收到的数据
  }
  decodeJson();
}


void  decodeJson() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(load);
  var = root["var"];
  //接收数据json处理区上界

  //添加其他自己的JSON收听处理方式就像这样  int temperature=root["temperature"];

  //接收数据json处理区下界
}


void setup() {
  //setup代码区域上界

  //Your Code

  //setup代码区域下界
  Serial.begin(9600);
  setup_wifi();//自动连WIFI接入网络
  client.setServer(mqtt_server_ip, 1883);//1883为端口号
  client.setCallback(callback); //用于接收服务器接收的数据
}


void loop() {
  if (!client.connected()) {
    reconnect();//确保连上服务器，否则一直等待。
  }
  client.loop();//MUC接收数据的主循环函数。
  //loop代码上届

  //Your Code

  //loop代码下界
  long now = millis();//记录当前时间
  if (now - lastMsg > 1000) {//每隔1000毫秒秒发一次数据
    encodeJson();
    client.publish("one", msg); //以one为TOPIC对外发送MQTT消息，这里的one需要修改成您的Topic
    lastMsg = now;//刷新上一次发送数据的时间
  }
}


void encodeJson() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root1 = jsonBuffer.createObject();
  //发送数据区上界

  //添加其他要发送的JSON包就像这样下面这句代码
  //root1["one"] = "one";

  //发送数据区下界
  root1.printTo(msg);
}
