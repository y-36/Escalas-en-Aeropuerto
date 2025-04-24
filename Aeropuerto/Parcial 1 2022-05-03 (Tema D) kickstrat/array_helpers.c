/*
@file array_helpers.c
@brief Array Helpers method implementation
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array_helpers.h"

/**
* @brief returns true when reach last line in flight file
* @return True when is the last line of the file, False otherwise
*/
static bool is_last_line(unsigned int hour, unsigned int type) {
  return  hour == HOURS - 1u && type == TYPE - 1u;
}

void array_dump(DelayTable a) {
  for (unsigned int type = 0u; type < TYPE; ++type) {
    for (unsigned int hour = 0u; hour < HOURS; ++hour) {
      Flight f = a[type][hour];
      fprintf(stdout, "%c: %s flight with %u passengers arrived at %u:00, with %u delay",
        f.code,
        f.type == 0 ? "last_mile" : "layover",
        f.passengers_amount,
        f.hour - 1,
        f.delay
      );
      if (!is_last_line(hour, type))
      {
        fprintf(stdout, "\n");
      }
    }
  }
}


unsigned int compensation_cost(DelayTable a, unsigned int hour) {
    unsigned int total = 0;
    for (unsigned int type = 0; type < TYPE; ++type) {
        unsigned int max_delay = (type == last_mile) ? MAX_LM_DELAY_ALLOWED : MAX_LAYOVER_DELAY_ALLOWED;
        for (unsigned int h = 0; h < hour; ++h) {
            Flight f = a[type][h];
            if (f.delay > max_delay) {
                unsigned int excess = f.delay - max_delay;
                total += (unsigned int)(excess * COMPENSATION_PER_MINUTE * f.passengers_amount);
            }
        }
    }
    return total;
}


void array_from_file(DelayTable array, const char *filepath) {
  FILE *file = NULL;

  file = fopen(filepath, "r");
  if (file == NULL) {
    fprintf(stderr, "File does not exist.\n");
    exit(EXIT_FAILURE);
  }

  char code;
  unsigned int hora_lm, delay_lm, passengers_lm;
  unsigned int hora_lo, delay_lo, passengers_lo;
  
  while (fscanf(file, "%u %u %u %u %u %u #%c#\n",
                  &hora_lm, &delay_lm, &passengers_lm,
                  &hora_lo, &delay_lo, &passengers_lo, &code) == 7) {
    /* COMPLETARDO: lectura de cada vuelo */
    
        Flight lm_flight = {
            .code = code,
            .type = last_mile,
            .hour = hora_lm - 1,
            .delay = delay_lm,
            .passengers_amount = passengers_lm
        };
        array[last_mile][lm_flight.hour] = lm_flight;
        
        
        Flight lo_flight = {
            .code = code,
            .type = layover,
            .hour = hora_lo - 1,
            .delay = delay_lo,
            .passengers_amount = passengers_lo
        };
        array[layover][lo_flight.hour] = lo_flight;

  }
  fclose(file);
}
