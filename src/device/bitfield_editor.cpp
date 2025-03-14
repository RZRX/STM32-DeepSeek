#include "bitfield_editor.h"
#include <QGridLayout>

BitFieldEditor::BitFieldEditor(quint32 *regPtr, const RegisterInfo &info, QWidget *parent)
    : QWidget(parent), m_regPtr(regPtr), m_info(info) {
    
    QGridLayout *layout = new QGridLayout(this);
    for (int i = 0; i < 32; ++i) {
        QCheckBox *bit = new QCheckBox(QString::number(i), this);
        connect(bit, &QCheckBox::toggled, [this, i](bool checked) {
            *m_regPtr = checked ? (*m_regPtr | (1 << i)) : (*m_regPtr & ~(1 << i));
        });
        layout->addWidget(bit, i/8, i%8);
    }
}

void BitFieldEditor::refresh() {
    quint32 val = *m_regPtr;
    for (int i = 0; i < 32; ++i) {
        m_bits[i]->setChecked(val & (1 << i));
    }
}
