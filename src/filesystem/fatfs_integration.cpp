#include "fatfs_integration.h"
#include <QStorageInfo>

FatFSIntegration::FatFSIntegration(QObject *parent)
    : QObject(parent), m_diskMounted(false) {
    
    connect(&m_diskMonitor, &QTimer::timeout, this, &FatFSIntegration::checkDisk);
    m_diskMonitor.start(5000);
}

bool FatFSIntegration::mountSDCard() {
    FRESULT res = f_mount(&m_fs, "0:", 1);
    if (res == FR_OK) {
        m_diskMounted = true;
        emit diskStatusChanged("SD Card mounted");
        return true;
    }
    return false;
}

void FatFSIntegration::formatSDCard() {
    MKFS_PARM opt = {FM_FAT32, 0, 0, 0, 0};
    FRESULT res = f_mkfs("0:", &opt, m_workBuffer, sizeof(m_workBuffer));
    emit operationCompleted(res == FR_OK ? "Format OK" : "Format failed");
}

void FatFSIntegration::checkDisk() {
    DWORD freeClusters;
    FATFS *fs;
    FRESULT res = f_getfree("0:", &freeClusters, &fs);
    if (res == FR_OK) {
        quint64 freeSpace = freeClusters * fs->csize * 512;
        emit diskSpaceUpdated(freeSpace);
    }
}
