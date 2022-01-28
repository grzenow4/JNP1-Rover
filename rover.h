#ifndef ROVER_H
#define ROVER_H

#include "operations.h"
#include <utility>

struct NotLanded : std::exception {
    char const *what() const throw() {
        return "NotLanded";
    }
};

class Rover {
public:
    Rover() = delete;
    Rover(Rover &&other) = delete;
    Rover(const Rover &&other) = delete;
    Rover(command_map_t _commands,
          sensor_list_t _sensors) {
        landed = false;
        commands = std::move(_commands);
        sensors = std::move(_sensors);
    }
    
    void land(coords_t coords, direction_t dir) {
        landed = true;
        stopped = false;
        coordinates = coords;
        direction = dir;
    }
    
    void execute(const std::string &string) {
        if (!landed)
            throw NotLanded();
        
        stopped = false;
        for (char c: string) {
            auto it = commands.find(c);
            if (it == commands.end()) {
                stopped = true;
                break;
            }
            
            if (!(it->second->execute(coordinates, direction, sensors))) {
                stopped = true;
                break;
            }
        }
    }
    
    std::string print_rover() const {
        if (!landed) {
            return "unknown";
        }
        
        std::string res;
        res += "(" + std::to_string(coordinates.real()) +
               ", " + std::to_string(coordinates.imag()) + ") ";
        
        switch (direction.real()) {
            case 1:
                res += "EAST";
                break;
            case -1:
                res += "WEST";
                break;
            case 0:
                res += direction.imag() == 1 ? "NORTH" : "SOUTH";
                break;
        }
        
        if (stopped)
            res += " stopped";
        
        return res;
    }

private:
    bool landed;
    bool stopped;
    coords_t coordinates;
    direction_t direction;
    command_map_t commands;
    sensor_list_t sensors;
};

std::ostream &operator<<(std::ostream &os, Rover const &rover) {
    return os << rover.print_rover();
}

class RoverBuilder {
public:
    RoverBuilder() = default;
    RoverBuilder(RoverBuilder &&other) = delete;
    RoverBuilder(const RoverBuilder &&other) = delete;
    
    RoverBuilder &program_command(char c,
                                  const std::shared_ptr<Operation> &operation) {
        commands_list[c] = operation;
        return *this;
    }
    
    RoverBuilder &add_sensor(std::unique_ptr<Sensor> &&sensor) {
        sensors.push_back(std::move(sensor));
        return *this;
    }
    
    Rover build() {
        return Rover(std::move(commands_list), std::move(sensors));
    }

private:
    command_map_t commands_list;
    sensor_list_t sensors;
};

#endif /* ROVER_H */
