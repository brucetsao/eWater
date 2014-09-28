void setup() {
  pinMode(4, INPUT);        // 4號接腳輸入
  pinMode(13, OUTPUT);      // 13號接腳輸出
}

void loop() {
  int i = digitalRead(4);   // 讀取4號接腳的值
  if (i == HIGH) {          // 如果4號接腳的值為高電位，即按下按鈕
    delay(1000);            // 延遲1秒後
    digitalWrite(13, HIGH); // 點亮13號接腳的LED燈
    delay(5000);            // 延遲5秒後
    digitalWrite(13, LOW);  // 熄滅
  }
}

