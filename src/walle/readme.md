# Adding Solar Animations
The steps to add a solar animation are as follows:
1. Declare your variable name to solar_animations.hpp:
```cpp
extern AnimateLeve startup_animation;
...
// Add your variable name here
extern AnimateLevel my_animation;
```
2. Define your vairable in solar_animations.cpp:
```cpp
AnimateLevel startup_animation = AnimateLevel();
...
// Define your variable here
AnimateLevel my_animation = AnimateLevel();
```
3. Create a setup_my_animation() function in solar_animations.cpp:
```cpp
void setup_my_animation() {
  my_animation.addKeyframe(0, 0);
  my_animation.addKeyframe(1000, 1);
  ...
}
```
4. Add your setup_my_animation() function to the setup_animations() function in solar_animations.cpp:
```cpp
void setup_animations() {
  setup_startup_animation();
  ...
  // Add your setup_my_animation() function here
  setup_my_animation();
}
```

Now your variable ```my_animation``` will be available to use in the main sketch. 