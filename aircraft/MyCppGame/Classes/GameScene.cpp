#include"GameScene.h"

bool GameScene::init(){
	if (!Layer::init())
		return false;
	auto spbk = Sprite::create("background.png");// add background原来背景
	spbk->setPosition(Point::ZERO);
	spbk->setAnchorPoint(Point::ZERO);
	this->addChild(spbk);
	spbk->setTag(10);

	auto spbk02 = Sprite::create("background.png");
	spbk02->setPosition(Point::ZERO);
	spbk02->setAnchorPoint(Point::ZERO);
	spbk02->setPositionY(spbk->getPositionY() + 480);// add 同一张background实现滚动
	this->addChild(spbk02);
	spbk02->setTag(11);

	score = 0;
	auto labScore = Label::create("score: 0", "Calibri", 20);   //SCORE BOARD
	labScore->setPosition(80, 440);
	labScore->setColor(Color3B::RED);
	this->addChild(labScore);
	labScore->setTag(120);

	auto spPlane = Sprite::create("plane.png");   //add  plane
	spPlane->setTag(110);
	spPlane->setPosition(Point(160, 240));
	this->addChild(spPlane);

	EventListenerTouchOneByOne* event = EventListenerTouchOneByOne::create();
	event->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	event->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	event->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(event, this);

	this->schedule(schedule_selector(GameScene::moveBackground), 0.1);  
	this->schedule(schedule_selector(GameScene::newBullet), 0.5);
	this->schedule(schedule_selector(GameScene::moveBullet), 0.01); 
	this->schedule(schedule_selector(GameScene::newEnemy), 0.5);  
	this->schedule(schedule_selector(GameScene::moveEnemy), 0.01); 

	this->scheduleUpdate(); 
	return true;
}

Scene* GameScene::createScene(){
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}
void GameScene::moveBackground(float t){
	auto spbk = this->getChildByTag(10);
	auto spbk02 = this->getChildByTag(11);
	spbk->setPositionY(spbk->getPositionY() - 10);
	if (spbk->getPositionY() < -480){
		spbk->setPositionY(0);
	}
	spbk02->setPositionY(spbk->getPositionY() + 480);

}
bool GameScene::onTouchBegan(Touch *touch, Event *unused_event){
	px = touch->getLocation().x;
	py = touch->getLocation().y;
	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event *unused_event){
	int mx = (touch->getLocation().x - px);
	int my = (touch->getLocation().y - py);
	auto spPlane = this->getChildByTag(110);
	spPlane->runAction(MoveBy::create(0, Point(mx, my)));//0 = 0ms
	px = touch->getLocation().x;               //再次初始化防止越移越多
	py = touch->getLocation().y;
}
void GameScene::onTouchEnded(Touch *touch, Event *unused_event){}

void GameScene::newBullet(float t){
	auto spPlane = this->getChildByTag(110);
	Sprite* bullet = Sprite::create("bullet.png");
	bullet->setPosition(spPlane->getPosition());
	this->addChild(bullet);
	this->allBullet.pushBack(bullet);
}
void GameScene::moveBullet(float t){
	for (int i = 0; i < allBullet.size(); i++){
		auto nowbullet = allBullet.at(i);
		nowbullet->setPositionY(nowbullet->getPositionY() + 3);
		if (nowbullet->getPositionY() > 480){
			nowbullet->removeFromParent();       //remove bullet from game's scene
			allBullet.eraseObject(nowbullet);     //erase the bullet from vector
			i--;         //the number of bullet in the group should minus 1
		}
	}
}
void GameScene::newEnemy(float t){
	Sprite* enemy = nullptr;
	int num = rand() % 10;
	if (num >= 3){    // 70% probability of enemy1
		enemy = Sprite::create("enemy1.png");
		enemy->setTag(1000);
	}
	else{       // 30% probability of enemy2
		enemy = Sprite::create("enemy2.png");
		enemy->setTag(2000);
	}
	enemy->setPosition(Point(rand() % 320, 500));  //random position
	this->addChild(enemy);
	this->allEnemy.pushBack(enemy);
}
void GameScene::moveEnemy(float t){
	for (int i = 0; i < allEnemy.size(); i++){
		auto nowenemy = allEnemy.at(i);
		nowenemy->setPositionY(nowenemy->getPositionY() - 3);
		if (nowenemy->getPositionY() < 0){
			nowenemy->removeFromParent();    //remove enemy from game's scene
			allEnemy.eraseObject(nowenemy);  //erase the enemy from vector
			i--;                       //the number of bullet in the group should minus 1
		}
	}
}

void GameScene::update(float t){
	auto spPlane = this->getChildByTag(110);
	Rect rp(spPlane->getPositionX(), spPlane->getPositionY(), 47, 56);     //get our plane's range

	for (int i = 0; i < allEnemy.size(); i++){
		auto nowenemy = allEnemy.at(i);
		Rect er(nowenemy->getPositionX(), nowenemy->getPositionY(), 40, 50);    //get enemy plane's range
		//check for collision
		if (rp.intersectsRect(er)){     //player's plane collide with enemy plane
			nowenemy->removeFromParent();  
			allEnemy.eraseObject(nowenemy);
			i--;
			this->pauseSchedulerAndActions();     //stop all action(background, plane, enemy)
			auto spover = Sprite::create("gameover.png");     //jump to gameover photo
			spover->setPosition(Point::ZERO);
			spover->setAnchorPoint(Point::ZERO);
			this->addChild(spover);
		}

		for (int j = 0; j < allBullet.size(); j++){
			auto nowbullet = allBullet.at(j);
			Rect br(nowbullet->getPositionX(), nowbullet->getPositionY(), 20, 20);        //get bullet's range

			if (er.intersectsRect(br)){      //player's bullet collide with enemy plane
				auto labScore = (Label*)this->getChildByTag(120);
				score += nowenemy->getTag();  //judge which kind of enemy
				labScore->setString(String::createWithFormat("score:%d", score)->_string);  //add points    
				nowbullet->removeFromParent();  //delete bullet
				allBullet.eraseObject(nowbullet);
				nowenemy->removeFromParent();  //delete enemy
				allEnemy.eraseObject(nowenemy);
				i--;
				break;	    //break the loop to begin next collision check
			}
		}
	}
}