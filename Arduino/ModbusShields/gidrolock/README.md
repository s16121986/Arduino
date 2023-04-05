# Контроллер клапанов воды

## Оборудование

* [Gidrolock Ultimate](https://gidrolock.ru/upload/iblock/274/yqozl28agq0eb50e2gosbfprjw13kbk4/Pasports_GIDROLOCK_ULTIMATE_2022_04_06_.pdf)

## Управление RS485

##### Карта регистров

| Регистр/адрес | Тип | Чтение/запись | Назначение | По умолчанию |
|:---:|:---:|:---:|:----------------------------------------|:---:|
| 0 | coil | RW | Состояние тревоги (0 - Открыть; 1 - Закрыть воду) |
| 1 | coil | RW | Состояние холодной воды (вкл./выкл.) |
| 2 | coil | RW | Состояние горячей воды (вкл./выкл.) |
| 0 | discrete | R | Состояние датчиков протечки |
| 0 | holding | RW | Время закрытия/открытия клапана (sec) | 1500 |
| 10 | input | R | Показаня холодной воды |
| 11 | input | R | Показаня горячей воды |
| 12 | input | R | Температура холодной воды |
| 13 | input | R | Температура горячей воды |

```
#Пример
#SlaveID=72

#Считать показания влажности
modbus_client -mrtu --debug -b9600 -d8 -pnone -s1 /dev/RS485 -a72 -t0x01 -r0x01
```

##### Кабели

| Gidrolock |  | Controller | Назначение |
|:----:|:----:|:----------:|:----------:|
| Черный | -> | Белый | Управление (замыкание на GND) |
| Коричневый | -> | Синий | +12V |
| Синий | -> | Черный | GND |

* Соединение черного провода с синим проводом (GND): закрытие шарового электропривода
* Отсоединение черного провода от синего провода (GND): открытие шарового электропривода.

## Датчики

* BME280

  _Датчик давления, температуры и влажности_

  [Подключение по шине I²C](https://arduinka.pro/blog/podklyuchenie/podklyuchenie-bmp280-k-arduino-uno/)

* Sharp Optical Dust Sensor GP2Y1010AU0F

  _Оптический датчик пыли Sharp GP2Y1010AU0F эффективен в обнаружении очень мелких частиц, и обычно используется в
  системах очистки воздуха_

  [Подключение](https://create.arduino.cc/projecthub/mircemk/diy-air-quality-monitor-with-sharp-gp2y1010au0f-sensor-7b0262)

  [Видео](https://www.youtube.com/watch?v=LKrntMQ4EuI)

* MH-Z19

  _Датчик углекислого газа_

  [Подключение ШИМ](https://arduino-ide.com/modules/48-podkljuchenie-mh-z19-k-arduino.html)

  [Подключение UART](https://github.com/crisap94/MHZ19)

* MQ135

  _Датчик утечки газа_

  [Подключение и описание](https://portal-pk.ru/news/285-mq135-datchik-uglekislogo-gaza-biblioteka-mq135-arduino-library.html)

## ТЗ

Здравствуйте, возьметесь за задачу?

Разработать принципиальную схему и спроектировать печатную плату управления шаровыми электроприводами. Управлять будет
STM32 по RS485. Спецификация:

- форм фактор платы - https://www.gainta.com/pdf/d3mg.pdf
- необходимые выводы - https://disk.yandex.ru/i/4KrjHvHoEkx25w
- шаровые электроприводы 2шт (3 pin, питание
  12В) (https://gidrolock.ru/upload/iblock/274/yqozl28agq0eb50e2gosbfprjw13kbk4/Pasports_GIDROLOCK_ULTIMATE_2022_04_06_.pdf)
- датчики протечки 2шт (2
  pin) (https://www.vseinstrumenti.ru/santehnika/tovary-dlya-vannoj-komnaty-i-tualeta/aksessuary-dlya-vannoj-komnaty/datchiki-protechki-vody/datchiki-i-klapany/gidrolock/wsp-5-metrov-014544/)

Здравствуйте, есть задача разработать принципиальную схему управления шаровыми электроприводами и обьяснить на ее
примере принцип работы компонентов. Управлять будет STM32 по RS485.

- необходимые выводы - https://disk.yandex.ru/i/4KrjHvHoEkx25w
- шаровые электроприводы 2шт (3 pin, питание
  12В) (https://gidrolock.ru/upload/iblock/274/yqozl28agq0eb50e2gosbfprjw13kbk4/Pasports_GIDROLOCK_ULTIMATE_2022_04_06_.pdf)


есть 
- шаровые электроприводы 2шт (питание
12В) (https://gidrolock.ru/upload/iblock/274/yqozl28agq0eb50e2gosbfprjw13kbk4/Pasports_GIDROLOCK_ULTIMATE_2022_04_06_.pdf)
- датчики протечки 2шт (https://www.vseinstrumenti.ru/santehnika/tovary-dlya-vannoj-komnaty-i-tualeta/aksessuary-dlya-vannoj-komnaty/datchiki-protechki-vody/datchiki-i-klapany/gidrolock/wsp-5-metrov-014544/)
- линия RS485
нужно сделать плату управления приводами на базе STM32, необходимые входы-выходы ниже