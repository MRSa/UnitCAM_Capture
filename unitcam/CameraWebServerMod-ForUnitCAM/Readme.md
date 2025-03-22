# CameraWebServerMod-ForUnitCAM

ESP32 Example の [CameraWebServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer) を [M5 Unit CAM](https://docs.m5stack.com/en/unit/unit_cam) で動作させるために、修正してみました。

[U109BB-flat_R3.stl](https://github.com/MRSa/GokigenOSDN_documents/blob/main/images/STL/U109BB-flat_R3.stl "Unit CAM用ベースボード") に、秋月電子通商で販売している[電池ボックス 単3×3本 リード線](https://akizukidenshi.com/catalog/g/g102667/)と一緒に固定させ、 秋月電子通商で販売している [ABS樹脂ケース(蝶番式・中) 117-TM](https://akizukidenshi.com/catalog/g/g107401/) に入れて、[CaseStand_117TM.stl](https://github.com/MRSa/GokigenOSDN_documents/blob/main/images/STL/CaseStand_117TM.stl)のスタンドで横向きで設置し、撮影できるようにしています。

![Unit CAM in 117-TM](https://github.com/MRSa/GokigenOSDN_documents/blob/main/images/attachments/UnitCAM2.jpg?raw=true)

## [CameraWebServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer)からの修正内容

* Unit CAM用の設定（SVGAサイズで画像の上下反転させて起動するようにしました。）

* 初回起動時、Unit CAMは Wi-Fi アクセスポイントモードで起動するようにしました。
  * Unit CAM の IPアドレスは、 192.168.20.1 です。 ( [http://192.168.20.1](http://192.168.20.1) にアクセスすると、Unit CAMにつながります。 )
* Wi-Fi のSSIDとパスワードをフラッシュメモリに書き込んで記録できるようにしました。
  * スタンドアローンで起動しているUnit CAMのアドレス(192.168.20.1)の場合は、「[http://192.168.20.1/setwifi?id=xxxxx&key=zzzzz]」(xxxxxはSSID、zzzzzはパスワード)で設定できます。
* Unit CAMが、設定したWi-FiのSSIDとパスワードを使って約１分間接続できなかった場合、Wi-Fiアクセスポイントモードで起動するようにしました。
  * 設定したパスワードが間違っていた場合は、アクセスポイントモードでSSIDとパスワードを再設定してみてください。
* httpのコマンドで、再起動できるようにしました。
  * 再起動後、新しいWi-Fiの設定が有効になります。( [http://192.168.20.1/restart](http://192.168.20.1/restart) で Unit CAMが再起動します。)

この対応により、Wi-Fi の SSID と パスワードを変更したくなったときに その都度 Arduino IDEでコンパイルし、Unit CAMにアップロードする必要がなくなりました。

## 追加したコマンドの一覧

以降の記述は、Unit CAMの IPアドレスが初期値の 192.168.20.1 だった場合です。

* [http://192.168.20.1/setwifi?id=WWWWW&key=XXXXXX](http://192.168.20.1/setwifi?id=WWWWW&key=XXXXXX)
  * Unit CAMのWi-Fi設定を SSID を WWWWW に、パスワードを XXXXX に設定します。
* [http://192.168.20.1/clearwifi](http://192.168.20.1/clearwifi)
  * Unit CAMのWi-Fi設定をクリアします。設定クリア後、Unit CAMを再起動させるとアクセスポイントモードで起動します。
* [http://192.168.20.1/chkwifi](http://192.168.20.1/chkwifi)
  * Unit CAMの現在のWi-Fi設定をJSON形式で応答します。Wi-Fi設定が正しく行われているか、確認にご使用ください。
* [http://192.168.20.1/restart](http://192.168.20.1/restart)
  * Unit CAMを再起動させます。 Wi-Fiの設定を変更した場合にご使用ください。

## その他

本ソースコードは、 [https://github.com/MRSa/GokigenOSDN_documents/tree/main/miscellaneous/M5/CameraWebServerMod-ForUnitCAM](https://github.com/MRSa/GokigenOSDN_documents/tree/main/miscellaneous/M5/CameraWebServerMod-ForUnitCAM) より管理場所を移動しました。

以上
