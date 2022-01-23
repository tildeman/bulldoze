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
int key_is_pressed=0;

Rect intersect(Rect a, Rect b){
	int x1=std::max(a.getMinX(),b.getMinX()), x2=std::min(a.getMaxX(),b.getMaxX()), y1=std::max(a.getMinY(),b.getMinY()), y2=std::min(a.getMaxY(),b.getMaxY());
	return Rect(x1,y1,x1-x2,y1-y2);
}

Scene* Bulldoze::createScene()
{
	return Bulldoze::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in BulldozeScene.cpp\n");
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

	std::random_device virus_pos;
	std::mt19937 gen(virus_pos());
	std::uniform_int_distribution<int> xpos(60,880);
	std::uniform_int_distribution<int> ypos(60,460);

	this->viruslist.resize(this->virus_count);
	for (int i=0;i<this->virus_count;i++){
		this->viruslist[i]=Sprite::create("virus.png");
		this->viruslist[i]->setScale(0.75);
		this->viruslist[i]->setPosition(Vec2(xpos(gen),ypos(gen)));
		this->addChild(this->viruslist[i]);
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
			viruslist[i]->setTexture("virus_dead.png");
		}
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
	float current_rotation=this->tank->getRotation();
	if (this->keys%2==1){
		current_rotation-=this->tank_turnspeed;
	}
	if (this->keys/2==1){
		current_rotation+=this->tank_turnspeed;
	}
	this->tank->setRotation(current_rotation);
	this->move();
	this->kill_bulldozed();
}

void Bulldoze::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
