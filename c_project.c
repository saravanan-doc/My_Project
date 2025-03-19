#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define THRESHOLD 95.0

void process_temperature_data(const char *input_file, const char *output_file) {
    FILE *fp = fopen(input_file, "r");
    FILE *out_fp = fopen(output_file, "w");

    if (fp == NULL || out_fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    float temp, total_temp = 0.0, max_temp = -1000.0, min_temp = 1000.0;
    int count = 0;
    char line[50];

    // Writing header for output file
    fprintf(out_fp, "Temperature\n");

    while (fgets(line, sizeof(line), fp)) {
        // Skip any leading non-numeric characters (e.g., BOM)
        char *ptr = line;
        while (*ptr && !isdigit(*ptr) && *ptr != '-' && *ptr != '.') {
            ptr++; // Move past non-numeric characters
        }

        if (sscanf(ptr, "%f", &temp) == 1) {
            total_temp += temp;
            if (temp > max_temp) max_temp = temp;
            if (temp < min_temp) min_temp = temp;
            count++;

            // Check for threshold violation
            if (temp > THRESHOLD) {
                fprintf(out_fp, "%.2f\n", temp);
            }
        }
    }

    fclose(fp);
    fclose(out_fp);

    // Print results
    if (count > 0) {
        printf("Average Temperature: %.2f\n", total_temp / count);
        printf("Maximum Temperature: %.2f\n", max_temp);
        printf("Minimum Temperature: %.2f\n", min_temp);
    } else {
        printf("No valid data found!\n");
    }
}

int main() {
    const char *input_file = "RTD.csv";
    const char *output_file = "High_Temperature_Alerts.csv";

    process_temperature_data(input_file, output_file);

    printf("Processing complete. Check '%s' for high temperature alerts.\n", output_file);

    return 0;
}

