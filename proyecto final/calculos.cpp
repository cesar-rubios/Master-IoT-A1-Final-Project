#include "calculos.h"
#include "mbed.h"
#include "sensors.h"
#include <algorithm>
#include <cstdio>
#include <limits>
#include <map>
#include <string>


// Constants
#define HOUR_IN_MS 30000 // 1 minute for testing, change to 3600000 for 1 hour

using namespace std;

// Variables to calculate statistics
float temp_sum = 0, humidity_sum = 0, brightness_sum = 0, soil_sum = 0;
float temp_max = -numeric_limits<float>::max(),
      temp_min = numeric_limits<float>::max();
float humidity_max = -numeric_limits<float>::max(),
      humidity_min = numeric_limits<float>::max();
float brightness_max = -numeric_limits<float>::max(),
      brightness_min = numeric_limits<float>::max();
float soil_max = -numeric_limits<float>::max(),
      soil_min = numeric_limits<float>::max();
map<string, int> color_count;
float x_max = -numeric_limits<float>::max(),
      x_min = numeric_limits<float>::max();
float y_max = -numeric_limits<float>::max(),
      y_min = numeric_limits<float>::max();
float z_max = -numeric_limits<float>::max(),
      z_min = numeric_limits<float>::max();

int counter = 0;

// Timer to track 1 hour duration
Timer timer;

void calculos() {
  printf("Hilo de cálculos iniciado\n");

  timer.start(); // Start the timer to track 1-hour duration

  while (true) {
    // Use try_get_for to get data with timeout (e.g., 100ms)
    DataSensors *message = nullptr;
    osEvent evt = queue.get(); // Wait indefinitely for a message
    message = (DataSensors *)evt.value.p;

    if (evt.status == osEventMessage && message != nullptr) {
      // Update sums for mean calculation
      temp_sum += message->temperature;
      humidity_sum += message->humidity;
      brightness_sum += message->brightness;
      soil_sum += message->soil;

      // Update max and min values
      temp_max = max(temp_max, message->temperature);
      temp_min = min(temp_min, message->temperature);

      humidity_max = max(humidity_max, message->humidity);
      humidity_min = min(humidity_min, message->humidity);

      brightness_max = max(brightness_max, message->brightness);
      brightness_min = min(brightness_min, message->brightness);

      soil_max = max(soil_max, message->soil);
      soil_min = min(soil_min, message->soil);

      // Count dominant colors
      color_count[message->dominant]++;

      x_max = max(x_max, message->x);
      y_max = max(y_max, message->y);
      z_max = max(z_max, message->z);

      x_min = min(x_min, message->x);
      y_min = min(y_min, message->y);
      z_min = min(z_min, message->z);

      mpool.free(message); // Free the allocated memory for the message

      counter ++;

      // Check if 1 hour has passed
      if (timer.read_ms() >= HOUR_IN_MS) {
        // Calculate mean values
        printf("---------------------------------------------MEDIAS------------"
               "------------- \n");
        float temp_mean =
            temp_sum / counter; // Use color_count.size() instead of
                                           // temperature_data.size()
        float humidity_mean = humidity_sum / counter;
        float brightness_mean = brightness_sum / counter;
        float soil_mean = soil_sum / counter;

        // Find the most frequent dominant color
        string dominant_color = "";
        int max_count = 0;
        for (const auto &color : color_count) {
          if (color.second > max_count) {
            max_count = color.second;
            dominant_color = color.first;
          }
        }

        // Send the statistics to the computer
        printf("Stats for the last hour:\n");
        printf("Temperature - Mean: %.2f °C, Max: %.2f °C, Min: %.2f °C\n",
               temp_mean, temp_max, temp_min);
        printf("Humidity - Mean: %.2f %%, Max: %.2f %%, Min: %.2f %%\n",
               humidity_mean, humidity_max, humidity_min);
        printf("Brightness - Mean: %.2f Lux, Max: %.2f Lux, Min: %.2f Lux\n",
               brightness_mean, brightness_max, brightness_min);
        printf("Soil Moisture - Mean: %.2f %% , Max: %.2f %% , Min: %.2f %%\n",
               soil_mean, soil_max, soil_min);
        printf("Dominant Color for the last hour: %s\n",
               dominant_color.c_str());
        printf("Accelerometer X - Max: %.2f m/s², Min: %.2f m/s²\n", x_max,
               x_min);
        printf("Accelerometer Y - Max: %.2f m/s², Min: %.2f m/s²\n", y_max,
               y_min);
        printf("Accelerometer Z - Max: %.2f m/s², Min: %.2f m/s²\n", z_max,
               z_min);

        // Reset sums and min/max values for the next hour
        temp_sum = humidity_sum = brightness_sum = soil_sum = 0;
        temp_max = humidity_max = brightness_max = soil_max =
            -numeric_limits<float>::max();
        temp_min = humidity_min = brightness_min = soil_min =
            numeric_limits<float>::max();
        color_count.clear();

        // Reset the timer for the next hour
        timer.reset();
        counter = 0;
      }
    }
  }
}