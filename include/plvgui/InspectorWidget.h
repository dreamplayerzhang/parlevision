#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <QDockWidget>
#include "RefPtr.h"
#include "PipelineElement.h"


class QFormLayout;

namespace Ui
{
    class InspectorWidget;
}

namespace plv
{
//    class PipelineElement;
}

using namespace plv;

namespace plvgui
{
    class InspectorWidget : public QDockWidget
    {
        Q_OBJECT

    public:
        explicit InspectorWidget(QWidget *parent = 0);
        ~InspectorWidget();

    public slots:
        /** set the currently selected target to this one
          * this automatically unselects any previous selections.
          */
        void setTarget(plv::RefPtr<plv::PipelineElement> element);
        void nothingSelected();
        void multipleSelected();

    protected:
        void changeEvent(QEvent *e);

    private:
        void clearSelection();
        void addRow(QFormLayout* form, RefPtr<PipelineElement> element, QString* name, QVariant* value);
        void addRow(QFormLayout* form, RefPtr<PipelineElement> element, QString* name, int value);
        void addRow(QFormLayout* form, RefPtr<PipelineElement> element, QString* name, double value);
        void addRow(QFormLayout* form, RefPtr<PipelineElement> element, QString* name, bool value);
        void addRow(QFormLayout* form, RefPtr<PipelineElement> element, QString* name, QString value, bool editable=true);
        Ui::InspectorWidget *ui;
        RefPtr<PipelineElement> element;
        QWidget* formContainer;

        const QString propertySlotSignature(QObject* obj, QString property);
    };
}
#endif // INSPECTORWIDGET_H