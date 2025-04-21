#include <QCoreApplication>
#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <QFileDialog>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QCheckBox>

class QEMULauncher : public QWidget {
    Q_OBJECT

public:
    QEMULauncher(QWidget *parent = nullptr) : QWidget(parent) {
        // UI setup
        isoPathLabel = new QLabel("Selecciona una imagen ISO:");
        memoryLabel = new QLabel("Selecciona la cantidad de memoria:");
        coresLabel = new QLabel("Selecciona el número de núcleos:");
        networkLabel = new QLabel("Activar red:");

        isoPathLineEdit = new QPushButton("Abrir ISO", this);
        memoryComboBox = new QComboBox(this);
        memoryComboBox->addItem("2 GB", "2G");
        memoryComboBox->addItem("4 GB", "4G");
        memoryComboBox->addItem("8 GB", "8G");
        memoryComboBox->addItem("16 GB", "16G");

        coresComboBox = new QComboBox(this);
        coresComboBox->addItem("1 Núcleo", "1");
        coresComboBox->addItem("2 Núcleos", "2");
        coresComboBox->addItem("4 Núcleos", "4");
        coresComboBox->addItem("8 Núcleos", "8");
        coresComboBox->addItem("16 Núcleos", "16");

        networkCheckBox = new QCheckBox("Activar red", this);
        networkCheckBox->setChecked(true); // Red activada por defecto

        launchButton = new QPushButton("Lanzar QEMU", this);
        exitButton = new QPushButton("Salir", this);

        // Layout setup
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(isoPathLabel);
        layout->addWidget(isoPathLineEdit);
        layout->addWidget(memoryLabel);
        layout->addWidget(memoryComboBox);
        layout->addWidget(coresLabel);
        layout->addWidget(coresComboBox);
        layout->addWidget(networkLabel);
        layout->addWidget(networkCheckBox);
        layout->addWidget(launchButton);
        layout->addWidget(exitButton);

        setLayout(layout);

        // Connect button signals
        connect(isoPathLineEdit, &QPushButton::clicked, this, &QEMULauncher::selectISO);
        connect(launchButton, &QPushButton::clicked, this, &QEMULauncher::launchQEMU);
        connect(exitButton, &QPushButton::clicked, this, &QEMULauncher::close);  // Conectar el botón de salir
    }

private slots:
    void selectISO() {
        // Mostrar un diálogo para seleccionar la imagen ISO
        isoPath = QFileDialog::getOpenFileName(this, "Selecciona una imagen ISO", "", "ISO Files (*.iso)");
        if (!isoPath.isEmpty()) {
            isoPathLineEdit->setText(isoPath);
        }
    }

    void launchQEMU() {
        if (isoPath.isEmpty()) {
            qDebug() << "Por favor, selecciona una imagen ISO primero.";
            return;
        }

        // Obtener la cantidad de memoria seleccionada
        QString memoryAmount = memoryComboBox->currentData().toString();

        // Obtener el número de núcleos seleccionados
        QString numCores = coresComboBox->currentData().toString();

        // Configurar los argumentos para QEMU
        QStringList args;
        args << "-cdrom" << isoPath
             << "-m" << memoryAmount
             << "-smp" << numCores  // Agregar el número de núcleos al comando
             << "-net" << "nic";

        // Solo agregar la red si el checkbox está marcado
        if (networkCheckBox->isChecked()) {
            args << "-net" << "user";
        }

        // Ejecutar QEMU sin bloquear el hilo principal
        QProcess::startDetached("qemu-kvm", args);

        qDebug() << "QEMU lanzado con éxito.";
    }

private:
    QPushButton *isoPathLineEdit;
    QComboBox *memoryComboBox;
    QComboBox *coresComboBox;
    QCheckBox *networkCheckBox;
    QPushButton *launchButton;
    QPushButton *exitButton;
    QLabel *isoPathLabel;
    QLabel *memoryLabel;
    QLabel *coresLabel;
    QLabel *networkLabel;
    QString isoPath;  // Variable para almacenar la ruta de la ISO
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Crear la ventana principal
    QEMULauncher w;
    w.setWindowTitle("QEMU Launcher");
    w.show();

    return a.exec();
}

#include "main.moc"
