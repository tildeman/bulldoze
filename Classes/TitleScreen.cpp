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

#include "TitleScreen.h"
#include "Bulldoze.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

Scene* TitleScreen::createScene()
{
	return TitleScreen::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in TitleScreenScene.cpp\n");
}

// on "init" you need to initialize your instance
bool TitleScreen::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Scene::init() )
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	/////////////////////////////
	// 2. add your codes below...

	auto whitebg = Sprite::create("white.png");
	whitebg->setAnchorPoint(Vec2(0,0));
	whitebg->setScaleX(960);
	whitebg->setScaleY(540);
	this->addChild(whitebg);

	this->ball = Sprite::create("nigerball.png");
	this->ball->setPosition(Vec2(300,150));
	this->ball->setScale(0.75);
	this->addChild(this->ball);

	this->cutscenevirus = Sprite::create("virus_cutscene_with_eyes.png");
	this->cutscenevirus->setPosition(Vec2(600,150));
	this->cutscenevirus->setScale(0.75);
	this->addChild(this->cutscenevirus);
	
	this->start=Button::create("button_default.png","button_selected.png","button_disabled.png");
	this->start->setPosition(Vec2(480,300));
	this->start->setTitleText("Start");
	this->start->setTitleFontName("fonts/ComicNeue-Bold.ttf");
	this->start->setTitleFontSize(64);
	this->start->setTitleColor(Color3B(0,0,0));

	this->start->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
		if (type==Widget::TouchEventType::ENDED){
			this->start_cutscene(sender,type);
		}
	});

	this->addChild(this->start);

	this->title=Label::createWithTTF("Something vs Corona 2","fonts/ComicNeue-Bold.ttf",72);
	this->title->setPosition(Vec2(480,450));
	this->title->setColor(Color3B(0,0,0));
	this->addChild(this->title);

	return true;
}

void TitleScreen::add_virus(Vec2 c){
	auto virus=Sprite::create("virus_cutscene_with_eyes.png");
	virus->setPosition(c);
	virus->setScale(0.75);
	this->addChild(virus);
}

void TitleScreen::start_cutscene(Ref* sender, Widget::TouchEventType type){
	this->start->setVisible(0);
	this->title->setVisible(0);
	
	auto v1=CallFunc::create([=](){
		TitleScreen::add_virus(Vec2(475,400));
	});
	auto v2=CallFunc::create([=](){
		TitleScreen::add_virus(Vec2(825,400));
	});
	auto v3=CallFunc::create([=](){
		TitleScreen::add_virus(Vec2(150,400));
		this->cutscenevirus->runAction(MoveTo::create(0,Vec2(700,150)));
		TitleScreen::add_virus(Vec2(400,150));
	});
	DelayTime* delay=DelayTime::create(0.25);
	auto virus=Sequence::create(delay,v1,delay,v2,delay,v3,nullptr);

	auto ohshitno=CallFunc::create([=](){
		this->ball->runAction(Sequence::create(MoveTo::create(0.75,Vec2(160,150)),ScaleBy::create(0,-1,1),MoveTo::create(0.75,Vec2(-150,150)),nullptr));
	});
	auto tomain=CallFunc::create([=](){
		Director::getInstance()->replaceScene(TransitionFade::create(1,Bulldoze::createScene()));
	});

	this->runAction(Sequence::create(Spawn::createWithTwoActions(virus,ohshitno),DelayTime::create(1),tomain,nullptr));
}

void TitleScreen::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
