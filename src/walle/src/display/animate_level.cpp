#include "animate_level.hpp"

AnimateLevel::AnimateLevel()
    : first_keyframe(), current_keyframe(), last_keyframe(), last_update_time_ms(), solar_bars(), num_bars_on() {}

void AnimateLevel::addKeyframe(int num_bars_on, unsigned int duration_ms) {
    // Creates Keyframe from the arguments and add it to the animation.  
    Keyframe* new_keyframe = new Keyframe;
    for(int i = 0; i < _BARS_NUM_BARS; i++) {
        new_keyframe->bar_status[i] = i < num_bars_on;
    }
    new_keyframe->duration_ms = duration_ms;
    new_keyframe->next = nullptr;

    this->_appendKeyframe(new_keyframe);
}

void AnimateLevel::addKeyframeFromArray(bool bar_status[_BARS_NUM_BARS], unsigned int duration_ms) {
    // Creates Keyframe from the arguments and add it to the animation.  
    Keyframe* new_keyframe = new Keyframe;
    for(int i = 0; i < _BARS_NUM_BARS; i++) {
        new_keyframe->bar_status[i] = bar_status[i];
    }
    new_keyframe->duration_ms = duration_ms;
    new_keyframe->next = nullptr;   

    this->_appendKeyframe(new_keyframe);
}

void AnimateLevel::addPauseKeyframe(unsigned int duration_ms) {
    // Pauses the animation for the given duration. This is done by adding a keyframe with the same bar status as the
    // current keyframe and the given duration.
    // Find the last keyframe
    Keyframe* keyframe = this->first_keyframe;
    while(keyframe->next != nullptr) {
        keyframe = keyframe->next;
    }
    this->addKeyframeFromArray(keyframe->bar_status, duration_ms);
}

// Create a function called _appendKeyframe that accepts a keyframe and appends it to the end of the animation. This
//  is a helper function for the overloaded addKeyframe function.
void AnimateLevel::_appendKeyframe(Keyframe *new_keyframe) {
    // If the keyframe is the first Keyframe in the animation, set it as the first keyframe and the current keyframe.
    // Otherwise, add it to the end of the animation.
    if (this->first_keyframe == nullptr) {
        this->first_keyframe = new_keyframe;
        this->current_keyframe = new_keyframe;
    } else {
        Keyframe *keyframe = this->first_keyframe;
        // We could also track a tail pointer to avoid this loop. Not too concerned about speed, so save the memory.
        while (keyframe->next != nullptr) {
            keyframe = keyframe->next;
        }
        keyframe->next = new_keyframe;
    }
}

void AnimateLevel::start() {
    // Start the animation by setting the current keyframe to the first keyframe and setting the running flag to true.
    this->current_keyframe = this->first_keyframe;
    this->last_keyframe = this->first_keyframe;
    // Force all the bars to match the first keyframe. Could use
    // solar_bars->setAllBars(this->current_keyframe->bar_status) but we also want to count the number of bars on so
    // loop manually.
    this->num_bars_on = 0;
    for(int i = 0; i < _BARS_NUM_BARS; i++) {
        this->solar_bars->setBar(i, this->current_keyframe->bar_status[i]);
        this->num_bars_on += this->current_keyframe->bar_status[i];
    }
    this->running = true;
}

void AnimateLevel::stop() {
    // Stop the animation by setting the running flag to false.
    this->running = false;
}

void AnimateLevel::update() {
    // First, check if we're running. If we're not, do nothing. Then see if the current keyfram is the same as the last
    // keyframe (current_keyframe is incremented when a keyframe finishes). If it is, check if the bars need to be
    // updated. If they do, update them. If it is not, calculate the time between each bar update and determine if we
    // are incrementing or decrementing the bars.
    static int num_bars_to_update = 0; // Number of bars that need to be updated
    static unsigned long time_between_bar_updates_ms = 0;
    static bool incrementing = false;

    if(this->isRunning()) {
        // If the current keyframe is the same as the last keyframe, we're still updating the bars for this keyframe.
        if(this->current_keyframe == this->last_keyframe) {
            // Check if enough time has passeded to update the bars. 
            if(millis() - this->last_update_time_ms >= time_between_bar_updates_ms) {
                if(incrementing && num_bars_to_update > 0) {
                    this->solar_bars->setBar(this->num_bars_on, true);
                    this->num_bars_on++;
                } else if (num_bars_to_update > 0) {
                    // need to subtract 1 from num_bars_on otherwise it turns off the bar above the last bar that was
                    // turned on. Don't have to do this for incrementing case because we are trying to turn on the bar
                    // above the last bar that was turned on.
                    this->solar_bars->setBar(this->num_bars_on-1, false);
                    this->num_bars_on--;
                }
                num_bars_to_update--;
                this->last_update_time_ms = millis();
                if(num_bars_to_update <= 0) { // Will be less than 0 if it started at 0 (i.e., a pause frame)
                    this->last_keyframe = this->current_keyframe;
                    this->current_keyframe = this->current_keyframe->next;
                    if(this->current_keyframe == nullptr) {
                        this->stop();
                    }
                }
            }
        } else {
            // If the current keyframe is not the same as the previous keyframe, we've transistioned to a new keyframe.
            // Calculated how many bars changed from the previous keyframe to the current keyframe and if we're
            // incrementing or decrementing the bars. Then, determine the number of milliseconds between each bar. We
            // also need to update the last_keyframe to the current_keyframe.
            num_bars_to_update = // Is < 0 if decrementing
                this->_getNumberOfBarsToUpdate(this->last_keyframe->bar_status, this->current_keyframe->bar_status);
            incrementing = num_bars_to_update >= 0;
            num_bars_to_update = abs(num_bars_to_update); // Fix the sign
            // Could segfault if num_bars_to_update = 0 and we still want to way for duration_ms to pass, so set 1 as 
            // the minimum divide by value here. This forces time_between_bar_updates_ms to be equal to duration_ms if
            // there are no bars to update 
            time_between_bar_updates_ms = this->current_keyframe->duration_ms / max(num_bars_to_update, 1);

            this->last_keyframe = this->current_keyframe;
        }
    }
}

Keyframe *AnimateLevel::getCurrentKeyframe() { 
    return this->current_keyframe; 
}

void AnimateLevel::setBars(SolarBars *solar_bars) {
    this->solar_bars = solar_bars;
}

bool AnimateLevel::isRunning() { 
    return this->running; 
}

void AnimateLevel::updateKeyframe(unsigned int index, bool bar_status[_BARS_NUM_BARS], unsigned int duration_ms) {
    // Update the keyframe at the given index with the given values.
    // Create the new keyframe
    Keyframe* new_keyframe = new Keyframe;
    for(int i = 0; i < _BARS_NUM_BARS; i++) {
        new_keyframe->bar_status[i] = bar_status[i];
    }
    new_keyframe->duration_ms = duration_ms;
    new_keyframe->next = nullptr;

    // Find the keyframe at the given index and update it. If the keyframe is the first keyframe, update it and set it
    // as the first keyframe. Otherwise, find the keyframe before it and update it. 
    if(index == 0) {
        new_keyframe->next = this->first_keyframe->next;
        this->first_keyframe = new_keyframe;
    } else {
        Keyframe* temp_keyframe = this->first_keyframe;
        // Need to start at i = 1 because we already have the first keyframe, so if i = 0, we will be one keyframe
        // ahead. We need index - 1 because we need to stop at the keyframe before the one we want to update. 
        for(int i = 1; i < index-1; i++) {
            temp_keyframe = temp_keyframe->next;
        }
        new_keyframe->next = temp_keyframe->next->next;
        temp_keyframe->next = new_keyframe;
    }
}

int AnimateLevel::_getNumberOfBarsToUpdate(bool last_bar_status[_BARS_NUM_BARS], bool current_bar_status[_BARS_NUM_BARS]) { 
    // Returns the difference in the number of bars there are on between last_bar_status and current_bar_status. Returns
    // a negative value if there are more bars on in last_bar_status than current_bar_status (i.e., decrementing)
    int num_bars_on = 0;
    for(int i = 0; i < _BARS_NUM_BARS; i++) {
        num_bars_on += current_bar_status[i] - last_bar_status[i];
    }

    return num_bars_on;
}
