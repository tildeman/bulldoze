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

#include "Bulldoze.h"
#include "ui/CocosGUI.h"
#include <random>

USING_NS_CC;
using namespace ui;

double pi=3.141592653589793;
std::vector<Vec2> virus_coords_vc;

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

Rect intersect(Rect a, Rect b){
	int x1=std::max(a.getMinX(),b.getMinX()), x2=std::min(a.getMaxX(),b.getMaxX()), y1=std::max(a.getMinY(),b.getMinY()), y2=std::min(a.getMaxY(),b.getMaxY());
	return Rect(x1,y1,x1-x2,y1-y2);
}

Scene* Bulldoze::createScene()
{
	return Bulldoze::create();
}

int Bulldoze::count_dead(){
	int c=0;
	for (int i=0;i<this->virus_count;i++) c+=this->virus_is_dead[i];
	return c;
}

void Bulldoze::key_is_pressed(EventKeyboard::KeyCode key,Event* event){
	if (key==EventKeyboard::KeyCode::KEY_LEFT_ARROW){
		this->keys+=1;
	}
	else if (key==EventKeyboard::KeyCode::KEY_RIGHT_ARROW){
		this->keys+=2;
	}
}

void Bulldoze::key_is_released(EventKeyboard::KeyCode key,Event* event){
	if (key==EventKeyboard::KeyCode::KEY_LEFT_ARROW){
		this->keys-=1;
	}
	else if (key==EventKeyboard::KeyCode::KEY_RIGHT_ARROW){
		this->keys-=2;
	}
}

// on "init" you need to initialize your instance
bool Bulldoze::init()
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

	this->greenbar=Sprite::create("white.png");
	this->greenbar->setColor(Color3B(0,255,0));
	this->greenbar->setScale(2,540);
	this->greenbar->setAnchorPoint(Vec2(0,0));
	this->greenbar->setVisible(0);
	this->addChild(greenbar);

	std::random_device virus_pos;
	std::mt19937 gen(virus_pos());
	std::uniform_int_distribution<int> xpos(60,880);
	std::uniform_int_distribution<int> ypos(60,460);

	this->viruslist.resize(this->virus_count);
	this->virus_is_dead.resize(this->virus_count);
	for (int i=0;i<this->virus_count;i++){
		this->viruslist[i]=Sprite::create("virus.png");
		this->viruslist[i]->setScale(0.75);
		this->viruslist[i]->setPosition(Vec2(xpos(gen),ypos(gen)));
		this->addChild(this->viruslist[i]);
		this->virus_is_dead[i]=0;
	}

	if (this->virus_dbhb){
		this->virus_coords.resize(virus_count);

		for (int i=0;i<virus_count;i++){
			this->virus_coords[i].resize(4);
			for (int j=0;j<4;j++){
				this->virus_coords[i][j]=Sprite::create("white.png");
				this->virus_coords[i][j]->setColor(Color3B(255,128,0));
				if (j%2) this->virus_coords[i][j]->setScaleY(100);
				else this->virus_coords[i][j]->setScaleX(100);
				this->addChild(this->virus_coords[i][j]);
			}
		}
	}

	this->tank=Sprite::create("tank.png");
	this->tank->setPosition(Vec2(-94,270));
	this->tank->setRotation(90);
	this->tank->setScale(2.0/3);
	this->addChild(this->tank);

	listener=EventListenerKeyboard::create();
	this->listener->onKeyPressed=CC_CALLBACK_2(Bulldoze::key_is_pressed,this);
	this->listener->onKeyReleased=CC_CALLBACK_2(Bulldoze::key_is_released,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listener,this);

	if (this->tank_dbhb){
		this->tank_coords.resize(4);
		for (int i=0;i<4;i++){
			this->tank_coords[i]=Sprite::create("white.png");
			this->tank_coords[i]->setColor(Color3B(255,128,0));
			if (i%2) this->tank_coords[i]->setScaleY(150);
			else this->tank_coords[i]->setScaleX(150);
			this->addChild(this->tank_coords[i]);
		}
	}

	if (debug_keys){
		check_keys=Label::createWithTTF(TTFConfig("fonts/arial.ttf",24),"Bulldoze count: ");
		check_keys->setAnchorPoint(Vec2(0,1));
		check_keys->setColor(Color3B(0,0,0));
		check_keys->setPosition(Vec2(0,540));
		this->addChild(check_keys);
	}

	this->scheduleUpdate();
	return true;
}


void Bulldoze::move(){
	Vec2 current_coords;
	float current_rotation=this->tank->getRotation();
	this->tank->getPosition(&current_coords.x,&current_coords.y);
	this->tank->setPosition(Vec2(current_coords.x+sin(current_rotation/180*pi)*this->tank_speed,current_coords.y+cos(current_rotation/180*pi)*this->tank_speed));
}

Rect Bulldoze::get_tankhitbox(){
	float current_rotation=this->tank->getRotation();
	Rect coords=this->tank->getBoundingBox();
	Rect hitbox=Rect(coords.getMidX()-sin(current_rotation/180*pi)*15-75,coords.getMidY()-cos(current_rotation/180*pi)*15-75,150,150);
	if (this->tank_dbhb){
		this->tank_coords[0]->setPosition(Vec2(hitbox.getMidX(),hitbox.getMinY()));
		this->tank_coords[1]->setPosition(Vec2(hitbox.getMinX(),hitbox.getMidY()));
		this->tank_coords[2]->setPosition(Vec2(hitbox.getMidX(),hitbox.getMaxY()));
		this->tank_coords[3]->setPosition(Vec2(hitbox.getMaxX(),hitbox.getMidY()));
	}
	return hitbox;
}

void Bulldoze::kill_bulldozed(){
	Rect tankhb=get_tankhitbox();
	std::vector<Rect> virushb=get_virushitboxes();
	Rect union_area;
	for (int i=0;i<virus_count;i++){
		union_area=intersect(tankhb,virushb[i]);
		if (tankhb.intersectsRect(virushb[i])&&union_area.size.width*union_area.size.height>=8000){
			this->viruslist[i]->setTexture("virus_dead.png");
			this->virus_is_dead[i]=1;
		}
	}
}

void Bulldoze::check_collision(){
	Size bounds=Director::getInstance()->getVisibleSize();
	Rect tankhb=get_tankhitbox();
	Vec2 coords=this->tank->getPosition();

	if (coords.x>tankhb.size.width&&(!return_to_start)) left_border_collision=1;

	bool cond=((tankhb.getMinX()<0)&&left_border_collision)||(tankhb.getMinY()<0)||(tankhb.getMaxX()>bounds.width)||(tankhb.getMaxY()>bounds.height);
	if (cond){
		this->active=0;
		this->tank->runAction(TintTo::create(0,Color3B(255,0,0)));
		auto tomain=CallFunc::create([=](){
			Director::getInstance()->replaceScene(TransitionFade::create(1,Loss::createScene()));
		});
		this->runAction(Sequence::create(DelayTime::create(1),tomain,nullptr));
	}

	if (tankhb.getMaxX()<(0-tankhb.size.width)/2&&this->return_to_start){
		this->active=0;
		virus_coords_vc.resize(this->virus_count);
		for (int j=0;j<this->virus_count;j++){
			virus_coords_vc[j]=this->viruslist[j]->getPosition();
		}
		Director::getInstance()->replaceScene(TransitionFade::create(1,Victory::createScene()));
	}
}

std::vector<Rect> Bulldoze::get_virushitboxes(){
	std::vector<Rect> hitbox;
	Rect coords;
	hitbox.resize(virus_count);
	for (int i=0;i<virus_count;i++){
		coords=this->viruslist[i]->getBoundingBox();
		hitbox[i]=Rect(coords.getMidX()-50,coords.getMidY()-50,100,100);
		if (this->virus_dbhb){
			this->virus_coords[i][0]->setPosition(Vec2(hitbox[i].getMidX(),hitbox[i].getMinY()));
			this->virus_coords[i][1]->setPosition(Vec2(hitbox[i].getMinX(),hitbox[i].getMidY()));
			this->virus_coords[i][2]->setPosition(Vec2(hitbox[i].getMidX(),hitbox[i].getMaxY()));
			this->virus_coords[i][3]->setPosition(Vec2(hitbox[i].getMaxX(),hitbox[i].getMidY()));
		}
	}
	return hitbox;
}

void Bulldoze::update(float delta){
	if (this->active){
		float current_rotation=this->tank->getRotation();
		if (this->keys%2==1){
			current_rotation-=this->tank_turnspeed;
		}
		if (this->keys/2==1){
			current_rotation+=this->tank_turnspeed;
		}
		this->tank->setRotation(current_rotation);
		this->move();
	}
	this->kill_bulldozed();
	if (active) this->check_collision();
	if (keys<0) keys=0;
	if (debug_keys){
		check_keys->setString(std::string("Bulldoze count: ")+std::to_string(count_dead()));
	}
	if (count_dead()==this->virus_count&&(!this->return_to_start)){
		this->return_to_start=1;
		this->greenbar->setVisible(1);
		this->left_border_collision=0;
	}
}

void Bulldoze::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

Scene* Loss::createScene()
{
	return Loss::create();
}

bool Loss::init(){
	if ( !Scene::init() )
	{
		return false;
	}

	Sprite* whitebg = Sprite::create("white.png");
	whitebg->setAnchorPoint(Vec2(0,0));
	whitebg->setScaleX(960);
	whitebg->setScaleY(540);
	this->addChild(whitebg);
	
	Sprite* vs_loss=Sprite::create("virus_cutscene_with_eyes.png");
	vs_loss->setScale(1.75);
	vs_loss->setPosition(Vec2(650,270));
	this->addChild(vs_loss);

	Sprite* tank=Sprite::create("tank.png");
	tank->setPosition(Vec2(200,250));
	tank->setRotation(10);
	this->addChild(tank);

	Sprite* ball=Sprite::create("nigerball.png");
	ball->setPosition(Vec2(200,80));
	ball->setScaleY(0.25);
	this->addChild(ball);

	TTFConfig lcfg=TTFConfig("fonts/ComicNeue-Bold.ttf",84);
	Label* loss=Label::createWithTTF(lcfg,"Loss :c");
	loss->setColor(Color3B(0,0,0));
	loss->setPosition(Vec2(200,450));
	this->addChild(loss);

	return true;
}

void Loss::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

Scene* Victory::createScene()
{
	return Victory::create();
}

bool Victory::init(){
	if ( !Scene::init() )
	{
		return false;
	}

	Sprite* whitebg = Sprite::create("white.png");
	whitebg->setAnchorPoint(Vec2(0,0));
	whitebg->setScaleX(960);
	whitebg->setScaleY(540);
	this->addChild(whitebg);

	int virus_count=virus_coords_vc.size();
	viruslist.resize(virus_count);
	for (int i=0;i<virus_count;i++){
		viruslist[i]=Sprite::create("virus.png");
		viruslist[i]->setPosition(Vec2(virus_coords_vc[i].x,(virus_coords_vc[i].y/10*4)+50));
		viruslist[i]->setScale(0.75,0.3);
		this->addChild(viruslist[i]);
	}

	Sprite* ball=Sprite::create("nigerball.png");
	ball->setPosition(Vec2(-200,200));
	ball->setScale(0.75);
	addChild(ball);

	TTFConfig vcfg=TTFConfig("fonts/ComicNeue-Bold.ttf",72);
	Label* victory=Label::createWithTTF(vcfg,"Victory :D");
	victory->setColor(Color3B(0,0,0));
	victory->setPosition(Vec2(500,400));
	victory->setOpacity(0);
	addChild(victory);

	ball->runAction(MoveTo::create(1,Vec2(200,250)));
	victory->runAction(Sequence::create(DelayTime::create(1),FadeIn::create(0),nullptr));
	return true;
}

void Victory::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
