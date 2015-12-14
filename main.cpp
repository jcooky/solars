#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cassert>

#include <osgGA/TrackballManipulator>
#include <osgGA/StandardManipulator>
#include <osgGA/CameraViewSwitchManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/CameraManipulator>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osgDB/readFile>
#include <osg/Camera>
#include <osg/Texture2D>

#include "context.h"
#include "util.h"
#include "selector.h"

#define SOURCE "C:\\Users\\user1\\Github\\solars"

#define MODEL SOURCE"\\model.osgt"

#include "skybox.h"

using namespace osg;

class CustomCameraManipulator : public osgGA::CameraManipulator {
	Matrix matrix;
public:
	CustomCameraManipulator() 
	:matrix(Matrix::identity()) {
	}

	void setByMatrix(const osg::Matrixd& matrix) override {
		this->matrix = matrix;
	}

	void setByInverseMatrix(const osg::Matrixd& matrix) override {
		Matrix r;
		matrix.inverse(r);

		this->matrix = r;
	}

	osg::Matrixd getMatrix() const override {
		return matrix;
	}

	osg::Matrixd getInverseMatrix() const override {
		Matrix r;
		matrix.inverse(r);
		return r;
	}
};

class SolarSystemApplication : public osgGA::GUIEventHandler {
    Context context;
    osg::MatrixTransform *sun, *mercury, *venus, *earth, *mars, *jupiter, *saturn, *uranus, *naptune;
    osgViewer::Viewer viewer;
    double timeLast;
	osg::Group *root;
	osg::MatrixTransform *rootTransform;
	SkyBox *skybox;

public:
    SolarSystemApplication() {
        timeLast = 0;
		skybox = new SkyBox();
    }

    osg::Group* startUpScene() {
        using osg::Group;
        using osg::PositionAttitudeTransform;
        using osg::Vec3d;
        using osg::Matrix;

		

        osg::Node *models = osgDB::readNodeFile(MODEL);
		osg::Image *posX = osgDB::readImageFile(SOURCE"\\galaxy/galaxy+X.tga");
		osg::Image *negX = osgDB::readImageFile(SOURCE"\\galaxy/galaxy-X.tga");
		osg::Image *posY = osgDB::readImageFile(SOURCE"\\galaxy/galaxy+Y.tga");
		osg::Image *negY = osgDB::readImageFile(SOURCE"\\galaxy/galaxy-Y.tga");
		osg::Image *posZ = osgDB::readImageFile(SOURCE"\\galaxy/galaxy+Z.tga");
		osg::Image *negZ = osgDB::readImageFile(SOURCE"\\galaxy/galaxy-Z.tga");

		skybox->setEnvironmentMap(0, posX, negX, posY, negY, posZ, negZ);

        root = new Group();
        osg::MatrixTransform *transform = new osg::MatrixTransform;
        transform->setMatrix(Matrix::translate(Vec3d(0, 0, 0)));
        transform->addChild(models);
        root->addChild(transform);

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable( new osg::ShapeDrawable(
			new osg::Sphere(osg::Vec3(), models->getBound().radius())) );
		geode->setCullingActive( false );

		skybox->addChild(geode.get());
		root->addChild(skybox);

		root->setName("ROOT");

        mercury = findNamedNode<osg::MatrixTransform>("Mercury", root);
        venus = findNamedNode<osg::MatrixTransform>("Venus", root);
        earth = findNamedNode<osg::MatrixTransform>("Earth", root);
        mars = findNamedNode<osg::MatrixTransform>("Mars", root);
        jupiter = findNamedNode<osg::MatrixTransform>("Jupiter", root);
        saturn = findNamedNode<osg::MatrixTransform>("Saturn", root);

        sun = findNamedNode<osg::MatrixTransform>("Sphere", root);

		rootTransform = transform;

        return root;
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
        switch (ea.getEventType()) {
            case osgGA::GUIEventAdapter::FRAME: {
                double t = ea.getTime();
                if (timeLast != 0) {

                    double deltaTime = t - timeLast;
#define ORBIT_SCALE 10
#define ROT_SCALE 0.01
                    double DT = deltaTime * 0.001;

//					osg::Matrix::rotate()

                    osg::Matrix orbit, rot, T, T_i;

                    orbit = osg::Matrix::rotate(47.36 * DT * ORBIT_SCALE, 0, 0, 1);
                    rot = osg::Matrix::rotate(10.892 * DT * ROT_SCALE, 0, 0, 1); //	~0.01°, 10.892 km/h (적도기준)
					T = Matrix::translate(saturn->getMatrix().getTrans());
					T_i = Matrix::inverse(T);

                    mercury->setMatrix(mercury->getMatrix() * T_i * rot * T * orbit);

                    orbit = osg::Matrix::rotate(35.02 * DT * ORBIT_SCALE, 0, 0, 1);
                    rot = osg::Matrix::rotate(6.52 * DT * ROT_SCALE, 0, 0, 1); //2.64°, 6.52 km/h (적도기준)
					T = Matrix::translate(saturn->getMatrix().getTrans());
					T_i = Matrix::inverse(T);

                    venus->setMatrix(venus->getMatrix() * T_i * rot * T * orbit);

                    orbit = osg::Matrix::rotate(29.783 * DT * ORBIT_SCALE, 0, 0, 1);
                    rot = osg::Matrix::rotate(1674.4 * DT * ROT_SCALE, 0, 0, 1); //23.439 281°, 465.11 m/sec = 1674.4 km/h (적도)
					T = Matrix::translate(saturn->getMatrix().getTrans());
					T_i = Matrix::inverse(T);

                    earth->setMatrix(earth->getMatrix() * T_i * rot * T * orbit);

                    orbit = osg::Matrix::rotate(24.077 * DT * ORBIT_SCALE, 0, 0, 1);
                    rot = osg::Matrix::rotate(868.22 * DT * ROT_SCALE, 0, 0, 1); //25.19°, 868.22 km/h (적도)
					T = Matrix::translate(saturn->getMatrix().getTrans());
					T_i = Matrix::inverse(T);

                    mars->setMatrix(mars->getMatrix() * T_i * rot * T * orbit);

                    orbit = osg::Matrix::rotate(13.05624 * DT * ORBIT_SCALE, 0, 0, 1);
                    rot = osg::Matrix::rotate(45360 * DT * ROT_SCALE, 0, 0, 1); //궤도면 기준 3.13°, 	12.6 km/s = 45360 km/h
					T = Matrix::translate(saturn->getMatrix().getTrans());
					T_i = Matrix::inverse(T);

                    jupiter->setMatrix(jupiter->getMatrix() * T_i * rot * T * orbit);

                    orbit = osg::Matrix::rotate(9.639 * DT * ORBIT_SCALE, 0, 0, 1);
                    rot = osg::Matrix::rotate(35500 * DT * ROT_SCALE, 0, 0, 1); //26.73°, 9.87 km/s = 35500 km/h (적도 기준)
					T = Matrix::translate(saturn->getMatrix().getTrans());
					T_i = Matrix::inverse(T);

                    saturn->setMatrix(saturn->getMatrix() * T_i * rot * T * orbit);

//    rot = osg::Matrix::rotate( 5.43 * deltaTime * 0.001, 0,0,1 );

//    sun->setMatrix(rot * sun->getMatrix());

					if (context.getViewNode() != "") {
						osg::MatrixTransform *cameraTransform = findNamedNode<osg::MatrixTransform>(context.getViewNode(), root);
//						Matrix matrix;
//						cameraTransform->computeLocalToWorldMatrix(matrix, NULL);
						
						viewer.getCameraManipulator()->setByMatrix(cameraTransform->getWorldMatrices()[0]);
					}
                }

                timeLast = t;
            }
            default:
                return false;
        }
    }

    int main() {
        using osgViewer::Viewer;
		using namespace osgGA;

        std::set<std::string> targetNames;
        targetNames.insert("Earth");
        targetNames.insert("Mars");
        targetNames.insert("Jupiter");
        targetNames.insert("Venus");
        targetNames.insert("Saturn");
        targetNames.insert("Mercury");
        targetNames.insert("Uranus");
        targetNames.insert("Naptune");

        osg::Group *root = startUpScene();

        osg::MatrixTransform *camera = findNamedNode<osg::MatrixTransform>("Camera", root);

		TrackballManipulator *manipulator = new TrackballManipulator();

        viewer.setUpViewInWindow(10, 10, 800, 600, 0);
        viewer.setSceneData(root);
        viewer.setCameraManipulator(manipulator);
        viewer.realize();

		Matrix matrix = camera->getWorldMatrices()[0];
		viewer.getCameraManipulator()->setByMatrix(matrix);

        IntersectionSelector selector(targetNames, root, &viewer, &context);
        KeyEventHandler *eventHandler = new KeyEventHandler(&context, &viewer, &selector);

        viewer.addEventHandler(eventHandler);
        viewer.addEventHandler(this);

        context.setOriginCamera(camera);

        while (!viewer.done()) {
            viewer.frame();
        }

        return 0;
    }
};

int main() {
    SolarSystemApplication *app = new SolarSystemApplication;
    return app->main();
}