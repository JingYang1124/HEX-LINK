#include "RTE.h"

TaskHandle_t OsTask_200ms_Handle;
TaskHandle_t OsTask_100ms_Handle;
TaskHandle_t OsTask_50ms_Handle;
TaskHandle_t OsTask_20ms_Handle;

void Task_Init(void){
  /**
   * Task creation
   */

  xTaskCreate(OsTask_200ms, // Task function
              "OsTask_200ms", // Task name
              128, // Stack size 
              NULL, 
              5, // Priority
              &OsTask_200ms_Handle); // Task handler

  xTaskCreate(OsTask_100ms, // Task function
              "OsTask_100ms", // Task name
              128, // Stack size 
              NULL, 
              4, // Priority
              &OsTask_100ms_Handle); // Task handler

  xTaskCreate(OsTask_50ms, // Task function
              "OsTask_50ms", // Task name
              128, // Stack size 
              NULL, 
              6, // Priority
              &OsTask_50ms_Handle); // Task handler
              
  xTaskCreate(OsTask_20ms, // Task function
              "OsTask_20ms", // Task name
              128, // Stack size 
              NULL, 
              2, // Priority
              &OsTask_20ms_Handle); // Task handler
  }


void OsTask_200ms(void *pvParameters)
{
  (void) pvParameters;

  for (;;)
  {
    #if defined(RTE_TASK_SERIAL_PRINT)
      Serial.println("======== OsTask_200ms ========");
    #endif
    
    vTaskDelay( 200 / portTICK_PERIOD_MS );
  }
}



void OsTask_100ms(void *pvParameters)
{
  (void) pvParameters;

  for (;;)
  {
    #if defined(RTE_TASK_SERIAL_PRINT)
      Serial.println("======== OsTask_100ms ========");
    #endif

    vTaskDelay( 100 / portTICK_PERIOD_MS );
  }
}

void OsTask_50ms(void *pvParameters)
{
  (void) pvParameters;

  for (;;)
  {
    #if defined(RTE_TASK_SERIAL_PRINT)
      Serial.println("======== OsTask_50ms ========");
    #endif   

    vTaskDelay( 50 / portTICK_PERIOD_MS );
  }
}

void OsTask_20ms(void *pvParameters)
{
  (void) pvParameters;

  for (;;)
  {
    #if defined(RTE_TASK_SERIAL_PRINT)
      Serial.println("======== OsTask_20ms ========");
    #endif  

    vTaskDelay( 20 / portTICK_PERIOD_MS );
  }
}
