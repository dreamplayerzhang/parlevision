#ifndef DUMMYPROCESSOR_H
#define DUMMYPROCESSOR_H

#include <QVariant>

#include "PipelineProcessor.h"
#include "Pin.h"
#include "Types.h"

namespace plv {

    class Pipeline;
    class OpenCVImage;

    class DummyProcessor : public PipelineProcessor
    {
        Q_OBJECT

        Q_CLASSINFO("author", "Michel & Richard")
        Q_CLASSINFO("name", "Dummy")
        Q_CLASSINFO("description", "A simple processor to demonstrate how to implement your own processors. "
                    "It doesn't do much, but flip your images.");

        Q_PROPERTY( int someInt READ getSomeInt WRITE setSomeInt NOTIFY someIntChanged  )
        Q_PROPERTY( double someDouble READ getSomeDouble WRITE setSomeDouble NOTIFY someDoubleChanged  )
        Q_PROPERTY( bool someBool READ getSomeBool WRITE setSomeBool NOTIFY someBoolChanged  )
        Q_PROPERTY( QString someString READ getSomeString WRITE setSomeString NOTIFY someStringChanged )
        Q_PROPERTY( int someVarWithNr1 READ getSomeVarWithNr1 WRITE setSomeVarWithNr1 NOTIFY someVarWithNr1Changed )
        Q_PROPERTY( int someVarWithNr2 READ getSomeVarWithNr2 WRITE setSomeVarWithNr2 NOTIFY someVarWithNr2Changed )
        Q_PROPERTY( plv::Enum customEnum READ getCustomEnum WRITE setCustomEnum NOTIFY customEnumChanged )

/******************************************************************************************/
        Q_PROPERTY( Priority priority READ getPriority WRITE setPriority NOTIFY priorityChanged )
        Q_ENUMS( Priority )

    public:
        enum Priority { High, Low, VeryHigh, VeryLow };
        Priority getPriority() const { return m_priority; }

    signals:
        void priorityChanged( Priority p );

    public slots:
        void setPriority( Priority p )
        {
            m_priority = p;
            emit( priorityChanged( p ) );
        }

/******************************************************************************************/

    public:
        plv::Enum getCustomEnum() const { return m_customEnum; }

    signals:
        void customEnumChanged( plv::Enum p );

    public slots:
        void setCustomEnum( plv::Enum c )
        {
            m_customEnum = c;
            emit( customEnumChanged( c ) );
            qDebug() << c.toString();
        }

/******************************************************************************************/
    public:
        DummyProcessor();
        ~DummyProcessor();

        virtual void init() throw (PipelineException);
        //virtual bool isBootstrapped() const;
        virtual bool isReadyForProcessing() const;
        virtual void process();

        /** propery methods */
        int getSomeInt() { return m_someInt; }
        double getSomeDouble() { return m_someDouble; }
        bool getSomeBool() { return m_someBool; }
        QString getSomeString() { return m_someString; }

        int getSomeVarWithNr1() { return m_someVarWithNr1; }
        int getSomeVarWithNr2() { return m_someVarWithNr2; }

    signals:
        void someIntChanged(int newValue);
        void someDoubleChanged(double newValue);
        void someBoolChanged(bool newValue);
        void someStringChanged(QString newValue);
        void someVarWithNr1Changed(int var);
        void someVarWithNr2Changed(int var);

    public slots:
        void setSomeInt(int i) {m_someInt = i; emit(someIntChanged(i));}
        void setSomeDouble(double d) {m_someDouble = d; emit(someDoubleChanged(d));}
        void setSomeBool(bool b) {m_someBool = b; emit(someBoolChanged(b));}
        void setSomeString(QString s) {m_someString = s; emit(someStringChanged(s));}
        void setSomeVarWithNr1(int var) { m_someVarWithNr1 = var; emit( someVarWithNr1Changed(var) ); }
        void setSomeVarWithNr2(int var) { m_someVarWithNr2 = var; emit( someVarWithNr2Changed(var) ); }

    private:
        InputPin<OpenCVImage>* m_inputPin;
        InputPin<OpenCVImage>* m_inputPinOptional;
        OutputPin<OpenCVImage>* m_outputPin;

        int m_someInt;
        double m_someDouble;
        bool m_someBool;
        QString m_someString;
        int m_someVarWithNr1;
        int m_someVarWithNr2;
        Priority m_priority;
        plv::Enum m_customEnum;
    };

}

#endif // DUMMYPROCESSOR_H
