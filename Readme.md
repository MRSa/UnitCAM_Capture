# UnitCAM_Capture : M5 Unit CAMを活用するためのあれこれ

このリポジトリには、[M5 Unit CAM](https://docs.m5stack.com/en/unit/unit_cam) を活用するためのあれこれを置きます。

## CameraWebServerMod-ForUnitCAM

ESP32 Example の [CameraWebServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer) を [M5 Unit CAM](https://docs.m5stack.com/en/unit/unit_cam) で動作させるために、修正してみました。

## Docker/unitcamserver

[M5 Unit CAM](https://docs.m5stack.com/en/unit/unit_cam) から周期的に画像を取得して保存するスクリプトを実行するコンテナです。

## Docker/nginx

画像を一覧表示する、などを行うnginxコンテナです。 **セキュリティに関しては未考慮です。** ご注意ください。
