#ifndef DIRECTION_H
#define DIRECTION_H

#include "sensor.h"
#include <complex>

// Zarówno współrzędne, jak i kierunek będziemy implementowali
// jako liczby zespolone.
// Współrzędne interpretujemy jak na płaszczyźnie Gaussa.
// Kierunek to liczba zespolona o module 1, którą powinniśmy dodać
// do współrzędnych, aby uzyskać przejście o jednostkę do przodu.
// Aby zmienić kierunek, należy pomnożyć go przez
// odpowiednią liczbę zespoloną leżącą na okręgu jednostkowym.
using coords_t = std::complex<coordinate_t>;
using direction_t = std::complex<coordinate_t>;

class Direction {
public:
    static constexpr direction_t EAST = {1, 0};
    static constexpr direction_t WEST = {-1, 0};
    static constexpr direction_t NORTH = {0, 1};
    static constexpr direction_t SOUTH = {0, -1};
};

#endif /* DIRECTION_H */
