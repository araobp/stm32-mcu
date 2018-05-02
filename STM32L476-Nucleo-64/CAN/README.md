# CAN

Try to see if CAN controller on STM32L476 interworks with my original CAN-UART adapter developed in ["can-bus" project](https://github.com/araobp/can-bus).

## Set up

```
                                    Teraterm on Win10 PC
                                           |
                                        UART/USB
 <STM32L476>                               |
CAN controller                       CAN controller MCP2515
      |                                    |
CAN tranceiver                       CAN tranceiver MCp2561
      |                                    |
    --+------- CAN bus (125kHz) -----------+--
```
