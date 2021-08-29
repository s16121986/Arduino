#Контроллер управления смывного бачка

##Управление RS485

#####Карта регистров
| Регистр/адрес | Тип | Чтение/запись | Назначение |
|:----:|:----:|:----------:|:----|
| 0 | coil | RW | Состояние контроллера (on/off) |
| 1 | coil | W | Смыв (половина) |
| 2 | coil | W | Смыв (полный) |
| 0 | holding | RW | Время неполного смыва (ms х 10) |
| 1 | holding | RW | Время полного смыва (ms х 10) |
| 2 | holding | RW | Время нахождения для неполного смыва (sec) |
| 3 | holding | RW | Время нахождения для полного смыва (sec) |
| 4 | holding | RW | Пауза перед автоматическим смывом (ms х 10) |
| 5 | holding | RW | Режим оптического датчика (on/off) |
| 6 | holding | RW | Режим звукового сигнала (on/off) |

```
#Пример
#SlaveID=75

#Считать показания влажности
modbus_client -mrtu --debug -b9600 -d8 -pnone -s1 /dev/RS485 -a75 -t0x04 -r11
```

##Моудули arduino

* Servo MG996R

    _Мощный сервопривод с крутящим моментом 11 кг•см_

    [Подключение](https://arduinomaster.ru/motor-dvigatel-privod/servoprivody-arduino-sg90-mg995-shema-podklyuchenie-upravlenie/)

* KGS-812A

    _Инфракрасный индукционный фотоэлектрический переключатель постоянного тока 5 В, активный смеситель, световой короб с объектом, Регулируемый датчик рассеянного отражения_

    [Описание](https://aliexpress.ru/item/32924566111.html?spm=2114.13010708.0.0.569733edH4fzUV&_ga=2.240327235.1751222717.1624299732-13203271.1589879025&_gac=1.95691246.1624083932.CjwKCAjwiLGGBhAqEiwAgq3q_tSzBMuBle1FD3e0juSIAgwwJt6U1ldDZepZajvxaWgK-oMccMq6-BoCS9YQAvD_BwE&sku_id=66168372472&item_id=32924566111)
    
    https://www.gammon.com.au/power
