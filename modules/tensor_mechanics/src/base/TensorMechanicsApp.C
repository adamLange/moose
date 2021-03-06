/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "TensorMechanicsApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "TensorMechanicsAction.h"
#include "DynamicTensorMechanicsAction.h"
#include "TensorMechanicsAxisymmetricRZAction.h"
#include "PoroMechanicsAction.h"
#include "PressureAction.h"

#include "StressDivergenceTensors.h"
#include "CosseratStressDivergenceTensors.h"
#include "StressDivergenceRZTensors.h"
#include "MomentBalancing.h"
#include "PoroMechanicsCoupling.h"
#include "InertialForce.h"
#include "Gravity.h"
#include "DynamicStressDivergenceTensors.h"

#include "LinearElasticMaterial.h"
#include "FiniteStrainElasticMaterial.h"
#include "FiniteStrainPlasticMaterial.h"
#include "FiniteStrainRatePlasticMaterial.h"
#include "FiniteStrainMohrCoulomb.h"
#include "FiniteStrainCrystalPlasticity.h"
#include "ComputeMultiPlasticityStress.h"
#include "CosseratLinearElasticMaterial.h"
#include "ElementPropertyReadFileTest.h"
#include "TwoPhaseStressMaterial.h"
#include "MultiPhaseStressMaterial.h"
#include "SimpleEigenStrainMaterial.h"
#include "ComputeElasticityTensor.h"
#include "ComputeIsotropicElasticityTensor.h"
#include "ComputeSmallStrain.h"
#include "ComputePlaneSmallStrain.h"
#include "ComputePlaneFiniteStrain.h"
#include "ComputeAxisymmetricRZSmallStrain.h"
#include "ComputeIncrementalSmallStrain.h"
#include "ComputeFiniteStrain.h"
#include "ComputeAxisymmetricRZFiniteStrain.h"
#include "ComputeLinearElasticStress.h"
#include "ComputeFiniteStrainElasticStress.h"
#include "ComputeEigenstrain.h"
#include "ComputeVariableEigenstrain.h"
#include "ComputeConcentrationDependentElasticityTensor.h"
#include "FiniteStrainHyperElasticViscoPlastic.h"
#include "LinearIsoElasticPFDamage.h"

#include "TensorMechanicsPlasticSimpleTester.h"
#include "TensorMechanicsPlasticTensile.h"
#include "TensorMechanicsPlasticTensileMulti.h"
#include "TensorMechanicsPlasticMohrCoulomb.h"
#include "TensorMechanicsPlasticMohrCoulombMulti.h"
#include "TensorMechanicsPlasticWeakPlaneTensile.h"
#include "TensorMechanicsPlasticWeakPlaneTensileN.h"
#include "TensorMechanicsPlasticWeakPlaneShear.h"
#include "TensorMechanicsPlasticJ2.h"
#include "TensorMechanicsHardeningConstant.h"
#include "TensorMechanicsHardeningGaussian.h"
#include "TensorMechanicsHardeningExponential.h"
#include "TensorMechanicsHardeningCutExponential.h"
#include "TensorMechanicsHardeningCubic.h"
#include "ElementPropertyReadFile.h"
#include "HyperElasticStress.h"
#include "FlowRateModel.h"
#include "RambergOsgoodHardening.h"
#include "EulerAngleFileReader.h"

#include "RankTwoAux.h"
#include "RankFourAux.h"
#include "TensorElasticEnergyAux.h"
#include "CrystalPlasticityRotationOutAux.h"
#include "RankTwoScalarAux.h"
#include "StressDivergencePFFracTensors.h"
#include "NewmarkAccelAux.h"
#include "NewmarkVelAux.h"

#include "Pressure.h"

template<>
InputParameters validParams<TensorMechanicsApp>()
{
  InputParameters params = validParams<MooseApp>();
  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  params.set<bool>("use_legacy_output_syntax") = false;
  return params;
}

TensorMechanicsApp::TensorMechanicsApp(const InputParameters & parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  TensorMechanicsApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
}

TensorMechanicsApp::~TensorMechanicsApp()
{
}

// External entry point for dynamic application loading
extern "C" void TensorMechanicsApp_registerApps() { TensorMechanicsApp::registerApps(); }
void
TensorMechanicsApp::registerApps()
{
  registerApp(TensorMechanicsApp);
}

// External entry point for dynamic object registration
extern "C" void TensorMechanicsApp__registerObjects(Factory & factory) { TensorMechanicsApp::registerObjects(factory); }
void
TensorMechanicsApp::registerObjects(Factory & factory)
{
  registerKernel(StressDivergenceTensors);
  registerKernel(CosseratStressDivergenceTensors);
  registerKernel(StressDivergenceRZTensors);
  registerKernel(MomentBalancing);
  registerKernel(StressDivergencePFFracTensors);
  registerKernel(PoroMechanicsCoupling);
  registerKernel(InertialForce);
  registerKernel(Gravity);
  registerKernel(DynamicStressDivergenceTensors);

  registerMaterial(LinearElasticMaterial);
  registerMaterial(FiniteStrainElasticMaterial);
  registerMaterial(FiniteStrainPlasticMaterial);
  registerMaterial(FiniteStrainMohrCoulomb);
  registerMaterial(FiniteStrainRatePlasticMaterial);
  registerMaterial(FiniteStrainCrystalPlasticity);
  registerMaterial(ComputeMultiPlasticityStress);
  registerMaterial(CosseratLinearElasticMaterial);
  registerMaterial(ElementPropertyReadFileTest);
  registerMaterial(TwoPhaseStressMaterial);
  registerMaterial(MultiPhaseStressMaterial);
  registerMaterial(SimpleEigenStrainMaterial);
  registerMaterial(ComputeElasticityTensor);
  registerMaterial(ComputeIsotropicElasticityTensor);
  registerMaterial(ComputeSmallStrain);
  registerMaterial(ComputePlaneSmallStrain);
  registerMaterial(ComputePlaneFiniteStrain);
  registerMaterial(ComputeAxisymmetricRZSmallStrain);
  registerMaterial(ComputeIncrementalSmallStrain);
  registerMaterial(ComputeFiniteStrain);
  registerMaterial(ComputeAxisymmetricRZFiniteStrain);
  registerMaterial(ComputeLinearElasticStress);
  registerMaterial(ComputeFiniteStrainElasticStress);
  registerMaterial(ComputeEigenstrain);
  registerMaterial(ComputeVariableEigenstrain);
  registerMaterial(ComputeConcentrationDependentElasticityTensor);
  registerMaterial(FiniteStrainHyperElasticViscoPlastic);
  registerMaterial(LinearIsoElasticPFDamage);

  registerUserObject(TensorMechanicsPlasticSimpleTester);
  registerUserObject(TensorMechanicsPlasticTensile);
  registerUserObject(TensorMechanicsPlasticTensileMulti);
  registerUserObject(TensorMechanicsPlasticMohrCoulomb);
  registerUserObject(TensorMechanicsPlasticMohrCoulombMulti);
  registerUserObject(TensorMechanicsPlasticWeakPlaneTensile);
  registerUserObject(TensorMechanicsPlasticWeakPlaneTensileN);
  registerUserObject(TensorMechanicsPlasticWeakPlaneShear);
  registerUserObject(TensorMechanicsPlasticJ2);
  registerUserObject(TensorMechanicsHardeningConstant);
  registerUserObject(TensorMechanicsHardeningGaussian);
  registerUserObject(TensorMechanicsHardeningExponential);
  registerUserObject(TensorMechanicsHardeningCutExponential);
  registerUserObject(TensorMechanicsHardeningCubic);
  registerUserObject(ElementPropertyReadFile);
  registerUserObject(HyperElasticStress);
  registerUserObject(FlowRateModel);
  registerUserObject(RambergOsgoodHardening);
  registerUserObject(EulerAngleFileReader);

  registerAux(RankTwoAux);
  registerAux(RankFourAux);
  registerAux(TensorElasticEnergyAux);
  registerAux(CrystalPlasticityRotationOutAux);
  registerAux(RankTwoScalarAux);
  registerAux(NewmarkAccelAux);
  registerAux(NewmarkVelAux);

  registerBoundaryCondition(Pressure);
}

// External entry point for dynamic syntax association
extern "C" void TensorMechanicsApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { TensorMechanicsApp::associateSyntax(syntax, action_factory); }
void
TensorMechanicsApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  syntax.registerActionSyntax("TensorMechanicsAction", "Kernels/TensorMechanics");
  syntax.registerActionSyntax("DynamicTensorMechanicsAction", "Kernels/DynamicTensorMechanics");
  syntax.registerActionSyntax("PoroMechanicsAction", "Kernels/PoroMechanics");
  syntax.registerActionSyntax("TensorMechanicsAxisymmetricRZAction", "Kernels/AxisymmetricRZ");

  syntax.registerActionSyntax("EmptyAction", "BCs/Pressure");
  syntax.registerActionSyntax("PressureAction", "BCs/Pressure/*");

  registerAction(TensorMechanicsAction, "add_kernel");
  registerAction(DynamicTensorMechanicsAction, "add_kernel");
  registerAction(PoroMechanicsAction, "add_kernel");
  registerAction(TensorMechanicsAxisymmetricRZAction, "add_kernel");
  registerAction(PressureAction, "add_bc");
}

