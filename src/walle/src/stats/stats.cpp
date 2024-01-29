#include "stats.hpp"

Stats::Stats(float alpha) : _alpha(alpha) {
    _stat_max = std::numeric_limits<double>::min();
    _stat_min = std::numeric_limits<double>::max();
    _average = std::numeric_limits<double>::quiet_NaN();
}

void Stats::setAlpha(float alpha) {
    _alpha = alpha;
}

float Stats::getAlpha() {
    return _alpha;
}

void Stats::addNumber(double number) {
    if (std::isnan(_average)) {
        _average = number;
    } else {
        _average = _alpha * number + (1 - _alpha) * _average;
    }

    if (number > _stat_max) {
        _stat_max = number;
    }

    if (number < _stat_min) {
        _stat_min = number;
    }
}

double Stats::average() {
    if (std::isnan(_average))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    
    return _average;
}

double Stats::max() {
    if (std::isnan(_average))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return _stat_max;
}

double Stats::min() {
    if (std::isnan(_average))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return _stat_min;
}
