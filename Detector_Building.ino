// Ranges of the LEDS
namespace RANGES {
  const float red_min = 40;
  const float red_max = 60;

  const float green_min = 20;
  const float green_max = 40;

  const float blue_min = 0;
  const float blue_max = 20;
}

// Constants used through out the program
namespace CONSTANTS {
  const int tmp36_pin = 0;

  const int delay_time = 2000;

  const int accuracy = 5;

  const float reference = 3.3;

  void separate() {
    for (int i = 0; i < 40; i++) { Serial.print("-"); }
  }
};

// Range Checker as a struct
struct CHECKER {
  public:
  float value;

  CHECKER(float value = 0.0) {
    value = value;
  }

  bool check_red() {
    return RANGES::red_min <= value && value <= RANGES::red_max;
  }

  bool check_blue() {
    return RANGES::blue_min <= value && value <= RANGES::blue_max;
  }

  bool check_green() {
    return RANGES::green_min <= value && value <= RANGES::green_max;
  }
};

// LED struct
struct LED {
  private:
  void light_LED(int pin, bool output_mode) {
    digitalWrite(pin, output_mode);
  }

  public:
  // enum that holds pin location
  enum COLOR {
    RED = 4,
    GREEN = 2,
    BLUE = 3,
    NONE = 0
  };

  // state that determines if the LED is On or Off
  enum STATE {
    ON,
    OFF
  };
  
  COLOR current_LED;
  bool is_on;

  LED(COLOR color = NONE) {
    current_LED = color;
    is_on = false;
  }

  LED set_color(COLOR color) {
    current_LED = color;
    return *this;
  }

  void set_state(STATE state) {
    switch (state) {
      case ON: 
        light_LED(current_LED, 1);
        is_on = true;
        break;
      case OFF: 
        light_LED(current_LED, 0);
        is_on = false;
        break;
    }
  }

  // returns string of LED information
  String to_string() {
    String ret = "";
    switch (current_LED) {
      case NONE: return "No LED";
      case RED: ret += "Color: Red";
      case BLUE: ret += "Color: BLUE";
      case GREEN: ret += "Color: GREEN";
    }

    ret += ", ";

    if (is_on) {ret += "State: ON";}
    else {ret += "State: OFF";}
    
    return ret;
  }
};

// Temperature as a struct
struct TEMPERATURE {
  public:
  float value;

  TEMPERATURE(float value = 0.0) {
    value = value;
  }

  TEMPERATURE to_fahrenheit() {
    float dummy = value * (9.0 / 5.0) + 32.0;
    return dummy;
  }

  TEMPERATURE to_kelvin() {
    float dummy = value + 273.15;
    return TEMPERATURE(dummy);
  }
};

// Voltage as a struct
struct VOLTAGE {
  public:
  float value;

  VOLTAGE(float value = 0.0) {
    value = value;
  }

  TEMPERATURE to_celsius() {
    float dummy = (value - 0.5) * 100.0;
    return TEMPERATURE(value);
  }
};

// Analog as a struct
struct ANALOG {
  public:
  float value;

  ANALOG(float value = 0.0) {
    value = value;
  }

  VOLTAGE to_voltage() {
    float dummy = value * CONSTANTS::reference / 1023.0;
    return VOLTAGE(dummy);
  }
};

// Detector struct
// all the magic happens here
struct DETECTOR {
  public:
  ANALOG analog;
  VOLTAGE voltage;
  TEMPERATURE celsius, fahrenheit, kelvin;

  DETECTOR(float &value) {
    analog = ANALOG(value);
    voltage = analog.to_voltage();
    celsius = voltage.to_celsius();
    fahrenheit = celsius.to_fahrenheit();
    kelvin = celsius.to_kelvin();
  }
};

// sets LED output modes
void init_LEDS() {
  pinMode(LED::COLOR::RED, OUTPUT);
  pinMode(LED::COLOR::GREEN, OUTPUT);
  pinMode(LED::COLOR::BLUE, OUTPUT);
}

/**
 * runs once, initialization of pinModes and Serial
 */
void setup() {
  analogReference(EXTERNAL);

  pinMode(CONSTANTS::tmp36_pin, INPUT);

  init_LEDS();

  Serial.begin(9600);
}

/**
 * Loop: Runs repeatedly until interrupted by error or loss of power
 */
void loop() {
  // reading analog input
  float input = analogRead(CONSTANTS::tmp36_pin);

  // creation of detector object
  DETECTOR detector = DETECTOR(input);

  // printing analog
  Serial.print("Analog Reading: ");
  Serial.println(detector.analog.value, CONSTANTS::accuracy);
  
  // printing voltage
  Serial.print("Voltage Reading: ");
  Serial.println(detector.voltage.value, CONSTANTS::accuracy);
  
  // printing temperature in celsius
  Serial.print("Temperature Reading (C): ");
  Serial.println(detector.celsius.value, CONSTANTS::accuracy);

  // printing temperature in fahrenheit
  Serial.print("Temperature Reading (F): ");
  Serial.println(detector.fahrenheit.value, CONSTANTS::accuracy);

  // printing temperature in kelvin
  Serial.print("Temperature Reading (K): ");
  Serial.println(detector.kelvin.value, CONSTANTS::accuracy);

  // creation of LED object that holds current LED
  LED current_LED = LED();

  // creation of Checker object to check ranges of the temperature to turn on LEDS
  CHECKER check = CHECKER(detector.celsius.value);
  
  // checking blue range
  if (check.check_blue()) {
    current_LED.set_color(LED::COLOR::BLUE).set_state(LED::STATE::ON);
  }

  // checking green range
  if (check.check_green()) {
   current_LED.set_color(LED::COLOR::GREEN).set_state(LED::STATE::ON);
  }

  // checking red range
  if (check.check_red()) {
    current_LED.set_color(LED::COLOR::RED).set_state(LED::STATE::ON);
  }

  // printing current LED status
  Serial.println(current_LED.to_string());

  // delay on LED lighting up for a set time (ms)
  delay(CONSTANTS::delay_time);

  // turning off current LED
  current_LED.set_state(LED::STATE::OFF);

  // printing current LED status
  Serial.println(current_LED.to_string());

  // formatter to see different runs
  CONSTANTS::separate();

  // delay before rerunning loop (ms)
  delay(CONSTANTS::delay_time);
}
