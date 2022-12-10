#include "mainwindow.h"

#include "initialization/meshinitializer.h"
#include "initialization/objfile.h"
#include "subdivision/loopsubdivider.h"
#include "ui_mainwindow.h"
#include "settings.h"


/**
 * @brief MainWindow::MainWindow Creates a new Main Window UI.
 * @param parent Qt parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->MeshGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
  ui->IsophotesGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
  ui->RendererGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
  //ui->MeshGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
}

/**
 * @brief MainWindow::~MainWindow Deconstructs the main window.
 */
MainWindow::~MainWindow() {
  delete ui;
  meshes.clear();
  meshes.squeeze();
}

/**
 * @brief MainWindow::importOBJ Imports an obj file and adds the constructed
 * half-edge to the collection of meshes.
 * @param fileName Path of the .obj file.
 */
void MainWindow::importOBJ(const QString& fileName) {
  OBJFile newModel = OBJFile(fileName);
  meshes.clear();
  meshes.squeeze();

  if (newModel.loadedSuccessfully()) {
    MeshInitializer meshInitializer;
    meshes.append(meshInitializer.constructHalfEdgeMesh(newModel));
    ui->MainDisplay->updateBuffers(meshes[0]);
    ui->MainDisplay->settings.modelLoaded = true;
    ui->MainDisplay->settings.renderBasicModel = true;

  }
  else {
    ui->MainDisplay->settings.modelLoaded = false;
  }
  if (ui->MainDisplay->settings.phongShadingRender || ui->MainDisplay->settings.isophotesRender){
    ui->MeshGroupBox->setEnabled(true);
  }
  ui->IsophotesGroupBox->setEnabled(ui->MainDisplay->settings.isophotesRender);
  ui->RendererGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
  ui->SubdivSteps->setValue(0);
  ui->frequencySteps->setValue(0);
  ui->MainDisplay->update();
}

// Don't worry about adding documentation for the UI-related functions.

void MainWindow::on_LoadOBJ_pressed() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Import OBJ File", "../", tr("Obj Files (*.obj)"));
  importOBJ(filename);
}

void MainWindow::on_MeshPresetComboBox_currentTextChanged(
    const QString& meshName) {
  importOBJ(":/models/" + meshName + ".obj");
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
  Subdivider* subdivider = new LoopSubdivider();
  for (int k = meshes.size() - 1; k < value; k++) {
    meshes.append(subdivider->subdivide(meshes[k]));
  }
  ui->MainDisplay->updateBuffers(meshes[value]);
  delete subdivider;
}

void MainWindow::on_phongShadingCheckBox_toggled(bool checkedPhong){
    ui->MainDisplay->settings.phongShadingRender = checkedPhong;
    if (!checkedPhong){
        importOBJ(":/models/" + ui->MeshPresetComboBox->currentText() + ".obj");
    }
    if (ui->MainDisplay->settings.phongShadingRender || ui->MainDisplay->settings.isophotesRender){
      ui->MeshGroupBox->setEnabled(true);
    }
    else{
        ui->MeshGroupBox->setEnabled(false);
    }
    update();
}


void MainWindow::on_isophotesCheckBox_toggled(bool checkedIsophote){
    ui->MainDisplay->settings.isophotesRender = checkedIsophote;
    ui->IsophotesGroupBox->setEnabled(checkedIsophote);
    if (!checkedIsophote){
        importOBJ(":/models/" + ui->MeshPresetComboBox->currentText() + ".obj");
    }
    else{
        ui->MainDisplay->settings.uniformUpdateRequired = true;

    }
    if (ui->MainDisplay->settings.phongShadingRender || ui->MainDisplay->settings.isophotesRender){
      ui->MeshGroupBox->setEnabled(true);
    }
    else{
        ui->MeshGroupBox->setEnabled(false);
    }
    int valueSubDivision = ui->SubdivSteps->value();
    ui->MainDisplay->updateBuffers(meshes[valueSubDivision]);
    update();
}


void MainWindow::on_frequencySteps_valueChanged(int freq){
    ui->MainDisplay->settings.frequencyIsophotes = freq;
    ui->MainDisplay->settings.uniformUpdateRequired = true;
    int valueSubDivision = ui->SubdivSteps->value();
    ui->MainDisplay->updateBuffers(meshes[valueSubDivision]);
    update();
}


void MainWindow::on_colorStripesComboBox_currentTextChanged(
        const QString &colorStripes){
    if(colorStripes == "Black & White"){
       ui->MainDisplay->settings.colorStripeCode=0;
    }
    else if(colorStripes == "Red & White"){
        ui->MainDisplay->settings.colorStripeCode=1;
    }
    else if(colorStripes == "Blue & White"){
       ui->MainDisplay->settings.colorStripeCode=2;
    }
    ui->MainDisplay->settings.uniformUpdateRequired = true;
    int valueSubDivision = ui->SubdivSteps->value();
    ui->MainDisplay->updateBuffers(meshes[valueSubDivision]);
    update();
    update();

}

