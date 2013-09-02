#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void menuSoundCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
    void targetMoveFinished(cocos2d::CCNode* sender);
    void projectileMoveFinished(cocos2d::CCNode* sender);
    
	void gameLogic(float dt);
	void updateGame(float dt);
    
	void registerWithTouchDispatcher();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    
protected:
    cocos2d::CCSprite *_player;
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;
    cocos2d::CCLabelTTF* _pLabel;
    
    int _projectilesDestroyed;
    int _projectilesMissed;
    
	void addTarget();
    void updateResultMsg();
    
    bool _bPlayBackgroundMusic;
};

#endif // __HELLOWORLD_SCENE_H__
