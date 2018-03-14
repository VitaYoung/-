#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	// void update(float dt) override;
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::Animate* attackAnimate;
	cocos2d::Animate* deadAnimate;
	cocos2d::Animate* runAnimate;
	cocos2d::Spawn* realmove;
	int dtime;
	float blood;
	cocos2d::ProgressTimer* pT;
	void pressw();
	void pressa();
	void presss();
	void pressd();
	void pressx();
	void pressy();
	void count(float dt);
};

#endif // __HELLOWORLD_SCENE_H__