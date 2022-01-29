/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __BULLDOZE_SCENE_H__
#define __BULLDOZE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>

class TitleScreen : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	cocos2d::ui::Button* ball;
	cocos2d::Sprite* cutscenevirus;
	cocos2d::ui::Button* start;
	cocos2d::Label* title;
	void start_cutscene(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void add_virus(cocos2d::Vec2 c);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	// implement the "static create()" method manually
	CREATE_FUNC(TitleScreen);
};

class Bulldoze : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	int virus_count=30;
	double tank_speed=2;
	double tank_turnspeed=1.5;
	char keys;
	bool tank_dbhb=0;
	bool virus_dbhb=0;
	bool debug_keys=0;
	bool active=1;
	bool left_border_collision=0;
	bool return_to_start=0;
	std::vector<cocos2d::Sprite*> viruslist;
	cocos2d::Sprite* tank;
	cocos2d::Sprite* greenbar;
	std::vector<cocos2d::Sprite*> tank_coords;
	std::vector<std::vector<cocos2d::Sprite*>> virus_coords;
	std::vector<bool> virus_is_dead;
	cocos2d::Label* check_keys;
	void move();
	void kill_bulldozed();
	void check_collision();
	int count_dead();
	cocos2d::Rect get_tankhitbox();
	std::vector<cocos2d::Rect> get_virushitboxes();

	cocos2d::EventListenerKeyboard* listener;
	void key_is_pressed(cocos2d::EventKeyboard::KeyCode key,cocos2d::Event* event);
	void key_is_released(cocos2d::EventKeyboard::KeyCode key,cocos2d::Event* event);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void update(float) override;
	
	// implement the "static create()" method manually
	CREATE_FUNC(Bulldoze);
};

class Loss : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	// implement the "static create()" method manually
	CREATE_FUNC(Loss);
};

class Victory : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	std::vector<cocos2d::Vec2> virus_coords;
	std::vector<cocos2d::Sprite*> viruslist;

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	// implement the "static create()" method manually
	CREATE_FUNC(Victory);
};

#endif // __BULLDOZE_SCENE_H__
