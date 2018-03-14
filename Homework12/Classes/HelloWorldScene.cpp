#include "HelloWorldScene.h"
#include "Monster.h"
#pragma execution_character_set("utf-8")
#define database UserDefault::getInstance()
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height/2));
	addChild(player, 4);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	blood = 100;
	pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
	addChild(pT,2);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,1);

	// 获取工厂，生成怪物，放置在场景中
	for (int i = 0; i < 3; i++) {
		auto fac = Factory::getInstance();
		auto m = fac->createMonster();
		float x = random(origin.x, visibleSize.width);
		float y = random(origin.y, visibleSize.height);
		m->setPosition(x, y);
		addChild(m, 3);
	}
	
	// 默认方向为右
	cid = 'D';

	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	// 攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
	}
	attack.pushBack(frame0);


	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
	dead.pushBack(frame0);



	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	run.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}


	// 引用Tmx文件
	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);

	// 虚拟控制键位
	auto labela = Label::createWithTTF("A", "fonts/arial.ttf", 36);
	auto controla = MenuItemLabel::create(labela, CC_CALLBACK_0(HelloWorld::pressa, this));
	controla->setPosition(50, 50);

	auto labels = Label::createWithTTF("S", "fonts/arial.ttf", 36);
	auto controls = MenuItemLabel::create(labels, CC_CALLBACK_0(HelloWorld::presss, this));
	controls->setPosition(Vec2(origin.x + 100, origin.y + 50));

	auto labelw = Label::createWithTTF("W", "fonts/arial.ttf", 36);
	auto controlw = MenuItemLabel::create(labelw, CC_CALLBACK_0(HelloWorld::pressw, this));
	controlw->setPosition(100, 100);

	auto labeld = Label::createWithTTF("D", "fonts/arial.ttf", 36);
	auto controld = MenuItemLabel::create(labeld, CC_CALLBACK_0(HelloWorld::pressd, this));
	controld->setPosition(150, 50);

	auto labely = Label::createWithTTF("Y", "fonts/arial.ttf", 36);
	auto controly = MenuItemLabel::create(labely, CC_CALLBACK_0(HelloWorld::pressy, this));
	controly->setPosition(Vec2(visibleSize.width - 30, 100));

	auto menu = Menu::create(controla, controls, controld, controlw, controly, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 4);
	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
		kill = 0;
		database->setIntegerForKey("kill", 0);
	}
	else {
		kill = database->getIntegerForKey("kill");
	}

	// 击杀数目
	time = Label::createWithTTF("0", "fonts/arial.ttf", 36);
	time->setPosition(Vec2(visibleSize.width - 50, visibleSize.height - 50));
	this->addChild(time, 1);

	char* c = new char[5];
	sprintf(c, "%d", kill);
	time->setString(c);


	schedule(schedule_selector(HelloWorld::count), 5.0f, kRepeatForever, 0);
	schedule(schedule_selector(HelloWorld::hitByMonster), 0.5f, kRepeatForever, 0);
	return true;
}

void HelloWorld::pressw() {
	int currentAnimateIndex = 0;
	for (int i = 0; i < 22; i++) {
		if (player->getDisplayFrame() == dead.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	for (int i = 1; i < 17; i++) {
		if (player->getDisplayFrame() == attack.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	for (int i = 0; i < 7; i++) {
		if (player->getDisplayFrame() == run.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	if (currentAnimateIndex == 0) {
		player->stopAction(deadAnimate);
		player->stopAction(attackAnimate);
		player->stopAction(runAnimate);
		player->stopAction(realmove);
		auto runAnimation = Animation::createWithSpriteFrames(run, 0.05f);
		auto runAnimate = Animate::create(runAnimation);
		if (player->getPosition().y + 80 < visibleSize.height) {
			auto moveto = MoveTo::create(0.4f, Vec2(player->getPosition().x, player->getPosition().y + 40));
			auto realmove = Spawn::createWithTwoActions(moveto, runAnimate);
			player->runAction(realmove);
		}
		else {
			player->runAction(runAnimate);
		}
	}
}

void HelloWorld::pressa() {
	int currentAnimateIndex = 0;
	for (int i = 0; i < 22; i++) {
		if (player->getDisplayFrame() == dead.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}

	for (int i = 1; i < 17; i++) {
		if (player->getDisplayFrame() == attack.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	for (int i = 0; i < 7; i++) {
		if (player->getDisplayFrame() == run.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	if (currentAnimateIndex == 0) {
		player->stopAction(deadAnimate);
		player->stopAction(attackAnimate);
		player->stopAction(runAnimate);
		player->stopAction(realmove);
		if (cid != 'A') {
			player->setFlippedX(true);
		}
		cid = 'A';
		auto runAnimation = Animation::createWithSpriteFrames(run, 0.05f);
		auto runAnimate = Animate::create(runAnimation);
		if (player->getPosition().x - 80 > 0) {
			auto moveto = MoveTo::create(0.4f, Vec2(player->getPosition().x - 40, player->getPosition().y));
			auto realmove = Spawn::createWithTwoActions(moveto, runAnimate);
			player->runAction(realmove);
		}
		else {
			player->runAction(runAnimate);
		}
	}
}

void HelloWorld::presss() {
	int currentAnimateIndex = 0;
	for (int i = 0; i < 22; i++) {
		if (player->getDisplayFrame() == dead.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	for (int i = 1; i < 17; i++) {
		if (player->getDisplayFrame() == attack.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	for (int i = 0; i < 7; i++) {
		if (player->getDisplayFrame() == run.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	if (currentAnimateIndex == 0) {
		player->stopAction(deadAnimate);
		player->stopAction(attackAnimate);
		player->stopAction(runAnimate);
		player->stopAction(realmove);
		auto runAnimation = Animation::createWithSpriteFrames(run, 0.05f);
		auto runAnimate = Animate::create(runAnimation);
		if (player->getPosition().y - 80 > 0) {
			auto moveto = MoveTo::create(0.4f, Vec2(player->getPosition().x, player->getPosition().y - 40));
			auto realmove = Spawn::createWithTwoActions(moveto, runAnimate);
			player->runAction(realmove);
		}
		else {
			player->runAction(runAnimate);
		}
	}
}

void HelloWorld::pressd() {
	int currentAnimateIndex = 0;
	for (int i = 0; i < 22; i++) {
		if (player->getDisplayFrame() == dead.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	for (int i = 1; i < 17; i++) {
		if (player->getDisplayFrame() == attack.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	for (int i = 0; i < 7; i++) {
		if (player->getDisplayFrame() == run.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	if (currentAnimateIndex == 0) {
		player->stopAction(deadAnimate);
		player->stopAction(attackAnimate);
		player->stopAction(runAnimate);
		player->stopAction(realmove);
		if (cid != 'D') {
			player->setFlipX(false);
		}
		cid = 'D';
		auto runAnimation = Animation::createWithSpriteFrames(run, 0.05f);
		auto runAnimate = Animate::create(runAnimation);
		if (player->getPosition().x + 80 < visibleSize.width) {
			auto moveto = MoveTo::create(0.4f, Vec2(player->getPosition().x + 40, player->getPosition().y));
			auto realmove = Spawn::createWithTwoActions(moveto, runAnimate);
			player->runAction(realmove);
		}
		else {
			player->runAction(runAnimate);
		}
	}
}

void HelloWorld::pressx() {
	player->stopAction(runAnimate);
	player->stopAction(attackAnimate);
	auto deadAnimation = Animation::createWithSpriteFrames(dead, 0.1f);
	deadAnimate = Animate::create(deadAnimation);
	player->runAction(deadAnimate);
	if (blood - 20 > 0) {
		blood = blood - 20;
		auto reduce = ProgressTo::create(1.0f, blood);
		pT->runAction(reduce);
	}
	else {
		blood = 0;
		auto reduce = ProgressTo::create(1.0f, blood);
		pT->runAction(reduce);
	}
	
}

void HelloWorld::pressy() {
	int currentAnimateIndex = 0;
	for (int i = 1; i < 17; i++) {
		if (player->getDisplayFrame() == attack.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	for (int i = 0; i < 22; i++) {
		if (player->getDisplayFrame() == dead.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	if (currentAnimateIndex == 0) {
		player->stopAction(runAnimate);
		player->stopAction(deadAnimate);

		Rect playerRect = player->getBoundingBox();
		Rect attackRect = Rect(playerRect.getMinX() - 60, playerRect.getMinY(), playerRect.getMaxX() - playerRect.getMinX() + 120, playerRect.getMaxY() - playerRect.getMinY());


		auto attackAnimation = Animation::createWithSpriteFrames(attack, 0.1f);
		attackAnimate = Animate::create(attackAnimation);

		player->runAction(attackAnimate);

		auto fac = Factory::getInstance();
		if (fac->attackMonster(attackRect)) {
			if (blood + 20 < 100) {
				blood = blood + 20;
				auto plus = ProgressTo::create(1.0f, blood);
				pT->runAction(plus);
			}
			else {
				blood = 100;
				auto plus = ProgressTo::create(1.0f, blood);
				pT->runAction(plus);
			}
			kill++;
			database->setIntegerForKey("kill", kill);
			char* c = new char[5];
			sprintf(c, "%d", kill);
			time->setString(c);
		}

	}

}

void HelloWorld::count(float dt) {


		// 获取工厂，生成怪物，放置在场景中
		auto fac = Factory::getInstance();
		auto m = fac->createMonster();
		float x = random(origin.x, visibleSize.width);
		float y = random(origin.y, visibleSize.height);
		m->setPosition(x, y);
		addChild(m, 3);

		// 向主角移动
		fac->moveMonster(player->getPosition(), 1.0f);


}

// 碰撞函数
void HelloWorld::hitByMonster(float dt) {
	int currentAnimateIndex = 0;
	for (int i = 0; i < 22; i++) {
		if (player->getDisplayFrame() == dead.at(i)) {
			currentAnimateIndex = i + 1;
		}
	}
	if (currentAnimateIndex == 0) {
		player->stopAction(runAnimate);
		auto fac = Factory::getInstance();
		Sprite* collision = fac->collider(player->getBoundingBox());
		if (collision != NULL) {
			fac->removeMonster(collision);
			this->pressx();
		}
	}
}