#ifdef CMAKE_BUILD
    #include "TranslateAllDialog.moc"
#endif
#include "TranslateAllDialog.h"

TranslateAllDialog::TranslateAllDialog(QWidget *parent)
	: QDialog(parent)
{
	double range = 99999.9;
	double step = 0.5;
	int decimals = 5;

	xBox = new QDoubleSpinBox(this);
	xBox->setRange(-range, range);
	xBox->setSingleStep(step);
	xBox->setValue(0.0);
	xBox->setDecimals(decimals);

	yBox = new QDoubleSpinBox(this);
	yBox->setRange(-range, range);
	yBox->setSingleStep(step);
	yBox->setValue(0.0);
	yBox->setDecimals(decimals);

	zBox = new QDoubleSpinBox(this);
	zBox->setRange(-range, range);
	zBox->setSingleStep(step);
	zBox->setValue(0.0);
	zBox->setDecimals(decimals);

	QGridLayout *main_layout = new QGridLayout(this);

	main_layout->addWidget(new QLabel("Translation:", this), 0, 0);
	main_layout->addWidget(new QLabel("x:", this), 1, 0);
	main_layout->addWidget(new QLabel("y:", this), 2, 0);
	main_layout->addWidget(new QLabel("z:", this), 3, 0);
	main_layout->addWidget(xBox, 1, 1);
	main_layout->addWidget(yBox, 2, 1);
	main_layout->addWidget(zBox, 3, 1);

	QPushButton* okButton = new QPushButton("&Ok", this);
	QPushButton* cancelButton = new QPushButton("&Cancel", this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	main_layout->addWidget(okButton, 4, 0);
	main_layout->addWidget(cancelButton, 4, 1);

	setLayout(main_layout);
	setWindowTitle("Translate All");
}

TranslateAllDialog::~TranslateAllDialog()
{

}
