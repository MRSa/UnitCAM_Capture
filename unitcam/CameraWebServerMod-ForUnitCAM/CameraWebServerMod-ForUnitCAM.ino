#include "esp_camera.h"
#include <WiFi.h>

//  Modified 'ESP32 CameraWebServer' example for the Unit CAM. : https://docs.m5stack.com/en/unit/unit_cam
//  ( https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer )
// -----------------------------------------------------------
//   Board: M5UnitCAM
//   Partition Scheme : Huge APP (3MB No OTA/1MB SPIFFS)
//   PSRAM : Disabled
// -----------------------------------------------------------

//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
//            You must select partition scheme from the board menu that has at least 3MB APP space.
//            Face Recognition is DISABLED for ESP32 and ESP32-S2, because it takes up from 15 
//            seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as well

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM
#include "camera_pins.h"

#include "Preferences.h"
// ===========================
// Enter your WiFi credentials
// ===========================
// const char* ssid = "**********";      // Do not need to set the SSID.
// const char* password = "**********";  // Do not need to set the password.

#define LED_GPIO_NUM       4  // NORMAL LED (Unit CAM)

void startCameraServer();
void setupLedFlash(int pin);

void startWifiAsAccessPointMode()
{
    const char standalone_ssid[] = "M5UnitCAM";
    const char pass[] = "M5U109";
    const IPAddress ip(192,168,20,1);
    const IPAddress subnet(255,255,255,0);
    WiFi.softAP(standalone_ssid,pass);
    delay(100);
    WiFi.softAPConfig(ip,ip,subnet);
    IPAddress myIP = WiFi.softAPIP();
    delay(100);
    Serial.println("");
    Serial.print("Start WiFi Standalone : ");
    Serial.println(myIP.toString().c_str());
    Serial.println("");
}

void prepareCamera()
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  //config.xclk_freq_hz = 20000000;
  config.xclk_freq_hz = 8000000;
  //config.frame_size = FRAMESIZE_UXGA;
  config.frame_size = FRAMESIZE_SVGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  //config.fb_location = CAMERA_FB_IN_PSRAM;
  config.fb_location = CAMERA_FB_IN_DRAM;
  //config.jpeg_quality = 12;
  config.jpeg_quality = 4;
  config.fb_count = 1;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }
  Serial.println("prepare camera");

  // camera init
  esp_err_t ret = esp_camera_init(&config);
  if (ret != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", ret);
    return;
  }

  Serial.println("initialize camera");

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
#if defined(CAMERA_MODEL_M5STACK_UNITCAM)
  s->set_framesize(s, FRAMESIZE_SVGA);

  int xclk = config.xclk_freq_hz;
  s->set_xclk(s, LEDC_TIMER_0, xclk);
  s->set_quality(s, 4);
  s->set_reg(s, 0x111, 0x80, 0x80);
  s->set_hmirror(s, 1);
  s->set_vflip(s, 1);

  Serial.println("");
#else
  // drop down frame size for higher initial frame rate
  if(config.pixel_format == PIXFORMAT_JPEG){
    s->set_framesize(s, FRAMESIZE_QVGA);
  }
#endif

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

}

void prepareWiFi()
{

  // ===== Read Wifi Keys
  Preferences preferences;
  preferences.begin("wifi_keys");
  String wifi_ssid = preferences.getString("ssid");
  String wifi_key = preferences.getString("key");
  preferences.end();

  Serial.print("Pref. SSID: ");
  Serial.println(wifi_ssid.c_str());
  Serial.print("Pref. KEY : ");
  Serial.println(wifi_key.c_str());
  if ((strlen(wifi_ssid.c_str()) <= 0)||(strlen(wifi_ssid.c_str()) <= 0))
  {
    Serial.println("");
    Serial.println(" Wi-Fi Key is not Registered... Run SoftAP Mode.");

    startWifiAsAccessPointMode();
  }
  else
  {
    WiFi.begin(wifi_ssid.c_str(), wifi_key.c_str());
    WiFi.setSleep(false);

    int connectDelay = 0;
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      connectDelay++;
      if (connectDelay > 90)
      {
        Serial.println("Connection Time out ... Change SoftAP Mode.");
        WiFi.disconnect();
        delay(500);
        WiFi.mode(WIFI_MODE_AP);

        startWifiAsAccessPointMode();
        break;
      }
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WiFi connected");
    }
  }

}

void sendCapturedImage()
{
  // ===== Read Connection mode
  Preferences preferences;
  preferences.begin("action_mode");
  bool push_mode = preferences.getBool("push_mode");
  bool user_pass = preferences.getBool("userpass");
  String device_id = preferences.getString("id");
  String report_interval = preferences.getString("interval");
  String report_count = preferences.getString("count");
  String report_method = preferences.getString("method");
  String report_url = preferences.getString("url");
  String report_user = preferences.getString("user");
  String report_pass = preferences.getString("pass");
  preferences.end();

  Serial.println("");
  Serial.println(" ----- Parameters -----");
  Serial.print("Mode:");
  if (push_mode)
  {
      Serial.println("Agent (Push)");
      Serial.print("device name: ");
      Serial.println(device_id);
      Serial.print("interval: ");
      Serial.println(report_interval);
      Serial.print("count: ");
      Serial.println(report_count);
      Serial.print("url: ");
      Serial.println(report_url);
    }
  else
  {
      Serial.println("Web Server (Pull)");
  }
  Serial.println(" - - - - - - - - - - - ");
  Serial.println("");
}


void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("");

  prepareCamera();

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  prepareWiFi();

  Preferences preferences;
  preferences.begin("action_mode");
  bool push_mode = preferences.getBool("push_mode");
  String report_interval = preferences.getString("interval");
  preferences.end();

  if (push_mode)
  {
    // ----- Agent(PUSH) MODE
    Serial.println("Agent (Push) Mode");
    sendCapturedImage();

    unsigned long interval = atoi(report_interval.c_str());
    if (interval <= 0)
    {
      interval = 120UL;
    }
    delay(3 * 1000); // wait 3 sec.
    Serial.print("... Enter Deep Sleep ... ");
    Serial.print(interval);
    Serial.println(" sec.");
    Serial.println("");
    //ESP.deepSleep(interval * 1000 * 1000UL);
    esp_sleep_enable_timer_wakeup(interval * 1000 * 1000UL);
    esp_deep_sleep_start();
    delay(1000);

    }
  else
  {
    // ----- Web Server(PULL) MODE
    Serial.println("Web Server (Pull) Mode");
    startCameraServer();
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
  }
}

void loop()
{
  delay(10000);

  Preferences preferences;
  preferences.begin("action_mode");
  bool push_mode = preferences.getBool("push_mode");
  String report_interval = preferences.getString("interval");
  preferences.end();
  if (push_mode)
  {
    Serial.println(" LOOP: Agent (Push) Mode");
    sendCapturedImage();

    unsigned long interval = atoi(report_interval.c_str()) * 1000UL;
    if (interval <= 0)
    {
      interval = 120UL;
    }
    //esp_sleep_enable_timer_wakeup(interval * 1000000); // -----
    //esp_light_sleep_start();
    delay(3000);
  }
}
