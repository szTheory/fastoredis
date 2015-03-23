#include "gui/dialogs/python_console_dialog.h"

#include "common/qt/convert_string.h"

#include "gui/gui_factory.h"
#include "translations/global.h"

#include "shell/python_shell.h"
#include "core/python_engine.h"

#include "core/settings_manager.h"

namespace fastoredis
{
    namespace
    {
        FastoEditorShell* shellCreate()
        {
            if(PythonEngine::instance().hasModule("redis-py")){
                return new RedisPyShell(SettingsManager::instance().autoCompletion());
            }
            else{
                return new PythonShell(SettingsManager::instance().autoCompletion());
            }
        }
    }

    PythonConsoleDialog::PythonConsoleDialog(const QString& filePath, QWidget* parent)
        : BaseConsoleDialog(filePath, parent, GuiFactory::instance().pythonIcon(), isPythonEnabled(), shellCreate(), "py"), worker_(NULL)
    {
        using namespace translations;
        worker_ = PythonEngine::instance().createWorker();

        VERIFY(connect(worker_, &PythonWorker::pythonStdOut, output_, &FastoEditor::append, Qt::QueuedConnection));
        VERIFY(connect(worker_, &PythonWorker::pythonStdErr, output_, &FastoEditor::append, Qt::QueuedConnection));
        VERIFY(connect(worker_, &PythonWorker::executeProgress, this, &PythonConsoleDialog::executeProgressChanged));

        retranslateUi();
    }

    PythonConsoleDialog::~PythonConsoleDialog()
    {
        delete worker_;
    }

    void PythonConsoleDialog::retranslateUi()
    {
        using namespace translations;
        setWindowTitle(trPythonConsole);
        BaseConsoleDialog::retranslateUi();
    }

    void PythonConsoleDialog::loadAndInstallFileImpl(const QString& path)
    {
        if(loadFromFile(path)){
            worker_->executeScript(filePath_, QStringList() << "install");
        }
    }

    void PythonConsoleDialog::executeImpl(const QString& script, const QStringList &args)
    {
        worker_->execute(script, args);
    }

    void PythonConsoleDialog::stopImpl()
    {
    }
}
