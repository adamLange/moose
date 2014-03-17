[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 4
  ny = 8
  xmax = 0.1
  ymax = 0.5
[]

[Variables]
  [./u]
    initial_condition = 1
  [../]
[]

[AuxVariables]
  [./multi_layered_average]
  [../]
  [./element_multi_layered_average]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Functions]
  [./axial_force]
    type = ParsedFunction
    value = 1000*y
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
  [./td]
    type = TimeDerivative
    variable = u
  [../]
  [./force]
    type = UserForcingFunction
    variable = u
    function = axial_force
  [../]
[]

[BCs]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 1
  dt = 0.001

  # Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  output_initial = true
  exodus = true
  [./console]
    type = Console
    perf_log = true
  [../]
[]

[Problem]
  coord_type = rz
  type = FEProblem
[]