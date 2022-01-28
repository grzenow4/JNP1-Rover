#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "direction.h"
#include <iostream>
#include <map>

class Operation {
public:
    // Zwraca informację czy operacja się powiodła. Jeśli operacja
    // się powiedzie, zmienia odpowiednio podane współrzędne i kierunek.
    virtual bool execute(coords_t &coords, coords_t &direction,
                         const sensor_list_t &sensors) = 0;
};

using command_map_t = std::map<char, std::shared_ptr<Operation>>;

class MoveForward : public Operation {
public:
    bool execute(coords_t &coords, coords_t &direction,
                 const sensor_list_t &sensors) override {
        coords_t new_coords = coords + direction;
        bool safe = true;

        for (const auto &sensor: sensors)
            safe &= sensor->is_safe(new_coords.real(), new_coords.imag());

        if (safe)
            coords = new_coords;

        return safe;
    }
};

class MoveBackward : public Operation {
public:
    bool execute(coords_t &coords, coords_t &direction,
                 const sensor_list_t &sensors) override {
        coords_t new_coords = coords - direction;
        bool safe = true;
        for (const auto &sensor: sensors)
            safe &= sensor->is_safe(new_coords.real(), new_coords.imag());

        if (safe)
            coords = new_coords;
        return safe;
    }
};

class RotateRight : public Operation {
public:
    bool execute([[maybe_unused]] coords_t &coords, coords_t &direction,
                 [[maybe_unused]] const sensor_list_t &sensors) override {
        direction *= Direction::SOUTH;
        return true;
    }
};

class RotateLeft : public Operation {
public:
	bool execute([[maybe_unused]] coords_t &coords, coords_t &direction,
				 [[maybe_unused]] const sensor_list_t &sensors) override {
        direction *= Direction::NORTH;
        return true;
    }
};

class Compose : public Operation {
public:
    using operation_ptr = std::shared_ptr<Operation>;
    using init_operations_t = std::initializer_list<operation_ptr>;

    Compose(init_operations_t init_operations) : operations(init_operations) {}

    bool execute(coords_t &coords, coords_t &direction,
                 const sensor_list_t &sensors) override {
        bool safe = true;

        for (const auto &operation: operations) {
            if (!operation->execute(coords, direction, sensors)) {
                safe = false;
                break;
            }
        }

        return safe;
    }

private:
    std::vector<std::shared_ptr<Operation>> operations;
};

std::shared_ptr<MoveForward> move_forward() {
    return std::make_shared<MoveForward>();
}

std::shared_ptr<MoveBackward> move_backward() {
    return std::make_shared<MoveBackward>();
}

std::shared_ptr<RotateRight> rotate_right() {
    return std::make_shared<RotateRight>();
}

std::shared_ptr<RotateLeft> rotate_left() {
    return std::make_shared<RotateLeft>();
}

std::shared_ptr<Compose> compose(Compose::init_operations_t init_operations) {
    return std::make_shared<Compose>(init_operations);
}

#endif /* OPERATIONS_H */
