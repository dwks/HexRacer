#ifdef CMAKE_BUILD
    #include "ScaleAllDialog.moc"
#endif

#include "ScaleAllDialog.h"

ScaleAllDialog::ScaleAllDialog(QWidget *parent)
	: QDialog(parent)
{

	double range = 99999.9;
	double step = 0.5;
	int decimals = 5;

	scaleBox = new QDoubleSpinBox(this);
	scaleBox->setRange(-range, range);
	scaleBox->setSingleStep(step);
	scaleBox->setValue(1.0);
	scaleBox->setDecimals(decimals);

	originXBox = new QDoubleSpinBox(this);
	originXBox->setRange(-range, range);
	originXBox->setSingleStep(step);
	originXBox->setValue(0.0);
	originXBox->setDecimals(decimals);

	originYBox = new QDoubleSpinBox(this);
	originYBox->setRange(-range, range);
	originYBox->setSingleStep(step);
	originYBox->setValue(0.0);
	originYBox->setDecimals(decimals);

	originZBox = new QDoubleSpinBox(this);
	originZBox->setRange(-range, range);
	originZBox->setSingleStep(step);
	originZBox->setValue(0.0);
	originZBox->setDecimals(decimals);

	QGridLayout *main_layout = new QGridLayout(this);

	main_layout->addWidget(new QLabel("Scale:", this), 0, 0);
	main_layout->addWidget(scaleBox, 0, 1);

	main_layout->addWidget(new QLabel("Origin:", this), 1, 0);
	main_layout->addWidget(originXBox, 1, 1);
	main_layout->addWidget(originYBox, 1, 2);
	main_layout->addWidget(originZBox, 1, 3);

	QPushButton* okButton = new QPushButton("&Ok", this);
	QPushButton* cancelButton = new QPushButton("&Cancel", this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	main_layout->addWidget(okButton, 2, 0);
	main_layout->addWidget(cancelButton, 2, 1);

	setLayout(main_layout);
	setWindowTitle("Scale All");
}

ScaleAllDialog::~ScaleAllDialog()
{

}
