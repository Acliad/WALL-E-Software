#include <Ps3Controller.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <DFMiniMp3.h>

// #define MOTOR_RIGHT_REVERSE_DIRECTION
// #define MOTOR_LEFT_REVERSE_DIRECTION

#define PCA9685_OSCILLATION_FREQ (25000000) // TODO: Needs to be tuned?
#define SERVO_FREQ_HZ (50) // Analog servos run at ~50 Hz updates

#define DEFAULT_AUDIO_VOLUME (24)
#define STARTUP_TRACK_INDEX (1) // Set to 0 if you don't want to play anything

#define MOTOR_LEFT_IDX  (0)
#define MOTOR_RIGHT_IDX (1)

#define THUMBSTICK_MAX (127)
#define THUMBSTICK_MIN (-128)

#define PI_OVER_FOUR (PI/4.0)

const unsigned int motor_neutral_us = 1500;
const unsigned int motor_min_us = 1000;
const unsigned int motor_max_us = 2000;
const unsigned int motor_range_us = motor_max_us - motor_min_us;

int lmotor_us = motor_neutral_us;
int rmotor_us = motor_neutral_us;

// Have to manually track button events as the controller library
// doesn't appear to poll frequently enough to properly capture button
bool circle_pressed_last = false;
bool circle_pressed = false;

bool pca9685_connected = false;

DfMp3_Status dfmp3_status;
unsigned int audio_current_track = 0;
unsigned int audio_num_tracks = 1;

// forward declare the notify class, just the name
class Mp3Notify; 
// define a handy type using serial and our notify class
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; 

DfMp3 dfmp3 = DfMp3(Serial2);
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

// Function prototypes
void map_thumbstick(int thumbstick_x, int thumbstick_y, int* lmotor_us, int* rmotor_us);

void setup() {
    Serial.begin(115200);

    /**************************************************************
    *                     PCA9685 Servo Driver                    *
    **************************************************************/
    Serial.println("Initializing PCA9685");
    pca9685_connected = pca9685.begin();
    if (!pca9685_connected) {
        Serial.println("Initialization failed...");
    } else {
        pca9685.setOscillatorFrequency(PCA9685_OSCILLATION_FREQ); 
        pca9685.setPWMFreq(SERVO_FREQ_HZ);  // Analog servos run at ~50 Hz updates
    }
    

    /**************************************************************
    *                       PS3 Controller                        *
    **************************************************************/
    Serial.println("Initializing PS3 Controller...");
    Ps3.begin();
    Serial.println("Waiting for controller to connect...");
    while(!Ps3.isConnected()) {
        // Waiting...
        delay(100); // Not necessary, just prevents pinging isConnected() a ton
    }
    Serial.println("PS3 Controller connected!");


    /***************************************************************
    *                     DFMini Audio Driver                      *
    ***************************************************************/
    dfmp3.begin();
    dfmp3.reset(); // Could cause popping; can be removed after development.
    audio_num_tracks = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    dfmp3.setVolume(DEFAULT_AUDIO_VOLUME);
    if (STARTUP_TRACK_INDEX) {
        dfmp3.playMp3FolderTrack(STARTUP_TRACK_INDEX);
    }
}

void loop() {
    dfmp3.loop(); 
    // Map thumbstick values to motor drive values
    map_thumbstick(Ps3.data.analog.stick.lx, -Ps3.data.analog.stick.ly, &lmotor_us, &rmotor_us);
    if (pca9685_connected) {
        pca9685.writeMicroseconds(MOTOR_LEFT_IDX, lmotor_us);
        pca9685.writeMicroseconds(MOTOR_RIGHT_IDX, rmotor_us);
    }

    circle_pressed = Ps3.data.analog.button.circle;
    dfmp3_status = dfmp3.getStatus();
    if (!circle_pressed_last && circle_pressed && (dfmp3_status.state != DfMp3_StatusState_Playing)) { 
        Serial.print("Playing Track #");
        Serial.println(audio_current_track + 1);
        dfmp3.playMp3FolderTrack(audio_current_track + 1); // +1 for 1 indexing of filenames
        audio_current_track = (audio_current_track + 1) % audio_num_tracks;
    }
    circle_pressed_last = circle_pressed;

    // Debug printing
    // static long last_update_time = 0;
    // if (millis() - last_update_time > 500) {
    //     last_update_time = millis();
    //     Serial.print("event.button_down = ");
    //     Serial.println(Ps3.event.button_down.circle);
    //     Serial.print("data.analog.button.circle = ");
    //     Serial.println(Ps3.data.analog.button.circle, DEC);
    //     // Serial.print("calcul rmotor_us = ");
    //     // Serial.println(rmotor_us);
    // }

}

/**
 * @brief Maps thumbstick input to motor thrust values.
 *
 * This function takes thumbstick input coordinates and maps them to motor thrust
 * values for a robotic system. It calculates the angle and magnitude of the input
 * and maps it to motor thrusts in four quadrants. Additionally, it allows for
 * reversing the direction of individual motors if needed.
 *
 * @param thumbstick_x The x-coordinate of the thumbstick input.
 * @param thumbstick_y The y-coordinate of the thumbstick input.
 * @param[out] lmotor_us Pointer to store the left motor pulse width value (microseconds).
 * @param[out] rmotor_us Pointer to store the right motor pulse width value (microseconds).
 */
void map_thumbstick(int thumbstick_x, int thumbstick_y, int* lmotor_us, int* rmotor_us) {
    // Calculate the angle and magnitude of the thumbstick input
    double theta = atan2(thumbstick_y, thumbstick_x);
    double r = sqrt(thumbstick_x * thumbstick_x + thumbstick_y * thumbstick_y) / ((double) -THUMBSTICK_MIN);

    // Ensure magnitude is within the valid range [0, 1]
    if (r > 1.0f) r = 1;

    double rmotor_thrust;
    double lmotor_thrust;

    // Map thumbstick input to motor thrusts in four quadrants
    if (theta >= 0.0 && theta <= HALF_PI) {
        // Quadrant 1
        rmotor_thrust = r * (theta - PI_OVER_FOUR) / PI_OVER_FOUR;
        lmotor_thrust = r;
    }
    if (theta > HALF_PI && theta <= PI) {
        // Quadrant 2
        rmotor_thrust = r;
        lmotor_thrust = r * (PI_OVER_FOUR - (theta - HALF_PI)) / PI_OVER_FOUR;
    }
    if (theta >= -PI && theta <= -HALF_PI) {
        // Quadrant 3
        rmotor_thrust = r * ((theta + HALF_PI) - PI_OVER_FOUR) / PI_OVER_FOUR;
        lmotor_thrust = -r;
    }
    if (theta > -HALF_PI && theta < 0.0) {
        // Quadrant 4
        rmotor_thrust = -r;
        lmotor_thrust = r * (theta + HALF_PI - PI_OVER_FOUR) / PI_OVER_FOUR;
    }

    // Reverse motor directions if specified
    #ifdef MOTOR_RIGHT_REVERSE_DIRECTION
        rmotor_thrust *= -1;
    #endif
    #ifdef MOTOR_LEFT_REVERSE_DIRECTION
        lmotor_thrust *= -1;
    #endif

    // Calculate motor pulse width values and store them
    *rmotor_us = (int)round(motor_range_us / 2.0 * rmotor_thrust + motor_neutral_us);
    *lmotor_us = (int)round(motor_range_us / 2.0 * lmotor_thrust + motor_neutral_us);
}


class Mp3Notify
{
public:
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
        if (source & DfMp3_PlaySources_Sd) {
            Serial.print("SD Card, ");
        }
        if (source & DfMp3_PlaySources_Usb) {
            Serial.print("USB Disk, ");
        }
        if (source & DfMp3_PlaySources_Flash) {
            Serial.print("Flash, ");
        }
        Serial.println(action);
    }

    static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
    {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }

    static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
        Serial.print("Play finished for #");
        Serial.println(track);  
    }

    static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "online");
    }

    static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "inserted");
    }

    static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "removed");
    }
};
