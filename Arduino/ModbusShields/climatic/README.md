#Климат контроллер
https://wirenboard.com/wiki/WB-MS_v_2_registers

##Управление RS485

#####Карта регистров
| Регистр/адрес | Тип | Чтение/запись | Назначение |
|:----:|:----:|:----------:|:---|
| 0 | coil | RW | Состояние датчиков (вкл./выкл.) |
| 0 | input | R | Температура |
| 1 | input | R | Влажность |
| 2 | input | R | Углекислый газ |
| 3 | input | R | Давление |
| 4 | input | R | Пыль |

```
#Пример
#SlaveID=71
#увеличенный таймаут 5с

#Считать показания влажности
modbus_client -mrtu --debug -b9600 -d8 -o5000 -pnone -s1 /dev/RS485 -a71 -t0x04 -r11
```

##Датчики

* BME280

    _Датчик давления, температуры и влажности_

    [Подключение по шине I²C](https://arduinka.pro/blog/podklyuchenie/podklyuchenie-bmp280-k-arduino-uno/)

* Sharp Optical Dust Sensor GP2Y1010AU0F

    _Оптический датчик пыли Sharp GP2Y1010AU0F эффективен в обнаружении очень мелких частиц, и обычно используется в системах очистки воздуха_

    [Подключение](https://create.arduino.cc/projecthub/mircemk/diy-air-quality-monitor-with-sharp-gp2y1010au0f-sensor-7b0262)
    
    [Видео](https://www.youtube.com/watch?v=LKrntMQ4EuI)

* MH-Z19

    _Датчик углекислого газа_

    [Подключение ШИМ](https://arduino-ide.com/modules/48-podkljuchenie-mh-z19-k-arduino.html)
    
    [Подключение UART](https://github.com/crisap94/MHZ19)

* MQ135

    _Датчик утечки газа_

    [Подключение и описание](https://portal-pk.ru/news/285-mq135-datchik-uglekislogo-gaza-biblioteka-mq135-arduino-library.html)
