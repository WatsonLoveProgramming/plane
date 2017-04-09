#include"GameScene.h"
#include"HelloWorldScene.h"
#include"SimpleAudioEngine.h"  //for music
using namespace CocosDenshion;    //for music

bool GameScene::init(){
	if (!Layer::init())
		return false;
	auto spbk = Sprite::create("background.png");// add backgroundԭ������
	spbk->setPosition(Point::ZERO);
	spbk->setAnchorPoint(Point::ZERO);
	this->addChild(spbk);
	spbk->setTag(10);

	auto spbk02 = Sprite::create("background.png");
	spbk02->setPosition(Point::ZERO);
	spbk02->setAnchorPoint(Point::ZERO);
	spbk02->setPositionY(spbk->getPositionY() + 680);// add ͬһ��backgroundʵ�ֹ���
	this->addChild(spbk02);
	spbk02->setTag(11);

	score = 0;
	auto labScore = Label::create("score: 0", "Calibri", 20);   //�÷ְ�
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
	Animation * ani = Animation::createWithSpriteFrames(allframe,0.1);  //ÿ֡���ʱ��0.1
	spPlane->runAction(RepeatForever::create(Animate::create(ani)));

	//���Ʒɻ��ƶ�
	EventListenerTouchOneByOne* event = EventListenerTouchOneByOne::create();
	event->setSwallowTouches(true);  //������󴫵ݴ����¼�
	event->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	event->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	event->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(event, this);

	this->schedule(schedule_selector(GameScene::moveBackground), 0.1);  //ÿ0.01sִ�У�����������һ��
	this->schedule(schedule_selector(GameScene::newBullet), 0.5);       //�ӵ�����ÿ0.5sһ��
	this->schedule(schedule_selector(GameScene::moveBullet), 0.01);      // �ӵ�ÿ0.01s�ƶ�һ��
	this->schedule(schedule_selector(GameScene::newEnemy), 0.5);       //�л�ÿ0.5sһ��
	this->schedule(schedule_selector(GameScene::moveEnemy), 0.01);      // �л�ÿ0.01s�ƶ�һ��
	
	this->scheduleUpdate();   //��ײ�����Ϸ�߼�
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("play.mp3",true);   //����bgm
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
	 px = touch->getLocation().x;               //�ٴγ�ʼ����ֹԽ��Խ��
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
			 allBullet.eraseObject(nowbullet);     //eraseobject����ɾ��vector�Ķ���
			 i--;         //eg������1��ɾ�˵�7�ű�ص�6��
		 }
	 }
 }

 void GameScene::newEnemy(float t){
	 Sprite* enemy = nullptr;
	 int num = rand() % 10;  
	 if (num >= 3){    //70%����aaa.png
		 enemy = Sprite::create("aaa.png");
		 enemy->setTag(1000);
	 }else{
		 enemy = Sprite::create("ccc.png");
		 enemy->setTag(2000);
	 }
	 enemy->setPosition(Point(rand()%300+10,500));  //������Ϊ10��310��������ճ����ߣ�
	 this->addChild(enemy);
	 this->allEnemy.pushBack(enemy);
 }
 void GameScene::moveEnemy(float t){   
	 for (int i = 0; i < allEnemy.size(); i++){
		 auto nowenemy = allEnemy.at(i);
		 nowenemy->setPositionY(nowenemy->getPositionY() - 3);
		 if (nowenemy->getPositionY() < 0){
			 nowenemy->removeFromParent();
			 allEnemy.eraseObject(nowenemy);  //eraseobject����ɾ��vector�Ķ���
			 i--; 
		 }
	 }
 }

 void GameScene::update(float t){
	 auto spPlane = this->getChildByTag(110);          
	 Rect rp(spPlane->getPositionX(), spPlane->getPositionY(), 47, 56);     //��ȡ��ҷɻ�Rect��Χ

	 for (int i = 0; i < allEnemy.size(); i++){    
		 auto nowenemy = allEnemy.at(i);               
		 Rect er(nowenemy->getPositionX(), nowenemy->getPositionY(), 40, 50);    //��ȡ�л�Rect��Χ
         //��ײ���
		 if (rp.intersectsRect(er)){     //����ҷɻ��ͷɻ���ײ���
			 newBomb(nowenemy->getPositionX(), nowenemy->getPositionY());   //��ը
			 nowenemy->removeFromParent();  //ɾ���л�
			 allEnemy.eraseObject(nowenemy);
			 i--;
			 //SimpleAudioEngine::getInstance()->playEffect("expo.mp3");  ��ը��Ч  pauseeffectʹ��ը��BGMͬʱ����
			 this->pauseSchedulerAndActions();             //ֹͣ���ж���
			 auto spover = Sprite::create("end.png");     //��ת��gameover����
			 spover->setPosition(Point::ZERO);
			 spover->setAnchorPoint(Point::ZERO);
			 this->addChild(spover);
			 //2s��ִ�з���menu���溯��
			 auto act = Sequence::create( DelayTime::create(2), CallFunc::create(this, callfunc_selector(GameScene::jumpToMenu)), NULL); 
			 this->runAction(act);
		 }

		 for (int j = 0; j < allBullet.size(); j++){
			 auto nowbullet = allBullet.at(j);           
			 Rect br(nowbullet->getPositionX(), nowbullet->getPositionY(), 20, 20);        //��ȡ�ӵ�Rect��Χ

			 if (er.intersectsRect(br)){      //������ӵ��ͷɻ���ײ���
				 auto labScore = (Label*)this->getChildByTag(120);
				 score += nowenemy->getTag();  //�ж����ֵл�
				 newBomb(nowbullet->getPositionX(), nowbullet->getPositionY());   //��ը
				 labScore->setString(String::createWithFormat("score:%d", score)->_string);  //�ӷ�     
				 nowbullet->removeFromParent();  //ɾ���ӵ�
				 allBullet.eraseObject(nowbullet);
				 nowenemy->removeFromParent();  //ɾ���л�
				 allEnemy.eraseObject(nowenemy);
				 i--;
				 //SimpleAudioEngine::getInstance()->playEffect("expo.mp3");  ��ը��Ч  pauseeffectʹ��ը��BGMͬʱ����
				 break;	    //����ɾ����������һ�ּ��
			 }     
		 }
	 } 
 }

 void GameScene::newBomb(int x, int y){      //��ըЧ��
	 Vector<SpriteFrame*> allframe;
	 for (int i = 0; i < 7; i++){
		 SpriteFrame* sf = SpriteFrame::create("boom.png", Rect(i * 44, 0, 44, 49));
		 allframe.pushBack(sf);
	 }
	 Animation* ani = Animation::createWithSpriteFrames(allframe, 0.03);
	 auto sprite = Sprite::create();
	 Action* act = Sequence::create(
		 Animate::create(ani),   //��������
		 CCCallFuncN::create(sprite,callfuncN_selector(GameScene::killme)),      //����ɾ���Լ�����   
		 NULL);
	 sprite->setPosition(Point(x, y));
	 sprite->runAction(act);
	 this->addChild(sprite);
 }

 void GameScene::killme(Node* pSender){       //ɾ���Լ�
	 pSender->removeFromParentAndCleanup(true);
 }

 void GameScene::jumpToMenu(){
	 SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	 Director::getInstance()->replaceScene(HelloWorld::createScene());
 }
