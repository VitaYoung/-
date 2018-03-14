#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	stoneLayer = Layer::create();
	stone = Sprite::create("stone.png");
	stone->setPosition(560, 480);
	stoneLayer->addChild(stone, 2);
	this->addChild(stoneLayer, 2);

	mouseLayer = Layer::create();
	mouseLayer->setPosition(Vec2(0, visibleSize.height / 2));
    mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouseLayer->addChild(mouse, 2);
	mouse->setPosition(Vec2(visibleSize.width / 2, 0));
	this->addChild(mouseLayer, 2);

	auto bg = Sprite::create("level-background-0.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	shootItem = MenuItemLabel::create(Label::createWithSystemFont("Shoot", "Marker Felt", 50), CC_CALLBACK_1(GameSence::shootMenuCallback, this));
	shootItem->setPosition(850, 450);
	auto menu = Menu::create(shootItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();

	mouse->stopAction(moveTo);
	auto cheese = Sprite::create("cheese.png");
	this->addChild(cheese, 1);
	cheese->setPosition(Vec2(location.x, location.y));
	auto fadeout = FadeOut::create(4.0f);
	cheese->runAction(fadeout);

	CCPoint temp = mouseLayer->convertToNodeSpace(location);
	moveTo = MoveTo::create(2, Vec2(temp.x, temp.y));
	mouse->runAction(moveTo);



	return true;
}

void GameSence::shootMenuCallback(Ref* pSender) {

	mouse->stopAction(moveTo);

	auto shoot = Sprite::create("stone.png");
	this->addChild(shoot, 1);
	shoot->setPosition(560, 480);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	CCPoint stonefinal = stoneLayer->convertToNodeSpace(mouse->getPosition());
	stonefinal.y += visibleSize.height / 2;
	auto stonedes = MoveTo::create(1, Vec2(stonefinal.x, stonefinal.y));
	auto stoneout = FadeOut::create(0.0f);
	auto seq = Sequence::create(stonedes, stoneout, nullptr);
	shoot->runAction(seq);

	auto diamond = Sprite::create("diamond.png");
	auto rotate = RotateBy::create(1.0f, 360.0f);
	diamond->runAction(rotate);
	mouseLayer->addChild(diamond, 1);
	diamond->setPosition(Vec2(mouse->getPosition().x, mouse->getPosition().y));


	auto x = random(0, (int)visibleSize.width - 50);
	auto y = random(0, (int)visibleSize.height - 50);
	auto newposition = stonefinal;
	newposition.x = x;
	newposition.y = y;
	CCPoint temp = mouseLayer->convertToNodeSpace(newposition);
	moveTo = MoveTo::create(2, Vec2(temp.x, temp.y));
	mouse->runAction(moveTo);
}
