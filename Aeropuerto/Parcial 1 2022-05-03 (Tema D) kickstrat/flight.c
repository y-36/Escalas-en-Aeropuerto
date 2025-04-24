/*
  @file layover.c
  @brief Implements flight structure and methods
*/
#include <stdlib.h>
#include "flight.h"


Flight flight_from_file(FILE* file)
{
    Flight flight;

    /* COMPLETADO */
    
    int res = fscanf(file, "%u %u %u #%c#\n",
                     &flight.hour, &flight.delay, &flight.passengers_amount, &flight.code);
    if (res != 4) {
        fprintf(stderr, "Invalid flight data.\n");
        exit(EXIT_FAILURE);
    }
    flight.hour -= 1;  // Convertir a rango 0-23

    return flight;
}
