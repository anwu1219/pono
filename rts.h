#ifndef RELATIONAL_TRANSITION_SYSTEM_H
#define RELATIONAL_TRANSITION_SYSTEM_H

#include <unordered_map>

#include "smt.h"

#include "fts.h"

namespace cosa
{

class RelationalTransitionSystem : public FunctionalTransitionSystem
{
 public:
   RelationalTransitionSystem(smt::SmtSolver & s)
     : FunctionalTransitionSystem(s), trans_(s->make_value(true)) {}

  /* Add constraint to the system
   * This is an invariant constraint, enforced over all time
   * Overloaded to add directly to trans_ instead of constraints_
   *   (constraints_ is unused for relational transition system)
   * @param constraint the boolean constraint term to add
   */
  void add_constraint(const smt::Term constraint);

  /* Sets init and trans to the provided values
   * @param init the new initial state constraints (boolean sort)
   * @param trans the new transition relation constraints (boolean sort)
  */
  void set_behavior(const smt::Term init, const smt::Term trans);

  /* Sets transition relation to the provided formula
   * @param trans the new transition relation
   */
  void set_trans(const smt::Term trans);

  /* Add to the initial state constraints
   * @param constraint new constraint on initial states
   */
  void constrain_init(const smt::Term constraint);

  /* Add to the transition relation constraints
   * @param constraint new constraint on transition relation
   */
  void constrain_trans(const smt::Term constraint);

  /* Map all next state variables to current state variables in the term
   * @param t the term to map
   * @return the term with all current state variables
   */
  smt::Term curr(const smt::Term term);

  /* Map all current state variables to next state variables in the term
   * @param t the term to map
   * @return the term with all next state variables
   */
  smt::Term next(const smt::Term term);

  /* @param sv the state variable to check
   * @return true if sv is a current state variable
   *
   * Returns false for any other term
   */
  bool is_curr_var(const smt::Term sv);

  /* @param sv the state variable to check
   * @return true if sv is a next state variable
   *
   * Returns false for any other term
   */
  bool is_next_var(const smt::Term sv);

  // getters

  /* Returns the transition relation
   * @return a boolean term representing the transition relation
   */
  smt::Term trans() { return trans_; };

  // overloaded
  smt::Term make_state(const std::string name, const smt::Sort sort);

 protected:
  smt::Term trans_;
  smt::UnorderedTermSet next_states_;
  smt::UnorderedTermMap states_map_;
  smt::UnorderedTermMap next_states_map_;

  // helpers and checkers

  /* Returns true iff all the symbols in the formula are current states or inputs */
  bool no_next(smt::Term term);

  // overloaded
  bool known_symbols(const smt::Term term);

};

}

#endif