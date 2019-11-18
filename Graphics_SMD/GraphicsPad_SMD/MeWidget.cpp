#include "MeWidget.h"
#include <Qt\qdebug.h>
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <MeGlWindow.h>
#include <DebugSlider.h>


MeWidget::MeWidget()
{
	QVBoxLayout* mainLayout;
	setLayout(mainLayout = new QVBoxLayout);
	QVBoxLayout* controlsLayout;
	mainLayout->addLayout(controlsLayout = new QVBoxLayout);
	mainLayout->addWidget(new MeGlWindow);

	QHBoxLayout* lightPositionLayout;
	controlsLayout->addLayout(lightPositionLayout = new QHBoxLayout);
	lightPositionLayout->addWidget(lightXSlider = new DebugSlider);
	lightPositionLayout->addWidget(lightYSlider = new DebugSlider(3, 0));
	lightPositionLayout->addWidget(lightZSlider = new DebugSlider);

	connect(lightXSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(lightYSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(lightZSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));

	sliderValueChanged();
}

void MeWidget::sliderValueChanged()
{

}