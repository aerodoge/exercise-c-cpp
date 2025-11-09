/**
 * @file Racer.cpp
 * @brief 参赛者抽象基类实现
 */

#include "Racer.h"
#include <algorithm>

Racer::Racer(std::string_view racerName, char racerSymbol)
    : name(racerName), symbol(racerSymbol) {}

int Racer::getPosition() const noexcept {
    return position;
}

std::string_view Racer::getName() const noexcept {
    return name;
}

char Racer::getSymbol() const noexcept {
    return symbol;
}

bool Racer::hasFinished() const noexcept {
    return position >= FINISH_LINE;
}

void Racer::reset() noexcept {
    position = START_LINE;
}

void Racer::ensureValidPosition() noexcept {
    position = std::max(position, START_LINE);
}
