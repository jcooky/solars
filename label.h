//
// Created by 박진성 on 2015. 12. 16..
//

#ifndef SOLARS_LABEL_H
#define SOLARS_LABEL_H

#include <sstream>

#include <osgWidget/WindowManager>
#include "context.h"

class ColorLabel : public osgWidget::Label {
protected:
	std::string label;
	Context *context;

public:
    ColorLabel(const char *label, Context *context) :
            osgWidget::Label("", ""), label(label), context(context) {
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
		std::stringstream ss;
		ss << "Camera." << label;

		context->setViewNode(ss.str());

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

class SelectLabelMenu : public ColorLabel {
    osg::ref_ptr <osgWidget::Window> _window;

public:
    SelectLabelMenu(const char *label, Context *context, const std::set<std::string> &labels) :
            ColorLabel(label, context) {
        _window = new osgWidget::Box(
                std::string("Menu_") + label,
                osgWidget::Box::VERTICAL,
                true
        );

		for (std::set<std::string>::const_iterator i = labels.begin();
			i != labels.end();
			++i) {
			_window->addWidget(new ColorLabel(i->c_str(), context));
		}

//        _window->addWidget(new ColorLabel("Open Some Stuff"));
//        _window->addWidget(new ColorLabel("Do It Now"));
//        _window->addWidget(new ColorLabel("Hello, How Are U?"));
//        _window->addWidget(new ColorLabel("Hmmm..."));
//        _window->addWidget(new ColorLabel("Option 5"));

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

class OverviewMenu : public ColorLabel {

	osgViewer::Viewer* viewer;
public:
	OverviewMenu(const char* label, Context* context, osgViewer::Viewer *viewer)
		: ColorLabel(label, context), viewer(viewer) {
			setColor(0.8f, 0.8f, 0.8f, 0.8f);
	}

	void managed(osgWidget::WindowManager *wm) {
        osgWidget::Label::managed(wm);
    }

	void positioned() {
        osgWidget::Label::positioned();

    }


	bool mousePush(double, double, const osgWidget::WindowManager*) override {
		context->setViewNode("");
		viewer->getCameraManipulator()->setByMatrix(context->getOriginCamera()->getMatrix());
		return true;
	}

	bool mouseLeave(double, double, const osgWidget::WindowManager*) override {
		setColor(0.8f, 0.8f, 0.8f, 0.8f);
		return true;
	}

};

class ExitLabelMenu : public ColorLabel {

public:
	ExitLabelMenu(const char* label, Context *context)
		: ColorLabel(label, context) {
			setColor(0.8f, 0.8f, 0.8f, 0.8f);
	}

	void positioned() {
        osgWidget::Label::positioned();

    }


    void managed(osgWidget::WindowManager *wm) {
        osgWidget::Label::managed(wm);

    }


	bool mousePush(double, double, const osgWidget::WindowManager*) override {
		exit(0);
		return true;
	}

	bool mouseLeave(double, double, const osgWidget::WindowManager*) override {
		setColor(0.8f, 0.8f, 0.8f, 0.8f);
		return true;
	}
};

#endif //SOLARS_LABEL_H
