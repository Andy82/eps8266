void Controller(void) {

    HTTP.on("/configs.json",handle_ConfigJSON);     // формирование configs.json страницы для передачи данных в web интерфейс

    // API для устройства
    HTTP.on("/wifi.scan.json", handle_wifi_scan);    // сканирование сети на доступные точки доступа
    HTTP.on("/ssdp", handle_ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
    HTTP.on("/ssid", handle_ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
    HTTP.on("/ssidap", handle_ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
    HTTP.on("/restartWiFi", handle_restartWiFi); //TODO: Implement //Перизапустить wifi попытаться узнать будущий ip адрес перезагрузить устройство
    HTTP.on("/restart", handle_restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
    HTTP.on("/wifi", handle_wifi); //TODO: Implement

    //HTTP страницы для работы с FFS
    HTTP.on("/list", HTTP_GET, handle_FileList); //list directory
    HTTP.on("/edit", HTTP_GET, []() { //загрузка редактора editor
        if (!handle_FileRead("/edit.htm")) HTTP.send(404, "text/plain", "FileNotFound");  });
    HTTP.on("/edit", HTTP_PUT, handle_FileCreate); //Создание файла
    HTTP.on("/edit", HTTP_DELETE, handle_FileDelete); //Удаление файла
    HTTP.on("/edit", HTTP_POST, []() { //загрузка файла
            HTTP.send(200, "text/plain", "");
        }, handle_FileUpload);

    // SSDP дескриптор
    HTTP.on("/description.xml", HTTP_GET, []() {  getSSDP();  });

    HTTP.on("/time", handle_Time);     // Синхронизировать время устройства по запросу вида /Time
    HTTP.on("/timezone", handle_time_zone);    // Установка времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3

    //HTTP страницы для работы с FFS
    HTTP.on("/mqtt", handle_Set_MQTT);
    // Установка параметров ddns
    HTTP.on("/ddns", handle_ddns);

    //Выполнение команды из запроса
    HTTP.on("/cmd", HTTP_GET, []() {
            sCmd.runCommand(HTTP.arg("command"), HttpCallback);
        });

    //called when the url is not defined here
    //use it to load content from SPIFFS
    HTTP.onNotFound([]() {
        if (!handle_FileRead(HTTP.uri()))
            HTTP.send(404, "text/plain", "FileNotFound");
        });
}