#ifndef OSCPRESENTER_HPP
#define OSCPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class OscView;

class OscPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    OscPresenter(OscView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~OscPresenter() {}

private:
    OscPresenter();

    OscView& view;
};

#endif // OSCPRESENTER_HPP
