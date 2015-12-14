//
// Created by 박진성 on 2015. 12. 7..
//

#ifndef SOLARS_SELECTOR_H
#define SOLARS_SELECTOR_H

#include "util.h"

typedef osgUtil::LineSegmentIntersector::Intersection Intersection;

using namespace osg;

class IntersectionSelector
{
    osgViewer::Viewer *viewer;
    std::set<std::string> targetNames;
    osg::Group *root;
    Context *context;

public:
    IntersectionSelector(std::set<std::string> targetNames, osg::Group *root, osgViewer::Viewer *viewer, Context *context)
    :targetNames(targetNames) {
        this->viewer = viewer;
        this->root = root;
        this->context = context;
    }

    virtual bool handle(const Intersection &intersection)
    {
        const osg::NodePath &nodes = intersection.nodePath;

        for (int i = nodes.size() - 1; i >= 0; i--)
        {
            std::cout << nodes[i]->getName() << std::endl;

            std::set<std::string>::iterator it = targetNames.find(nodes[i]->getName());
            if (it != targetNames.end()) {
				std::stringstream ss;
				ss << "Camera." << *it;
//				osg::Node *node = findNamedNode<osg::Node>(*it, root);
//				static_cast<osgGA::NodeTrackerManipulator *>(viewer->getCameraManipulator())->setTrackNodePath(node->getParentalNodePaths()[0]);

				context->setViewNode(ss.str());

                return true;
            }
        }

//        std::cout << std::endl;

        return false;
    }

    virtual void select(int i, Intersection &intersection)
    {
        std::cout << "Select: " << i << std::endl;
        const osg::Vec3d &p = intersection.getLocalIntersectPoint();
        std::cout << "Point: " << p[0] << " " << p[1] << " " << p[2] << std::endl;
    }
};

class KeyEventHandler
        : public osgGA::GUIEventHandler
{
protected:
    float _mX, _mY;
    osgViewer::Viewer *m_viewer;
    IntersectionSelector *m_selector;
    Context *context;

public:

    KeyEventHandler(Context *context, osgViewer::Viewer *node, IntersectionSelector *selector)
            : osgGA::GUIEventHandler(), context(context)
    {
        m_viewer = node;
        m_selector = selector;
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        switch (ea.getEventType())
        {
            case(osgGA::GUIEventAdapter::DOUBLECLICK):
            {
                if (ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
//                    std::cout << "Keydown!" << std::endl;

                    osg::ref_ptr<osgUtil::LineSegmentIntersector> ray = new osgUtil::LineSegmentIntersector(
                            osgUtil::Intersector::PROJECTION, ea.getXnormalized(), ea.getYnormalized());
                    osgUtil::IntersectionVisitor visitor(ray);

                    m_viewer->getCamera()->accept(visitor);

                    if (ray->containsIntersections()) {
                        std::multiset<Intersection> intersections = ray->getIntersections();

                        for (std::multiset<Intersection>::iterator i = intersections.begin();
                            i != intersections.end(); ++i) {

                            if (m_selector->handle(*i)) {
                                return true;
                            }
                        }
                    }
                }
                return false;
            }
            case (osgGA::GUIEventAdapter::KEYUP):
            {
                if (ea.getKey() == osgGA::GUIEventAdapter::KEY_BackSpace) {
					context->setViewNode("");
                    m_viewer->getCameraManipulator()->setByMatrix(context->getOriginCamera()->getMatrix());
                }
            }
            default:
                return false;
        }
    }
};


#endif //SOLARS_SELECTOR_H
