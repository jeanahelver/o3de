/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <GemCatalog/GemRequirementDialog.h>
#include <GemCatalog/GemRequirementListView.h>
#include <GemCatalog/GemRequirementFilterProxyModel.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSpacerItem>

namespace O3DE::ProjectManager
{
    GemRequirementDialog::GemRequirementDialog(GemModel* model, const QVector<QModelIndex>& gemsToAdd, QWidget* parent)
        : QDialog(parent)
    {
        setWindowTitle(tr("Manual setup is required"));
        setModal(true);

        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setMargin(0);
        vLayout->setContentsMargins(25, 10, 25, 10);
        vLayout->setSizeConstraint(QLayout::SetFixedSize);
        setLayout(vLayout);

        QHBoxLayout* instructionLayout = new QHBoxLayout();
        instructionLayout->setMargin(0);

        QLabel* instructionIconLabel = new QLabel();
        instructionIconLabel->setPixmap(QIcon(":/Warning.svg").pixmap(32, 32));
        instructionLayout->addWidget(instructionIconLabel);

        instructionLayout->addSpacing(10);

        QLabel* instructionLabel = new QLabel(tr("The following Gem(s) require manual setup before the project can be built successfully."));
        instructionLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        instructionLayout->addWidget(instructionLabel);

        QSpacerItem* instructionSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        instructionLayout->addSpacerItem(instructionSpacer);

        vLayout->addLayout(instructionLayout);

        vLayout->addSpacing(20);

        GemRequirementFilterProxyModel* proxModel = new GemRequirementFilterProxyModel(model, gemsToAdd, this);

        GemRequirementListView* m_gemListView = new GemRequirementListView(proxModel, proxModel->GetSelectionModel(), this);
        vLayout->addWidget(m_gemListView);

        QDialogButtonBox* dialogButtons = new QDialogButtonBox();
        dialogButtons->setObjectName("footer");
        vLayout->addWidget(dialogButtons);

        QPushButton* cancelButton = dialogButtons->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
        cancelButton->setProperty("secondary", true);
        QPushButton* continueButton = dialogButtons->addButton(tr("Continue"), QDialogButtonBox::ApplyRole);

        connect(cancelButton, &QPushButton::clicked, this, &GemRequirementDialog::CancelButtonPressed);
        connect(continueButton, &QPushButton::clicked, this, &GemRequirementDialog::ContinueButtonPressed);
    }

    QDialogButtonBox::ButtonRole GemRequirementDialog::GetButtonResult()
    {
        return m_buttonResult;
    }

    void GemRequirementDialog::CancelButtonPressed()
    {
        m_buttonResult = QDialogButtonBox::RejectRole;
        close();
    }

    void GemRequirementDialog::ContinueButtonPressed()
    {
        m_buttonResult = QDialogButtonBox::ApplyRole;
        close();
    }

} // namespace O3DE::ProjectManager
