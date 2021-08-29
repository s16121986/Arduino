#Контроллер увлажнителя воздуха

##Управление RS485

#####Карта регистров
| Регистр/адрес | Тип | Чтение/запись | Назначение |
|:----:|:----:|:----------:|:---|
| 0 | coil | RW | Состояние (вкл./выкл.) |
| 1 | coil | RW | Наличие воды (при записи - налить воды) |
| 0 | discrete input | R | Состояние фильтра |
| 0 | holding | RW | Мощность вентилятора (0..100%) |

```
#Пример
#SlaveID=73

#Считать показания влажности
modbus_client -mrtu --debug -b9600 -d8 -pnone -s1 /dev/RS485 -a73 -t0x01 -r0x00
```

##Самодельный увлажнитель и комплектующие
* [Вариант 1](https://www.youtube.com/watch?v=9MsUBx0P6Wo)
* [Вариант 2](https://www.youtube.com/watch?v=5DoEKvVV5zI)
* [Испарительный фильтр](https://breeeth.com/catalog/filtry_breeeth/breeeth_natural_filtr_isparitelnyy/)

##Датчики

* BME280

    _Датчик воды_

    [Подключение по шине I²C](https://arduinka.pro/blog/podklyuchenie/podklyuchenie-bmp280-k-arduino-uno/)
