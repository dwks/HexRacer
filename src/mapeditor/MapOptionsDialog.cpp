#ifdef CMAKE_BUILD
    #include "MapOptionsDialog.moc"
#endif

#include "MapOptionsDialog.h"

MapOptionsDialog::MapOptionsDialog(const MapOptions& options, QWidget *parent)
	: QDialog(parent)
{
	bgBloomBox = new QCheckBox(this);
	bgBloomBox->setChecked(options.getBGBloomEnable());

	numLapsBox = new QSpinBox(this);
	numLapsBox->setMinimum(1);
	//numLapsBox->setValue(options.getNumLaps());

	QPushButton* okButton = new QPushButton("&Ok", this);
	QPushButton* cancelButton = new QPushButton("&Cancel", this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout* layout = new QGridLayout(this);
	layout->addWidget(new QLabel(QString("Background Bloom:"), this), 0, 0);
	layout->addWidget(bgBloomBox, 0, 1);
	/*layout->addWidget(new QLabel(QString("Number of Laps:"), this), 1, 0);
	layout->addWidget(numLapsBox, 1, 1);*/
	layout->addWidget(okButton, 2, 0);
	layout->addWidget(cancelButton, 2, 1);

	setWindowTitle("Map Options");
	setLayout(layout);
}

MapOptionsDialog::~MapOptionsDialog()
{

}

void MapOptionsDialog::showOptionsDialog(MapOptions& options, QWidget *parent) {
	MapOptionsDialog dialog(options, parent);
	if (dialog.exec()) {
		options.setBGBloomEnable(dialog.bgBloomBox->isChecked());
		//options.setNumLaps(dialog.numLapsBox->value());
	}
}