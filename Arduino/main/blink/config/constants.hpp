//------------Port codes------------

#define IO_PING_BIT               48 // "0"
#define IO_CONFIG_BIT             59 // ";"
#define IO_EVENT_BIT              60 // "<"
#define IO_TRIGGER_BIT            61 // "="
#define IO_STATE_BIT              62 // ">"
#define IO_STOP_BIT              255 // "�"
#define IO_DEBUG_BIT              33 // "!"

//------------Modules ID------------

#define T_ALL                    254 // Всем

#define T_HOME                   253 // Дом

#define T_KITCHEN                203 // Кухня
#define M_KH_SWITCH               32 // Выключатель

#define T_BEDROOM                204 // Спальня
#define M_BD_PROJECTOR            36 // Датчик состояния проектора
#define M_BD_SWITCH               45 // Выключатель
#define M_BD_LIGHTNING_SWITCH     56 // Выключатель подсветки

#define T_BALCONY                205 // Балкон
#define M_BL_WARMFLOOR            24 // Теплый пол
#define M_BL_SWITCH               39 // Выключатель

#define T_BATHROOM               202 // Ванная
#define M_BT_SWITCH_FAN           16 // Выключатель вытяжки
#define M_BT_SWITCH               17 // Выключатель свет
#define M_BT_MOTION               22 // Датчик движения
#define M_BT_WARMFLOOR            30 // Теплый пол
#define M_BT_LEAK                 37 // Датчик протечки

#define T_HALLWAY                201 // Прихожая
#define M_HL_SWITCH_HALL           9 // Выключатель холл
#define M_HL_SWITCH               11 // Выключатель
#define M_HL_DOOR                 12 // Датчик двери
#define M_HL_MOTION               13 // Датчик движения

#define T_STORAGE                206 // Кладовка
#define M_ST_SWITCH                4 // Выключатель
#define M_ST_DOOR                  5 // Датчик двери

#define M_BD_CURTAINS             47 //

//-------------Commands-------------

#define C_DISABLE                  1 // Деактивировать
#define C_ENABLE                   2 // Активировать
#define C_ON                       3 // Включить
#define C_OFF                      4 // Выключить
#define C_TOGGLE                   5 // Переключить
#define C_PWM                      6 // ШИМ
#define C_TEMPERATURE             21 // Температура
#define C_DEBUG                  251 // Отладка
#define C_SETPIN                 252 // Задать pin
#define C_PING                   253 // ping
#define C_STATE                  254 // Состояние

//--------------Events--------------

#define EVENT_BUTTON_PRESS        11 //
//#define EVENT_BUTTON_RELEASE      12 //
#define EVENT_BUTTON_HOLD         13 //
#define EVENT_SENSOR_STATE        14 //
#define EVENT_MOTION_DETECT       15 //

#define EVENT_INIT                 1 //