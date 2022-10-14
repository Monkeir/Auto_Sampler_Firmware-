# Auto_Sampler_Firmware-
Прошивка для автосамплера ASVL-50/100. Сделан на основе платы калиматора ножа Димы Рекуца (https://github.com/xSouln/Firmware_CK/tree/example_pin_control).

Описание загрузки и прошивки

Установить следующее ПО: 

  1.IAR embedded software for ARM ver. 8+
  
  2.STM32CubeMX
  
  3.ST-link utility (не обязательно)
  
  4.Bootloader от Рекуца https://github.com/xSouln/Desktop_Bootloader_STM32
  
Вносим переменные среды, как указано по ссылке https://github.com/xSouln/Firmware_CK/tree/example_pin_control

Открываем STM32CubeMX и скачиваем библиотеки для контроллера серии F4: Help -> Manage Embedded software packages -> SMT32F4 

![STM32_package](https://user-images.githubusercontent.com/70260832/195881131-91e9da96-4bf8-4445-af74-d4eb7e97135a.png)

В случае, если недоступен пакет для скачивания - ищем на гитхабе/в папке обмена департамента

Копируем скачанный репозиторий в папку с проектом


Скачиваем репозиторий с прошивкой. Внутри бутлоадер (папка Bootloader CK) и основная прошивка (папка EWARM). В зависимости от того, на какой скорости будет работать протокол обмена, меняем параметр baudrate в файле usart.c проекта Bootloader CK (по дефолту 500000, но для автосамплера выставляем 460800, т.к. латроникс не поддерживает 500000), тоже самое делаем для основной прошивки.



Подключаем к плате питание 24 вольта и программатор, например st-link. 

Прошиваем бутлоадер напрямую через IAR или же через ST-Link utility (предварительно скомпилировав прошивку в IAR).

Компилируем основную прошивку.

Открываем Desktop Bootloader STM32 (программа для Winwdows) и следуем инструкции из https://github.com/xSouln/Desktop_Bootloader_STM32. 

Точно выполняем пуннкты из "Загрузка прошивки"! Если CRC не меняется, то делаете что-то неправильно.
