#include <Wire.h>
#include <OneWire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "EEPROM.h"
#include <stdint.h>
#include <math.h>
#include <esp_task_wdt.h>
#include <string.h>

#define FIRMWAREVERSION "v210319"

uint16_t Gi_Test_Reset_Counter = 99;
//======PID==================================================
#define PID_K_P 	3.0
#define PID_K_I 	-0.1
#define PID_K_D 	50

#define TEMP_DEFAULT_C	1.115	

double Gd_PID_k_P = PID_K_P;
double Gd_PID_k_I = PID_K_I;
double Gd_PID_k_D = PID_K_D;
//----------------------------
double Gd_PID_Diff = 0.0;
double Gd_ADC = 100.0;

double Gd_Cal_PID_P = 0.0;
double Gd_Cal_PID_I = 0.0;
double Gd_Cal_PID_D = 0.0;
//-----------------------------------------------------------

/* 
esp_task_wdt_reset();
ESP.restart()
*/

//====WiFi===================================================
#define MAX_SRV_CLIENTS		4
WiFiMulti wifiMulti;
WiFiClient client;
WiFiServer server(10001);
WiFiClient serverClients[MAX_SRV_CLIENTS];
boolean Gb_WiFi_Connected = false;
//===========================================================


#define PIN_BLUE_LED			2

#define Pin_SCLK 				14
#define Pin_MISO 				13
#define	Pin_SS					15

#define PIN_CURRENT_0 			34		
#define PIN_VOLTAGE_0			35	
#define PIN_CURRENT_1			36	
#define PIN_VOLTAGE_1			39	

// Buttons UP NOTES: must short GPIO-16 and GPIO-5 together.  GPIO-16 cannot be used as input!!!!	190621 
#define PIN_BUTTON_UP_			16
#define PIN_BUTTON_UP			5

#define PIN_BUTTON_DN			23
#define PIN_BUTTON_SELECT		18
#define PIN_BUTTON_COMMON		0

#define PIN_PULL_LO_FOR_RESET	32
#define PIN_TC_PWR_AUX			26
#define PIN_TC_PWR_MAIN			25

//-----------------------------------------------
#define SET_TEMP_MAX_C			35.0
#define SET_TEMP_MIN_C			-9.9
#define SET_TEMP_DEFAULT_C		1.0

#define SET_TEMP_MAX_F			95.0
#define SET_TEMP_MIN_F			-15.0
#define SET_TEMP_DEFAULT_F		33.0

// Address in the ESP-32 EEPROM - starting from top down..
#define FIRST_TIME_MARKER_VALUE		B10101010

#define FLASH_ADD						0
#define FLASH_ADD_FIRST_TIME_MARKER		FLASH_ADD 
#define FLASH_ADD_SET_TEMP_UNITS		FLASH_ADD + 1
#define FLASH_ADD_SET_TEMP_C			FLASH_ADD + 2 
#define FLASH_ADD_PID_K_P				FLASH_ADD + 2 + 4
#define FLASH_ADD_PID_K_I				FLASH_ADD + 2 + 8
#define FLASH_ADD_PID_K_D				FLASH_ADD + 2 + 12
#define FLASH_ADD_TEST					FLASH_ADD + 2 + 16
#define FLASH_ADD_TEMP_MAIN_OFFSET		FLASH_ADD + 2 + 18
#define FLASH_ADD_TEMP_AUX_OFFSET		FLASH_ADD + 2 + 20
//-----------------------------------------------
#define POWER_SUPPLY_0 			0
#define POWER_SUPPLY_1 			1

//===================Display=====================

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET 	4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif
//------------------------------------------------

//uninitalised pointers to SPI objects
//SPIClass * hspi = NULL;
#define cCR			13
#define cLF			10

#define LED_ESP_WROOM_32_BLUE 2

// char Gc_Blink;
char Gc_inValue = 0;
char Gc_Debug;

//----------------------------------------------------------
boolean Gb_DS18B20_Resetting = false;
char Gc_DS18B20_Skip_Update = 0;

// If DS18B02 has temperature change after n second, reset it by cutting the power for 2 seconds.
#define DS18B20_RESET_TIME_MS	2000

#define DS18B02_RESET_DELAY_IN_500MS  30		// 15 Seconds to wait for temperature change.
int16_t Gi_DS18B02_Reset_Delay_500ms = 0;
int16_t Gi_DS18B02_Reset_Counter = 0;
double Gd_Temp_Main_C_Old = -100.0;


char Gc_Watchdog = 5;
//=========================================================

#define BUF_SIZE 	2048
char Gs_Buf_Console[BUF_SIZE];
char *Gs_Buf_Console_ptr = (char *) &Gs_Buf_Console[0];
uint16_t Gi_Buf_Console_Index = 0;

char Gs_Buf_Serial_In[BUF_SIZE];
char Gs_Tx_Buff[BUF_SIZE];
int Gi_TX_Buff_Index = 0;

uint16_t Gi_Buf_Index = 0;

#define ONE_WIRE_BUS_0 		32                // DS18B20 data wire is connected to input xx
#define ONE_WIRE_BUS_1		33
OneWire ds(ONE_WIRE_BUS_0);        // create a oneWire instance to communicate with temperature IC
OneWire ds1(ONE_WIRE_BUS_1);        // create a oneWire instance to communicate with temperature IC

#define OUTPUT_ENABLE_0		19
#define OUTPUT_ENABLE_1		27

byte Gb_data[12];
byte Gb_addr[8];

float Gf_Old_Temp0;
float Gf_Old_Temp1;

char Gb_Output_Enabled_0;
char Gb_Output_Enabled_1;

double Gd_Temper_Set_C = 1.0;
float Gf_Temper_Set_EEPROM_C = 0.0;

boolean Gb_metrics;
boolean Gb_metrics_EEPROM;

boolean Gb_DS18B20_Found;
boolean Gb_DS18B20_Powered_Down;

boolean Gb_DS18B20_Found1;

char Gc_Update_EEPROM_CountDown = 0;
#define UPDATE_EEPROM_DELAY_SECONDS		5

#define UINT	unsigned int

double Gd_Temp_Main_C = -127.0;
float  Gf_Temp_Main_C_Offset = 0.0;
double Gd_Temp_Aux_C = -127.0;
float  Gf_Temp_Aux_C_Offset = 0.0;
double Gd_Temp_Main_F = -127.0;
double Gd_Temp_Aux_F = -127.0;

unsigned long Gl_Time_Event_GoFullScale;


#define OPER_MODE_1_ONLY	1
#define OPER_MODE_2_ONLY	2
#define OPER_MODE_BOTH		3

char Gc_Oper_Mode = OPER_MODE_BOTH;

boolean Gb_Display_Offset;
char Gc_Val = 0;
char Gc_DAC_Target = 255;
char Gc_Val_Saved = 0;
char c_Indicator = 0;

char Gc_Button_Delay = 0;
char Gc_Button_Select_Holding = 0;
char Gc_Button_Select_Holding2 = 0;
//==================================================================================
uint16_t Gi_Debug1 = 0;
uint16_t Gi_Debug2 = 0;
uint16_t Gi_Debug3 = 0;
uint16_t Gi_Debug4 = 0;

float Gf_Debug = 0;
char Gs_My_IP[16];

//----------------------------------------------------------------------------------
uint8_t Connect_2_WiFi(char *Gc_AP, char *Gc_Password);
void StrDo(char *c_Destination, char *c_Append, uint16_t i_Len);
void Do_PID(double d_tempCurrent, double d_tempSet);
void Int_2_Str(int32_t i, char *Str);
void Double_2_Str(double d_Number, char c_places, char *Str);
void Add_2_Tx_Buff(char *Gs_Tx_Buff, int *Gi_TX_Buff_Index, char *s_Str);
void Monitor_n_Read_TCP(void);
//==================================================================================

void StrDo(char *c_Destination, char *c_Append, uint16_t i_Len){
	uint16_t i_End, i_Index;
	
	// First, locate the length of c_Destination
	i_End = 0;
	while (c_Destination[i_End] != 0x00){
		if (i_End == 65535) return; // safety
		i_End++;
	}
	// i_End is now the location last character of c_Destination
	
	i_Index = 0;	// Attaching Append to cDestination
	while (i_Index < i_Len) c_Destination[i_End++] = c_Append[i_Index++];
	
	c_Destination[i_End] = 0;	// Add the terminating character to string
	return;
}

#define sLF				10		// ASCII Linefeed
#define sCR				13		// ASCII CariageReturn

#define INBOUND_MAX 	16
double Gd_Inbound[INBOUND_MAX];

void Parse_Inbound(char *c_String){
	/* c_String is expected in the following format: 
		>1234.3 234 34.34545 ....[sLF][sCR]
		All numbers are seperated by ' ' and are considered as double.
		Then each number is assigned to Gd_Inbound[]
	*/

	boolean b_Left_of_Decimal = true;
	boolean b_Neg_Number = false;	
	char c_Inbound_Index;
	char c, c_Decimal_Places_Counter;
	uint16_t i_Index; 
	uint16_t i_left;	// left of the decimal place
	uint16_t i_right;	// right of the decimal place
	double f;
	
	c_Inbound_Index = 0;
	i_left = 0;
	i_right = 0;
	c_Decimal_Places_Counter = 0;
	
	for (i_Index = 0; i_Index < BUF_SIZE; i_Index++){
		c = c_String[i_Index];
		switch(c){
			case '>':
				// Start of the line
				c_Inbound_Index = 0;
				i_left = 0;
				i_right = 0;
				c_Decimal_Places_Counter = 0;
			break;	

			case '-':
				b_Neg_Number = true;
			break;	
				
			case sCR:
				f = (double)i_right;	// Work on the decimal portion
				for (c = 0; c < c_Decimal_Places_Counter; c++) f /= 10.0;				
				Gd_Inbound[c_Inbound_Index++] = (double)i_left + f;	// Combine to make the whole number
				return;	// We're done!
			break;
			
			case ' ':
				// Got a number	
				f = (double)i_right;	// Work on the decimal portion
				for (c = 0; c < c_Decimal_Places_Counter; c++) f /= 10.0;				
				// Combine to make the whole number
				if(b_Neg_Number) Gd_Inbound[c_Inbound_Index++] = -((double)i_left + f);	
				else Gd_Inbound[c_Inbound_Index++] = (double)i_left + f;
				
				if (c_Inbound_Index == INBOUND_MAX) return;	// safety
				
				// Reset values
				i_left = i_right = c_Decimal_Places_Counter = 0;
				b_Left_of_Decimal = true;
				b_Neg_Number = false;
			break;
				
			case '.':
				b_Left_of_Decimal = false;	// We're now on the right side of the decimal place
			break;
				
			default:
				if(b_Left_of_Decimal) i_left = i_left * 10 + (c - '0');
				else {
					i_right = i_right * 10 + (c - '0');
					c_Decimal_Places_Counter++;
				}
			break;
		}		
	}
	return;
}

#define CMD_DEFAULTS			1
#define CMD_PID_VALUES			2
#define CMD_TARGET_TEMPERATURE	3
#define CMD_RESET				98
#define CMD_FACTORY_RESET		99
	
void Process_Inbound(void){
	char c_Command = (char)Gd_Inbound[0];
	switch (c_Command){
		case CMD_DEFAULTS:
			Gd_PID_k_P = PID_K_P;
			Gd_PID_k_I = PID_K_I;
			Gd_PID_k_D = PID_K_D;
			EEPROM.writeFloat(FLASH_ADD_PID_K_P, Gd_PID_k_P);
			EEPROM.writeFloat(FLASH_ADD_PID_K_I, Gd_PID_k_I);
			EEPROM.writeFloat(FLASH_ADD_PID_K_D, Gd_PID_k_D);
			EEPROM.commit(); delay(1000);	
			break;
			
		case CMD_PID_VALUES:
			Gd_PID_k_P = Gd_Inbound[1];
			Gd_PID_k_I = Gd_Inbound[2];
			Gd_PID_k_D = Gd_Inbound[3];
			EEPROM.writeFloat(FLASH_ADD_PID_K_P, Gd_PID_k_P);
			EEPROM.writeFloat(FLASH_ADD_PID_K_I, Gd_PID_k_I);
			EEPROM.writeFloat(FLASH_ADD_PID_K_D, Gd_PID_k_D);
			EEPROM.commit(); delay(1000);	
			break;
			
		case CMD_TARGET_TEMPERATURE:
			Gd_Temper_Set_C = Gf_Temper_Set_EEPROM_C = Gd_Inbound[1];
			EEPROM.writeFloat(FLASH_ADD_SET_TEMP_C, Gf_Temper_Set_EEPROM_C);
			EEPROM.commit(); delay(1000);	
			break;
			
		case CMD_RESET:
			ESP.restart();
			break;
			
		case CMD_FACTORY_RESET:
			EEPROM.writeByte(FLASH_ADD_FIRST_TIME_MARKER, FIRST_TIME_MARKER_VALUE + 1);	// Mess up the marker
			EEPROM.commit(); delay(1000);
			ESP.restart();
			break;
	}
}

void Toggle_LED(void){
	static char c;
	if(c++ & 1) digitalWrite(PIN_BLUE_LED, HIGH);
	else digitalWrite(PIN_BLUE_LED, LOW);
}

float Conv_C2F(float d){
	return d * (9.0/5.0) + 32.0;
}

#define MINIMUM_ACCEPTABLE_CONVERSION_TIME_IN_MS	200
uint16_t Gi_Conversion_Error_Value = 0;
#define CONVERSION_ERROR_MAX 	1024

float Gf_Temp_Main_C_Old;

void xTask1_Read_Temp0(void *pvParameters){
	byte i;	
	while(1){
		if (Gb_DS18B20_Found && !Gc_DS18B20_Skip_Update){
			ds.reset();
			ds.select(Gb_addr);
			ds.write(0x44, 1);        // start conversion, with parasite power on at the end

			vTaskDelay(900);     // maybe 750ms is enough, maybe not
			// we might do a ds.depower() here, but the reset will take care of it.
		  
			//present = ds.reset();
			ds.reset();
			ds.select(Gb_addr);    
			ds.write(0xBE);         // Read Scratchpad

			for ( i = 0; i < 9; i++) {           // we need 9 bytes
				Gb_data[i] = ds.read();
			}

			int16_t raw = (Gb_data[1] << 8) | Gb_data[0];
			Gd_Temp_Main_C = (float)raw / 16.0;
			Gd_Temp_Main_C += Gf_Temp_Main_C_Offset;
			
			Gd_Temp_Main_F = Conv_C2F(Gd_Temp_Main_C);		
		}
		else {
			vTaskDelay(1);
			//esp_task_wdt_reset();
		}
	}
}

byte Gb_data1[12];
byte Gb_addr1[8];

void xTask1_Read_Temp1(void *pvParameters){
	byte i;	
	while(1){
		ds1.reset();
		ds1.select(Gb_addr1);
		ds1.write(0x44, 1);        // start conversion, with parasite power on at the end

		vTaskDelay(900);     // maybe 750ms is enough, maybe not
		// we might do a ds.depower() here, but the reset will take care of it.
	  
		//present = ds.reset();
		ds1.reset();
		ds1.select(Gb_addr1);    
		ds1.write(0xBE);         // Read Scratchpad

		for ( i = 0; i < 9; i++) {           // we need 9 bytes
			Gb_data1[i] = ds1.read();
		}

		int16_t raw = (Gb_data1[1] << 8) | Gb_data1[0];
		Gd_Temp_Aux_C = (float)raw / 16.0;
		Gd_Temp_Aux_F = Conv_C2F(Gd_Temp_Main_C);		

		vTaskDelay(100);
	}
}

int Gi_ADC_Current_0;
int Gi_ADC_Current_1;
int Gi_ADC_Voltage_0;
int Gi_ADC_Voltage_1;

#define ADC_BLOCK_SIZE 			256
char Gc_Cur_Index_0 = 0;
int Gi_Cur_ADC_0[ADC_BLOCK_SIZE];
unsigned long Gi_Cur_Sum_0 = 0;

char Gc_Volt_Index_0 = 0;
int Gi_Volt_ADC_0[ADC_BLOCK_SIZE];
unsigned long Gi_Volt_Sum_0 = 0;

char Gc_Cur_Index_1 = 0;
int Gi_Cur_ADC_1[ADC_BLOCK_SIZE];
unsigned long Gi_Cur_Sum_1 = 0;

char Gc_Volt_Index_1 = 0;
int Gi_Volt_ADC_1[ADC_BLOCK_SIZE];
unsigned long Gi_Volt_Sum_1 = 0;

#define CURRENT_HI_ACTUAL_0		3.203
#define CURRENT_HI_ADC_0		3180
#define CURRENT_LO_ACTUAL_0		1.971
#define CURRENT_LO_ADC_0		1864
float Gf_Current_Slope_0;
float Gf_Current_0;

#define VOLTAGE_HI_ACTUAL_0		32.81
#define VOLTAGE_HI_ADC_0		3817
#define VOLTAGE_LO_ACTUAL_0		20.40
#define VOLTAGE_LO_ADC_0		2097
float Gf_Voltage_Slope_0;
float Gf_Voltage_0;

void ADCs_Init(void){
	Gf_Current_Slope_0 = (CURRENT_HI_ADC_0 - CURRENT_LO_ADC_0) / (CURRENT_HI_ACTUAL_0 - CURRENT_LO_ACTUAL_0);
	Gf_Voltage_Slope_0 = (VOLTAGE_HI_ADC_0 - VOLTAGE_LO_ADC_0) / (VOLTAGE_HI_ACTUAL_0 - VOLTAGE_LO_ACTUAL_0);
}

void xTask0_Read_ADC(void *pvParameters){
	int i_ADC_Val;
	float d_temp;
	static char c_Switch = 0;
	
	while(1){	
		// This l-oops through at a rate of ~1000Hz or 250Hz for each channel
		
		switch (c_Switch){
		case 0:
			// Read current ADC value 0
			i_ADC_Val= analogRead(PIN_CURRENT_0);
			// Take out the oldest value
			Gi_Cur_Sum_0 -= Gi_Cur_ADC_0[Gc_Cur_Index_0];
			// Add in newest value
			Gi_Cur_Sum_0 += i_ADC_Val;
			// Calculate current average 0
			Gi_ADC_Current_0 = Gi_Cur_Sum_0 / ADC_BLOCK_SIZE;
			// Store the newst value and increment index (note: "auto" wrap around index)
			Gi_Cur_ADC_0[Gc_Cur_Index_0++] = i_ADC_Val;

			break;

		case 1:
			// Read current ADC value 0
			i_ADC_Val = analogRead(PIN_VOLTAGE_0);
			// Take out the oldest value
			Gi_Volt_Sum_0 -= Gi_Volt_ADC_0[Gc_Volt_Index_0];
			// Add in newest value
			Gi_Volt_Sum_0 += i_ADC_Val;
			// Calculate current average 0
			Gi_ADC_Voltage_0 = Gi_Volt_Sum_0 / ADC_BLOCK_SIZE;
			// Store the newst value and increment index (note: "auto" wrap around index)
			Gi_Volt_ADC_0[Gc_Volt_Index_0++] = i_ADC_Val;

			break;
			
		case 2:
			// Read current ADC value 1
			i_ADC_Val= analogRead(PIN_CURRENT_1);
			// Take out the oldest value
			Gi_Cur_Sum_1 -= Gi_Cur_ADC_1[Gc_Cur_Index_1];
			// Add in newest value
			Gi_Cur_Sum_1 += i_ADC_Val;
			// Calculate current average 1
			Gi_ADC_Current_1 = Gi_Cur_Sum_1 / ADC_BLOCK_SIZE;
			// Store the newst value and increment index (note: "auto" wrap around index)
			Gi_Cur_ADC_1[Gc_Cur_Index_1++] = i_ADC_Val;

			break;

		case 3:
			// Read current ADC value 1
			i_ADC_Val = analogRead(PIN_VOLTAGE_1);
			// Take out the oldest value
			Gi_Volt_Sum_1 -= Gi_Volt_ADC_1[Gc_Volt_Index_1];
			// Add in newest value
			Gi_Volt_Sum_1 += i_ADC_Val;
			// Calculate current average 1
			Gi_ADC_Voltage_1 = Gi_Volt_Sum_1 / ADC_BLOCK_SIZE;
			// Store the newst value and increment index (note: "auto" wrap around index)
			Gi_Volt_ADC_1[Gc_Volt_Index_1++] = i_ADC_Val;

			break;			
			
		case 4:
			// Just catching a ride here...
			if (Gb_WiFi_Connected) Monitor_n_Read_TCP();
			break;
		}
				
		c_Switch++;
		if (c_Switch > 4) c_Switch = 0;
		vTaskDelay(1);
	}
}

 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
int Gi_PID_1kHz_Clock;
 
void IRAM_ATTR onTimer() {
	// 1 kHz
	//portENTER_CRITICAL_ISR(&timerMux);
	if (Gc_Button_Delay) Gc_Button_Delay--;
	Gi_PID_1kHz_Clock++;
	//portEXIT_CRITICAL_ISR(&timerMux);
}

#define ON		1
#define OFF		0
void Output_Control(char c_Channel, char c_State){
	if (c_Channel == 0){
		if (c_State){
			Gb_Output_Enabled_0 = 1;
			digitalWrite(OUTPUT_ENABLE_0, HIGH);
		}
		else {
			Gb_Output_Enabled_0 = 0;
			digitalWrite(OUTPUT_ENABLE_0, LOW);
		}
	}
	else {
		if (c_State){
			Gb_Output_Enabled_1 = 1;
			digitalWrite(OUTPUT_ENABLE_1, HIGH);
		}
		else {
			Gb_Output_Enabled_1 = 0;
			digitalWrite(OUTPUT_ENABLE_1, LOW);
		}
	}
}

uint16_t Gl_Last_ms_IP_Recvd = 0;
uint16_t Gl_Last_Sent_Delay = 0;
#define TX_DELAY_MS 				150	

void Monitor_n_Read_TCP(void){
	uint16_t i;	
	//char c_Inbound[1024];
	
	if (wifiMulti.run() == WL_CONNECTED) {
	
		//check if there are any new clients
		if (server.hasClient()) {
			for (i = 0; i < MAX_SRV_CLIENTS; i++) {
				//find free/disconnected spot
				if (!serverClients[i] || !serverClients[i].connected()) {
					if (serverClients[i]) serverClients[i].stop();
					serverClients[i] = server.available();
					if (!serverClients[i]) {
						//sprintf(Gs_Conn_Broken, "%d Connection Broken!", i);
						Gb_WiFi_Connected = false;
					}
					break;
				}
			}

			if (i >= MAX_SRV_CLIENTS) {
				server.available().stop();
			}
		}

		for (i = 0; i < MAX_SRV_CLIENTS; i++) {
			if (serverClients[i] && serverClients[i].connected()) {
				if (serverClients[i].available()) {
					//get data from the telnet client 				
					if (Gb_WiFi_Connected){
						while (serverClients[i].available()) {
							Gs_Buf_Console_ptr[Gi_Buf_Console_Index++] = (serverClients[i].read());
						}
						Gs_Buf_Console_ptr[Gi_Buf_Console_Index] = 0;
						
						Serial1.print(Gs_Buf_Console);
						
						Gl_Last_ms_IP_Recvd = millis() + 300;
					}
				}
			}
			else {
				if (serverClients[i]) {
					serverClients[i].stop();
				}
			}
		}
	}
	else {
		Serial1.println("WiFi not connected!");
		for (i = 0; i < MAX_SRV_CLIENTS; i++) {
			if (serverClients[i]) serverClients[i].stop();
		}
		delay(1000);
	}
}

#define rx_pin 			16
#define tx_pin 			17
#define BAUD_DEBUG		115200
	
void setup() {
	char c;

	Serial.begin(115200);	// Serial Monitor
	Serial1.begin(BAUD_DEBUG, SERIAL_8N1, rx_pin, tx_pin);	
	
	Gb_DS18B20_Found = 0;
	Gb_DS18B20_Powered_Down = false;
		
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
	display.clearDisplay();
	display.setTextColor(WHITE);
	display.setTextSize(1);

	display.setCursor(20,0);
	display.clearDisplay();
	display.println("Apex Instruments");

	display.setCursor(15,20);
	display.setTextSize(3);
	display.println("TEP400");
	
	display.setTextSize(2);
	display.setCursor(26,50);
	display.print(FIRMWAREVERSION);
	
	display.display();
	
	timer = timerBegin(0, 1, true);
	timerAttachInterrupt(timer, &onTimer, true);
	
	//timerAlarmWrite(timer, 50000, true);	// 800 Hz or 200/channel (4 channels)
	timerAlarmWrite(timer, 40000, true);	// 1000 Hz
	timerAlarmEnable(timer);
	
	pinMode(PIN_BLUE_LED, OUTPUT);
	
	//-------------------------------------------------------------
	pinMode(PIN_TC_PWR_AUX, OUTPUT);		// 26
	digitalWrite(PIN_TC_PWR_AUX, HIGH);
	
	pinMode(PIN_TC_PWR_MAIN, OUTPUT);		// 25
	digitalWrite(PIN_TC_PWR_MAIN, HIGH);
	//-------------------------------------------------------------
	
	pinMode(PIN_PULL_LO_FOR_RESET, OUTPUT);
	digitalWrite(PIN_PULL_LO_FOR_RESET, LOW);
	
	// Buttons
	pinMode(PIN_BUTTON_DN, INPUT_PULLDOWN);
	pinMode(PIN_BUTTON_SELECT, INPUT_PULLDOWN);
	pinMode(PIN_BUTTON_UP, INPUT_PULLDOWN);	
	pinMode(PIN_BUTTON_COMMON, OUTPUT);	
	digitalWrite(PIN_BUTTON_COMMON, HIGH);

	pinMode(PIN_BUTTON_UP_, INPUT_PULLDOWN);	
		
	ADCs_Init();
	pinMode(PIN_CURRENT_0, INPUT);
	pinMode(PIN_VOLTAGE_0, INPUT);
	pinMode(PIN_CURRENT_1, INPUT);
	pinMode(PIN_VOLTAGE_1, INPUT);
  
	pinMode(Pin_SCLK, OUTPUT);
	pinMode(Pin_MISO, OUTPUT);
	pinMode(Pin_SS, OUTPUT);
	
	pinMode(LED_ESP_WROOM_32_BLUE, OUTPUT);
	
	pinMode(OUTPUT_ENABLE_0, OUTPUT);
	pinMode(OUTPUT_ENABLE_1, OUTPUT);
	
	digitalWrite(Pin_SS, 1);	// Disable
	digitalWrite(Pin_SCLK, 0);
	

	Gl_Time_Event_GoFullScale = millis();
	
	Serial.println("DAC Ready!");
	
	for(c = 0; c < INBOUND_MAX; c++) Gd_Inbound[c] = 0.0;
	
	// if(digitalRead(WIFI_NOT_ENABLED) == 0){
		// ;	// No soup you you!
	// }
	// else {
	
		// WiFi.setHostname("Chiller");
		
		// // Trying to connect to DiS
		// if (Connect_2_WiFi("Ngo3", "5060708090")){
		// //if (Connect_2_WiFi("AI", "46787863687")){
			// server.begin();
			// server.setNoDelay(true);
			// // Save MY ip address to string
			
			// sprintf(Gs_My_IP, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
			// Serial.println("+++++++++++++++++++++++++++++++++++++++++++++");
			// Serial.println(Gs_My_IP);
			// Serial.println("+++++++++++++++++++++++++++++++++++++++++++++");
			// Gb_WiFi_Connected = true;
		// }
		// else {
			// // Trying to connect to Apex Instruments
			// //if (Connect_2_WiFi("Ngo3", "5060708090")){
			// if (Connect_2_WiFi("AI", "46787863687")){
				// server.begin();
				// server.setNoDelay(true);
				// // Save MY ip address to string
				// sprintf(Gs_My_IP, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
				// Serial.println("+++++++++++++++++++++++++++++++++++++++++++++");
				// Serial.println(Gs_My_IP);
				// Serial.println("+++++++++++++++++++++++++++++++++++++++++++++");
				// Gb_WiFi_Connected = true;
			// }
			// else {		
				// Serial.println("---------------------------------------------");
				// Serial.println(">>>> NO WiFi <<<<");
				// Serial.println("---------------------------------------------");
				// Gb_WiFi_Connected = false;
			// }
		// }
	// }
	
	// Setup watch-dog
	esp_task_wdt_init(6, true);	// 6 seconds
	esp_task_wdt_add(NULL);

	xTaskCreatePinnedToCore(xTask0_Read_ADC, "ADC_0", 4096, (void *)1, 1, NULL, 0);
	xTaskCreatePinnedToCore(xTask1_Read_Temp0, "Temp_0", 4096, (void *)1, 1, NULL, 0);
	xTaskCreatePinnedToCore(xTask1_Read_Temp1, "Temp_1", 4096, (void *)1, 1, NULL, 1);
	xTaskCreatePinnedToCore(xTask1_Tx_2_PC, "xTask1_Tx_2_PC", 4096, (void *)1, 1, NULL, 1);
	
	char c_len = Serial.available();	// flush buffer
	if (c_len)Serial.readBytes(Gs_Buf_Serial_In, c_len);
	
	Gc_Val = 0;
	Pot0(Gc_Val);
	Pot1(Gc_Val);
	
	Gb_Display_Offset = false;
	
	Output_Control(POWER_SUPPLY_0, OFF);
	Output_Control(POWER_SUPPLY_1, OFF);
	delay(2000);
	Output_Control(POWER_SUPPLY_0, ON);
	Output_Control(POWER_SUPPLY_1, ON);

	// Get info from EEPROM
	if (!EEPROM.begin(512)){
		;
	}
	else {
		if (EEPROM.readByte(FLASH_ADD_FIRST_TIME_MARKER) != FIRST_TIME_MARKER_VALUE){
			// defaults
			EEPROM.writeByte(FLASH_ADD_FIRST_TIME_MARKER, FIRST_TIME_MARKER_VALUE);
			Gd_PID_k_P = PID_K_P; 
			EEPROM.writeFloat(FLASH_ADD_PID_K_P, Gd_PID_k_P);
			Gd_PID_k_I = PID_K_I; 
			EEPROM.writeFloat(FLASH_ADD_PID_K_I, Gd_PID_k_I);
			Gd_PID_k_D = PID_K_D; 
			EEPROM.writeFloat(FLASH_ADD_PID_K_D, Gd_PID_k_D);
			
			Gb_metrics = 1;
			Gb_metrics_EEPROM = Gb_metrics;
			EEPROM.writeByte(FLASH_ADD_SET_TEMP_UNITS, Gb_metrics);
					
			Gd_Temper_Set_C = Gf_Temper_Set_EEPROM_C = TEMP_DEFAULT_C;
			EEPROM.writeFloat(FLASH_ADD_SET_TEMP_C, Gf_Temper_Set_EEPROM_C);
			
			Gf_Temp_Main_C_Offset = 0.0;
			EEPROM.writeFloat(FLASH_ADD_TEMP_MAIN_OFFSET, Gf_Temp_Main_C_Offset);

			Gf_Temp_Aux_C_Offset = 0.0;
			EEPROM.writeFloat(FLASH_ADD_TEMP_AUX_OFFSET, Gf_Temp_Aux_C_Offset);
			
			EEPROM.writeInt(FLASH_ADD_TEST, 0);		//
			EEPROM.commit(); delay(1000);
		}	
		else{
			// Remove this on release version
			// Gi_Test_Reset_Counter = EEPROM.readInt(FLASH_ADD_TEST) + 1;
			// EEPROM.writeInt(FLASH_ADD_TEST, Gi_Test_Reset_Counter);
			// EEPROM.commit(); delay(1000);
			
			Gd_PID_k_P = EEPROM.readFloat(FLASH_ADD_PID_K_P);
			Gd_PID_k_I = EEPROM.readFloat(FLASH_ADD_PID_K_I);
			Gd_PID_k_D = EEPROM.readFloat(FLASH_ADD_PID_K_D);
			
			Gf_Temp_Main_C_Offset = EEPROM.readFloat(FLASH_ADD_TEMP_MAIN_OFFSET);
			// UNUSUAL! The condition below can be TRUE if value returned 
			//          from EEPROM.readFloat(FLASH_ADD_TEMP_MAIN_OFFSET) has not been properly initilized.
			if (Gf_Temp_Main_C_Offset != Gf_Temp_Main_C_Offset) Gf_Temp_Main_C_Offset = 0.0;
			// Range check
			if (Gf_Temp_Main_C_Offset > 9.9 || Gf_Temp_Main_C_Offset < -9.9) Gf_Temp_Main_C_Offset = 0.0;	
			
			
			Gf_Temp_Aux_C_Offset = EEPROM.readFloat(FLASH_ADD_TEMP_AUX_OFFSET);
			// UNUSUAL! The condition below can be TRUE if value returned 
			//          from EEPROM.readFloat(FLASH_ADD_TEMP_AUX_OFFSET) has not been properly initilized.
			if (Gf_Temp_Aux_C_Offset != Gf_Temp_Aux_C_Offset) Gf_Temp_Aux_C_Offset = 0.0;
			// Range check			
			if (Gf_Temp_Aux_C_Offset > 10.0 || Gf_Temp_Aux_C_Offset < -10.0) Gf_Temp_Aux_C_Offset = 0.0;
			
			Gf_Temper_Set_EEPROM_C = EEPROM.readFloat(FLASH_ADD_SET_TEMP_C);
			// Safety check
			
			// NOTE: if Gf_Temper_Set_EEPROM_C is NOT A NUMBER, they can't even compare to themselves and return FALSE
			if (Gf_Temper_Set_EEPROM_C == Gf_Temper_Set_EEPROM_C){
				// Gf_Temper_Set_EEPROM_C is a number
				//   Do range check
				if (Gf_Temper_Set_EEPROM_C < SET_TEMP_MIN_C || Gf_Temper_Set_EEPROM_C > SET_TEMP_MAX_C ){
					Gf_Temper_Set_EEPROM_C = SET_TEMP_DEFAULT_C;
					EEPROM.writeFloat(FLASH_ADD_SET_TEMP_C, Gf_Temper_Set_EEPROM_C);
					EEPROM.commit(); delay(1000);						
				}		
			}
			else {
				// Gf_Temper_Set_EEPROM_C is NOT A NUMBER
				Gf_Temper_Set_EEPROM_C = SET_TEMP_DEFAULT_C;
				EEPROM.writeFloat(FLASH_ADD_SET_TEMP_C, Gf_Temper_Set_EEPROM_C);
				EEPROM.commit(); delay(1000);			
			}			
			Gd_Temper_Set_C = Gf_Temper_Set_EEPROM_C;
				
			//----------------------------	
			Gb_metrics = EEPROM.readByte(FLASH_ADD_SET_TEMP_UNITS);
			Gb_metrics_EEPROM = Gb_metrics;
		}
	}

	delay(100);
	Serial1.println("Start!");
}

void Pot0(char c_Value){
	digitalWrite(Pin_SS, 0);	// Enable Chip
	// Send address 00
	digitalWrite(Pin_MISO, 0); 
	digitalWrite(Pin_SCLK, 1); digitalWrite(Pin_SCLK, 0);
	digitalWrite(Pin_MISO, 0);
	digitalWrite(Pin_SCLK, 1); digitalWrite(Pin_SCLK, 0);

	for (char c = 0; c < 8; c++){
		if (c_Value & 0b10000000) digitalWrite(Pin_MISO, 1);
		else digitalWrite(Pin_MISO, 0);
		c_Value<<=1;
		digitalWrite(Pin_SCLK, 1); digitalWrite(Pin_SCLK, 0);
	}
	digitalWrite(Pin_SS, 1);	// Disable Chip
}

void Pot1(char c_Value){
	digitalWrite(Pin_SS, 0);	// Enable Chip
	// Send address 01
	digitalWrite(Pin_MISO, 0); 
	digitalWrite(Pin_SCLK, 1); digitalWrite(Pin_SCLK, 0);
	digitalWrite(Pin_MISO, 1);
	digitalWrite(Pin_SCLK, 1); digitalWrite(Pin_SCLK, 0);

	for (char c = 0; c < 8; c++){
		if (c_Value & 0b10000000) digitalWrite(Pin_MISO, 1);
		else digitalWrite(Pin_MISO, 0);
		c_Value<<=1;
		digitalWrite(Pin_SCLK, 1); digitalWrite(Pin_SCLK, 0);
	}
	digitalWrite(Pin_SS, 1);	// Disable Chip
}

#define BUTTON_DELAY_200ms	200
char Gc_Button_Velocity = 0;

char Gc_Alive;
char Gc_Skip_Update_Display = 0;
uint16_t Gi_Alive = 0;

void DS18B02_Reset(void){
	uint16_t l_Reset_Delay;
	
	Gi_DS18B02_Reset_Counter++;
	Gb_DS18B20_Resetting = true;
	Gb_DS18B20_Powered_Down = true;
	l_Reset_Delay = millis() + DS18B20_RESET_TIME_MS;
	while(l_Reset_Delay > millis()){
		Gb_DS18B20_Found = 0;
		pinMode(ONE_WIRE_BUS_0, OUTPUT);
		digitalWrite(ONE_WIRE_BUS_0, LOW);
		esp_task_wdt_reset();
	}
	Gb_DS18B20_Powered_Down = false;
}


void DS18B20_Monitor(void){

	if (!Gb_DS18B20_Found && !Gb_DS18B20_Powered_Down){	
		Gi_Debug1++;
		Gc_DS18B20_Skip_Update = 4; // The first few readings usually incorrect, so don't display it!
		if ( !ds.search(Gb_addr)) {			
			ds.reset_search();
			delay(250);
			return;
		}

		if (OneWire::crc8(Gb_addr, 7) != Gb_addr[7]) {
			//Serial.println("CRC is not valid!");
			return;
		}
		else {
			Gb_DS18B20_Found = true;	
			esp_task_wdt_init(4, true);
		}
	}
}

void DS18B20_Monitor1(void){
	if (!Gb_DS18B20_Found1){	
		if ( !ds1.search(Gb_addr1)) {			
			ds1.reset_search();
			delay(250);
			return;
		}

		if (OneWire::crc8(Gb_addr1, 7) != Gb_addr1[7]) {
			//Serial.println("CRC is not valid!");
			return;
		}
		else {
			Gb_DS18B20_Found1 = true;	
			esp_task_wdt_init(4, true);
		}
	}
}

uint16_t Gl_Delay_Update_Display;
uint16_t Gi_Rx_Len_Old = 0;
uint32_t Gl_Last_Rx_Rcvd = 0;

// Main....
void loop() {
	boolean b_Commit;
	float d_Step_Size = 0.1;
	uint16_t i, i_Len;
	char cData, c;
	
	DS18B20_Monitor();
	DS18B20_Monitor1();	
	
	//portENTER_CRITICAL_ISR(&timerMux);
	
	if (Gi_PID_1kHz_Clock > 499) {
		// Also a .5 second timer for monitoring the DS18B02
		
		if (Gc_Watchdog){
			Gc_Watchdog--;
			esp_task_wdt_reset();
		}

		Gi_PID_1kHz_Clock = 0 ;
		Gc_Alive++;
				
		Do_PID(Gd_Temp_Main_C, Gd_Temper_Set_C);
			
		if (Gc_Update_EEPROM_CountDown){
			b_Commit = false;
			if (Gc_Update_EEPROM_CountDown == 1){
			
				if (Gf_Temper_Set_EEPROM_C != Gd_Temper_Set_C){
					Gf_Temper_Set_EEPROM_C = Gd_Temper_Set_C;
					EEPROM.writeFloat(FLASH_ADD_SET_TEMP_C, Gf_Temper_Set_EEPROM_C);
					b_Commit = true;
				}
				
				if (Gb_metrics_EEPROM != Gb_metrics){
					Gb_metrics_EEPROM = Gb_metrics;
					EEPROM.writeByte(FLASH_ADD_SET_TEMP_UNITS, Gb_metrics);
					b_Commit = true;					
				}	
				
				if(b_Commit){
					EEPROM.commit(); delay(1000);
				}
			}
			Gc_Update_EEPROM_CountDown--;
		}
		
		// Piggyback to monitor the DS18B02
		// If the temperature hasn't change for so many seconds, reset the DS18B02
		if (Gd_Temp_Main_C_Old != Gd_Temp_Main_C){
			Gd_Temp_Main_C_Old = Gd_Temp_Main_C;
			Gi_DS18B02_Reset_Delay_500ms = 0;
		}
		else {
			if(Gi_DS18B02_Reset_Delay_500ms++ > DS18B02_RESET_DELAY_IN_500MS){
				Gi_DS18B02_Reset_Delay_500ms = 0;
				DS18B02_Reset();
				Toggle_LED();
			}
		}		
	}
		
	if (Gl_Time_Event_GoFullScale < millis()){
		Gl_Time_Event_GoFullScale = millis() + 10;
		if (Gc_DAC_Target != Gc_Val){
			if (Gc_DAC_Target > Gc_Val)	Gc_Val++;
			else Gc_Val--;
			Gc_Val_Saved = Gc_Val;
			
			switch(Gc_Oper_Mode){
				case OPER_MODE_1_ONLY:
					Pot0(Gc_Val);
					Pot1(0);			
				break;
				
			case OPER_MODE_2_ONLY:
					Pot0(0);
					Pot1(Gc_Val);			
				break;
				
			case OPER_MODE_BOTH:
					Pot0(Gc_Val);
					Pot1(Gc_Val);
				break;
			}
		 				
		}
	}
	
	if (!Gc_Button_Delay){
	
		if (Gb_metrics) d_Step_Size = 0.1;
		else d_Step_Size = 0.055;
		
		if (digitalRead(PIN_BUTTON_UP)){
			// Gc_Button_Delay = BUTTON_DELAY_200ms;
			// Gc_Update_EEPROM_CountDown = UPDATE_EEPROM_DELAY_SECONDS;	// Keep from writting to the EEPROM too quickly
			// Gd_Temper_Set_C += ((float)Gc_Button_Velocity * d_Step_Size);
			// if (Gd_Temper_Set_C > SET_TEMP_MAX_C) Gd_Temper_Set_C = SET_TEMP_MAX_C;
			// Gc_Skip_Update_Display = 2;
			// Update_Display((float)Gd_Temp_Main_C, (float)Gd_Temper_Set_C);	//1
			// Gc_Button_Velocity++;
			
			// if(Gb_Display_Offset && !Gc_Button_Select_Holding2){
				// Gc_Button_Select_Holding2 = BUTTON_DELAY_200ms;
				// Gf_Temp_Main_C_Offset += d_Step_Size;
				// if (Gf_Temp_Main_C_Offset > 9.9) Gf_Temp_Main_C_Offset = 9.9; // 9.9C is the maximum offset value
				// // Gc_Oper_Mode++;
				// // if (Gc_Oper_Mode > OPER_MODE_BOTH) Gc_Oper_Mode = OPER_MODE_1_ONLY;
			// }
			

			
			if(Gb_Display_Offset){
				Gc_Button_Select_Holding2 = BUTTON_DELAY_200ms;
				Gf_Temp_Main_C_Offset += 0.1;
				if (Gf_Temp_Main_C_Offset > 9.9) Gf_Temp_Main_C_Offset = 9.9; // 9.9C is the maximum offset value
			}			
			else{
				Gc_Button_Delay = BUTTON_DELAY_200ms;
				Gc_Update_EEPROM_CountDown = UPDATE_EEPROM_DELAY_SECONDS;	// Keep from writting to the EEPROM too quickly
				Gd_Temper_Set_C += ((float)Gc_Button_Velocity * d_Step_Size);
				if (Gd_Temper_Set_C > SET_TEMP_MAX_C) Gd_Temper_Set_C = SET_TEMP_MAX_C;
				Gc_Skip_Update_Display = 2;
				Update_Display((float)Gd_Temp_Main_C, (float)Gd_Temper_Set_C);	//1
				Gc_Button_Velocity++;					
			}
			
			
		}
		if (digitalRead(PIN_BUTTON_DN)){
			// Gc_Button_Delay = BUTTON_DELAY_200ms;
			// Gc_Update_EEPROM_CountDown = UPDATE_EEPROM_DELAY_SECONDS;	// Keep from writting to the EEPROM too quickly
			// Gd_Temper_Set_C -= ((float)Gc_Button_Velocity * d_Step_Size);
			// if (Gd_Temper_Set_C < SET_TEMP_MIN_C) Gd_Temper_Set_C = SET_TEMP_MIN_C;
			// Gc_Skip_Update_Display = 2;
			// Update_Display((float)Gd_Temp_Main_C, (float)Gd_Temper_Set_C);	//2
			// Gc_Button_Velocity++;
			
			// if(Gb_Display_Offset && !Gc_Button_Select_Holding2){
				// Gc_Button_Select_Holding2 = BUTTON_DELAY_200ms;
				// Gf_Temp_Main_C_Offset -= d_Step_Size;
				// if (Gf_Temp_Main_C_Offset < -9.9) Gf_Temp_Main_C_Offset = -9.9; // -9.9C is the minimum offset value
				// // Gc_Oper_Mode--;
				// // if (Gc_Oper_Mode < OPER_MODE_1_ONLY) Gc_Oper_Mode = OPER_MODE_BOTH;
			// }
			
			if(Gb_Display_Offset){
				Gc_Button_Select_Holding2 = BUTTON_DELAY_200ms;
				Gf_Temp_Main_C_Offset -= 0.1;
				if (Gf_Temp_Main_C_Offset < -9.9) Gf_Temp_Main_C_Offset = -9.9; // -9.9C is the minimum offset value
			}
			else {
				Gc_Button_Delay = BUTTON_DELAY_200ms;
				Gc_Update_EEPROM_CountDown = UPDATE_EEPROM_DELAY_SECONDS;	// Keep from writting to the EEPROM too quickly
				Gd_Temper_Set_C -= ((float)Gc_Button_Velocity * d_Step_Size);
				if (Gd_Temper_Set_C < SET_TEMP_MIN_C) Gd_Temper_Set_C = SET_TEMP_MIN_C;
				Gc_Skip_Update_Display = 2;
				Update_Display((float)Gd_Temp_Main_C, (float)Gd_Temper_Set_C);	//2
				Gc_Button_Velocity++;
			}	
			
		}
		
		if (digitalRead(PIN_BUTTON_SELECT)){
			Gc_Button_Delay = BUTTON_DELAY_200ms;
			Gc_Update_EEPROM_CountDown = UPDATE_EEPROM_DELAY_SECONDS;	// Keep from writting to the EEPROM too quickly
			Gc_Button_Select_Holding += 1;
			if (Gc_Button_Select_Holding > 10){	// 2 Seconds
				Gc_Button_Select_Holding = 0;
				Gb_metrics = !Gb_metrics;
				Gc_Skip_Update_Display = 2;
				Update_Display((float)Gd_Temp_Main_C, (float)Gd_Temper_Set_C);	//3
			}
			
			// Displaying OFFSET mode: Going into
			Gc_Button_Select_Holding2 += 1;
			if (Gc_Button_Select_Holding2 > 20){	// 4 Seconds
				Gc_Button_Select_Holding2 = 0;
				Gb_Display_Offset = true;
			}
			
			// Displaying OFFSET mode: Going out of
			if (Gb_Display_Offset && Gc_Button_Select_Holding2 > 10){
				Gc_Button_Select_Holding2 = 0;
				EEPROM.writeFloat(FLASH_ADD_TEMP_MAIN_OFFSET, Gf_Temp_Main_C_Offset);
				EEPROM.commit(); delay(1000);				
				Gb_Display_Offset = false;
			}
		}	
		else {
			Gc_Button_Select_Holding = 0;
			if(Gc_Button_Select_Holding2) Gc_Button_Select_Holding2--;
		}
	}
	
	if (!digitalRead(PIN_BUTTON_UP) & !digitalRead(PIN_BUTTON_DN) & !digitalRead(PIN_BUTTON_SELECT)) Gc_Button_Velocity = 1;
	else {
		if (Gc_Button_Velocity > 10) Gc_Button_Velocity = 10;
	}
	
	if (Gl_Delay_Update_Display < millis() ){
		Gl_Delay_Update_Display = millis() + 250;
		if (Gc_Skip_Update_Display) Gc_Skip_Update_Display--;	
		else {
			Update_Display((float)Gd_Temp_Main_C, (float)Gd_Temper_Set_C);
		}
	}
	
	i_Len = Serial.available();
	if (i_Len){
		Serial.readBytes(Gs_Buf_Serial_In, i_Len);
		StrDo(Gs_Buf_Console, Gs_Buf_Serial_In, i_Len);	// Attach to Gs_Buff_Console
	}

	// If the length of Gs_Buf_Console has not changed in 200 millisecond, process the string
	if (Gs_Buf_Console[0] != 0){	// Do only if length of Gs_Buf_Console is not zero
		if (Gi_Rx_Len_Old != i_Len) {
			Gi_Rx_Len_Old = i_Len;
			Gl_Last_Rx_Rcvd = millis();		// Save the time that was last recieved
		}
		else {
			if (Gl_Last_Rx_Rcvd + 200 < millis()) {	// If no data is recived after 200ms, read the buffer
				
				if (Gl_Last_ms_IP_Recvd < millis()){		
					Gi_Rx_Len_Old = 0;
					
					
					
					Parse_Inbound(Gs_Buf_Console);
					Process_Inbound();
					Gs_Buf_Console[0] = 0;	// Clear the string
					Gs_Buf_Console[1] = 0;	// Clear the string
					Gi_Buf_Console_Index = 0;
				}
			}
		}
	}
}

void Add_2_Tx_Buff(char *Gs_Tx_Buff, int *Gi_TX_Buff_Index, char *s_Str){
	char c = 0;
	char c_Char;
	int i_Index = Gi_TX_Buff_Index[0];
	
	c_Char = s_Str[c];
	while(c_Char != 0){
		Gs_Tx_Buff[i_Index++] = c_Char;
		c++;
		c_Char = s_Str[c];
	}
	Gs_Tx_Buff[i_Index++] = ',';
	Gs_Tx_Buff[i_Index++] = ' ';
	
	Gs_Tx_Buff[i_Index] = 0; // Attach terminating character but NOT increment Gi_TX_Buff_Index
	Gi_TX_Buff_Index[0] = i_Index;
}

void Double_2_Str(double d_Number, char c_Places, char *Str){
	boolean b_Neg_Number;
	boolean b_Less_Than_One;
	char s_Temp[16];
	char c, c_Index;;
	uint32_t i = 0;
	double d_Num_Saved;

	//if (c_Places > 5) c_Places = 5;	// Limit to 5 decimal places

	if (d_Number < 0) {
		b_Neg_Number = true;
		d_Number = -d_Number;
	}
	else b_Neg_Number = false;
	
	// Small trick so that numnber like 0.0x, 0.00x, ... would be correct.
	if (d_Number < 1.0) {
		b_Less_Than_One = true;	
		d_Number += 1.0;
	}
	else b_Less_Than_One = false;

	c_Index = 0;
	if (d_Number == 0.0){
		if (c_Places == 0){
			Str[c_Index++] = '0'; 
		}
		else{
			Str[c_Index++] = '0'; Str[c_Index++] = '.';
			while (c_Places--) Str[c_Index++] = '0';
		}
		Str[c_Index] = 0;	// Terminate the string 
		return;
	}
	else {
		d_Num_Saved = d_Number;
		for (c = 0; c < c_Places; c++) d_Number *= 10.0;
		d_Number += 0.5;	// Rounding up
		
		i = (uint32_t)d_Number;
		if (i == 0){
			Str[0] = '0';
			Str[1] = 0;	
			return;
		}
		else{
			c = 0;
			while(i){
				if (i < 10)	s_Temp[c++] = i + '0';
				else s_Temp[c++] = i % 10 + '0';
				i /= 10;
			}
				
			//build the string 
			Str[c] = 0;	// Terminate the string..first
			c--;
			
			c_Index = 0;
			
			// Allocate spaces for Negative sign and possible leading "0."
			if (b_Neg_Number) c_Index++;
			if (d_Num_Saved < 1.0) {c_Index++;}	// for the leading "0." characters

			do {
				Str[c_Index++] = s_Temp[c];	// reversed order
				
			}
			while (c--);
			Str[c_Index] = 0;	// Terminate the string 
			
			
			// Put back the decimal 
			for (c = 0; c <= c_Places; c++){
				Str[c_Index + 1] = Str[c_Index]; 
				c_Index--;
			}
			
			if (b_Neg_Number) {
				Str[0] = '-';	
				if (b_Less_Than_One) {
					Str[1] = '0';	// Take out the '+ 1' that we added earlier
					Str[2] = '.';
				}
				else Str[c_Index + 1] = '.';			
			}
			else{
				if (b_Less_Than_One) {
					Str[0] = '0';	// Take out the '+ 1' that we added earlier
					Str[1] = '.';
				}
				else Str[c_Index + 1] = '.';						
			}	
		}	
	}
	return;
}

void Int_2_Str(int32_t i, char *Str){
	boolean b_Neg_Number;
	char c, c_Index;
	char s_Temp[16];

	if (i < 0) {
		b_Neg_Number = true;
		i = -i;
	}
	else b_Neg_Number = false;
	
	if (i == 0){
		Str[0] = '0';
		Str[1] = 0;
	}
	else{
		c = 0;
		while(i){
			if (i < 10)	s_Temp[c++] = i + '0';
			else s_Temp[c++] = i % 10 + '0';
			i /= 10;
		}
			
		//build the string 
		Str[c] = 0;	// Terminate the string..first
		c--;
		c_Index = 0;
		if (b_Neg_Number) Str[c_Index++] = '-';
		do Str[c_Index++] = s_Temp[c];	// reversed order
		while (c--);
	}
	Str[c_Index] = 0; // Terminate the string
	return;
}

//=========================================
void Do_Display_Offset(float d_tempCurrent){
	UINT iTemp;
	float f_Temp;
	char cOut[8];

	display.setTextSize(1);	
	// Display Set temperature
	display.setCursor(0, 0);
	display.clearDisplay();
	display.print("Offset: ");	
	
	display.setTextSize(2);	

	if (Gf_Temp_Main_C_Offset < 0) cOut[0] = '-';
	else cOut[0] = ' ';
	
	f_Temp = abs(Gf_Temp_Main_C_Offset);
	iTemp = (UINT)f_Temp;
		
	cOut[1] = iTemp + '0';
	cOut[2] = '.';
	
	iTemp = (int)(f_Temp * 10.0);
	cOut[3] = (iTemp % 10) + '0';

	cOut[4] = ' ';
	cOut[5] = 'C';
	cOut[6] = 0;	// terminate the string
	display.print(cOut);

	display.setTextSize(3);
	display.setCursor(5, 26);
	
	Gb_metrics = true;
	Display_Curr_Temp(d_tempCurrent);
			
	display.display();
	
	return;
}

// ========================================

#define CMD_DISABLE_CONSOLE_PID		'D'
#define DISP_TEMP_CURRENT_Y			26

void Display_Curr_Temp(float d_tempCurrent){
	unsigned int iTemp;
	float f_Temp;
	char cOut[8];
	
	if(Gc_DS18B20_Skip_Update) Gc_DS18B20_Skip_Update--;
	display.setTextSize(4);
	if (Gb_metrics) f_Temp = d_tempCurrent;
	else f_Temp = Conv_C2F(d_tempCurrent);

	if (f_Temp >= 100.0) {
		display.setCursor(5, DISP_TEMP_CURRENT_Y);	
		cOut[5] = 0;	// Null
		iTemp = (unsigned int)(f_Temp * 10);
		cOut[4] = (iTemp % 10) + '0';
		iTemp /= 10;
		cOut[3] = '.';
		cOut[2] = (iTemp % 10) + '0';
		iTemp /= 10;		
		cOut[1] = (iTemp % 10) + '0';
		iTemp /= 10;
		cOut[0] = iTemp + '0';	
	}
	else {
		if (f_Temp > 0.0){
			iTemp = (unsigned int)(f_Temp * 10.0);
			if (iTemp < 100){
				cOut[3] = 0; 	// Null
				cOut[2] = (iTemp % 10) + '0';
				iTemp /= 10;
				cOut[1] = '.';
				cOut[0] = iTemp + '0';
				display.setCursor(32, DISP_TEMP_CURRENT_Y);
			}
			else {
				cOut[4] = 0; 	// Null
				cOut[3] = (iTemp % 10) + '0';
				iTemp /= 10;
				cOut[2] = '.';
				cOut[1] = (iTemp % 10) + '0';
				iTemp /= 10;
				cOut[0] = iTemp + '0';
				display.setCursor(20, DISP_TEMP_CURRENT_Y);
			}
		}
		else {	// negative numbers
			iTemp = (unsigned char)(abs(f_Temp) * 10);
			cOut[0] = '-';
			if (iTemp < 100){
				cOut[4] = 0; 	// Null
				cOut[3] = (iTemp % 10) + '0';
				iTemp /= 10;
				cOut[2] = '.';
				cOut[1] = iTemp + '0';
				display.setCursor(18, DISP_TEMP_CURRENT_Y);
			}
			else {
				cOut[5] = 0; 	// Null
				cOut[4] = (iTemp % 10) + '0';
				iTemp /= 10;
				cOut[3] = '.';
				cOut[2] = (iTemp % 10) + '0';
				iTemp /= 10;
				cOut[1] = iTemp + '0';
				display.setCursor(8, DISP_TEMP_CURRENT_Y);						
			}
			
			//Get rid of '-0.0'
			if (cOut[1] == '0' && cOut[3] == '0') cOut[0] = ' ';
		}			
	}
	display.print(cOut);

}

void Update_Display(float d_tempCurrent, float d_tempSet) {
	unsigned int iTemp;
	float f_Temp;
	char cOut[8];

	// 201231
	if (Gb_Display_Offset){
		Do_Display_Offset(d_tempCurrent);
		return;
	}

	display.setTextSize(2);	
	// Display Set temperature
	display.setCursor(0, 0);
	display.clearDisplay();
	display.print("Set:");	
		
	cOut[6] = 0;	// NULL
	if (Gb_metrics) {
		f_Temp = d_tempSet;
		cOut[5] = 'C';	
	}
	else {
		f_Temp = Conv_C2F(d_tempSet);
		cOut[5] = 'F';
	}	
	cOut[4] = ' ';
	
	iTemp =(unsigned int)(fabs(f_Temp) * 10.0);		
	cOut[3] = (iTemp % 10) + '0';
	iTemp /= 10;
	cOut[2] = '.';
	cOut[1] = (iTemp % 10) + '0';
	iTemp /= 10;
	cOut[0] = iTemp + '0';
		
	if (cOut[0] == '0'){
		if (f_Temp <= -0.1) cOut[0] = '-';	// Leading zero is replaced by the '-' sign
		else cOut[0] = ' ';        			// Get rid of leading zero
	}	
		
	display.setCursor(56, 0);
	display.print(cOut);
	
	// Display Current Temperature
	if (Gb_DS18B20_Found){
		Gb_DS18B20_Resetting = false;
		Display_Curr_Temp(d_tempCurrent);

	}
	else {
		if (Gb_DS18B20_Resetting){
			;
		}
		else{
			display.setTextSize(3);
			display.setCursor(16, DISP_TEMP_CURRENT_Y);
			display.print("NO TC!");
			display.display();
			return;
		}
	}

	// dRAW PWR
	iTemp = Gc_DAC_Target;
	iTemp /= 2;

	display.drawLine(0, 62, iTemp, 62, WHITE);
	display.drawLine(0, 63, iTemp, 63, WHITE);

	//if (Gc_Alive & 1) display.drawLine(0, 15, 1, 15, WHITE);
	display.setTextSize(1);	
	display.setCursor(119, 16);
	
	switch(Gc_Alive){
		case 0:
			display.print("|");
		break;
		
		case 1:
			display.print("/");
		break;
		
		case 2:
			display.print("-");
		break;
		
		case 3:
			display.print("\\");
		break;	

		case 4:
			display.print("|");
		break;	
		
		case 5:
			display.print("/");
		break;	
		
		case 6:
			display.print("-");
		break;	
		
		case 7:
			display.print("\\");
			Gc_Alive = 0;
		break;	
	}
	
	if (Gb_WiFi_Connected){
		display.setTextSize(1);	
		display.setCursor(25, 16);
		display.print(Gs_My_IP);
	}
	
	display.display();
}

//=================================================================
uint8_t Connect_2_WiFi(char *Gc_AP, char *Gc_Password) {
	wifiMulti.addAP(Gc_AP, Gc_Password);

	for (int loops = 2; loops > 0; loops--) {
		if (wifiMulti.run() == WL_CONNECTED) {
			return 1;
			break;
		}
		else {
			delay(1000);
		}
	}
	return 0;
}

//---------------PID Stuff----------------------------
// Porportional
double Do_P2(double d_Temp_Diff){
	return d_Temp_Diff * Gd_PID_k_P;
}

// Intergral
#define INTEGRAL_RANGE		20
double Gd_Integral[INTEGRAL_RANGE];
double Do_I2(double d_Temp_Diff){
	static char c_OneTime = true;
	static char c_Intergral_Index = 0;
	char c;
	double d_Sum = 0;
	
	if (c_OneTime){
		c_OneTime = false;
		for (c = 0; c < INTEGRAL_RANGE; c++) Gd_Integral[c] = d_Temp_Diff;
	}
	
	Gd_Integral[c_Intergral_Index++] = d_Temp_Diff;
	if (c_Intergral_Index == INTEGRAL_RANGE) c_Intergral_Index = 0;	// Range check
	for (c = 0; c < INTEGRAL_RANGE; c++) d_Sum += Gd_Integral[c];
	
	return (d_Sum * Gd_PID_k_I);	
}

// Diff
double Do_D2(double d_tempCurrent, double d_Temp_Diff){
    static double d_Val_Old = 9999.99;
	double d_Temp;
	
	if (d_Val_Old == 9999.99) d_Val_Old = d_tempCurrent;	// First time only

	d_Temp = Gd_PID_k_D * (d_tempCurrent - d_Val_Old);
    d_Val_Old = d_tempCurrent;
    return d_Temp;	
}

void Do_PID(double d_tempCurrent, double d_tempSet){
	static char cDelay = 5;	
	double d_Temp_Diff;

	if (cDelay == 0){	// Just in the beginning
		d_Temp_Diff = d_tempCurrent - d_tempSet;
		
		Gd_PID_Diff = d_Temp_Diff;
		
		Gd_Cal_PID_P = Do_P2(d_Temp_Diff);
		Gd_Cal_PID_I = Do_I2(d_Temp_Diff);
		Gd_Cal_PID_D = Do_D2(d_tempCurrent, d_Temp_Diff);
	
		Gd_ADC += Gd_Cal_PID_P + Gd_Cal_PID_I + Gd_Cal_PID_D;
		
		// Range check
		if (Gd_ADC > 255.0) Gd_ADC = 255.0;
		if (Gd_ADC < 0.0) Gd_ADC = 0.0;	
		
		// Performance test...
	}
	else cDelay--;
	
	// Special case: if more than 8 degrees, go full power!
	if (d_Temp_Diff > 8.0) Gd_ADC = 250.0;
	
	// DAC
	Gc_DAC_Target = (unsigned char)Gd_ADC;
	
	// Ther is a minium value that the DAC can control the power supply, at ZERO, turn OFF the power.
	if (Gc_DAC_Target){
		Output_Control(POWER_SUPPLY_0, ON);
		Output_Control(POWER_SUPPLY_1, ON);
	}
	else{
		Output_Control(POWER_SUPPLY_0, OFF);
		Output_Control(POWER_SUPPLY_1, OFF);
	}	
}

//===========================================================================

void xTask1_Tx_2_PC(void *pvParameters){
	char c;
	char s_Str[32];
	int16_t i_ADC_Voltage;
	int16_t i_ADC_Current;
	float d_temp;
	float Gf_Current;
	
	while(1){
		vTaskDelay(330);		// THIS loop is ~3 times/second	
		
		// Determine witch channel to use
		if (Gi_ADC_Current_0 > Gi_ADC_Current_1){
			i_ADC_Voltage = Gi_ADC_Voltage_0;
			i_ADC_Current = Gi_ADC_Current_0;
		}
		else {
			i_ADC_Voltage = Gi_ADC_Voltage_1;
			i_ADC_Current = Gi_ADC_Current_1;		
		}
		
		Gi_TX_Buff_Index = 0;		// Start the Tx_Buff			
		Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, ">CHIL");

		// 1 - Temperature 1 *
		if (Gd_Temp_Main_C < -10.0) Gd_Temp_Main_C = Gf_Old_Temp0;
		else Gf_Old_Temp0 = Gd_Temp_Main_C;	
		Double_2_Str(Gd_Temp_Main_C, 3, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 2 - DAC *
		Int_2_Str(Gc_Val_Saved, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 3 - Current *
		if (i_ADC_Current > 10){
			d_temp = Gi_ADC_Current_0 - CURRENT_LO_ADC_0;
			d_temp /= Gf_Current_Slope_0;
			Gf_Current = CURRENT_LO_ACTUAL_0 + d_temp;
			
			d_temp = Gi_ADC_Current_1 - CURRENT_LO_ADC_0;
			d_temp /= Gf_Current_Slope_0;
			Gf_Current += (CURRENT_LO_ACTUAL_0 + d_temp);
		}
		else Gf_Current = 0.0;
		Double_2_Str(Gf_Current, 1, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 4 - Voltage *
		Serial.print(" ");
		if (i_ADC_Voltage > 100){
			d_temp = i_ADC_Voltage - VOLTAGE_LO_ADC_0;
			d_temp /= Gf_Voltage_Slope_0;
			Gf_Voltage_0 = VOLTAGE_LO_ACTUAL_0 + d_temp;
		}
		else Gf_Voltage_0 = 0.0;
		Double_2_Str(Gf_Voltage_0, 1, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 5 - Power *
		Double_2_Str(Gf_Current * Gf_Voltage_0, 1, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 6 - Coefficent - P
		Double_2_Str(Gd_PID_k_P, 1, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 7 - Coefficent - I
		Double_2_Str(Gd_PID_k_I, 2, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 8 - Coefficent - D
		Double_2_Str(Gd_PID_k_D, 0, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 9 - Debug: DS28B02 Reset counter
		Int_2_Str(Gi_DS18B02_Reset_Counter, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 10 - Debug: Seconds since reset
		Int_2_Str((millis() / 1000UL), s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);

		// 11
		Double_2_Str(Gd_Cal_PID_P, 2, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		// 12	
		Double_2_Str(Gd_Cal_PID_I, 2, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		// 13
		Double_2_Str(Gd_Cal_PID_D, 2, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		// 14
		Double_2_Str(Gd_PID_Diff, 3, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);			
		
		// 15
		Double_2_Str(Gd_Temper_Set_C, 3, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 16
		if (Gb_Output_Enabled_0) Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, "1");
		else Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, "0");

		// 17
		Int_2_Str(Gi_Alive++, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);
		
		// 18
		Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, FIRMWAREVERSION);
		
		// 19
		Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, " IP:");
		// 20
		Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, Gs_My_IP);
		// 21
		
		// 22 - Temperature Aux *
		if (Gd_Temp_Aux_C < -10.0) Gd_Temp_Aux_C = Gf_Old_Temp1;
		else Gf_Old_Temp1 = Gd_Temp_Aux_C; 		
		Double_2_Str(Gd_Temp_Aux_C, 3, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);	

		// 23 - OFFSET
		Double_2_Str(Gf_Temp_Main_C_Offset, 1, s_Str); Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, s_Str);	

		Add_2_Tx_Buff(Gs_Tx_Buff, &Gi_TX_Buff_Index, " EOB");
		// 24
		Gs_Tx_Buff[Gi_TX_Buff_Index++] = sCR;
		Gs_Tx_Buff[Gi_TX_Buff_Index++] = sLF;
		
		Serial.print(Gs_Tx_Buff);
		
		for (c = 0; c < MAX_SRV_CLIENTS; c++) {
			if (serverClients[c] && serverClients[c].connected()) {
				serverClients[c].write(Gs_Tx_Buff, Gi_TX_Buff_Index);
			}
		}

		// It seems that when the unit "hung", it not sending data to via TCP (perhaps also USB),
		// so resetting the watch dog here seems to be a good idea.
		Gc_Watchdog = 5;
	}
}