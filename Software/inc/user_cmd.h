#ifndef _USER_CMD_H_
#define _USER_CMD_H_

#define USER_CMD_SUCCESS (0)
#define USER_CMD_FAIL    (-1)
#define CMD_IS_PARSED    (0)
#define CMD_NOT_FINISHED (-1)
#define CMD_IS_RUNNING   (-2)
#define CMD_IS_WRONG     (-3)

extern signed char getUserCmd(void);
extern signed char excuteCmd(void);
extern void sendSuccessToUser(void);
extern void sendBusyToUser(void);
extern void sendFailToUser(void);

#endif
