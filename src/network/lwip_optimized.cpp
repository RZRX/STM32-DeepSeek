#include "lwip_optimized.h"
#include <lwip/opt.h>
#include <lwip/tcp.h>

LWIPOptimized::LWIPOptimized(QObject *parent)
    : QObject(parent) {
    // Настройка параметров для embedded систем
    MEM_SIZE = 4096;       // Уменьшение размера памяти
    TCP_MSS = 536;         // Оптимизация для медленных сетей
    TCP_SND_BUF = 2048;    // Буфер отправки
    lwip_init();
}

err_t LWIPOptimized::tcpOptimizedReceive(void *arg, struct tcp_pcb *pcb, 
                                       struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Закрытие соединения
        tcp_close(pcb);
        return ERR_OK;
    }
    // Быстрая обработка данных без копирования
    QByteArray data((char*)p->payload, p->len);
    emit dataReceived(data);
    pbuf_free(p);
    return ERR_OK;
}

void LWIPOptimized::enableHardwareChecksum() {
    // Использование аппаратного расчёта контрольных сумм
    #if CHECKSUM_GEN_IP
        ip_chksum = inet_chksum_hw;
    #endif
}
