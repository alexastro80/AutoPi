#ifndef FRAME_HPP
#define FRAME_HPP
#include <QtWidgets>
#include <App/Worker.hpp>
class Frame : public QWidget
{
Q_OBJECT

   public:
    Frame(QWidget *_parent = nullptr) : QWidget(_parent) { parent = _parent;}

    inline bool IsFullscreen() { return fullscreen; }
    void SetWorker(Worker* _worker = nullptr) { worker = _worker; }
    inline void ToggleFullscreen() { fullscreen = !fullscreen; }
    void Update() { paintEvent(nullptr);}
   protected:
    void mouseDoubleClickEvent(QMouseEvent *)
        { ToggleFullscreen(); emit doubleClicked(fullscreen); }
    inline void enterEvent(QEvent *) { setFocus(); }
    void paintEvent(QPaintEvent * event){
     	QPainter painter(this);
     	if(worker != nullptr) worker->Paint(painter);
     }
   private:
    Worker* worker = nullptr;
    bool fullscreen = false;
    QWidget* parent = nullptr;

   signals:
    void doubleClicked(bool fullscreen);
    void toggle(bool enable);
};
#endif
