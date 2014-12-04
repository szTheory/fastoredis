#include "gui/fasto_view.h"

#include <QToolButton>
#include <QAbstractItemModel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QKeyEvent>

#include "common/macros.h"

#include "gui/gui_factory.h"
#include "gui/fasto_editor.h"

namespace fastoredis
{
    FastoEditorView::FastoEditorView(const QString &delemitr, QWidget* parent)
        : QWidget(parent)
    {
        findPanel_ = new QFrame(this);
        findLine_ = new QLineEdit(this);
        close_ = new QToolButton(this);
        next_ = new QPushButton("Next", this);
        prev_ = new QPushButton("Previous", this);
        caseSensitive_ = new QCheckBox("Match case", this);

        close_->setIcon(GuiFactory::instance().close16Icon());
        close_->setToolButtonStyle(Qt::ToolButtonIconOnly);
        close_->setIconSize(QSize(16, 16));
        findLine_->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(6, 0, 6, 0);
        layout->setSpacing(4);
        layout->addWidget(close_);
        layout->addWidget(findLine_);
        layout->addWidget(next_);
        layout->addWidget(prev_);
        layout->addWidget(caseSensitive_);

        findPanel_->setFixedHeight(HeightFindPanel);
        findPanel_->setLayout(layout);

        QVBoxLayout *mainL = new QVBoxLayout;
        editor_ = new FastoEditorOutput(delemitr);

        jsonRadioButton_ = new QRadioButton;
        csvRadioButton_ = new QRadioButton;
        rawRadioButton_ = new QRadioButton;

        VERIFY(connect(jsonRadioButton_, SIGNAL(toggled(bool)), this, SLOT(viewChanged(bool))));
        VERIFY(connect(csvRadioButton_, SIGNAL(toggled(bool)), this, SLOT(viewChanged(bool))));
        VERIFY(connect(rawRadioButton_, SIGNAL(toggled(bool)), this, SLOT(viewChanged(bool))));

        QHBoxLayout* radLaout = new QHBoxLayout;
        radLaout->addWidget(jsonRadioButton_);
        radLaout->addWidget(csvRadioButton_);
        radLaout->addWidget(rawRadioButton_);

        mainL->addLayout(radLaout);
        mainL->addWidget(editor_);
        mainL->addWidget(findPanel_);
        setLayout(mainL);

        findPanel_->hide();

        VERIFY(connect(close_, SIGNAL(clicked()), findPanel_, SLOT(hide())));
        VERIFY(connect(next_, SIGNAL(clicked()), this, SLOT(goToNextElement())));
        VERIFY(connect(prev_, SIGNAL(clicked()), this, SLOT(goToPrevElement())));

        jsonRadioButton_->setChecked(true);
        retranslateUi();
    }

    void FastoEditorView::setModel(QAbstractItemModel* model)
    {
        editor_->setModel(model);
    }

    void FastoEditorView::viewChanged(bool checked)
    {
        if (!checked){
            return;
        }

        if(jsonRadioButton_->isChecked()){
            editor_->viewChanged(JSON);
            return;
        }

        if(csvRadioButton_->isChecked()){
            editor_->viewChanged(CSV);
            return;
        }

        if(rawRadioButton_->isChecked()){
            editor_->viewChanged(RAW);
            return;
        }
    }

    void FastoEditorView::setReadOnly(bool readOnly)
    {
        editor_->setReadOnly(readOnly);
    }

    void FastoEditorView::changeEvent(QEvent *e)
    {
        if(e->type() == QEvent::LanguageChange){
            retranslateUi();
        }

        QWidget::changeEvent(e);
    }

    void FastoEditorView::keyPressEvent(QKeyEvent *keyEvent)
    {
        bool isFocusScin = editor_->isActiveWindow();
        bool isShowFind = findPanel_->isVisible();
        if (Qt::Key_Escape == keyEvent->key() && isFocusScin && isShowFind) {
            findPanel_->hide();
            editor_->setFocus();
            return keyEvent->accept();
        }
        else if (Qt::Key_Return == keyEvent->key() && (keyEvent->modifiers() & Qt::ShiftModifier) && isFocusScin && isShowFind) {
            goToPrevElement();
        }
        else if (Qt::Key_Return == keyEvent->key() && isFocusScin && isShowFind) {
            goToNextElement();
        }
        else if (((keyEvent->modifiers() & Qt::ControlModifier) && keyEvent->key() == Qt::Key_F) && isFocusScin) {
            findPanel_->show();
            findPanel_->setFocus();
            //findPanel_->selectAll();
            return keyEvent->accept();
        }
        else {
            return QWidget::keyPressEvent(keyEvent);
        }
    }

    void FastoEditorView::goToNextElement()
    {
        findElement(true);
    }

    void FastoEditorView::goToPrevElement()
    {
        findElement(false);
    }

    void FastoEditorView::findElement(bool forward)
    {
        const QString &text = findLine_->text();
        if (!text.isEmpty()) {
            bool re = false;
            bool wo = false;
            bool looped = true;
            int index = 0;
            int line = 0;
            editor_->getCursorPosition(&line, &index);

            if (!forward)
               index -= editor_->selectedText().length();

            editor_->setCursorPosition(line, 0);
            bool isFounded = editor_->findFirst(text, re, caseSensitive_->checkState() == Qt::Checked, wo, looped, forward, line, index);

            if (isFounded) {
                editor_->ensureCursorVisible();
            }
            else {
                QMessageBox::warning(this, tr("Search"),tr("The specified text was not found."));
            }
        }
    }

    void FastoEditorView::retranslateUi()
    {
        jsonRadioButton_->setText(tr("Json"));
        csvRadioButton_->setText(tr("Csv"));
        rawRadioButton_->setText(tr("Raw text"));
    }
}