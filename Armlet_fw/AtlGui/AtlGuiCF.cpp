#include "AtlGuiCF.h"
#include "cmd_uart.h"
void CallMainToReason()
{
    Uart.Printf("MainToReason\r");
}
void CallReasonToMain()
{
    Uart.Printf("ReasonToMain\r");
}
