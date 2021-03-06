/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "AddPostprocessorAction.h"
#include "Factory.h"
#include "FEProblem.h"

template<>
InputParameters validParams<AddPostprocessorAction>()
{
  InputParameters params = validParams<MooseObjectAction>();
  return params;
}

AddPostprocessorAction::AddPostprocessorAction(InputParameters params) :
    MooseObjectAction(params)
{
}

void
AddPostprocessorAction::act()
{
  _problem->addPostprocessor(_type, _name, _moose_object_pars);
}

