#include "anomaly_detector.h"
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>

AnomalyDetector::AnomalyDetector(const QString &modelPath, QObject *parent)
    : QObject(parent), m_model(tflite::FlatBufferModel::BuildFromFile(modelPath.toUtf8())) {
    
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder(*m_model, resolver)(&m_interpreter);
    m_interpreter->AllocateTensors();
}

float AnomalyDetector::predictAnomaly(const QVector<float> &sensorData) {
    float* input = m_interpreter->typed_input_tensor<float>(0);
    for (int i = 0; i < sensorData.size(); ++i) {
        input[i] = sensorData[i];
    }
    
    m_interpreter->Invoke();
    
    float* output = m_interpreter->typed_output_tensor<float>(0);
    return output[0]; // Возвращает вероятность аномалии 0..1
}

void AnomalyDetector::retrainModel(const QVector<QVector<float>> &newData) {
    // Онлайн-дообучение модели (упрощенная реализация)
    const int epochs = 10;
    for (int i = 0; i < epochs; ++i) {
        for (const auto &sample : newData) {
            predictAnomaly(sample); // Псевдо-обучение через inference
        }
    }
}
