/**
 * @file stats.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of a simple Stats class, which is used to calculate statistics on a
 * stream of numbers.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef STATS_HPP
#define STATS_HPP

#include <Arduino.h>
#include <limits>

/**
 * @brief The Stats class represents a statistical analysis tool.
 * 
 * This class provides methods to calculate the average, maximum, and minimum values of a set of numbers. Averages are
 * computed with an exponential moving average algorithm, and the maximum and minimum values are updated as new numbers
 * are added to the set. 
 */
class Stats {
public:
    /**
     * @brief Constructs a Stats object with the specified alpha value.
     * 
     * @param alpha The alpha value used in the average calculations.
     */
    Stats(float alpha);
    
    /**
     * @brief Sets the alpha value used in the average calculations.
     * 
     * @param alpha The new alpha value.
     */
    void setAlpha(float alpha);
    
    /**
     * @brief Gets the current alpha value used in the average calculations.
     * 
     * @return The current alpha value.
     */
    float getAlpha();

    /**
     * @brief Adds a number to the set of numbers for analysis.
     * 
     * @param number The number to be added.
     */
    void addNumber(double number);
    
    /**
     * @brief Calculates the exponential moving average of the numbers in the set.
     * 
     * @return The average value.
     */
    double average();
    
    /**
     * @brief Calculates the maximum value in the set.
     * 
     * @return The maximum value.
     */
    double max();
    
    /**
     * @brief Calculates the minimum value in the set.
     * 
     * @return The minimum value.
     */
    double min();

private:
    float _alpha;       ///< The alpha value used in the calculations.
    double _average;    ///< The average value of the numbers in the set.
    double _stat_max;   ///< The maximum value in the set.
    double _stat_min;   ///< The minimum value in the set.
};

#endif // STATS_HPP
