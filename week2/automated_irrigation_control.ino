#define MOISTURE_PIN 34  
#define PUMP_PIN 23      

float filteredADC = 2048.0;      
const float alpha = 0.2;        
bool pumpActive = false;         

const int ADC_dry = 4095;        
const int ADC_wet = 0;           

void setup() {
  Serial.begin(115200);
  
  digitalWrite(PUMP_PIN, HIGH); 
  pinMode(PUMP_PIN, OUTPUT);
  
  Serial.println("--- System Initialized Safely ---");
}

void loop() {
  int rawADC = analogRead(MOISTURE_PIN);
  filteredADC = (alpha * rawADC) + ((1.0 - alpha) * filteredADC);
  
  int moisture = map((int)filteredADC, ADC_dry, ADC_wet, 0, 100);
  moisture = constrain(moisture, 0, 100); 
  
  if (moisture < 30) {
    pumpActive = true;   
  } 
  else if (moisture > 45) {
    pumpActive = false;  
  }

  if (pumpActive) {
    digitalWrite(PUMP_PIN, LOW);   
  } else {
    digitalWrite(PUMP_PIN, HIGH);  
  }

  Serial.print("Raw ADC: "); Serial.print(rawADC);
  Serial.print(" | Soil Moisture: "); Serial.print(moisture);
  Serial.print("% | Relay/Pump State: "); Serial.println(pumpActive ? "ON" : "OFF");

  delay(200); 
}