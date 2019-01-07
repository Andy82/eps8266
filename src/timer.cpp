#include <time.h>               //Содержится в пакете
#include <timer.h> 

int timeZone;

void Time_init() {
  timeZone = jConfig.jRead("timezone").toInt();
  timeSynch(timeZone);
}

void timeSynch(int zone) {
  if  (jConfig.jRead("WifiMode") == "STA") {
    // Настройка соединения с NTP сервером
    configTime(zone * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");
    int i = 0;
    logger.log("Waiting for time");
    while (!time(nullptr) && i < 20) {
      logger.log(".");
      i++;
      delay(1000);
    }
    delay(100); //Задержка что бы время в системе отобразилось
    logger.log(GetDate() + " " + GetTime());
    jConfig.jWrite("systemStartedTime", GetDate() + "/" + GetTime()); 
    jConfig.jWrite("time", GetTime());
  }
}


// Получение текущего времени
String GetTime() {
    time_t now = time(nullptr); // получаем время с помощью библиотеки time.h
    String Time = ""; // Строка для результатов времени
    Time += ctime(&now); // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
    int i = Time.indexOf(":"); //Ишем позицию первого символа :
    Time = Time.substring(i - 2, i + 6); // Выделяем из строки 2 символа перед символом : и 6 символов после
    return Time; // Возврашаем полученное время
}

// Получение даты
String GetDate() {
    time_t now = time(nullptr); // получаем время с помощью библиотеки time.h
    String Data = ""; // Строка для результатов времени
    Data += ctime(&now); // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
    int i = Data.lastIndexOf(" "); //Ишем позицию последнего символа пробел
    String Time = Data.substring(i - 8, i+1); // Выделяем время и пробел
    Data.replace(Time, ""); // Удаляем из строки 8 символов времени и пробел
    Data.replace("\n", ""); // Удаляем символ переноса строки
    return Data; // Возврашаем полученную дату
}
