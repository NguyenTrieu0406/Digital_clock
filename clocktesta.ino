int counter[6] = {0, 0, 0, 0, 0, 0};
int counter1[6] = {0, 0, 0, 0, 0, 0};
int save1;
int save2;
int save3;
int save4;
int save5;
int save6;
int but3;
int but2;
int waitnum;
int waitnuma;
int waitnumb;
int wait1;
int wait2;
int wait3;
int wait4;
int wait5;
unsigned char binary_pattern[] = {  // hiển thị trên màn các số 0,1,2,3,4,5,6,7,8,9
  0b11111100,
  0b01100000,
  0b11011010,
  0b11110010,
  0b01100110,
  0b10110110,
  0b10111110,
  0b11100000,
  0b11111110,
  0b11110110,
};
unsigned int n = 10;
unsigned int c = 0;
int count_button_3 = 1;
int count_button_1 = 0;
int counter_second = 0;
int counter_second1 = 0;
int count_button_2[6] = {1, 1, 1, 1, 1, 1};
int button_3_value;
int button_2_value ;
int button_1_value;
int button_4_value;
int count_button_4 = 0;
int button_5_value;
int count_button_5 = 0;
volatile bool execute = false;
volatile bool execute1 = false;
volatile bool execute2 = false;
volatile bool execute3 = false;
volatile bool execute4 = false;
volatile bool execute5 = false;
volatile bool execute6 = false;
volatile bool execute_normal = false;
volatile bool execute_a = false;
volatile bool execute_b = false;
bool checkB4 = true;
int lastButton1State = HIGH;
int reading1;
int lastDebounceCount1 = 0;
int lastButton2State = HIGH;
int reading2;
int lastDebounceCount2 = 0;
int lastButton3State = HIGH;
int reading3;
int lastDebounceCount3 = 0;
int lastButton4State = HIGH;
int reading4;
int lastDebounceCount4 = 0;
int lastButton5State = HIGH;
int reading5;
int lastDebounceCount5 = 0;
void setup()
{
  DDRB |= 0b00110100;
  DDRD |= 0b11111100;
  DDRC |= 0b00000000;
  PORTC = 0b00000011;
  PORTB = 0b00001011;
  PORTD = (0 << PD1) | (1 << PD0);
  Serial.begin(9600);
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  // Set the value that the counter has to reach before it triggers an interrupt
  OCR1A = 49870;
  OCR1B = 3;
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);
  //
  sei();
}
ISR(TIMER1_COMPA_vect)
{
  OCR1A += 49870;
  if (checkB4){
  counter_second++;
  }
  second_timer1();
  execute4 = true;
}
ISR(TIMER1_COMPB_vect)
{
  OCR1B += 3;
  execute_normal = true;
  execute_a = true;
  execute_b = true;
  execute2 = true;
  execute3 = true;
  execute1 = true;
  execute5 = true;
  execute6 = true;
}
void clock_signal(void) {
  PORTB = (1 << PB2);
}
void latch_enable(void) // hàm điều khiển latch của IC
{
  PORTB |= (1 << PB5);
  PORTB |= (0 << PB5);
}
void send_data(unsigned int data_out) // hàm điều khiển data
{
  int i;
  for (i = 0 ; i < 8 ; i++)
  {
    if ((data_out >> i) & (0x01)) { // sử dụng phương pháp dịch bit và AND với số hệ hex  0000 0001 theo cho mỗi số hiển thị để thiết lập data cho IC
      PORTB = (1 << PB4) | ( 0 << PB2);
    }
    else {
      PORTB = (0 << PB4) | ( 0 << PB2);
    }
    PORTB   = (1 << PB2);
  }
  latch_enable(); // chuyển dữ liệu đầu ra
}
unsigned int a = 0;
void invidual_control(unsigned int a) { // hàm này phục vụ cho việc quét led
  if (a == 0) {
    PORTD = (0 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2) ;
  }
  if (a == 1) {
    PORTD = (1 << PD7) | (0 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2);
  }
  if (a == 2) {
    PORTD = (1 << PD7) | (1 << PD6) | (0 << PD5) | (1 << PD4) | (1 << PD3) | (1 << PD2);
  }
  if (a == 3) {
    PORTD = (1 << PD7) | (1 << PD6) | (1 << PD5) | (0 << PD4) | (1 << PD3) | (1 << PD2);
  }
  if (a == 4) {
    PORTD = (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (0 << PD3) | (1 << PD2);
  }
  if (a == 5) {
    PORTD = (1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3) | (0 << PD2);
  }
}
void normal_mode(void) {
  if (execute_normal) {
    waitnum++;
    if (waitnum == 1) {
      waitnum = 0;
      send_data(binary_pattern[counter[c]]);
      invidual_control(a);
      a++;
      if (a > 5) {
        a = 0;
      }
      c++;
      if (c > 5 )
        c = 0;
    }
    execute_normal = false;
  }
}
void first_number_a(void) { // các hàm tương tự như hàm này phục vụ cho việc hiển thị khi thay đổi số trong led
  if (execute_a) {
    waitnuma++;
    if (waitnuma == 1) {
      waitnuma = 0;
      send_data(binary_pattern[counter1[c]]);
      invidual_control(a);  // số đc thay đổi sẽ giữ nguyên, các số còn lại nhấp nhá
      a++;
      if (a > 5) {
        a = 1;
      }
      c++;
      if (c > 5 ) {
        c = 1;
      }
    }
    execute_a = false;
  }
}
void first_number_b(void) {
  if (execute_b) {
    send_data(binary_pattern[counter1[0]]);
    invidual_control(0);
    execute_b = false;
  }
}
void second_number_a(void) {
  if (execute_a) {
    waitnuma++;
    if (waitnuma == 1) {
      waitnuma = 0;
      send_data(binary_pattern[counter1[c]]);
      invidual_control(a);
      a++;
      if (a == 1) {
        a = 2;
      }
      if (a > 5) {
        a = 0;
      }
      c++;
      if (c == 1) {
        c = 2;
      }
      if (c > 5 ) {
        c = 0;
      }
    }
    execute_a = false;
  }
}
void second_number_b(void) {
  if (execute_b) {
    send_data(binary_pattern[counter1[1]]);
    invidual_control(1);
    execute_b = false;
  }
}
void third_number_a(void) {
  if (execute_a) {
    waitnuma++;
    if (waitnuma == 1) {
      waitnuma = 0;
      send_data(binary_pattern[counter1[c]]);
      invidual_control(a);
      a++;
      if (a == 2) {
        a = 3;
      }
      if (a > 5) {
        a = 0;
      }
      c++;
      if (c == 2) {
        c = 3;
      }
      if (c > 5 ) {
        c = 0;
      }
    }
    execute_a = false;
  }
}
void third_number_b(void) {
  if (execute_b) {
    send_data(binary_pattern[counter1[2]]);
    invidual_control(2);
    execute_b = false;
  }
}
void fourth_number_a(void) {
  if (execute_a) {
    waitnuma++;
    if (waitnuma == 1) {
      waitnuma = 0;
      send_data(binary_pattern[counter1[c]]);
      invidual_control(a);
      a++;
      if (a == 3) {
        a = 4;
      }
      if (a > 5) {
        a = 0;
      }
      c++;
      if (c == 3) {
        c = 4;
      }
      if (c > 5 ) {
        c = 0;
      }
    }
    execute_a = false;
  }
}
void fourth_number_b(void) {
  if (execute_b) {
    send_data(binary_pattern[counter1[3]]);
    invidual_control(3);
    execute_b = false;
  }
}
void fifth_number_a(void) {
  if (execute_a) {
    waitnuma++;
    if (waitnuma == 1) {
      waitnuma = 0;
      send_data(binary_pattern[counter1[c]]);
      invidual_control(a);
      a++;
      if (a == 4) {
        a = 5;
      }
      if (a > 5) {
        a = 0;
      }
      c++;
      if (c == 4) {
        c = 5;
      }
      if (c > 5 ) {
        c = 0;
      }
    }
    execute_a = false;
  }
}
void fifth_number_b(void) {
  if (execute_b) {
    send_data(binary_pattern[counter1[4]]);
    invidual_control(4);
    execute_b = false;
  }
}
void sixth_number_a(void) {
  if (execute_a) {
    waitnuma++;
    if (waitnuma == 1) {
      waitnuma = 0;
      send_data(binary_pattern[counter1[c]]);
      invidual_control(a);
      a++;
      if (a >= 5) {
        a = 0;
      }
      c++;
      if (c >= 5) {
        c = 0;
      }
    }
    execute_a = false;
  }
}
void sixth_number_b(void) {
  if (execute_b) {
    send_data(binary_pattern[counter1[5]]);
    invidual_control(5);
    execute_b = false;
  }
}
void button_3(void) { // hàm này điều khiển nút S1
  if (execute3) {
    wait3++;
    execute3 = false;
  }
  //  PORTB |= (1 << PB5);
  // reading3 = PORTB&0b00100000;
  if (reading3 != lastButton3State) {
    lastDebounceCount3 = wait3;
  }

  if ((wait3 - lastDebounceCount3) > 2) {
    if (reading3 != button_3_value) {
      button_3_value = reading3;
      if ( button_3_value == 0) {
        count_button_3++;
        if (count_button_3 > 6 ) {
          count_button_3 = 1;
        }
      }
    }
  }
  lastButton3State = reading3;
}
void button_2(void) { // hàm điều khiển nút S2 trong sơ đồ mạch, dùng để đếm số lần nhấn của nút và biến chúng thành giá trị hiển thị trên led
  if (execute2) {
    wait2++;
    execute2 = false;
  }
  // reading2 = digitalRead(B2);
  if (reading2 != lastButton2State) {
    lastDebounceCount2 = wait2;
  }

  if ((wait2 - lastDebounceCount2) > 2) {
    if (reading2 != button_2_value) {
      button_2_value = reading2;
      if ( button_2_value == 0 && count_button_3 == 1 ) {
        count_button_2[0] ++;
        if (count_button_2[0] > n  ) {
          count_button_2[0] = 1;
        }
      }
      if ( button_2_value == 0 && count_button_3 == 2 ) {
        count_button_2[1] ++;
        if (count_button_2[1] > n  ) {
          count_button_2[1] = 1;
        }
      }
      if ( button_2_value == 0 && count_button_3 == 3 ) {
        count_button_2[2] ++;
        if (count_button_2[2] > n  ) {
          count_button_2[2] = 1;
        }
      }
      if ( button_2_value == 0 && count_button_3 == 4) {
        count_button_2[3] ++;
        if (count_button_2[3] > n  ) {
          count_button_2[3] = 1;
        }
      }
      if ( button_2_value == 0 && count_button_3 == 5) {
        count_button_2[4] ++;
        if (count_button_2[4] > n  ) {
          count_button_2[4] = 1;
        }
      }
      if ( button_2_value == 0 && count_button_3 == 6) {
        count_button_2[5] ++;
        if (count_button_2[5] > n  ) {
          count_button_2[5] = 1;
        }
      }
    }
  }
  lastButton2State = reading2;
}
void controller(void) {    // hàm này dùng để giới hạn sự thay đổi khi thay đổi các giá trị trên led
  if (count_button_3 == 1) { //(led ngoài cùng giới hạn từ 0 - 2, led ở vị trí thứ hai từ 0 - 9 ),
    n = 3;                  // vị trí 3 từ 0 - 5, cuối từ 0 - 9
  }
  if (count_button_3 == 2) {
    n = 10;
  }
  if (count_button_3 == 3) {
    n = 6;
  }
  if (count_button_3 == 4) {
    n = 10;
  }
  if (count_button_3 == 5) {
    n = 6;
  }
  if (count_button_3 == 6) {
    n = 10;
  }
}
void changer(void) { // hàm này để thay đổi các bị trí và giá trị trên led khi nhấn S2 và S3
  button_3();
  button_2();
  if (count_button_3 == 1) {
    counter1[0] = count_button_2[0] - 1;
  }
  if (count_button_3 == 2) {
    counter1[1] = count_button_2[1] - 1;
  }
  if (count_button_3 == 3) {
    counter1[2] = count_button_2[2] - 1;
  }
  if (count_button_3 == 4) {
    counter1[3] = count_button_2[3] - 1;
  }
  if (count_button_3 == 5) {
    counter1[4] = count_button_2[4] - 1;
  }
  if (count_button_3 == 6) {
    counter1[5] = count_button_2[5] - 1;
  }
}
void second_timer1(void) {
  if (counter_second == 5) {
    counter_second = 0;
    counter[5]++;
    //count_button_2[5] = counter[5];
  }
  if ( counter[5] > 9) {
    counter[5] = 0;
    counter[4]++;
    //count_button_2[4] = counter[4];
  }
  if ( counter[4] > 5) {
    counter[4] = 0;
    counter[3]++;
    //count_button_2[3] = counter[3];
  }
  if ( counter[3] > 9) {
    counter[3] = 0;
    counter[2]++;
    //count_button_2[2] = counter[2];
  }
  if ( counter[2] > 5) {
    counter[2] = 0;
    counter[1]++;
    //count_button_2[1] = counter[1];
  }
  if ( counter[1] > 9 && counter[0] < 2 ) {
    counter[1] = 0;
    counter[0]++;
    //count_button_2[0] = counter[0];
  }
  if ( counter[0] == 2 && counter[1] > 3) {
    counter[1] = 0;
    counter[0] = 0;
    //count_button_2[0] = counter[0];
    // count_button_2[1] = counter[1];
  }
}
void save_value1(void) {
  counter[5] = counter1[5];
  counter[4] = counter1[4];
  counter[3] = counter1[3];
  counter[2] = counter1[2];
  counter[1] = counter1[1];
  counter[0] = counter1[0];
}
void save_value2(void) {
  counter1[5] = counter[5];
  counter1[4] = counter[4];
  counter1[3] = counter[3];
  counter1[2] = counter[2];
  counter1[1] = counter[1];
  counter1[0] = counter[0];
}
void save_value3(void) {
  count_button_2[0] = counter1[0] + 1;
  count_button_2[1] = counter1[1] + 1;
  count_button_2[2] = counter1[2] + 1;
  count_button_2[3] = counter1[3] + 1;
  count_button_2[4] = counter1[4] + 1;
  count_button_2[5] = counter1[5] + 1;
}
void chosen_blink1(void) { // hàm này để nháy các led còn lại khi chọn led cần thay đổi
  if (count_button_3 == 1 && count_button_1 == 1 ) {
    if (execute4) {
      first_number_b();
      execute4 = false;
    }
    else {
      first_number_a();
    }
  }
  if (count_button_3 == 2 && count_button_1 == 1) {
    if (execute4) {
      second_number_b();
      execute4 = false;
    }
    else {
      second_number_a();
    }
  }
  if (count_button_3 == 3 && count_button_1 == 1) {
    if (execute4) {
      third_number_b();
      execute4 = false;
    }
    else {
      third_number_a();
    }
  }
  if (count_button_3 == 4 && count_button_1 == 1) {
    if (execute4) {
      fourth_number_b();
      execute4 = false;
    }
    else {
      fourth_number_a();
    }
  }
  if (count_button_3 == 5 && count_button_1 == 1) {
    if (execute4) {
      fifth_number_b();
      execute4 = false;
    }
    else {
      fifth_number_a();
    }
  }
  if (count_button_3 == 6 && count_button_1 == 1) {
    if (execute4) {
      sixth_number_b();
      execute4 = false;
    }
    else {
      sixth_number_a();
    }
  }
}
void control_button1(void) { // hàm này để điều khiển nút S3
  if (execute1) {
    wait1++;
    execute1 = false;
  }
  // reading1 = digitalRead(But1);
  if (reading1 != lastButton1State) {
    lastDebounceCount1 = wait1;
  }
  if ((wait1 - lastDebounceCount1) > 10) {
    if (reading1 != button_1_value) {
      button_1_value = reading1;
      if ( button_1_value == 0) {
        count_button_3  = 1;
        count_button_4  = 0;
        count_button_5  = 0;
        count_button_1++;
        if (count_button_1 > 1 ) {
          count_button_1 = 0;
        }
      }
    }
  }
  lastButton1State = reading1;
}
void control_button4(void) { // hàm này để điều khiển nút S3
  if (execute5) {
    wait4++;
    execute5 = false;
  }
  if (reading4 != lastButton4State) {
    lastDebounceCount4 = wait4;
  }
  if ((wait4 - lastDebounceCount4) > 2) {
    if (reading4 != button_4_value) {
      button_4_value = reading4;
      if ( button_4_value == 0) {
        count_button_4++;
        checkB4 = false;
        if (count_button_4 > 1 ) {
          count_button_4 = 0;
          checkB4 = true;
        }
      }
    }
  }
  lastButton4State = reading4;
}
void control_button5(void) { // hàm này để điều khiển nút S3
  if (execute6) {
    wait5++;
    execute6 = false;
  }
  if (reading5 != lastButton5State) {
    lastDebounceCount5 = wait5;
  }
  if ((wait5 - lastDebounceCount5) > 2) {
    if (reading5 != button_5_value) {
      button_5_value = reading5;
      if ( button_5_value == 0) {
        count_button_5++;
        if (count_button_5 > 1 ) {
          count_button_5 = 0;
        }
      }
    }
  }
  lastButton5State = reading5;
}
void button_1(void) {
  control_button4();
  control_button5();
  save_value2();
  if (count_button_4 == 1 ) {
    checkB4 = false;
  }
  if (count_button_5 == 1) {
    count_button_5 = 0;
    counter1[5] = 0;
    counter1[4] = 0;
    counter1[3] = 0;
    counter1[2] = 0;
    counter1[1] = 0;
    counter1[0] = 0;
  }
  controller();
  save_value3();
  changer();
  save_value1();
}
void loop() {
  PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB3);
  PORTC |= (1 << PC0) | (1 << PC1);
  reading2 = PINB & 0b00000010;
  reading1 = PINB & 0b00000001;
  reading3 = PINB & 0b00001000;
  reading4 = PINC & 0b00000001;
  reading5 = PINC & 0b00000010;
  control_button1();
  if (count_button_1 == 0) {
    normal_mode();
    checkB4 = true;
  }
  else  {
    chosen_blink1();
    button_1();
  }
  Serial.println(reading1);

}
