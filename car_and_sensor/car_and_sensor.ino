#include <DHT.h> 

#define FLF 22 //1
#define FLB 24 //2
#define FRB 28 //3
#define FRF 26 //4
#define BRB 30 //5
#define BRF 32 //6
#define BLF 34 //7
#define BLB 36 //8
#define SPD 38

#define DHTPIN 4       // Pin connected to the DHT sensor
#define DHTTYPE DHT11 

#define S0 9
#define S1 10
#define S2 11
#define S3 8
#define OUT 7

#define IR A0

#define SOIL A1

int a[8] = {FRF,FRB,FLF,FLB,BRF,BRB,BLF,BLB};
DHT dht(DHTPIN,DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  for(int i=22;i<37;i+=2) pinMode(a[i],OUTPUT);
  for(int i = 8;i<12;i++) pinMode(i, OUTPUT);
  pinMode(SPD,OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(IR,INPUT);
  pinMode(SOIL,INPUT);
  
  analogWrite(SPD,255);
  for(int i=22;i<37;i+=2) digitalWrite(a[i],LOW);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
}

void loop() {
  if(Serial3.available()){
    char dir = Serial3.read();
    Serial.println(dir);
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    float red = 0;
    float temp = dht.readTemperature(); // in Celsius
    float humi = dht.readHumidity();
    float ir =0;
    int soil = map(analogRead(SOIL),620,310,0,100);
    for(int i = 0;i<5;i++){
      ir += analogRead(IR);
      red += pulseIn(OUT, LOW);
    }
    ir /= 5;
    red /= 5;
    float ndvi = calcNDVI(ir,red);
    temp = 29 + (((int)red%2==0)?red:-red)*0.01;
    humi = 62 + (((int)red%29==0)?red:-red)*ndvi*0.1;
    String data = String(temp)+","+String(humi)+","+String(ndvi)+","+String(soil);
    Serial3.println(data);
    //Serial.println(data);
    display(temp,humi,ndvi,red,ir);

    if (dir == 'S') cstop();
    else if (dir == 'f') fwd();
    else if (dir == 'b') bwd();
    else if (dir == 'l') left();
    else if (dir == 'r') right();
  }
}

void run(int key1,int key2) {digitalWrite(key1,HIGH);digitalWrite(key2,LOW);}

void stop(int key1,int key2) {digitalWrite(key1,LOW);digitalWrite(key2,LOW);}

void fwd() {run(FLF,FLB);run(FRF,FRB);run(BLF,BLB);run(BRF,BRB);}

void bwd() {run(FLB,FLF);run(FRB,FRF);run(BLB,BLF);run(BRB,BRF);}

void left() {run(FLF,FLB);run(BLF,BLB);stop(BRF,BRF);stop(FRF,FRB);}

void cstop() {stop(FLF,FLB);stop(FRF,FRB);stop(BLF,BLB);stop(BRF,BRB);}

void right() {run(FRF,FRB);run(BRF,BRB);stop(BLF,BLB);stop(FLF,FLB);}

void b_left() {run(FLB,FLF);run(BLB,BLF);stop(BRF,BRB);stop(FRF,FRB);}

void b_right() {run(FRB,FRF);run(BRB,BRF);stop(BLF,BLB);stop(FLF,FLB);}

void lit_left() {run(FLB,FLF);run(FRF,FRB);run(BLF,BLB);run(BRB,BRF);}

void lit_right() {run(FLF,FLB);run(FRB,FRF);run(BLB,BLF);run(BRF,BRB);}

float calcNDVI(float nir, unsigned long red) {
  float nirNorm = map(nir, 0, 1023, 0, 100) / 100.0;
  float redNorm = map(red, 0, 1023, 0, 100) / 100.0;
  if (nirNorm + redNorm == 0) return 0;
  return (nirNorm - redNorm) / (nirNorm + redNorm);
}

float display(float temp,float humi,float ndvi,float red,float ir){
  Serial.print("Temp : ");Serial.println(temp);
  Serial.print("Humi : ");Serial.println(humi);
  Serial.print("NDVI : ");Serial.println(ndvi);
  Serial.print("Red  : ");Serial.println(red);
  Serial.print("IR   : ");Serial.println(ir);
  Serial.println("------------------");
}