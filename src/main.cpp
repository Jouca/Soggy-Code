#include "Geode/c++stl/string.hpp"
#include "Geode/cocos/actions/CCActionInterval.h"
#include <Geode/Geode.hpp>
#include <Geode/modify/SecretLayer5.hpp>
#include <Geode/modify/RewardUnlockLayer.hpp>

using namespace geode::prelude;

class $modify(TheWraith, SecretLayer5) {
    struct Fields {
        std::string text = "";
    };

    void onSubmit(cocos2d::CCObject* sender) {
        this->m_fields->text = this->m_textInput->getString();
        std::transform(
            this->m_fields->text.begin(), 
            this->m_fields->text.end(), 
            this->m_fields->text.begin(), 
            [](unsigned char c){ return std::tolower(c); }
        );

        SecretLayer5::onSubmit(sender);
    }

    void showSuccessAnimationWrapper() {
        this->showSuccessAnimation();
    }

    void fadeInLoadingCircleWrapper() {
        this->m_circleSprite->fadeInCircle(false, 0.5, 0.0);
    }

    void showSoggyCat() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite* sprite = CCSprite::create("soggycat.png"_spr);
        sprite->setPosition(ccp(winSize.width / 2, (winSize.height / 2) + 69));
        sprite->setScale(0.2f);

        CCNode* rewardPage = as<CCNode*>(cocos2d::CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(1));
        CCLayer* layer = as<CCLayer*>(rewardPage->getChildren()->objectAtIndex(0));
        layer->addChild(sprite);
    }

    void showCollectRewardWrapper() {
        GJRewardItem* item = GJRewardItem::create();
        RewardUnlockLayer* layer = RewardUnlockLayer::create(2, nullptr);

        // Put layer in scene
        cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(layer);

        layer->showCollectReward(item);

        // Show soggycat after 1 second
        cocos2d::CCDelayTime* delay = cocos2d::CCDelayTime::create(2);
        cocos2d::CCCallFunc* call = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraith::showSoggyCat));

        cocos2d::CCSequence* sequence = cocos2d::CCSequence::create(
            delay,
            call,
            nullptr
        );

        cocos2d::CCDirector::sharedDirector()->getRunningScene()->runAction(sequence);
    }

    void onlineRewardStatusFailed() {
        if (this->m_fields->text == "soggy") {
            cocos2d::CCDelayTime* delayCompleteAnimation = cocos2d::CCDelayTime::create(4);
            cocos2d::CCCallFunc* callCompleteAnimation = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraith::showSuccessAnimationWrapper));
            cocos2d::CCDelayTime* delayOpenChest = cocos2d::CCDelayTime::create(1);
            cocos2d::CCCallFunc* callFadeInLoadingCircle = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraith::fadeInLoadingCircleWrapper));
            cocos2d::CCCallFunc* callOpenChest = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraith::showCollectRewardWrapper));

            cocos2d::CCSequence* sequence = cocos2d::CCSequence::create(
                delayCompleteAnimation,
                callCompleteAnimation,
                delayOpenChest,
                callFadeInLoadingCircle,
                callOpenChest,
                nullptr
            );

            cocos2d::CCDirector::sharedDirector()->getRunningScene()->runAction(sequence);
        } else {
            SecretLayer5::onlineRewardStatusFailed();
        }
    }
};