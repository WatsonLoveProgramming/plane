#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs()
{
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
	GLView::setGLContextAttrs(glContextAttrs);
}


bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::createWithRect("MyCppGame", Rect(0, 0, 320, 480));
		director->setOpenGLView(glview);
	}

	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	// Set the design resolution
	glview->setDesignResolutionSize(320, 480, ResolutionPolicy::NO_BORDER);


	auto scene = GameScene::createScene();
	director->runWithScene(scene);

	return true;
}


void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

}
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

}
