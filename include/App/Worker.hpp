#ifndef WORKER_HPP
#define WORKER_HPP
#include <QtWidgets>
class Worker : public QObject {
 Q_OBJECT

public:
    Worker( bool _nightMode = false, QWidget *_frame = nullptr) { nightMode = _nightMode; frame = _frame; }
    ~Worker() {if (frame != nullptr) delete frame;}

    inline virtual void SetOpacity(unsigned int alpha) {  }
    inline virtual void UpdateSize() {  }
    inline virtual void SetNightMode(bool mode) {  }
    inline virtual void SendKeyEvent(QKeyEvent *event) {  }
    inline virtual QWidget* Display() { return nullptr; }
    void Show() { };
    void Unshow() { };
protected:
    QWidget* frame;
private:
    bool nightMode;
   
};

#endif