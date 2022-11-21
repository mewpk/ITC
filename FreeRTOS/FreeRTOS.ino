#include <Arduino_FreeRTOS.h>
#include "queue.h"
#define RED 2
#define YELLOW 3
#define GREEN 4
#define SW1 10
#define SW2 11
#define SW3 12
QueueHandle_t ledQueueG;
QueueHandle_t ledQueueY;
QueueHandle_t ledQueueR;

void setup() {
  Serial.begin(115200);
  ledQueueG =  xQueueCreate(5, sizeof(int32_t));
  ledQueueY =  xQueueCreate(5, sizeof(int32_t));
  ledQueueR =  xQueueCreate(5, sizeof(int32_t));
  xTaskCreate(vSenderTaskG, "Sender Task GREEN", 128, NULL, 1, NULL);
  xTaskCreate(vSenderTaskY, "Sender Task GREEN", 128, NULL, 1, NULL);
  xTaskCreate(vSenderTaskR, "Sender Task GREEN", 128, NULL, 1, NULL);
  xTaskCreate(vReceiverTaskG, "Receiver Task GREEN", 64, NULL, 1, NULL);
  xTaskCreate(vReceiverTaskY, "Receiver Task YELLOW", 64, NULL, 1, NULL);
  xTaskCreate(vReceiverTaskR, "Receiver Task RED", 64, NULL, 1, NULL);
}


void vSenderTaskG(void *pvParameters) {
  int32_t valueToSendG = 0;
  pinMode(SW3, INPUT);
  int buttonStateG = LOW;
  int lastButtonStateG = LOW;
  unsigned long lastDebounceTimeG = 0;
  unsigned long debounceDelay = 50;

  while (1) {
    int readingG = digitalRead(SW3);
    if (readingG != lastButtonStateG) {
      lastDebounceTimeG = millis();
    }
    if ((millis() - lastDebounceTimeG) > debounceDelay) {
      if (readingG != buttonStateG) {
        buttonStateG = readingG;
        valueToSendG = buttonStateG;
        if (buttonStateG == HIGH) {
          xQueueSend(ledQueueG, &valueToSendG, 0);
          vTaskDelay(10);
        }
      }
    }
    lastButtonStateG = readingG;
  }
}

void vSenderTaskY(void *pvParameters) {
  int32_t valueToSendY = 0;
  pinMode(SW2, INPUT);
  unsigned long debounceDelay = 50;
  int buttonStateY = LOW;
  int lastButtonStateY = LOW;
  unsigned long lastDebounceTimeY = 0;

  while (1) {
    int readingY = digitalRead(SW2);
    if (readingY != lastButtonStateY) {
      lastDebounceTimeY = millis();
    }
    if ((millis() - lastDebounceTimeY) > debounceDelay) {
      if (readingY != buttonStateY) {
        buttonStateY = readingY;
        valueToSendY = buttonStateY;
        if (buttonStateY == HIGH) {
          xQueueSend(ledQueueY, &valueToSendY, 0);
          vTaskDelay(10);
        }
      }
    }
    lastButtonStateY = readingY;
  }
}

void vSenderTaskR(void *pvParameters) {
  int32_t valueToSendR = 0;
  pinMode(SW1, INPUT);
  unsigned long debounceDelay = 50;
  int buttonStateR = LOW;
  int lastButtonStateR = LOW;
  unsigned long lastDebounceTimeR = 0;
  while (1) {
    int readingR = digitalRead(SW1);
    if (readingR != lastButtonStateR) {
      lastDebounceTimeR = millis();
    }
    if ((millis() - lastDebounceTimeR) > debounceDelay) {
      if (readingR != buttonStateR) {
        buttonStateR = readingR;
        valueToSendR = buttonStateR;
        Serial.println(buttonStateR);
        if (buttonStateR == HIGH) {
          xQueueSend(ledQueueR, &valueToSendR, 0);
          vTaskDelay(10);
        }
      }
    }
    lastButtonStateR = readingR;
  }
}

void vReceiverTaskG(void *pvParameters) {
  int32_t valueReceived;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(GREEN, OUTPUT);
  int check = 1;
  unsigned long startBlink;
  unsigned long startTime;
  while (1) {
    int temp = xQueueReceive(ledQueueG, &valueReceived, xTicksToWait);
    if (temp == 1 && check) {
      startTime = millis();
      startBlink = millis();
      digitalWrite(GREEN, HIGH);
      check = 0;
    }
    if (millis() - startTime >= 3000) {
      check = 1;
      digitalWrite(GREEN, LOW);
    } else {
      if (millis() - startBlink >= 500) {
        digitalWrite(GREEN, !digitalRead(GREEN));
        startBlink = millis();
      }
    }
    vTaskDelay(1);
  }
}

void vReceiverTaskY(void *pvParameters) {
  int32_t valueReceived;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(YELLOW, OUTPUT);
  unsigned long startBlink;
  unsigned long startTime = 0;
  int blinkCheck = 0;
  while (1) {
    if (xQueueReceive(ledQueueY, &valueReceived, xTicksToWait) == 1 && millis() - startTime > 200) {
      blinkCheck = !blinkCheck;
      startTime = millis();
      startBlink = millis();
      digitalWrite(YELLOW, HIGH);
    }
    if (blinkCheck == 1) {
      if (millis() - startBlink >= 200) {
        digitalWrite(YELLOW, !digitalRead(YELLOW));
        startBlink = millis();
      }
    } else {
      digitalWrite(YELLOW, LOW);
    }
    vTaskDelay(10);
  }
}


void vReceiverTaskR(void *pvParameters) {
  int32_t valueReceived;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(RED, OUTPUT);
  unsigned long startTime = 0;
  int times = 0;
  while (1) {
    if (xQueueReceive(ledQueueR, &valueReceived, xTicksToWait) == 1) {
      startTime = millis();
      digitalWrite(RED, HIGH);
      times += 3000;
    }
    if (millis() - startTime >= times) {
      digitalWrite(RED, LOW);
      times = 0;
    }
    vTaskDelay(1);
  }
}

void loop() {
}
