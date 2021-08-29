#Контроллер клапанов воды
##Оборудование
* [Gidrolock Ultimate](file:///D:/YandexDisk/Документы/Инструкции/Сантехника/GIDROLOCK_ULTIMATE_.pdf)

##Управление RS485

#####Карта регистров
| Регистр/адрес | Тип | Чтение/запись | Назначение | По умолчанию |
|:---:|:---:|:---:|:----------------------------------------|:---:|
| 0 | coil | RW | Состояние холодной воды (вкл./выкл.) |
| 1 | coil | RW | Состояние горячей воды (вкл./выкл.) |
| 0 | holding | RW | Время закрытия/открытия клапана (sec) | 1500 |
| 10 | holding | R | Показаня холодной воды |
| 11 | holding | R | Показаня горячей воды |
| 12 | holding | R | Температура холодной воды |
| 13 | holding | R | Температура горячей воды |

```
#Пример
#SlaveID=72

#Считать показания влажности
modbus_client -mrtu --debug -b9600 -d8 -pnone -s1 /dev/RS485 -a72 -t0x01 -r0x01
```

#####Кабели
| Gidrolock |  | Controller | Назначение |
|:----:|:----:|:----------:|:----------:|
| Черный | -> | Белый | Управление (замыкание на GND) |
| Коричневый | -> | Синий | +12V |
| Синий | -> | Черный | GND |

* Соединение черного провода с синим проводом (GND): закрытие шарового электропривода
* Отсоединение черного провода от синего провода (GND): открытие шарового электропривода.

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
