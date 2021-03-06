/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "RankFourAux.h"

template<>
InputParameters validParams<RankFourAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addClassDescription("Access a component of a RankFourTensor");

  //add stuff here
  params.addRequiredParam<MaterialPropertyName>("rank_four_tensor", "The rank four material tensor name");
  params.addRequiredRangeCheckedParam<unsigned int>("index_i", "index_i >= 0 & index_i <= 2", "The index i of ijkl for the tensor to output (0, 1, 2)");
  params.addRequiredRangeCheckedParam<unsigned int>("index_j", "index_j >= 0 & index_j <= 2", "The index j of ijkl for the tensor to output (0, 1, 2)");
  params.addRequiredRangeCheckedParam<unsigned int>("index_k", "index_k >= 0 & index_k <= 2", "The index k of ijkl for the tensor to output (0, 1, 2)");
  params.addRequiredRangeCheckedParam<unsigned int>("index_l", "index_l >= 0 & index_l <= 2", "The index l of ijkl for the tensor to output (0, 1, 2)");

  return params;
}

RankFourAux::RankFourAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _tensor(getMaterialProperty<ElasticityTensorR4>("rank_four_tensor")),
    _i(getParam<unsigned int>("index_i")),
    _j(getParam<unsigned int>("index_j")),
    _k(getParam<unsigned int>("index_k")),
    _l(getParam<unsigned int>("index_l"))
{
}

Real
RankFourAux::computeValue()
{
  return _tensor[_qp](_i, _j, _k, _l);
}

