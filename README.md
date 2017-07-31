# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

[img_60mph]: ./src/fig/test_PI_PID_60mph.png ""
[img_50mph]: ./src/fig/test_PI_PID_50mph.png ""

# Description

 * The PID parameter is tuned by Ziegler-Nichols' Ultimate Gain method.
 * For Speed control, PID controller is also used. The reference speed is designed to be linear from 0 to 50.0[mph] within 15.0[sec].
 * The gain Kp is the coefficient to the error between the current speed and reference speed. 
 * The gain Ki is the coefficient to the integral of error from the initial time. If reference speed is simply desgined to be 50mph from starting point, the integral of error gets pretty big. Thus, the overshoot duration lasts longer.
 * The gain Kd is the coefficient to the gradient of the error variance. This tries to stabilize the change of the speed.
 * The controller successfully enabled the car to run entire course with 50mph and 60mph(narrowly).

Speed [50mph]         | Speed [60mph]
:-----------------------:|:-------------------------:
![alt text][img_50mph]   |  ![alt text][img_60mph]


## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 
