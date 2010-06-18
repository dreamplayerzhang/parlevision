#ifndef PIPELINESCENE_H
#define PIPELINESCENE_H

#include <QGraphicsScene>
#include "RefPtr.h"
#include "Pipeline.h"
#include "PipelineElement.h"


class QObject;

namespace plv {
}

namespace plvgui {

    /** A QGraphicsScene that wraps a Pipeline
      * to provide a graphical canvas for all the elements to live on.
      * This is only the model, it needs a QGraphicsView to render.
      */

    class PipelineScene : public QGraphicsScene
    {
        Q_OBJECT

    public:
        PipelineScene(plv::Pipeline* pipeline, QObject* parent);

    public slots:
        void add(plv::PipelineElement* e);
        void add(plv::RefPtr<plv::PipelineElement> e);

    private:
        plv::RefPtr<plv::Pipeline> m_pipeline;



    };

}

#endif // PIPELINESCENE_H