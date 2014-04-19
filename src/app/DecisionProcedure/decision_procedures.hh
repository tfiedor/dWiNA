#ifndef __DEC_PROC__H__
#define __DEC_PROC__H__

// VATA headers
#include <vata/bdd_bu_tree_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>
#include <vata/util/binary_relation.hh>
#include <vata/util/aut_description.hh>
#include <vata/util/convert.hh>

// MONA headers
#include "../Frontend/ast.h"
#include "../Frontend/ident.h"

#include <deque>

#include "mtbdd/ondriks_mtbdd.hh"
#include "containers/VarToTrackMap.hh"
#include "containers/StateSet.hh"
#include "automata.hh"

extern VarToTrackMap varMap;

// < Module Typedefs >
typedef bool TSatExample;
typedef bool TUnSatExample;
typedef std::deque<unsigned int> VariableSet;
typedef std::deque<VariableSet> PrefixListType;

using Automaton = VATA::BDDBottomUpTreeAut;
using StateType = size_t;
using StateHT = std::unordered_set<StateType>;
using StateTuple = std::vector<StateType>;
using MTBDDLeafStateSet = VATA::Util::OrdVector<StateType>;
using TransMTBDD = VATA::MTBDDPkg::OndriksMTBDD<MTBDDLeafStateSet>;
typedef StateHT FinalStatesType;
typedef StateHT StateSetType;
typedef StateType MacroState; // For now

// < Module Functions >
int decideWS1S(Automaton aut, TSatExample & example, TUnSatExample & counterExample, PrefixListType formulaPrefixSet, PrefixListType negFormulaPrefixSet);
int decideWS2S(Automaton aut, TSatExample & example, TUnSatExample & counterExample);
TSatExample findSatisfyingExample();
TUnSatExample findUnsatisfyingExample();
bool existsSatisfyingExample(FinalStatesType fm);
bool existsUnsatisfyingExample(FinalStatesType fm, StateHT qm);
PrefixListType convertPrefixFormulaToList(ASTForm* formula);
void closePrefix(PrefixListType & prefix, IdentList* freeVars, bool negationIsTopmonst);
FinalStatesType computeFinalStates(Automaton aut);
inline void getMTBDDForStateTuple(const TransMTBDD* & bdd, Automaton aut, const StateTuple &);
const MTBDDLeafStateSet & getInitialStatesOfAutomaton(Automaton aut);

bool StateIsFinal(MacroState state, unsigned level);

#endif
