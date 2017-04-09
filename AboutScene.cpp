#include"AboutScene.h"
#include"HelloWorldScene.h"


bool AboutScene::init(){
	if (!Layer::init())
		return false;
	auto spbk = Sprite::create("about.png");
	spbk->setPosition(Vec2(0,0));
	spbk->setAnchorPoint(Point::ZERO);
	this->addChild(spbk);
	EventListenerTouchOneByOne* touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = [](Touch*touch, Event*event){
		Director::getInstance()->replaceScene(HelloWorld::createScene());
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch,this);
	return true;
}

Scene* AboutScene::createScene(){
	auto scene = Scene::create();
	auto layer = AboutScene::create();
	scene->addChild(layer);
	return scene;


}