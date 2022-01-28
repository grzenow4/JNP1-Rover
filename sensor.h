#ifndef SENSOR_H
#define SENSOR_H

#include <memory>
#include <vector>

using coordinate_t = long long;

class Sensor {
public:
    virtual bool is_safe(coordinate_t x, coordinate_t y) = 0;
};

using sensor_list_t = std::vector<std::unique_ptr<Sensor>>;

#endif /* SENSOR_H */
