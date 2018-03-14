#include "Thunder.h"
#include <algorithm>

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = Thunder::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool Thunder::init() {
  if (!Layer::init()) {
    return false;
  }
  // ���ʯͷ������
  rock = 1;

  isMove = false;  // �Ƿ����ɴ�
  visibleSize = Director::getInstance()->getVisibleSize();

  // ��������
  auto bgsprite = Sprite::create("bg.jpg");
  bgsprite->setPosition(visibleSize / 2);
  bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
                     visibleSize.height / bgsprite->getContentSize().height);
  this->addChild(bgsprite, 0);

  // �����ɴ�
  player = Sprite::create("player.png");
  player->setAnchorPoint(Vec2(0.5, 0.5));
  player->setPosition(visibleSize.width / 2, player->getContentSize().height);
  player->setName("player");
  this->addChild(player, 1);

  // ��ʾ��ʯ���ӵ�����
  enemysNum = Label::createWithTTF("enemys: 0", "fonts/arial.TTF", 20);
  enemysNum->setColor(Color3B(255, 255, 255));
  enemysNum->setPosition(50, 60);
  this->addChild(enemysNum, 3);
  bulletsNum = Label::createWithTTF("bullets: 0", "fonts/arial.TTF", 20);
  bulletsNum->setColor(Color3B(255, 255, 255));
  bulletsNum->setPosition(50, 30);
  this->addChild(bulletsNum, 3);

  addEnemy(5);        // ��ʼ����ʯ
  preloadMusic();     // Ԥ��������
  playBgm();          // ���ű�������
  explosion();        // ������ը֡����

  // ��Ӽ�����
  addTouchListener();
  addKeyboardListener();
  addCustomListener();

  // ������
  schedule(schedule_selector(Thunder::update), 0.04f, kRepeatForever, 0);

  return true;
}

//Ԥ���������ļ�
void Thunder::preloadMusic() {
  // Todo
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");

	audio->preloadEffect("music/explore.mp3");
	audio->preloadEffect("music/fire.mp3");
}

//���ű�������
void Thunder::playBgm() {
  // Todo
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/bgm.mp3", true);
}

//��ʼ����ʯ
void Thunder::addEnemy(int n) {
  enemys.clear();
  for (unsigned i = 0; i < 3; ++i) {
    char enemyPath[20];
    sprintf(enemyPath, "stone%d.png", 3 - i);
    double width = visibleSize.width / (n + 1.0),
           height = visibleSize.height - (50 * (i + 1));
    for (int j = 0; j < n; ++j) {
      auto enemy = Sprite::create(enemyPath);
      enemy->setAnchorPoint(Vec2(0.5, 0.5));
      enemy->setScale(0.5, 0.5);
      enemy->setPosition(width * (j + 1), height);
      enemys.push_back(enemy);
      addChild(enemy, 1);
    }
  }
}

// ��ʯ�����ƶ��������µ�һ��(�ӷ���)
void Thunder::newEnemy() {
  // Todo
	list<Sprite*>::iterator it = enemys.begin();
	for (; it != enemys.end();) {
		(*it)->setPosition(Vec2((*it)->getPosition().x, (*it)->getPosition().y - 50));
		it++;
	}
	char enemyPath[20];
	sprintf(enemyPath, "stone%d.png", rock);
	for (int i = 0; i < 5; i++) {
		auto enemy = Sprite::create(enemyPath);
		enemy->setAnchorPoint(Vec2(0.5, 0.5));
		enemy->setScale(0.5, 0.5);
		double width = visibleSize.width / (5 + 1.0);
		enemy->setPosition(Vec2(width * (i + 1) - 86, visibleSize.height - 50));
		enemys.push_back(enemy);
		addChild(enemy, 1);
	}
	rock++;
	if (rock == 4) {
		rock = 1;
	}
}

// �ƶ��ɴ�
void Thunder::movePlane(char c) {
  // Todo
	switch (c)
	{
	case 'A':
		if (player->getPosition().x - 20 >= 0) {
			moveto = MoveTo::create(0.2f, Vec2(player->getPosition().x - 20, player->getPosition().y));
			player->runAction(moveto);
		}
		else {
			player->stopAction(moveto);
			moveto = MoveTo::create(0.2f, Vec2(0, player->getPosition().y));
			player->runAction(moveto);
		}
		break;
	case 'D':
		if (player->getPosition().x + 20 <= visibleSize.width) {
			moveto = MoveTo::create(0.2f, Vec2(player->getPosition().x + 20, player->getPosition().y));
			player->runAction(moveto);
		}
		else {
			player->stopAction(moveto);
			moveto = MoveTo::create(0.2f, Vec2(visibleSize.width, player->getPosition().y));
			player->runAction(moveto);
		}
		break;
	default:
		break;
	}
}

//�����ӵ�
void Thunder::fire() {
  auto bullet = Sprite::create("bullet.png");
  bullet->setAnchorPoint(Vec2(0.5, 0.5));
  bullets.push_back(bullet);
  bullet->setPosition(player->getPosition());
  addChild(bullet, 1);
  SimpleAudioEngine::getInstance()->playEffect("music/fire.wav", false);

  // �Ƴ��ɳ���Ļ����ӵ�
  // Todo
  bullet->runAction(MoveBy::create(1.0f, Vec2(0, visibleSize.height)));

  
}

// �иը����֡
void Thunder::explosion() {
  // Todo
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
	explore.reserve(8);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(188.8 * i, 0, 188.8, 160)));
		explore.pushBack(frame);
	}
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(188.8 * i, 160, 188.8, 160)));
		explore.pushBack(frame);
	}
}

void Thunder::update(float f) {
  // ʵʱ����ҳ������ʯ���ӵ�����(����ɾ��)
  // Ҫ��������ʾ��ȷ(�ӷ���)
	cocos2d:list<Sprite*>::iterator it = bullets.begin();
  for (; it != bullets.end();) {
	  if ((*it)->getPosition().y >= visibleSize.height) {
		  it = bullets.erase(it);
	  }
	  else {
		  it++;
	  }
  }
  char str[15];
  sprintf(str, "enemys: %d", enemys.size());
  enemysNum->setString(str);
  sprintf(str, "bullets: %d", bullets.size());
  bulletsNum->setString(str);

  // �ɴ��ƶ�
  if (isMove)
    this->movePlane(movekey);

  static int ct = 0;
  static int dir = 4;
  ++ct;
  if (ct == 120)
    ct = 40, dir = -dir;
  else if (ct == 80) {
    dir = -dir;
    newEnemy();  // ��ʯ�����ƶ��������µ�һ��(�ӷ���)
  }
  else if (ct == 20)
    ct = 40, dir = -dir;

  //��ʯ�����ƶ�
  for (Sprite* s : enemys) {
    if (s != NULL) {
      s->setPosition(s->getPosition() + Vec2(dir, 0));
    }
  }

  // �ַ��Զ����¼�
  EventCustom e("meet");
  _eventDispatcher->dispatchEvent(&e);
}

// �Զ�����ײ�¼�
void Thunder::meet(EventCustom * event) {
  // �ж��ӵ��Ƿ������ʯ��ִ�ж�Ӧ����
  // Todo
	list<Sprite*>::iterator bullettemp = bullets.begin();
    list<Sprite*>::iterator enemytemp = enemys.begin();
	for (; bullettemp != bullets.end();) {
		int flag = 0;
		for (; enemytemp != enemys.end();) {
			Rect enemyRect = (*enemytemp)->getBoundingBox();
			Rect meetRect = Rect(enemyRect.getMinX() - 5, enemyRect.getMinY() - 5, enemyRect.getMaxX() - enemyRect.getMinX() + 10, enemyRect.getMaxY() - enemyRect.getMinY() + 10);
			if (meetRect.containsPoint((*bullettemp)->getPosition())) {
				auto explosionAnimation = Animation::createWithSpriteFrames(explore, 0.1f);
				auto explosionAnimate = Animate::create(explosionAnimation);
				auto disappear = FadeOut::create(0.01f);
				(*bullettemp)->runAction(disappear);
				auto seq = Sequence::createWithTwoActions(explosionAnimate, disappear);
				(*enemytemp)->runAction(seq);
				
				bullettemp = bullets.erase(bullettemp);
				enemytemp = enemys.erase(enemytemp);

				auto audio = SimpleAudioEngine::getInstance();
				audio->playEffect("music/explore.wav", false, 1.0f, 1.0f, 1.0f);
				flag = 1;
				break;
			}
			else {
				enemytemp++;
			}
		}
		enemytemp = enemys.begin();
		if (flag == 0) {
			bullettemp++;
		}
	}

  // �ж���Ϸ�Ƿ������ִ�ж�Ӧ����
  // Todo
	enemytemp = enemys.begin();
	Rect playerRect = player->getBoundingBox();
	Rect meetRect = Rect(playerRect.getMinX() - 5, playerRect.getMinY() - 5, playerRect.getMaxX() - playerRect.getMinX() + 10, playerRect.getMaxY() - playerRect.getMinY() + 10);
	for (; enemytemp != enemys.end();) {
		if (meetRect.containsPoint((*enemytemp)->getPosition())) {
			auto explosionAnimation = Animation::createWithSpriteFrames(explore, 0.1f);
			auto explosionAnimate = Animate::create(explosionAnimation);
			auto disappear = FadeOut::create(0.01f);
			auto seq = Sequence::createWithTwoActions(explosionAnimate, disappear);
			player->runAction(seq);
			auto gameover = Sprite::create("gameOver.png");
			gameover->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
			addChild(gameover, 3);
			auto dispatcher = Director::getInstance()->getEventDispatcher();
			dispatcher->removeAllEventListeners();
			unschedule(schedule_selector(Thunder::update));
			break;
		}
		else {
			enemytemp++;
		}
	}
}

// ����Զ��������
void Thunder::addCustomListener() {
  // Todo
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto customlistener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	dispatcher->addEventListenerWithFixedPriority(customlistener, 1);

}

// ��Ӽ����¼�������
void Thunder::addKeyboardListener() {
  // Todo
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto keyboardlistener = EventListenerKeyboard::create();
	keyboardlistener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	keyboardlistener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, this);
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_A:
    movekey = 'A';
    isMove = true;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
  case EventKeyboard::KeyCode::KEY_D:
    movekey = 'D';
    isMove = true;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    fire();
    break;
  }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
    isMove = false;
    break;
  }
}

// ��Ӵ����¼�������
void Thunder::addTouchListener() {
  // Todo
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
}

// ����������ڵ�
bool Thunder::onTouchBegan(Touch *touch, Event *event) {
	fire();
  return true;
}

void Thunder::onTouchEnded(Touch *touch, Event *event) {
  isClick = false;
}

// ����갴ס�ɴ���ɿ��Ʒɴ��ƶ� (�ӷ���)
void Thunder::onTouchMoved(Touch *touch, Event *event) {
  // Todo
}
