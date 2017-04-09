#include"GameScene.h"
#include"HelloWorldScene.h"
#include"SimpleAudioEngine.h"  //for music
using namespace CocosDenshion;    //for music

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
	spbk02->setPositionY(spbk->getPositionY() + 680);// add 同一张background实现滚动
	this->addChild(spbk02);
	spbk02->setTag(11);

	score = 0;
	auto labScore = Label::create("score: 0", "Calibri", 20);   //得分板
	labScore->setPosition(50, 440);
	labScore->setColor(Color3B::RED);
	this->addChild(labScore);
	labScore->setTag(120);

	auto spPlane = Sprite::create("plane.png");   //add  plane
	spPlane->setTag(110);
	spPlane->setPosition(Point(160, 240));
	this->addChild(spPlane);
	Vector<SpriteFrame*> allframe;                //plane animation
	for (int i = 0; i < 4; i++){
		SpriteFrame * sf = SpriteFrame::create("plane.png", Rect(i * 47, 0, 47, 56));
		allframe.pushBack(sf);
	}
	Animation * ani = Animation::createWithSpriteFrames(allframe,0.1);  //每帧间隔时间0.1
	spPlane->runAction(RepeatForever::create(Animate::create(ani)));

	//控制飞机移动
	EventListenerTouchOneByOne* event = EventListenerTouchOneByOne::create();
	event->setSwallowTouches(true);  //运行向后传递触摸事件
	event->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	event->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	event->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(event, this);

	this->schedule(schedule_selector(GameScene::moveBackground), 0.1);  //每0.01s执行（背景滚动）一次
	this->schedule(schedule_selector(GameScene::newBullet), 0.5);       //子弹发射每0.5s一颗
	this->schedule(schedule_selector(GameScene::moveBullet), 0.01);      // 子弹每0.01s移动一次
	this->schedule(schedule_selector(GameScene::newEnemy), 0.5);       //敌机每0.5s一个
	this->schedule(schedule_selector(GameScene::moveEnemy), 0.01);      // 敌机每0.01s移动一次
	
	this->scheduleUpdate();   //碰撞检测游戏逻辑
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("play.mp3",true);   //播放bgm
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
	 if (spbk->getPositionY() < -680){
		 spbk->setPositionY(0);
	 }
	 spbk02->setPositionY(spbk->getPositionY()+680);
 
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
		 if (nowbullet->getPositionY() > Director::getInstance()->getWinSize().height){
			 nowbullet->removeFromParent();
			 allBullet.eraseObject(nowbullet);     //eraseobject才是删除vector的对象
			 i--;         //eg。。第1颗删了第7颗变回第6颗
		 }
	 }
 }

 void GameScene::newEnemy(float t){
	 Sprite* enemy = nullptr;
	 int num = rand() % 10;  
	 if (num >= 3){    //70%概率aaa.png
		 enemy = Sprite::create("aaa.png");
		 enemy->setTag(1000);
	 }else{
		 enemy = Sprite::create("ccc.png");
		 enemy->setTag(2000);
	 }
	 enemy->setPosition(Point(rand()%300+10,500));  //横坐标为10到310随机数（空出两边）
	 this->addChild(enemy);
	 this->allEnemy.pushBack(enemy);
 }
 void GameScene::moveEnemy(float t){   
	 for (int i = 0; i < allEnemy.size(); i++){
		 auto nowenemy = allEnemy.at(i);
		 nowenemy->setPositionY(nowenemy->getPositionY() - 3);
		 if (nowenemy->getPositionY() < 0){
			 nowenemy->removeFromParent();
			 allEnemy.eraseObject(nowenemy);  //eraseobject才是删除vector的对象
			 i--; 
		 }
	 }
 }

 void GameScene::update(float t){
	 auto spPlane = this->getChildByTag(110);          
	 Rect rp(spPlane->getPositionX(), spPlane->getPositionY(), 47, 56);     //获取玩家飞机Rect范围

	 for (int i = 0; i < allEnemy.size(); i++){    
		 auto nowenemy = allEnemy.at(i);               
		 Rect er(nowenemy->getPositionX(), nowenemy->getPositionY(), 40, 50);    //获取敌机Rect范围
         //碰撞检测
		 if (rp.intersectsRect(er)){     //若玩家飞机和飞机碰撞检测
			 newBomb(nowenemy->getPositionX(), nowenemy->getPositionY());   //爆炸
			 nowenemy->removeFromParent();  //删除敌机
			 allEnemy.eraseObject(nowenemy);
			 i--;
			 //SimpleAudioEngine::getInstance()->playEffect("expo.mp3");  爆炸音效  pauseeffect使爆炸和BGM同时播放
			 this->pauseSchedulerAndActions();             //停止所有动作
			 auto spover = Sprite::create("end.png");     //跳转到gameover画面
			 spover->setPosition(Point::ZERO);
			 spover->setAnchorPoint(Point::ZERO);
			 this->addChild(spover);
			 //2s后执行返回menu画面函数
			 auto act = Sequence::create( DelayTime::create(2), CallFunc::create(this, callfunc_selector(GameScene::jumpToMenu)), NULL); 
			 this->runAction(act);
		 }

		 for (int j = 0; j < allBullet.size(); j++){
			 auto nowbullet = allBullet.at(j);           
			 Rect br(nowbullet->getPositionX(), nowbullet->getPositionY(), 20, 20);        //获取子弹Rect范围

			 if (er.intersectsRect(br)){      //若玩家子弹和飞机碰撞检测
				 auto labScore = (Label*)this->getChildByTag(120);
				 score += nowenemy->getTag();  //判断哪种敌机
				 newBomb(nowbullet->getPositionX(), nowbullet->getPositionY());   //爆炸
				 labScore->setString(String::createWithFormat("score:%d", score)->_string);  //加分     
				 nowbullet->removeFromParent();  //删除子弹
				 allBullet.eraseObject(nowbullet);
				 nowenemy->removeFromParent();  //删除敌机
				 allEnemy.eraseObject(nowenemy);
				 i--;
				 //SimpleAudioEngine::getInstance()->playEffect("expo.mp3");  爆炸音效  pauseeffect使爆炸和BGM同时播放
				 break;	    //两者删除后重新下一轮检测
			 }     
		 }
	 } 
 }

 void GameScene::newBomb(int x, int y){      //爆炸效果
	 Vector<SpriteFrame*> allframe;
	 for (int i = 0; i < 7; i++){
		 SpriteFrame* sf = SpriteFrame::create("boom.png", Rect(i * 44, 0, 44, 49));
		 allframe.pushBack(sf);
	 }
	 Animation* ani = Animation::createWithSpriteFrames(allframe, 0.03);
	 auto sprite = Sprite::create();
	 Action* act = Sequence::create(
		 Animate::create(ani),   //创建动画
		 CCCallFuncN::create(sprite,callfuncN_selector(GameScene::killme)),      //调用删除自己函数   
		 NULL);
	 sprite->setPosition(Point(x, y));
	 sprite->runAction(act);
	 this->addChild(sprite);
 }

 void GameScene::killme(Node* pSender){       //删除自己
	 pSender->removeFromParentAndCleanup(true);
 }

 void GameScene::jumpToMenu(){
	 SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	 Director::getInstance()->replaceScene(HelloWorld::createScene());
 }
