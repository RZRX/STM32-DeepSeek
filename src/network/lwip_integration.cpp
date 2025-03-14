#include "lwip_integration.h"
#include <lwip/init.h>
#include <lwip/dhcp.h>

LwIPIntegration::LwIPIntegration(QObject *parent)
    : QObject(parent), m_netif(nullptr) {
    
    lwip_init();
}

void LwIPIntegration::initEthernet(const QString &mac, const QString &ip) {
    m_netif = netif_add(&m_netif_struct,
        ip_address(ip), 
        ip_address("255.255.255.0"),
        ip_address("192.168.1.1"),
        ethernet_address(mac),
        ethernet_init,
        tcpip_input);
    
    netif_set_up(m_netif);
    dhcp_start(m_netif);
}

void LwIPIntegration::startHTTPServer() {
    struct tcp_pcb *pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, 80);
    tcp_listen(pcb);
    
    tcp_accept(pcb, [](void *arg, struct tcp_pcb *newpcb, err_t err) {
        if (err == ERR_OK) {
            // Обработка HTTP запросов
        }
        return ERR_OK;
    });
}

QString LwIPIntegration::getIP() const {
    return ipaddr_ntoa(&m_netif->ip_addr);
}
