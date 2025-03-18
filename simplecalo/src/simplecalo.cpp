//**************************************************************************
// \file simplecalo.cpp
// \brief:  Implementation of simple sandwich calorimeter DD4hep geometry
// \author: Lorenzo Pezzotti
// \date:   March 2025
//**************************************************************************

// Includers from DD4hep
#include "DDRec/Vector3D.h"
#include <DD4hep/DetFactoryHelper.h>

using namespace dd4hep;
using namespace dd4hep::rec; // for dd4hep::rec::Vector3D

// Includers from project files
// #include "simplecalo.hh"

// Build simple calo geometry
//
static Ref_t create_detector(Detector &description, xml_h e,
                             SensitiveDetector sens) {
  std::cout << "--> simplecalo::create_detector() start" << std::endl;

  // Get info from the xml file
  //
  sens.setType("calorimeter");
  xml_det_t x_det = e;
  std::string det_name = x_det.nameStr();
  std::cout << "--> Going to create " << det_name << ", with ID: " << x_det.id()
            << std::endl;
  xml_dim_t x_dim = x_det.dimensions();

  const double CaloX = x_dim.x();
  const double CaloY = x_dim.y();
  const double CaloZ = x_dim.z();
  std::cout << "--> calo dimensions from XML description: x " << CaloX / m
            << " m, y " << CaloY / m << " m, z " << CaloZ / m << " m"
            << std::endl;

  // Info for subdetectors
  //
  xml_det_t x_calo = x_det.child(_Unicode(calo));
  xml_det_t x_calolayer = x_det.child(_Unicode(caloLayer));
  xml_det_t x_abslayer = x_det.child(_Unicode(absLayer));
  xml_det_t x_senslayer = x_det.child(_Unicode(sensLayer));
  xml_det_t x_cell = x_det.child(_Unicode(cell));

  auto iscellsens = x_cell.isSensitive();

  const double CaloLayerX = x_calolayer.x();
  const double CaloLayerY = x_calolayer.y();
  const double CaloLayerZ = x_calolayer.z();

  const double AbsLayerX = x_abslayer.x();
  const double AbsLayerY = x_abslayer.y();
  const double AbsLayerZ = x_abslayer.z();

  const double SensLayerX = x_senslayer.x();
  const double SensLayerY = x_senslayer.y();
  const double SensLayerZ = x_senslayer.z();

  const double CellX = x_cell.x();
  const double CellY = x_cell.y();
  const double CellZ = x_cell.z();

  // Create the geometry
  //

  // Create a container for the calorimeter
  //
  Box Calo(CaloX / 2., CaloY / 2., CaloZ / 2.);
  Volume CaloVol("CaloVol", Calo,
                 description.material(x_calo.attr<std::string>(_U(material))));
  CaloVol.setVisAttributes(description, x_calo.visStr());

  // Create a container for a calorimeter layer
  // (absorber + active elements)
  //
  Box CaloLayer(CaloLayerX / 2., CaloLayerY / 2., CaloLayerZ / 2.);
  Volume CaloLayerVol(
      "CaloLayerVol", CaloLayer,
      description.material(x_calolayer.attr<std::string>(_U(material))));
  CaloLayerVol.setVisAttributes(description, x_calolayer.visStr());

  // Place twenty calorimeter layers inside the container
  //
  for (std::size_t i = 2; i < 20; i++) {
    CaloVol.placeVolume(
        CaloLayerVol, i,
        Position(0., 0., -CaloZ / 2. + CaloLayerZ / 2. + i * CaloLayerZ));
  }

  // Place an absorber layer inside the calorimeter layer
  //
  Box AbsLayer(AbsLayerX / 2., AbsLayerY / 2., AbsLayerZ / 2.);
  Volume AbsLayerVol(
      "AbsLayerVol", AbsLayer,
      description.material(x_abslayer.attr<std::string>(_U(material))));
  AbsLayerVol.setVisAttributes(description, x_abslayer.visStr());
  CaloLayerVol.placeVolume(AbsLayerVol, 1,
                           Position(0., 0., -CaloLayerZ / 2. + AbsLayerZ / 2.));

  // Place an active layer inside the calorimeter layer
  //
  Box SensLayer(SensLayerX / 2., SensLayerY / 2., SensLayerZ / 2.);
  Volume SensLayerVol(
      "SensLayerVol", SensLayer,
      description.material(x_senslayer.attr<std::string>(_U(material))));
  SensLayerVol.setVisAttributes(description, x_senslayer.visStr());
  CaloLayerVol.placeVolume(SensLayerVol, 1,
                           Position(0., 0., CaloLayerZ / 2. - SensLayerZ / 2.));

  // Place many active cells (pixels) inside the calorimeter layer
  //
  Box Cell(CellX / 2., CellY / 2., CellZ / 2.);
  Volume CellVol("CellVol", Cell,
                 description.material(x_cell.attr<std::string>(_U(material))));
  CellVol.setVisAttributes(description, x_cell.visStr());
  double x, y = 0;
  for (std::size_t i = 0; i < 10; i++) {
    y = SensLayerY / 2. - CellY / 2. - i * CellY;
    for (std::size_t j = 0; j < 10; j++) {
      x = -SensLayerX / 2. + CellX / 2. + j * CellX;
      SensLayerVol.placeVolume(CellVol, 10 * i + j, Position(x, y, 0.));
    }
  }

  // Finalize geometry
  //
  DetElement subdet(det_name, x_det.id());
  Volume motherVolume = description.pickMotherVolume(subdet);
  // Place the calo container inside the mother volume
  PlacedVolume CaloPlaced = motherVolume.placeVolume(CaloVol);
  subdet.setPlacement(CaloPlaced);

  std::cout << "--> simplecalo::create_detector() end" << std::endl;
  return subdet;
}

DECLARE_DETELEMENT(simplecalo, create_detector)

//**************************************************************************
