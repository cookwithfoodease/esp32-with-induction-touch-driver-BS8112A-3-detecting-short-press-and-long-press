# ESP32 Touch Input Processing with BS8112A-3 and Buzzer Feedback

This project implements a touch input processing system using the **ESP32 microcontroller**, the **BS8112A-3 capacitive touch chip**, and a **buzzer** for audio feedback. The code detects and handles both **short** and **long presses** on touch inputs and provides configurable sensitivity.

---

## Features

### 1. **Touch Input Handling**
- Detects both **short presses** and **long presses** using touch inputs.
- Configurable **long press threshold** set to **2000 ms (2 seconds)**.
- Short presses and long presses are identified and logged to the Serial Monitor.

### 2. **Audio Feedback**
- **Buzzer Alerts**:
  - Short buzz for short presses.
  - Long buzz for long presses.

### 3. **Sensitivity Calibration**
- Adjustable touch sensitivity for the **BS8112A-3 capacitive touch chip**.
- Sensitivity values range from **8** (lowest) to **63** (highest).
- Default sensitivity is set to **100**.

### 4. **I²C Communication**
- Reads touch inputs from **BS8112A-3** via I²C interface.
- Uses registers `0x08` and `0x09` to fetch key statuses.

---

## Hardware Requirements

- **ESP32 Development Board**
- **BS8112A-3 Capacitive Touch Chip**
- **Buzzer**
- **Touch Input Setup** (e.g., touch-sensitive buttons or pads)
- **Resistors** (for pull-up configuration, if needed)
- **Jumper Wires**
- **Power Source**

---

## Software Requirements

- **Arduino IDE** (v1.8 or later)
- Install the following libraries via Arduino Library Manager:
  - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
  - [Adafruit LED Backpack Library](https://github.com/adafruit/Adafruit_LED_Backpack)

---

## How It Works

### **Touch Input Detection**
- The **BS8112A-3** chip monitors touch inputs and registers button statuses via I²C.
- Button status is fetched by reading from:
  - **Register `0x08`**: Low byte of the key status.
  - **Register `0x09`**: High byte of the key status.

### **Short and Long Press Handling**
1. When a touch input is detected:
   - If it’s a **short press** (less than 2000 ms), the system triggers a short buzz and logs the button ID.
   - If it’s a **long press** (2000 ms or more), the system triggers a long buzz and logs the button ID with a negative value.
2. Button release resets the detection mechanism for subsequent presses.

### **Sensitivity Calibration**
- The touch sensitivity of the **BS8112A-3** chip is calibrated at startup.
- Sensitivity is configured via register addresses ranging from **0xB5** to **0xC0**, covering all keys.

---

## Code Overview

### **Setup Function (`setup()`)**:
1. Initializes Serial communication for debugging.
2. Sets up I²C communication for the **BS8112A-3** chip.
3. Configures the **buzzer pin** as an output.
4. Calibrates touch sensitivity with a default value of **100**.

### **Loop Function (`loop()`)**:
1. Continuously checks for touch inputs via the `processTouch()` function.
2. Processes detected inputs:
   - Logs "Short press detected" or "Long press detected" to the Serial Monitor.
   - Activates the buzzer for feedback.

---

## Key Functions

### 1. **`processTouch()`**
- Reads the button status and determines if it's a short or long press.
- Triggers the corresponding buzzer feedback and logs the event.

### 2. **`buzzIndicator(int duration)`**
- Activates the buzzer for the specified duration in milliseconds.
- Used for both short and long press feedback.

### 3. **`readRegister(uint8_t reg)`**
- Reads a single byte from a specified I²C register on the **BS8112A-3** chip.

### 4. **`writeRegister(uint8_t reg, uint8_t value)`**
- Writes a single byte to a specified I²C register on the **BS8112A-3** chip.

### 5. **`calibrateTouchSensitivity(int level)`**
- Adjusts the sensitivity of all touch keys by writing to their corresponding I²C registers.

---

## Example Outputs

1. **Short Press:**
   - Button ID: `1`
   - Output: `Short press detected for button: 1`
   - Buzzer: Short buzz (100 ms)

2. **Long Press:**
   - Button ID: `1`
   - Output: `Long press detected for button: -1`
   - Buzzer: Long buzz (300 ms)

---

## Future Enhancements

1. Add functionality for multiple button actions.
2. Implement touch gestures (e.g., swipe).
3. Integrate a display module for real-time feedback.
4. Enhance feedback mechanisms using RGB LEDs or displays.

---

## License

This project is open-source and available under the [MIT License](LICENSE).

---

## Author

Developed by sundeep goud malkapuram

