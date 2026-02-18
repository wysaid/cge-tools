/*
 * filterAdjustWidgetBase.h
 *
 *  Template base class for filter adjustment widgets.
 *  Adapted from grafix's BasicAdjustCommonWidget pattern for the
 *  open-source CGE filter interface (CGEImageFilterInterfaceAbstract).
 *
 *  Author: Wang Yang
 *  Mail: admin@wysaid.org
 */

#pragma once

#include "mainwindow.h"
#include "cge.h"

// Template base class that unifies the common boilerplate across all
// filter adjustment widgets: UI setup, filter lifecycle (lazy creation
// in checkFilter(), automatic removal in destructor), and the factory
// macro for two-phase initialization.
//
// Template Parameters:
//   FilterType - The concrete CGE filter class (e.g. CGEBrightnessFilter)
//   QTUIType   - The generated Qt UI class (e.g. Ui::BrightnessAdjustWidget)
//
// Usage pattern:
//   class BrightnessAdjustWidget : public FilterAdjustWidgetBase<CGEBrightnessFilter, Ui::BrightnessAdjustWidget>
//   {
//       Q_OBJECT
//   public:
//       using FilterAdjustWidgetBase::FilterAdjustWidgetBase;
//       FILTER_ADJUST_CREATE_FUNC(BrightnessAdjustWidget);
//   protected:
//       bool init() override;
//       CGEImageFilterInterfaceAbstract* createFilter() override { return CGE::createBrightnessFilter(); }
//   ...
//   };
template <class FilterType, class QTUIType>
class FilterAdjustWidgetBase : public QWidget
{
public:
    explicit FilterAdjustWidgetBase(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr)
    {
        m_ui.setupUi(this);
    }

    ~FilterAdjustWidgetBase()
    {
        if (m_filter != nullptr)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

    // Optional two-phase initialization: subclass may override this to set up
    // signals/slots and validators when using the FILTER_ADJUST_CREATE_FUNC factory.
    // Default returns true. Subclasses that prefer constructor-based init can
    // simply call their own initXxx() methods from the constructor instead.
    virtual bool init() { return true; }

    // Subclass must override this to call the appropriate CGE factory function.
    virtual CGE::CGEImageFilterInterfaceAbstract* createFilter() = 0;

protected:
    // Lazily create the filter and append it to the handler.
    // Returns true if filter is valid.
    bool checkFilter()
    {
        if (m_filter == nullptr)
        {
            m_filter = static_cast<FilterType*>(createFilter());
            if (m_filter != nullptr)
            {
                m_mainWindow->appendFilter(m_filter);
            }
        }
        return m_filter != nullptr;
    }

    QTUIType m_ui;
    MainWindow* m_mainWindow;
    FilterType* m_filter;
};

// Factory macro for two-phase initialization pattern.
// Creates the widget and calls init(); deletes on failure.
#define FILTER_ADJUST_CREATE_FUNC(cls)                                 \
    static inline cls* create(MainWindow* mainWindow, QWidget* parent) \
    {                                                                  \
        cls* instance = new cls(mainWindow, parent);                   \
        if (!instance->init())                                         \
        {                                                              \
            delete instance;                                           \
            instance = nullptr;                                        \
            CGE_LOG_ERROR("create " #cls " failed!");                  \
        }                                                              \
        return instance;                                               \
    }
