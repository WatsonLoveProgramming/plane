#include "cocos2d.h"
using namespace cocos2d;

class GameScene :Layer{
public:
	bool init();
	CREATE_FUNC(GameScene);
	static Scene* createScene();
	void moveBackground(float t);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	int px, py; //plane's coordinate
	Vector<Sprite*> allBullet;   //collection of bullets
	void newBullet(float t);
	void moveBullet(float t);
	Vector<Sprite*> allEnemy;    //collection of enemy planes
	void newEnemy(float t);
	void moveEnemy(float t);


	void update(float t);
	int score; 

};
