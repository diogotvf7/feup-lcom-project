#define FPS                           30
#define PACKETS_PER_INTERRUPT         10
#define QUEUE_LIMIT                   20
#define XMIT_TRIES                    5
#define RCVR_TRIES                    5
#define UART_cOOLDOWN                 15
#define ROUND_TIME 60


// ESC    |  1       |  2     |  3      |  4     |  5     |  6     |  7     |  8     |  9       |  0     |  ?     |  «       |  BACK    |  INS
// 0x01   |  0x02    |  0x03  |  0x04   |  0x05  |  0x06  |  0x07  |  0x08  |  0x09  |  0x0a    |  0x0b  |  0x0c  |  0x0d    |  0x0e    |  0xe052
// -----------------------------------------------------------------------------------------------------------------------------------------------
// TAB    |  Q       |  W     |  E      |  R     |  T     |  Y     |  U     |  I     |  O       |  P     |  +     |  ´       |  ENTER   |  DEL
// 0x0f   |  0x10    |  0x11  |  0x12   |  0x13  |  0x14  |  0x15  |  0x16  |  0x17  |  0x18    |  0x19  |  0x1a  |  0x1b    |  0x1c    |  0xe053
// -----------------------------------------------------------------------------------------------------------------------------------------------
// CAPS   |  A       |  S     |  D      |  F     |  G     |  H     |  J     |  K     |  L       |  Ç     |  º     |  ~       |          |  PGUP
// 0x3a   |  0x1e    |  0x1f  |  0x20   |  0x21  |  0x22  |  0x23  |  0x24  |  0x25  |  0x26    |  0x27  |  0x28  |  0x2b    |          |  0xe049
// -----------------------------------------------------------------------------------------------------------------------------------------------
// SHIFT  |  <       |  Z     |  X      |  C     |  V     |  B     |  N     |  M     |  ,       |  .     |  -     |  SHIFT   |  UP      |  PGDN
// 0x2a   |  0x56    |  0x2c  |  0x2d   |  0x2e  |  0x2f  |  0x30  |  0x31  |  0x32  |  0x33    |  0x34  |  0x35  |  0x36    |  0x0e48  |  0xe051
// -----------------------------------------------------------------------------------------------------------------------------------------------
// CTRL   |  WIN     |  ALT   |  SPACE  |        |        |        |        |        |  ALTGR   |  FN    |  MENU  |  LEFT    |  DOWN    |  RIGHT
// 0x1d   |  0x0e56  |  0x38  |  0x39   |        |        |        |        |        |  0x0e38  |        |        |  0x0e4b  |  0x0e50  |  0x0e4d

