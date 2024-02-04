# Adding Solar Animations
The steps to add a solar animation are as follows:
1. Declare your variable name to display_animations.hpp:
```cpp
extern AnimateLeve animate_display_startup;
...
// Add your variable name here
extern AnimateLevel my_animation;
```
2. Define your vairable in display_animations.cpp:
```cpp
AnimateLevel animate_display_startup = AnimateLevel();
...
// Define your variable here
AnimateLevel my_animation = AnimateLevel();
```
3. Create a setup_my_animation() function in display_animations.cpp:
```cpp
void setup_my_animation() {
  my_animation.addKeyframe(0, 0);
  my_animation.addKeyframe(1000, 1);
  ...
}
```
4. Add your setup_my_animation() function to the setup_animations() function in display_animations.cpp:
```cpp
void setup_animations() {
  setup_startup();
  ...
  // Add your setup_my_animation() function here
  setup_my_animation();
}
```

Now your variable ```my_animation``` will be available to use in the main sketch. 

### Notes
- A keyframe with a single change will perform that change at the beginning of the keyframe. For example, if 2 bars are on and you request 3 to be on with a duration of 1 second, the 3rd bar will turn on immediately and the keyframe will wait for 1 second to complete. If you have two or more changes in a keyframe, the changes will be evenly distributed throughout the duration of the keyframe, ending with a change. For example, if 2 bars are on and you request 5 to be on with a duration of 1 second, the 3rd bar will turn on immediately, the 4th bar will turn on after 0.5 seconds, and the 5th bar will turn on after 1 second.
- The sun's status is always evaluated at the beginning of a keyframe.