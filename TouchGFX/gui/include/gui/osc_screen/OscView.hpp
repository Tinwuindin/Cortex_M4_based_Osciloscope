#ifndef OSCVIEW_HPP
#define OSCVIEW_HPP

#include <gui_generated/osc_screen/OscViewBase.hpp>
#include <gui/osc_screen/OscPresenter.hpp>
#include "stm32f4xx_hal.h"

class OscView : public OscViewBase
{
public:
    OscView();
    virtual ~OscView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
    int tickCounter;
    float dato;
    ADC_HandleTypeDef hadc1;
    void handleTickEvent();
};

#endif // OSCVIEW_HPP
