#ifndef APP_TYPES_H
#define APP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CMD_NONE = 0,
    CMD_TOGGLE,
    // 后续可以继续扩展其他命令
} CmdType;

/*Day11: Sensor 事件类型 */
typedef enum
{
    SENSOR_EVT_READ = 1,     // 定时器触发：读取一次传感器
} SensorEvt_t;

#ifdef __cplusplus
}
#endif

#endif /* APP_TYPES_H */
