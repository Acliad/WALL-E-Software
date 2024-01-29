#ifndef STATS_HPP
#define STATS_HPP

#include <Arduino.h>
#include <limits>

// Define your class or functions here
class Stats {
public:
    Stats(float alpha);
    
    void setAlpha(float alpha);
    float getAlpha();

    void addNumber(double number);
    double average();
    double max();
    double min();

private:
    float _alpha;
    double _average;
    double _stat_max;
    double _stat_min;
};

#endif // STATS_HPP
