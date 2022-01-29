float SAMPLING_FREQUENCY;
float TARGET_FREQUENCY = 977;  //Select a target frquency. Pay attention to hardware limitations, target must be less than half the sampling freqency!
const int SAMPLE_SIZE = 1024;  //Select a sample size, must be a power of 2
int ADCCENTER = 2048;          //Select the middle point of the ADC, for example ESP32 = 12 bit = 4096 = 2048 middle point.
const uint8_t sensorPin = 32;  //Select input pin, must be an analog input
int delay = 400;               //Optional, select a delay in microseconds(us) to lower the sampling frequency if needed (uncomment line 25)
float coeff;
int data[SAMPLE_SIZE];

void setup () {
  Serial.begin(115200);
}

void loop ()
{
  Serial.println(Goertzel(sensorPin));
}

float Goertzel(uint8_t sensorPin)
{
  unsigned long start = micros();
  for (int index = 0; index < SAMPLE_SIZE; index++)
  {
    testData[index] = analogRead(sensorPin);
    //delayMicroseconds(delay);
  }
  unsigned long end = micros();
  unsigned long delta = end - start;
  unsigned long period = delta / SAMPLE_SIZE;
  SAMPLING_FREQUENCY = 1000000.0 / period;
  float omega = (2.0 * PI * TARGET_FREQUENCY) / SAMPLING_FREQUENCY;
  coeff = 2.0 * cos(omega);
  float Q2 = 0;
  float Q1 = 0;
  for (int index = 0; index < SAMPLE_SIZE; index++)
  {
    float Q0;
    Q0 = coeff * Q1 - Q2 + (float) (data[index] - ADCCENTER);
    Q2 = Q1;
    Q1 = Q0;
  }
  return sqrt(Q1 * Q1 + Q2 * Q2 - coeff * Q1 * Q2);
}
