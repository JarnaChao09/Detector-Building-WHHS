// Declaring PIN locations
int tmp36_pin = A0;

/**
   LED pin locations: NONE defaults to -1
*/
enum LED {
  red = 4,
  green = 2,
  blue = 3,
  NONE = 0
};

// declaring reference voltage
float reference = 3.3;

// Declaring accuracy
int accuracy = 5;

// Declaring LED ranges
// *change these values*
// ============*START CHANGE*============
// RED: Hot temperatures
float red_min = 40;
float red_max = 60;
// GREEN: Moderate temperatures
float green_min = 20;
float green_max = 40;
// BLUE: Cold temperatures
float blue_min = 0;
float blue_max = 20;
// ============*END CHANGE*==============

/**
   Init: initializing all LEDs pin and setting to mode output
*/
void init_LEDS() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}
/**
   function: lighting output of specific LED to HIGH
*/
void light_LED(int pin, bool output_mode) {
  digitalWrite(pin, output_mode);
}
/**
   Setup: Run Once, initialization of pin modes and Serial objects
*/
void setup() {
  // referenence 3.3v loop
  analogReference(EXTERNAL);

  // initializing tmp36 pin and setting tmp36 to input
  pinMode(tmp36_pin, INPUT);

  init_LEDS();

  // begin serial stream
  Serial.begin(9600);
}

/**
   function: converts analog value into voltage value
*/
float analog_to_voltage(float analog) {
  return analog * reference / 1023.0;
}

/**
   function: converts voltage value into celsius temperature value
*/
float voltage_to_celsius(float volt) {
  return (volt - 0.5) * 100.0;
}

/**
   function: converts celsius value into kelvin temperature value
*/
float celsius_to_kelvin(float celsius) {
  return celsius + 273.15;
}

/**
   function: converts celsius value into fahrenheit temperature value
*/
float celsius_to_fahrenheit(float celsius) {
  return celsius * (9.0 / 5.0) + 32.0;
}

/**
   Loop: Runs repeatedly until interrupted by error or loss of power
*/
void loop() {
  // defining variables and calculating values
  // ======================*CALCULATING VALUES*==============================
  float analog = analogRead(tmp36_pin);
  float voltage = analog_to_voltage(analog);
  float temperature_celsius = voltage_to_celsius(voltage);
  float temperature_fahrenhiet = celsius_to_fahrenheit(temperature_celsius);
  float temperature_kelvin = celsius_to_kelvin(temperature_celsius);
  // ========================================================================

  // printing values with Serial
  // ==================*BEGIN PRINT*===================
  // printing analog
  Serial.print("Analog Reading: ");
  Serial.println(analog, accuracy);

  // printing voltage
  Serial.print("Voltage Reading: ");
  Serial.println(voltage, accuracy);

  // printing temperature in celsius
  Serial.print("Temperature Reading (C): ");
  Serial.println(temperature_celsius, accuracy);

  // printing temperature in fahrenhiet
  Serial.print("Temperature Reading (F): ");
  Serial.println(temperature_fahrenhiet, accuracy);

  // printing temperature in kelvin
  Serial.print("Temperature Reading (K): ");
  Serial.println(temperature_kelvin, accuracy);
  // ===================*END PRINT*====================

  LED current_LED = NONE;
  // ===================*TURNING ON LEDS*=================
  if (blue_min <= temperature_celsius && temperature_celsius < blue_max) {
    Serial.println("Lighting up LED blue");
    light_LED(blue, HIGH);
    current_LED = blue;
  }

  if (green_min <= temperature_celsius && temperature_celsius < green_max) {
    Serial.println("Lighting up LED green");
    light_LED(green, HIGH);
    current_LED = green;
  }

  if (red_min <= temperature_celsius && temperature_celsius < red_max) {
    Serial.println("Lighting up LED blue");
    light_LED(red, HIGH);
    current_LED = red;
  }

  // delay for 5000 milliseconds (ms)
  delay(5000);

  // turning off current_LED
  switch (current_LED) {
    case NONE:
      // no LED lighted
      Serial.println("No LED was turned on");
      break;
    case red:
      Serial.println("Turning off LED red");
      light_LED(red, LOW);
      break;
    case green:
      Serial.println("Turning off LED green");
      light_LED(green, LOW);
      break;
    case blue:
      Serial.println("Turning off LED blue");
      light_LED(blue, LOW);
      break;
    default:
      Serial.println("RUNNING: Default Case in Switch Case");
      break;
  }
  // ==================*TURNING OFF LEDS*=================

  // delay for 5000 milliseconds (ms)
  delay(5000);
}
