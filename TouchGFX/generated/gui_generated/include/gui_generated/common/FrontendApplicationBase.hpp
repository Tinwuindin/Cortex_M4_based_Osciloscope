/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef FRONTENDAPPLICATIONBASE_HPP
#define FRONTENDAPPLICATIONBASE_HPP

#include <mvp/MVPApplication.hpp>
#include <gui/model/Model.hpp>

class FrontendHeap;

class FrontendApplicationBase : public touchgfx::MVPApplication
{
public:
    FrontendApplicationBase(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplicationBase() { }

    virtual void changeToStartScreen()
    {
        gotoMenuScreenNoTransition();
    }

    // Menu
    void gotoMenuScreenNoTransition();

    void gotoMenuScreenSlideTransitionNorth();

    // Osc
    void gotoOscScreenSlideTransitionSouth();

protected:
    touchgfx::Callback<FrontendApplicationBase> transitionCallback;
    FrontendHeap& frontendHeap;
    Model& model;

    // Menu
    void gotoMenuScreenNoTransitionImpl();

    void gotoMenuScreenSlideTransitionNorthImpl();

    // Osc
    void gotoOscScreenSlideTransitionSouthImpl();
};

#endif // FRONTENDAPPLICATIONBASE_HPP