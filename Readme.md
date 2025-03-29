# UnitCAM_Capture : M5 Unit CAMを活用するためのあれこれ

このリポジトリには、[M5 Unit CAM](https://docs.m5stack.com/en/unit/unit_cam) を活用するためのあれこれを置きます。

## リポジトリの中身について

### CameraWebServerMod-ForUnitCAM

ESP32 Example の [CameraWebServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer) を [M5 Unit CAM](https://docs.m5stack.com/en/unit/unit_cam) で動作させるために、修正してみました。

### Docker/unitcamserver

[M5 Unit CAM](https://docs.m5stack.com/en/unit/unit_cam) から周期的に画像を取得して保存するスクリプトを実行するコンテナです。 定期的に curlで Unit CAMに接続して画像を取得するスクリプトを走らせるだけです。

### Docker/nginx

画像を一覧表示する、などを行うnginxコンテナです。 **セキュリティに関しては未考慮です。** ご注意ください。

## 簡単な使い方

![システム概要](https://github.com/MRSa/UnitCAM_Capture/blob/main/images/overall.png?raw=true)

1. 画像受信先の ngnixコンテナを起動します。
   * この後設定する、Unit CAMと同じネットワークに存在する場所にしてください。
2. Unit CAMにプログラムを転送し、起動します。
   * Unit CAMが Standalone モードで立ち上がります。
3. Unit CAMのWi-Fiアクセスポイントを探して接続し、Webブラウザで接続します。
4. Webブラウザから、Unit CAMに対し、Wi-Fiの設定を行います。
   * [http://192.168.20.1/setwifi?id=WWWWW&key=XXXXXX](http://192.168.20.1/setwifi?id=WWWWW&key=XXXXXX)
5. 続けて、Unit CAMから nginxコンテナに対して画像を定期的に送信するよう設定を行います。
   * [http://192.168.20.1/setpush?id=XXXXX&interval=YYYY&count=ZZZZZ&url=http://ooo.ooo.ooo.ooo:8765/dav/](setpush?id=XXXXX&interval=YYYY&count=ZZZZZ&url=http://ooo.ooo.ooo.ooo:8765/dav/)
6. 設定が反映されているか、念のため確認します。
   * [http://192.168.20.1/chkwifi](http://192.168.20.1/chkwifi)
   * [http://192.168.20.1/chkmode](http://192.168.20.1/chkmode)
7. Unit CAMを再起動 or 一度電源断して起動することで、設定が更新され、画像の定期送信が始まります。
   * 設定が間違っていた場合、Standalone モードで立ち上がりますので、設定を見直してください。
8. nginx コンテナにWebブラウザにアクセスし、画像が転送されていることを確認します。
   * [http://ooo.ooo.ooo.ooo:8765/images/YYYYMMDD](http://ooo.ooo.ooo.ooo:8765/images/YYYYMMDD) : 転送した画像ファイルの一覧(年月日別、　YYYYMMDD は年月日)
   * [http://ooo.ooo.ooo.ooo:8765/images/YYYYMMDD/thumbnails](http://ooo.ooo.ooo.ooo:8765/images/YYYYMMDD/thumbnails) : 転送した画像ファイルのサムネイル一覧(年月日別、　YYYYMMDD は年月日)
