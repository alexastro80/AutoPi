#ifndef FRAME_HPP
#define FRAME_HPP
#include <QtWidgets>
class Frame : public QWidget
{
Q_OBJECT

   public:
    Frame(QWidget *parent) : QWidget(parent) {}

    inline bool IsFullscreen() { return fullscreen; }
    inline void ToggleFullscreen() { fullscreen = !fullscreen; }

   protected:
    void mouseDoubleClickEvent(QMouseEvent *)
        { ToggleFullscreen(); emit doubleClicked(fullscreen); }
    inline void enterEvent(QEvent *) { setFocus(); }

   private:
    bool fullscreen = false;

   signals:
    void doubleClicked(bool fullscreen);
    void toggle(bool enable);
};
#endif
