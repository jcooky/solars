//
// Created by 박진성 on 2015. 12. 7..
//

#ifndef SOLARS_CONTEXT_H
#define SOLARS_CONTEXT_H


#include <osg/MatrixTransform>

class Context {
    osg::MatrixTransform *originCamera;
    std::string viewNode;

public:
    Context() {
		viewNode = "";
    }

    osg::MatrixTransform *getOriginCamera() const {
        return originCamera;
    }

    void setOriginCamera(osg::MatrixTransform *originCamera) {
        Context::originCamera = originCamera;
    }

    const std::string &getViewNode() const { return viewNode; }
	void setViewNode(const std::string &viewNode) { this->viewNode = viewNode;}
};


#endif //SOLARS_CONTEXT_H
