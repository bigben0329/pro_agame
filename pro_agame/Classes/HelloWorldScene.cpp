#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

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
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    
    // add a "sound" icon to operate the sound
    CCMenuItemImage *pSoundItem = CCMenuItemImage::create(
                                                          "sound.png",
                                                          "sound2.png",
                                                          this,
                                                          menu_selector(HelloWorld::menuSoundCallback) );
    pSoundItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, CCDirector::sharedDirector()->getWinSize().height - 20) );
    
    // create menu, it's an autorelease object
    CCMenu* pSoundMenu = CCMenu::create(pSoundItem, NULL);
    pSoundMenu->setPosition( CCPointZero );
    this->addChild(pSoundMenu, 1);


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    _pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    _pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(_pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("agame.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    pSprite->setScaleX(size.width/pSprite->getContentSize().width);
    pSprite->setScaleY(size.height/pSprite->getContentSize().height);
    
    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    
    /////////////////////////////
    // 4. add your codes below...
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    _player = CCSprite::create("Player_l.png", CCRectMake(0, 0, 108, 160) );
    _player->setPosition( ccp(origin.x + _player->getContentSize().width/2,
                             origin.y + visibleSize.height/2) );
    this->addChild(_player);
    
    this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0 );
    this->setTouchEnabled(true);
    
    _targets = new CCArray;
    _projectiles = new CCArray;
    
    // use updateGame instead of update, otherwise it will conflit with SelectorProtocol::update
    // see http://www.cocos2d-x.org/boards/6/topics/1478
    this->schedule( schedule_selector(HelloWorld::updateGame) );
    
    _bPlayBackgroundMusic = true;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("backgroundmusic.mp3", true);
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::menuSoundCallback(CCObject* pSender)
{
    if (_bPlayBackgroundMusic) {
        _bPlayBackgroundMusic = false;
        CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    else
    {
        _bPlayBackgroundMusic = true;
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
}


// cpp with cocos2d-x
void HelloWorld::addTarget()
{
	CCSprite *target = CCSprite::create("Target0.png", CCRectMake(0,0,108,160) );
    
	// Determine where to spawn the target along the Y axis
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	float minY = target->getContentSize().height/2;
	float maxY = winSize.height -  target->getContentSize().height/2;
	int rangeY = (int)(maxY - minY);
	// srand( TimGetTicks() );
	int actualY = ( rand() % rangeY ) + (int)minY;
    
	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated
	target->setPosition(
                        ccp(winSize.width + (target->getContentSize().width/2),
                            CCDirector::sharedDirector()->getVisibleOrigin().y + actualY) );
	this->addChild(target);
    
	// Determine speed of the target
	int minDuration = (int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration - minDuration;
	// srand( TimGetTicks() );
	int actualDuration = ( rand() % rangeDuration ) + minDuration;
    
    
    CCSpriteFrame *frame0 = CCSpriteFrame::create("Target0.png", CCRectMake(0, 0, 108,160));
    CCSpriteFrame *frame1 = CCSpriteFrame::create("Target1.png", CCRectMake(0, 0, 108,160));
    CCArray *animFrames = new CCArray(6);
    animFrames->addObject(frame0);
    animFrames->addObject(frame1);
    CCAnimation *animation  = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);
    //最后根据动画模板创建动画
    CCAnimate *animate = CCAnimate::create(animation);
    //定义一个动作 让飞龙 CCJUMPto  高度是10为  飞行4次
    CCActionInterval* seq = (CCActionInterval*)(CCSequence::create(
                                                                   CCJumpTo::create(2.0f,CCPointMake(200,300),10,4),
                                                                   NULL) );
    //如果2个动作在一个总动作里 那么只会执行完一个再执行另一个
    //所以这分别定义2个动作 让飞行和前进同时发生 边飞边播放动画
    //CCRepeatForever永远重复该动作 于是你看见飞龙不停的飞啊飞
    //target->runAction(CCRepeatForever::create( seq ) );
    target->runAction(CCRepeatForever::create( animate ) );
    
	// Create the actions
	CCFiniteTimeAction* actionMove = CCMoveTo::create( (float)actualDuration,
                                                      ccp(0 - target->getContentSize().width/2, actualY) );
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this,
                                                             callfuncN_selector(HelloWorld::targetMoveFinished));
	target->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );
    
	// Add to targets array
	target->setTag(1);
	_targets->addObject(target);
}

void HelloWorld::targetMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);
    
    _targets->removeObject(sprite);
    
    if( 1 == sprite->getTag() )
    {
        _projectilesMissed++;
        updateResultMsg();
    }

    if( 10 == _projectilesMissed )
    {
		GameOverScene *gameOverScene = GameOverScene::create();
        gameOverScene->getLayer()->getLabel()->setString("You Lose :[");
        CCDirector::sharedDirector()->replaceScene(gameOverScene);
    }

}

void HelloWorld::projectileMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);
    
    _projectiles->removeObject(sprite);

}

void HelloWorld::gameLogic(float dt)
{
	this->addTarget();
}


// cpp with cocos2d-x
void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
    
	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);
    
	// Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSprite *projectile = CCSprite::create("Projectile.png", CCRectMake(0, 0, 20, 20));
	projectile->setPosition( ccp(origin.x+20, origin.y+winSize.height/2) );
    
	// Determinie offset of location to projectile
	float offX = location.x - projectile->getPosition().x;
	float offY = location.y - projectile->getPosition().y;
    
	// Bail out if we are shooting down or backwards
	if (offX <= 0) return;
    
	// Ok to add now - we've double checked position
	this->addChild(projectile);
    
	// Determine where we wish to shoot the projectile to
	float realX = origin.x+winSize.width + (projectile->getContentSize().width/2);
	float ratio = offY / offX;
	float realY = (realX * ratio) + projectile->getPosition().y;
	CCPoint realDest = ccp(realX, realY);
    
	// Determine the length of how far we're shooting
	float offRealX = realX - projectile->getPosition().x;
	float offRealY = realY - projectile->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
	float velocity = 480/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;
    
	// Move projectile to actual endpoint
	projectile->runAction( CCSequence::create(
                                              CCMoveTo::create(realMoveDuration, realDest),
                                              CCCallFuncN::create(this,
                                                                  callfuncN_selector(HelloWorld::projectileMoveFinished)),
                                              NULL) );
    
	// Add to projectiles array
	_projectiles->addObject(projectile);
    
    
    //add a player animate
    CCSpriteFrame *frame0 = CCSpriteFrame::create("Player_l0.png", CCRectMake(0, 0, 108,160));
    CCSpriteFrame *frame1 = CCSpriteFrame::create("Player_l1.png", CCRectMake(0, 0, 108,160));
    CCArray *animFrames = new CCArray(2);
    animFrames->addObject(frame0);
    animFrames->addObject(frame1);
    CCAnimation *animation  = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);
    CCAnimate *animate = CCAnimate::create(animation);
    _player->runAction( CCSequence::create(
                                          CCRepeat::create( animate,1 ),
                                          NULL) );

    
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("biubiu.mp3");
}

void HelloWorld::updateGame(float dt)
{
	CCArray *projectilesToDelete = new CCArray;
    CCObject* it = NULL;
    CCObject* jt = NULL;
    
	// for (it = _projectiles->begin(); it != _projectiles->end(); it++)
    CCARRAY_FOREACH(_projectiles, it)
	{
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
                                           projectile->getPosition().x - (projectile->getContentSize().width/2),
                                           projectile->getPosition().y - (projectile->getContentSize().height/2),
                                           projectile->getContentSize().width,
                                           projectile->getContentSize().height);
        
		CCArray* targetsToDelete =new CCArray;
        
		// for (jt = _targets->begin(); jt != _targets->end(); jt++)
        CCARRAY_FOREACH(_targets, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			CCRect targetRect = CCRectMake(
                                           target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().height/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);
            
			// if (CCRect::CCRectIntersectsRect(projectileRect, targetRect))
            if (projectileRect.intersectsRect(targetRect))
			{
				targetsToDelete->addObject(target);
			}
		}
        
		// for (jt = targetsToDelete->begin(); jt != targetsToDelete->end(); jt++)
        CCARRAY_FOREACH(targetsToDelete, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			_targets->removeObject(target);
            
            //add hit logic here
            target->setTag(2);
            target->stopAllActions();
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("a~.mp3");
            
            CCSpriteFrame *frame0 = CCSpriteFrame::create("Target2.png", CCRectMake(0, 0, 108,160));
            CCSpriteFrame *frame1 = CCSpriteFrame::create("Target3.png", CCRectMake(0, 0, 108,160));
            CCArray *animFrames = new CCArray(2);
            animFrames->addObject(frame0);
            animFrames->addObject(frame1);
            CCAnimation *animation  = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);
            CCAnimate *animate = CCAnimate::create(animation);
            
            target->runAction( CCSequence::create(
                                   CCRepeat::create( animate,3 ),
                                   CCCallFuncN::create(this,
                                       callfuncN_selector(HelloWorld::targetMoveFinished)),
                                   NULL) );
            
			_projectilesDestroyed++;
			if (_projectilesDestroyed >= 10)
			{
				GameOverScene *gameOverScene = GameOverScene::create();
				gameOverScene->getLayer()->getLabel()->setString("You Win!");
				CCDirector::sharedDirector()->replaceScene(gameOverScene);
			}
            updateResultMsg();
		}
        
		if (targetsToDelete->count() > 0)
		{
			projectilesToDelete->addObject(projectile);
		}
		targetsToDelete->release();
	}
    
	// for (it = projectilesToDelete->begin(); it != projectilesToDelete->end(); it++)
    CCARRAY_FOREACH(projectilesToDelete, it)
	{
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}
	projectilesToDelete->release();
}

void HelloWorld::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}

void HelloWorld::updateResultMsg()
{
    char szResultStr[256] = {0};
    snprintf(szResultStr, sizeof(szResultStr), "hit num %d miss num %d",
             _projectilesDestroyed, _projectilesMissed);
    _pLabel->setString(szResultStr);
}
