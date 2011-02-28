#ifdef CMAKE_BUILD
    #include "CubeMapDialog.moc"
#endif

#include "CubeMapDialog.h"

CubeMapDialog::CubeMapDialog(const CubeMapFile& file, QWidget *parent)
	: QDialog(parent)
{

	QGridLayout *main_layout = new QGridLayout(this);

	QSignalMapper* browseButtonMapper = new QSignalMapper(this);
	connect(browseButtonMapper, SIGNAL(mapped(int)), this, SLOT(setFilename(int)));

	for (int i = 0; i < 6; i++) {
		main_layout->addWidget(new QLabel(CubeMapFile::getSideTitle(i).c_str(), this), i, 0);
		directoryBox[i] = new QLineEdit(file.getSideFile(i).c_str(), this);
		main_layout->addWidget(directoryBox[i], i, 1);
		QPushButton* browseButton = new QPushButton("...", this);
		connect(browseButton, SIGNAL(clicked()), browseButtonMapper, SLOT(map()));
		browseButtonMapper->setMapping(browseButton, i);
		main_layout->addWidget(browseButton, i, 2);
	}

	QPushButton* okButton = new QPushButton("&Ok", this);
	QPushButton* cancelButton = new QPushButton("&Cancel", this);
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	main_layout->addWidget(okButton, 6, 0);
	main_layout->addWidget(cancelButton, 6, 1);

	setLayout(main_layout);
	setWindowTitle("Cube Map Files");

	directory = "";

}

CubeMapDialog::~CubeMapDialog()
{

}

CubeMapFile CubeMapDialog::showCubeMapDialog(const CubeMapFile& file, QWidget *parent) {
	CubeMapDialog dialog(file, parent);
	int ret = dialog.exec();
	if (ret)
		return dialog.getCubeMapFile();
	else
		return file;
}

CubeMapFile CubeMapDialog::getCubeMapFile() const {
	CubeMapFile file;
	for (int i = 0; i < 6; i++) {
		file.setSideFile(i, directoryBox[i]->displayText().toStdString());
	}
	return file;
}

void CubeMapDialog::setFilename(int side_index) {

	QString qfilename = QFileDialog::getOpenFileName (0,
		tr("Open Image File"),
		directory,
		"Image File (*.jpg *.png *.bmp *.gif *.tif)");

	if (!qfilename.isNull()) {
		directory = qfilename;
		directoryBox[side_index]->setText(qfilename);
	}

}