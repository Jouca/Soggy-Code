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

    void onSubmit(cocos2d::CCObject * sender) {
        this->m_fields->text = this->m_textInput->getString();
        std::transform(
            this->m_fields->text.begin(),
            this->m_fields->text.end(),
            this->m_fields->text.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );

        SecretLayer5::onSubmit(sender);
    }

    void showSuccessAnimationWrapper() {
        this->showSuccessAnimation();
    }

    void fadeInLoadingCircleWrapper() {
        this->m_circleSprite->fadeInCircle(false, 0.5, 0.0);
    }

    void deleteWhite(float dt) {
        RewardUnlockLayer* rewardPage = CCDirector::sharedDirector()->getRunningScene()->getChildByType<RewardUnlockLayer>(0);
        if (!rewardPage) return;
        CCSprite* sog = static_cast<CCSprite*>(rewardPage->m_mainLayer->getChildByID("sog"_spr));
        CCSprite* white = static_cast<CCSprite*>(rewardPage->m_mainLayer->getChildByID("white"_spr));

        sog->setVisible(true);
        white->removeFromParentAndCleanup(true);
    }

    void showSoggyCatWithJumpscare() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        RewardUnlockLayer* rewardPage = CCDirector::sharedDirector()->getRunningScene()->getChildByType<RewardUnlockLayer>(0);
        if (!rewardPage) return;

        CCLayerColor* white = CCLayerColor::create(ccc4(255, 255, 255, 255), winSize.width, winSize.height);
        white->setID("white"_spr);

        CCSprite* sprite = CCSprite::create("soggycat.png"_spr);
        sprite->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        sprite->setScaleX(winSize.width / sprite->getContentWidth());
        sprite->setScaleY(winSize.height / sprite->getContentHeight());
        sprite->setVisible(false);
        sprite->setID("sog"_spr);

        schedule(schedule_selector(TheWraith::deleteWhite), 0, 0, 0.125f);

        rewardPage->m_mainLayer->addChild(white, 10);
        rewardPage->m_mainLayer->addChild(sprite, 10);
    }

    void showSoggyCat() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite* sprite = CCSprite::create("soggycat.png"_spr);
        sprite->setPosition(ccp(winSize.width / 2, (winSize.height / 2) + 69));
        sprite->setScale(0.2f);

        RewardUnlockLayer* rewardPage = CCDirector::sharedDirector()->getRunningScene()->getChildByType<RewardUnlockLayer>(0);
        if (!rewardPage) return;
        rewardPage->m_mainLayer->addChild(sprite);
        this->m_wraithButton->setEnabled(true);
    }

    void closeSog() {
        RewardUnlockLayer* rewardPage = CCDirector::sharedDirector()->getRunningScene()->getChildByType<RewardUnlockLayer>(0);
        if (!rewardPage) return;

        rewardPage->removeFromParentAndCleanup(true);

        this->m_wraithButton->setEnabled(true);
    }

    void showCollectRewardWrapper() {
        GJRewardItem* item = GJRewardItem::create();
        RewardUnlockLayer* layer = RewardUnlockLayer::create(2, nullptr);

        // Put layer in scene
        cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(layer);

        layer->showCollectReward(item);

        // Show soggycat after 1 second
        cocos2d::CCDelayTime* delay = cocos2d::CCDelayTime::create(2);
        cocos2d::CCSequence* sequence;
        if (Mod::get()->getSettingValue<bool>("jumpscare")) {
            cocos2d::CCCallFunc* call = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraith::showSoggyCatWithJumpscare));
            cocos2d::CCCallFunc* closeRewards = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraith::closeSog));

            sequence = cocos2d::CCSequence::create(
                delay,
                call,
                cocos2d::CCDelayTime::create(0.3f),
                closeRewards,
                nullptr
            );
        } else {
            cocos2d::CCCallFunc* call = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraith::showSoggyCat));

            sequence = cocos2d::CCSequence::create(
                delay,
                call,
                nullptr
            );
        }

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
        }
        else {
            SecretLayer5::onlineRewardStatusFailed();
        }
    }
};
