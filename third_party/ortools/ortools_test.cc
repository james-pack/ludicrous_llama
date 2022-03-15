#include "glog/logging.h"
#include "gtest/gtest.h"
#include "ortools/constraint_solver/constraint_solver.h"
#include "ortools/linear_solver/linear_solver.h"
#include "ortools/sat/cp_model.h"

namespace {

namespace ortools = operations_research;

TEST(ORTools, CanCompile) {
  // Instantiate the solver.
  ortools::Solver solver("CpSimple");

  // Create the variables.
  constexpr int64_t num_vals = 3;
  ortools::IntVar* const x = solver.MakeIntVar(0, num_vals - 1, "x");
  ortools::IntVar* const y = solver.MakeIntVar(0, num_vals - 1, "y");
  ortools::IntVar* const z = solver.MakeIntVar(0, num_vals - 1, "z");

  // Constraint 0: x != y..
  solver.AddConstraint(solver.MakeAllDifferent({x, y}));
  EXPECT_EQ(1, solver.constraints());

  // Solve the problem.
  ortools::DecisionBuilder* const db =
      solver.MakePhase({x, y, z}, ortools::Solver::CHOOSE_FIRST_UNBOUND, ortools::Solver::ASSIGN_MIN_VALUE);

  solver.NewSearch(db);
  while (solver.NextSolution()) {
    // Iterate the solutions. The solver.solutions() call below only counts the solutions that are iterated.
  }
  solver.EndSearch();
  EXPECT_EQ(18, solver.solutions());
}

TEST(ORTools, CanRunBasicExample) {
  ortools::sat::CpModelBuilder cp_model;

  const ortools::Domain domain(0, 2);
  const ortools::sat::IntVar x = cp_model.NewIntVar(domain).WithName("x");
  const ortools::sat::IntVar y = cp_model.NewIntVar(domain).WithName("y");
  const ortools::sat::IntVar z = cp_model.NewIntVar(domain).WithName("z");

  cp_model.AddNotEqual(x, y);
  cp_model.AddNotEqual(x, z);
  cp_model.AddNotEqual(y, z);

  const ortools::sat::CpSolverResponse response = Solve(cp_model.Build());
  EXPECT_EQ(ortools::sat::CpSolverStatus::OPTIMAL, response.status());

  EXPECT_NE(SolutionIntegerValue(response, x), SolutionIntegerValue(response, y));
  EXPECT_NE(SolutionIntegerValue(response, y), SolutionIntegerValue(response, z));
  EXPECT_NE(SolutionIntegerValue(response, z), SolutionIntegerValue(response, x));
  EXPECT_EQ(3,
            SolutionIntegerValue(response, x) + SolutionIntegerValue(response, y) + SolutionIntegerValue(response, z));
}

TEST(ORTools, CanMaximizeSimpleObjective) {
  // Create the linear solver with the GLOP backend.
  std::unique_ptr<ortools::MPSolver> solver(ortools::MPSolver::CreateSolver("GLOP"));

  // Create the variables x and y with x bound to [0, 1] and y bound to [0, 2].
  ortools::MPVariable* const x = solver->MakeNumVar(0.f, 1.f, "x");
  ortools::MPVariable* const y = solver->MakeNumVar(0.f, 2.f, "y");

  EXPECT_EQ(2, solver->NumVariables());

  // Create a linear constraint, 0 <= x + y <= 2.
  ortools::MPConstraint* const ct = solver->MakeRowConstraint(0.0, 2.0, "ct");
  ct->SetCoefficient(x, 1);
  ct->SetCoefficient(y, 1);

  EXPECT_EQ(1, solver->NumConstraints());

  // Create the objective function, 3 * x + y.
  ortools::MPObjective* const objective = solver->MutableObjective();
  objective->SetCoefficient(x, 3);
  objective->SetCoefficient(y, 1);
  objective->SetMaximization();

  solver->Solve();

  EXPECT_EQ(4.f, objective->Value());
  EXPECT_EQ(1.f, x->solution_value());
  EXPECT_EQ(1.f, y->solution_value());
}

TEST(ORTools, CanConstrainOnFloats) {
  // Create the linear solver with the GLOP backend.
  std::unique_ptr<ortools::MPSolver> solver(ortools::MPSolver::CreateSolver("GLOP"));

  // Create the variables x and y with x bound to [0, 1] and y bound to [0, 2].
  ortools::MPVariable* const x = solver->MakeNumVar(0.9f, 1.f, "x");
  ortools::MPVariable* const y = solver->MakeNumVar(1.f, 2.f, "y");

  EXPECT_EQ(2, solver->NumVariables());

  // Create a linear constraint, 1.9 <= x + y <= 2.
  ortools::MPConstraint* ct = solver->MakeRowConstraint(1.9, 2.0, "ct");
  ct->SetCoefficient(x, 1);
  ct->SetCoefficient(y, 1);

  EXPECT_EQ(1, solver->NumConstraints());

  solver->Solve();

  EXPECT_GE(x->solution_value(), 0.f);
  EXPECT_LE(x->solution_value(), 1.f);
  EXPECT_GE(x->solution_value(), 0.f);
  EXPECT_LE(y->solution_value(), 2.f);

  EXPECT_GE(x->solution_value() + y->solution_value(), 1.9f);
  EXPECT_LE(x->solution_value() + y->solution_value(), 2.f);
}

TEST(ORTools, CanChooseFromMultipleExclusiveOptions) {
  // This test verifies that we can find a way to frame a problem that involves choosing from a set of discrete options.
  // For example, choose one part from many that would work and fulfill the requirements. Note that in some problems,
  // there may not be any particular criteria to optimize or otherwise guide the choice.

  std::unique_ptr<ortools::MPSolver> solver(ortools::MPSolver::CreateSolver("GLOP"));

  ortools::MPVariable* const part_1 = solver->MakeIntVar(0.f, 5.f, "part_1");
  ortools::MPVariable* const part_2 = solver->MakeIntVar(3.f, 9.f, "part_2");

  EXPECT_EQ(2, solver->NumVariables());

  // Values of part_1 and part_2 must be the equal:
  ortools::MPConstraint* ct = solver->MakeRowConstraint(0.f, 0.f, "ct2");
  ct->SetCoefficient(part_1, 1.f);
  ct->SetCoefficient(part_2, -1.f);

  EXPECT_EQ(1, solver->NumConstraints());

  solver->Solve();

  EXPECT_EQ(part_1->solution_value(), part_2->solution_value());
}

TEST(ORTools, WillChooseStableAnswerBasedOnHint) {
  // Verify that we can use hints and other tactics to ensure that the solution changes as little as possible between
  // iterations. If a user changes one value, we want to minimize the amount the overall solution changes, as long as
  // the constraints are satisified. Note that this seems to only work with some solvers, not all. In particular, SCIP
  // appears to provide stable solutions using this technique, but GLOP does not. Other techniques, like directly
  // populating the protos or using the DecisionBuilder interface, may allow for stable solutions with other solvers.
  std::unique_ptr<ortools::MPSolver> solver(ortools::MPSolver::CreateSolver("SCIP"));

  ortools::MPVariable* const part_1 = solver->MakeIntVar(0.f, 4.f, "part_1");
  ortools::MPVariable* const part_2 = solver->MakeIntVar(3.f, 9.f, "part_2");

  // Values of part_1 and part_2 must be the equal:
  ortools::MPConstraint* ct = solver->MakeRowConstraint(0.f, 0.f, "ct");
  ct->SetCoefficient(part_1, 1.f);
  ct->SetCoefficient(part_2, -1.f);

  solver->Solve();

  const double prev_solution_value{part_1->solution_value()};
  // Modify the upper bound, but look for same solution as before.
  part_1->SetUB(5.f);
  part_2->SetLB(2.f);

  ortools::MPVariable* const part_3 = solver->MakeIntVar(0.f, 10.f, "part_3");
  ortools::MPConstraint* ct2 = solver->MakeRowConstraint(0.f, 0.f, "ct2");
  ct2->SetCoefficient(part_1, -2.f);
  ct2->SetCoefficient(part_3, 1.f);

  std::vector<std::pair<const ortools::MPVariable*, double>> hints{};
  hints.emplace_back(part_1, prev_solution_value);
  hints.emplace_back(part_2, prev_solution_value);
  solver->SetHint(hints);

  ortools::MPSolverParameters parameters{};
  parameters.SetIntegerParam(ortools::MPSolverParameters::IntegerParam::INCREMENTALITY,
                             ortools::MPSolverParameters::IncrementalityValues::INCREMENTALITY_ON);
  solver->Solve(parameters);

  EXPECT_EQ(prev_solution_value, part_1->solution_value());
  EXPECT_EQ(part_1->solution_value(), part_2->solution_value());
  EXPECT_EQ(2.f * part_1->solution_value(), part_3->solution_value());
}

}  // namespace
