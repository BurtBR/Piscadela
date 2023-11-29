#ifndef WORKERCODER_H
#define WORKERCODER_H

#include <QObject>

class WorkerCoder : public QObject{
    Q_OBJECT

private:
    bool beginframe = true;
    bool headertext = true;
    bool binarytext = true;
    bool commatext = true;

    QString CharToBinary(QChar &c);

public:
    explicit WorkerCoder(QObject *parent = nullptr);

signals:
    void Translated(QString text);

public slots:
    void Translate(QString text);
    void SetInsertBeginFrame(bool value);
    void SetInsertHeader(bool value);
    void SetInsert0b(bool value);
    void SetInsertComma(bool value);
};

#endif // WORKERCODER_H
