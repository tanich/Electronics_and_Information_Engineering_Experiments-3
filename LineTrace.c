#include "config1827.h"
#include <xc.h>

//-----------------------------------------------------------
typedef unsigned char uchar;  // unsigned char を uchar に省略
void initpic();               // PICマイコンの初期化
void go(uchar);	              //直進
void stop(uchar);             //停止
void turn_r(uchar);           //右回転
void turn_l(uchar);           //左回転
void led_g(uchar);            //LED 緑色
void led_r(uchar);            //LED 赤色
void led_off(uchar);          //LED 消灯
void d_ms(uchar);             //__delay_ms()
void set_TMR0();              //TMR0割り込み設定
void Switch();                //スイッチ
uchar button = 0;             //スイッチの初期値OFF
void trace();                 //トレース
static void interrupt irq();  //割り込み
int i = 0;

//-----------------------------------------------------------
void main() {
	initpic();
	while(1){
		Switch();
		if(button == 0) { //センサーチェック用
			uchar g_led = PORTA & 0b00001111;
			PORTB = 0b10000000 + g_led;
			stop(1);
		}else{
			trace();
		}
	}
}

//モータ制御(周期10ms)--------------------------------------------------
void go(uchar num){
	PORTA = 0b11000000;
	d_ms(num);
	/*
	PORTA = 0b00000000;
	d_ms(10-num);
	*/
}

void turn_r(uchar num){
	PORTA = 0b10000000;
	d_ms(num);
	/*
	PORTA = 0b00000000;
	d_ms(10-num);
	*/
}

void turn_l(uchar num){
	PORTA = 0b01000000;
	d_ms(num);
	/*
	PORTA = 0b00000000;
	d_ms(10-num);
	*/
}

void stop(uchar num){
	PORTA = 0b00000000;
	d_ms(num);
}

//トレース----------------------------------------------------
void trace(){
	uchar r_led = PORTA & 0b00001111;
	PORTB = 0b10000000 + r_led;
	if(r_led == 0b1111){
		stop(1);
	}
	if(r_led == 0b1110){
		turn_r(1);
	}
	if(r_led == 0b1101){
		turn_r(1);
	}
	if(r_led == 0b1100){
		turn_r(1);
	}
	if(r_led == 0b1011){
		turn_l(1);
	}
	if(r_led == 0b1010){
		turn_r(1);
	}
	if(r_led == 0b1001){
		go(1);
	}
	if(r_led == 0b1000){
		go(1);
	}
	if(r_led == 0b0111){
		turn_l(1);
	}
	if(r_led == 0b0110){
		go(1);
	}
	if(r_led == 0b0101){
		turn_l(1);
	}
	if(r_led == 0b0100){
		turn_r(1);
	}
	if(r_led == 0b0011){
		turn_l(1);
	}
	if(r_led == 0b0010){
		turn_l(1);
	}
	if(r_led == 0b0001){
		go(1);
	}
	if(r_led == 0b0000){
		go(1);
	}
}

//LED--------------------------------------------------------
void led_g(uchar num){
	PORTB = 0b10001111;
	d_ms(num);
}

void led_r(uchar num){
	PORTB = 0b01001111;
	d_ms(num);
}

void led_off(uchar num){
	PORTB = 0b00000000;
	d_ms(num);
}

//待ち時間------------------------------------------------------
void d_ms(uchar num){	//変数は不可なので1msをたくさんやる
	for(uchar i=0; i< num; i++){
		__delay_ms(1);
	}
}

//スイッチ------------------------------------------------------
void Switch(){
	if(RA4 == 0) {
		if(button == 1) {
			button = 0;
		}else{
			button = 1;
		}
	}
}

//タイマ０割り込み設定--------------------------------------------
void set_TMR0(){
	OPTION_REG = 0b11000111;  //下位3bitでプリスケーラを指定
	TMR0 = 6;                 //TMRレジスタの初期設定
	TMR0IE = 1;               //タイマ割り込み可能
	GIE = 1;                  //割り込み全体の許可
}

//割り込みプログラム--------------------------------------------
static void interrupt irq(){
	if(TMR0IF == 1) {
		TMR0IF = 0;
		TMR0 = 
	}
}

//-----------------------------------------------------------
void initpic(){

	// PIC マイコンの初期化
	// 指導書（ソフトウェア編  p.10）

	// 動作クロックを4MHz にする
	OSCCON = 0b01101000;

	// PORTA, PORTB の初期化
	ANSELA = 0x00;
	ANSELB = 0x00;                        // Disable ADC
	TRISA  = 0b11111111; __delay_ms(500); // PICkit での ICSP 対策
	TRISA  = 0b00111111;                  // RA7,6:MotorLR  RA5,4:Switch  RA3-0:Sensor
	TRISB  = 0b00110000;                  // RB7,6:DynLEDHigh  RB5,4:ExtBoard  RB3-0:DynLEDLow

	// 出力値の初期化
	PORTA  = 0b00000000; // Motor Off
	PORTB  = 0b00000000; // LED Off
}
