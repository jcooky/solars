//
// Created by 박진성 on 2015. 12. 16..
//

#ifndef SOLARS_LABEL_H
#define SOLARS_LABEL_H

#include <osgWidget/WindowManager>

struct ColorLabel : public osgWidget::Label {
    ColorLabel(const char *label) :
            osgWidget::Label("", "") {
        setFont("fonts/Vera.ttf");
        setFontSize(14);
        setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
        setColor(0.3f, 0.3f, 0.3f, 1.0f);
        addHeight(18.0f);
        setCanFill(true);
        setLabel(label);
        setEventMask(osgWidget::EVENT_MOUSE_PUSH | osgWidget::EVENT_MASK_MOUSE_MOVE);
    }

    virtual bool mousePush(double, double, const osgWidget::WindowManager *) {
        return true;
    }

    virtual bool mouseEnter(double, double, const osgWidget::WindowManager *) {
        setColor(0.6f, 0.6f, 0.6f, 1.0f);

        return true;
    }

    virtual bool mouseLeave(double, double, const osgWidget::WindowManager *) {
        setColor(0.3f, 0.3f, 0.3f, 1.0f);

        return true;
    }
};

class ColorLabelMenu : public ColorLabel {
    osg::ref_ptr <osgWidget::Window> _window;

public:
    ColorLabelMenu(const char *label) :
            ColorLabel(label) {
        _window = new osgWidget::Box(
                std::string("Menu_") + label,
                osgWidget::Box::VERTICAL,
                true
        );

        _window->addWidget(new ColorLabel("Open Some Stuff"));
        _window->addWidget(new ColorLabel("Do It Now"));
        _window->addWidget(new ColorLabel("Hello, How Are U?"));
        _window->addWidget(new ColorLabel("Hmmm..."));
        _window->addWidget(new ColorLabel("Option 5"));

        _window->resize();

        setColor(0.8f, 0.8f, 0.8f, 0.8f);
    }

    void managed(osgWidget::WindowManager *wm) {
        osgWidget::Label::managed(wm);

        wm->addChild(_window.get());

        _window->hide();
    }

    void positioned() {
        osgWidget::Label::positioned();

        _window->setOrigin(getX(), getHeight());
        _window->resize(getWidth());
    }

    bool mousePush(double, double, const osgWidget::WindowManager *) {
        if (!_window->isVisible()) _window->show();

        else _window->hide();

        return true;
    }

    bool mouseLeave(double, double, const osgWidget::WindowManager *) {
        if (!_window->isVisible()) setColor(0.8f, 0.8f, 0.8f, 0.8f);

        return true;
    }
};


#endif //SOLARS_LABEL_H
