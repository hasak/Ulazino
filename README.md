# Ulazino

This project implements an **RFID-based access control system** using an ESP8266. It supports multiple admin and user RFID cards, controls an output (like a door lock), and includes a simple web interface for remote unlocking.

## Hardware

* **ESP8266** (NodeMCU or Wemos D1 Mini)
* **MFRC522 RFID module**

  * **SDA/SS** → D1
  * **RST** → D2
  * **SPI (SCK, MOSI, MISO)** → standard ESP8266 SPI pins
* **Relay / Door Control** → D3
* **Optional buzzer** → any digital pin (currently commented out)

## Pin Configuration

| Function          | Pin         |
| ----------------- | ----------- |
| RFID SS           | D1          |
| RFID RST          | D2          |
| Door/Relay        | D3          |
| Built-in LED      | LED_BUILTIN |
| (Optional) Buzzer | 5           |

Logic levels:

```cpp
#define HI 0 // Active LOW relay
#define LO 1
```

## Wi-Fi

Connects to a predefined Wi-Fi network:

```cpp
const char* ssid     = "Rezidencija Hasak";
const char* password = "pisenamodemu";
```

The ESP uses a **static IP**:

```cpp
IPAddress local_IP(192, 168, 1, 9);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
```

## Software Features

* **Admin cards**: Can be defined in the `admini[]` array. They have full access.
* **User cards**: Can be added to the `ulazi[]` array.
* **Web unlock**: Access a page `http://192.168.1.9:3221/otkljucaj?kod=pustimeunutra` to unlock remotely.
* **Output control**: Relay is active for 5 seconds when access is granted.
* **Serial monitoring**: Prints scanned RFID tags and access status for debugging.

## RFID Handling

* Each card UID is converted to a string for comparison.
* Function `provjeri()` checks if the scanned card is in `ulazi[]`.
* Function `jelAdmin()` checks if the scanned card is in `admini[]`.
* If access is granted, `prosao()` triggers the relay and optional buzzer.
* If access is denied, `nijeProsao()` can trigger the buzzer.

## Web Interface

* Simple HTML page returns a message and closes itself:

```html
<!doctype html>
<html lang='bs'>
<head><title>Otkljucavanje...</title></head>
<body>
<p>Ova stranica bi se trebala zatvoriti sama...</p>
<script>window.history.back();</script>
</body>
</html>
```

* Unlock command via URL: `/otkljucaj?kod=pustimeunutra`.

## Notes

* **SPI and RFID library** must be installed (`MFRC522` library for Arduino).
* Optional buzzer code is commented out but can be enabled for feedback.
* Check your relay logic; this sketch uses **active LOW**.
* Ensure proper security if exposing the web unlock functionality.

