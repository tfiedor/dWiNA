/*****************************************************************************
 *  gaston - We pay homage to Gaston, an Africa-born brown fur seal who
 *    escaped the Prague Zoo during the floods in 2002 and made a heroic
 *    journey for freedom of over 300km all the way to Dresden. There he
 *    was caught and subsequently died due to exhaustion and infection.
 *    Rest In Piece, brave soldier.
 *
 *  Copyright (c) 2015  Tomas Fiedor <ifiedortom@fit.vutbr.cz>
 *      Notable mentions: Ondrej Lengal <ondra.lengal@gmail.com>
 *
 *  Description:
 *		Global header file for Gaston tool, containing options for
 *		enabling/disabling debug messages, optimizations, measuring
 *		and some globally used enumerations and using directives.
 *****************************************************************************/

#ifndef __DWINA_ENV__H__
#define __DWINA_ENV__H__

#include <boost/dynamic_bitset.hpp>
#include <boost/functional/hash.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <list>
#include <typeinfo>
#include <vata/bdd_bu_tree_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>
#include "../Frontend/dfa.h"
#include "utils/cached_binary_op.hh"
#include "mtbdd/ondriks_mtbdd.hh"

/*****************************
 * FORWARD CLASS DECLARATION *
 *****************************/
class SymbolicAutomaton;
class ZeroSymbol;
class Term;
class ASTForm;
template<class A, class B, class C, class D, void (*E)(A const&),void (*F)(B&)>
class BinaryCache;
template<class A>
class PairCompare;
template<class A>
class PrePairCompare;

struct ResultHashType;
struct SubsumptionHashType;
struct PreHashType;

/***********************
 * GLOBAL ENUMERATIONS *
 ***********************/
enum Decision {SATISFIABLE, UNSATISFIABLE, VALID, INVALID};
enum AutType {SYMBOLIC_BASE, BINARY, INTERSECTION, UNION, PROJECTION, BASE, COMPLEMENT};
enum TermType {TERM, TERM_EMPTY, TERM_FIXPOINT, TERM_PRODUCT, TERM_BASE, TERM_LIST, TERM_CONTINUATION};
enum ProductType {E_INTERSECTION, E_UNION};
enum FixpointTermSem {E_FIXTERM_FIXPOINT, E_FIXTERM_PRE};
enum ComparisonType {E_BY_SAME_PTR, E_BY_DIFFERENT_TYPE, E_BY_STRUCTURE};
enum UnfoldedInType {E_IN_SUBSUMPTION, E_IN_ISECT_NONEMPTY, E_IN_COMPARISON, E_IN_NOWHERE};
enum SubsumptionResult {E_FALSE, E_TRUE, E_PARTIALLY};
enum ExampleType {SATISFYING, UNSATISFYING};

namespace Gaston {
	void dumpResultKey(std::pair<Term*, ZeroSymbol*> const& s);
	void dumpResultData(std::pair<Term*, bool>& s);
	void dumpTermKey(Term* const& s);
	void dumpSubsumptionKey(std::pair<Term*, Term*> const& s);
	void dumpSubsumptionData(SubsumptionResult& s);
	void dumpPreKey(std::pair<size_t, ZeroSymbol*> const& s);
	void dumpPreData(VATA::Util::OrdVector<size_t>& s);

/***************************
 * GLOBAL USING DIRECTIVES *
 ***************************/
	using Automaton 			 = VATA::BDDBottomUpTreeAut;
	using Formula_ptr            = ASTForm*;
	using Term_ptr				 = Term*;
	using Term_raw 				 = Term*;
	using ResultType			 = std::pair<Term_ptr, bool>;

	using StateType				 = size_t;
	using StateTuple 			 = std::vector<StateType>;
	using StateToStateTranslator = VATA::AutBase::StateToStateTranslWeak;
	using StateToStateMap        = std::unordered_map<StateType, StateType>;

	using SymbolicAutomaton_ptr	 = std::shared_ptr<SymbolicAutomaton>;
	using SymbolicAutomaton_raw	 = SymbolicAutomaton*;

	using Symbol				 = ZeroSymbol;
	using Symbol_ptr			 = Symbol*;
	using SymbolList			 = std::list<Symbol>;

	using BitMask				 = boost::dynamic_bitset<>;
	using VarType				 = size_t;
	using VarList                = VATA::Util::OrdVector<StateType>;
	using VarValue			     = char;
	using TrackType				 = Automaton::SymbolType;

	using TermHash 				 = boost::hash<Term_raw>;
	using TermCompare			 = std::equal_to<Term_raw>;
	using TermCache				 = BinaryCache<Term_raw, SubsumptionResult, TermHash, TermCompare, dumpTermKey, dumpSubsumptionData>;
	using ResultKey				 = std::pair<Term_raw, Symbol_ptr>;
	using ResultCache            = BinaryCache<ResultKey, ResultType, ResultHashType, PairCompare<ResultKey>, dumpResultKey, dumpResultData>;
	using SubsumptionKey		 = std::pair<Term_raw, Term_raw>;
	using SubsumptionCache       = BinaryCache<SubsumptionKey, SubsumptionResult, SubsumptionHashType, PairCompare<SubsumptionKey>, dumpSubsumptionKey, dumpSubsumptionData>;

	using WorkListTerm           = Term;
	using WorkListTerm_raw       = Term*;
	using WorkListTerm_ptr       = Term_ptr;
	using WorkListSet            = std::vector<std::shared_ptr<WorkListTerm>>;

	using BaseAutomatonType      = DFA;
	using BaseAutomatonStateSet  = VATA::Util::OrdVector<StateType>;
	using BaseAutomatonMTBDD	 = VATA::MTBDDPkg::OndriksMTBDD<BaseAutomatonStateSet>;

	using PreKey				 = std::pair<StateType, Symbol_ptr>;
	using PreCache				 = BinaryCache<PreKey, BaseAutomatonStateSet, PreHashType, PrePairCompare<PreKey>, dumpPreKey, dumpPreData>;
}

/*************************
 * ADDITIONAL STRUCTURES *
 *************************/

class NotImplementedException : public std::exception {
public:
	virtual const char* what() const throw () {
		return "Functionality not implemented yet";
	}
};

class MonaFailureException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Mona Failed on BDDs\n";
	}
};

/****************
 * DEBUG MACROS *
 ****************/
#define G_DEBUG_FORMULA_AFTER_PHASE(str) std::cout << "\n\n[*] Formula after '" << str << "' phase:\n"
#define G_NOT_IMPLEMENTED_YET(str) assert(false && "TODO: '" str "' Not Supported yet")

/*********************************
 * OTHER METHODS RELATED DEFINES *
 *********************************/

/* >>> Debugging Options <<< *
 *****************************/
#define DEBUG_FORMULA_PREFIX 			false
#define DEBUG_VALIDITY_TEST 			false
#define DEBUG_GROUDNESS 				false
#define DEBUG_FINAL_STATES 				false
#define DEBUG_PRUNING_OF_FINAL_STATES 	false
#define DEBUG_VARIABLE_SETS 			false
#define DEBUG_BDDS 						true

/* >>> Optimizations <<< *
 *************************/
#define USE_PRUNED_UNION_FUNCTOR 		false
#define PRUNE_BY_RELATION 				false		// [TODO] What's the difference with BY_SUBSUMPTION?
#define PRUNE_BY_SUBSUMPTION 			false
#define USE_STATECACHE 					false
#define USE_BDDCACHE 					false 		// BDD Cache is temporary disable due to the memory leaks
#define SMART_BINARY 					true
#define SMART_FLATTEN 					true
#define CONSTRUCT_ALWAYS_DTA 			true

/***********************************
 * SYMBOLIC METHOD RELATED DEFINES *
 ***********************************/

/* >>> Printing Options <<< *
 ****************************/
#define PRINT_PRETTY					true

/* >>> Debugging Options <<< *
 *****************************/
#define DEBUG_AUTOMATA_ADDRESSES		false
#define DEBUG_EXAMPLE_PATHS				true
#define DEBUG_BASE_AUTOMATA 			false
#define DEBUG_FIXPOINT 					false
#define DEBUG_FIXPOINT_SYMBOLS			false
#define DEBUG_INITIAL_APPROX 			false
#define DEBUG_INTERSECT_NON_EMPTY 		false
#define DEBUG_TERM_UNIQUENESS			false
#define DEBUG_TERM_CREATION				false
#define DEBUG_SYMBOL_CREATION			true
#define DEBUG_CACHE_MEMBERS				true
#define DEBUG_WORKSHOPS					false
#define DEBUG_TERM_SUBSUMPTION 			false
#define DEBUG_TERM_CACHE_COMPARISON		false
#define DEBUG_CONTINUATIONS 			false
#define DEBUG_NO_WORKSHOPS				false
#define DEBUG_PRE						false
#define DEBUG_GENERATE_DOT_AUTOMATON	true
#define DEBUG_COMPUTE_FULL_FIXPOINT 	false
#define DEBUG_COMPARE_WORKLISTS			true
#define DEBUG_VARMAP					false

/*
 * >>> Automata stats options
 *****************************/
#define PRINT_STATS_PROJECTION			true
#define PRINT_STATS_PRODUCT				true
#define PRINT_STATS_NEGATION			true
#define PRINT_STATS_BASE				true
#define PRINT_STATS						false

/* >>> Dumping Options <<< *
 ***************************/
#define DUMP_NO_SYMBOL_TABLE			true
#define DUMP_INTERMEDIATE_AUTOMATA		true
#define DUMP_EXAMPLES					true

/* >>> Measuring Options <<< *
 *****************************/
#define MEASURE_STATE_SPACE 			true	// < Measures how many instances of terms were created
#define MEASURE_CACHE_HITS 				false	// < Prints the statistics for each cache on each node
#define MEASURE_CONTINUATION_CREATION	true	// < Measures how many continuations are created
#define MEASURE_CONTINUATION_EVALUATION	true	// < Measures how many continuations are actually unfolded
#define MEASURE_RESULT_HITS				true	// < Measure how many times the result hits in cache
#define MEASURE_SYMBOLS					true	// < Measure how many symbols are created
#define MEASURE_PROJECTION				true	// < Measures several things about projection (how many steps, how big, etc.)
#define MEASURE_POSTPONED				true	// < Measures how many terms are postponed and how many are processed
#define MEASURE_ALL						true	// < Measure everything, not really useful
#define MEASURE_COMPARISONS				false	// < Measure how many times we sucessfully compared and how
#define MEASURE_SUBSUMEDBY_HITS			true	// < Measure how many times subsumedBy cache worked

/* >>> Anti-Prenexing Options <<< *
 **********************************/
#define ANTIPRENEXING_FULL				false
#define ANTIPRENEXING_DISTRIBUTIVE		false

/*
 * >>> Unique Terms options *
 ****************************/
#define UNIQUE_BASE						true
#define UNIQUE_PRODUCTS					true
#define UNIQUE_LISTS					true
#define UNIQUE_FIXPOINTS				true
#define UNIQUE_CONTINUATIONS			true

/* >>> Other Options <<< *
 *************************/
#define AUT_ALWAYS_DETERMINISTIC		false
#define AUT_ALWAYS_CONSTRAINT_FO		true
#define AUT_CONSTRUCT_BY_MONA			true

/* >>> Optimizations <<< *
 *************************/
#define OPT_DONT_CACHE_CONT				false	// < Do not cache terms containing continuations
#define OPT_DONT_CACHE_UNFULL_FIXPOINTS false	// < Do not cache fixpoints that were not fully computed
#define OPT_EQ_THROUGH_POINTERS			true	// < Test equality through pointers, not by structure
#define OPT_GENERATE_UNIQUE_TERMS		true	// < Use Workshops to generate unique pointers
// ^- NOTE! From v1.0 onwards, disable this will introduce not only leaks, but will fuck everything up!
#define OPT_TERM_HASH_BY_APPROX			true	// < Include stateSpaceApprox into hash (i.e. better distribution of cache)
#define OPT_ANTIPRENEXING				true	// < Transform formula to anti-prenex form (i.e. all of the quantifiers are deepest on leaves)
#define OPT_DRAW_NEGATION_IN_BASE 		true	// < Negation is handled on formula level and not on computation level on base automata
#define OPT_CREATE_QF_AUTOMATON 		true	// < Transform quantifier-free automaton to formula
#define OPT_REDUCE_AUT_EVERYTIME		false	// < Call reduce everytime VATA automaton is created (i.e. as intermediate result)
#define OPT_REDUCE_AUT_LAST				true	// < Call reduce after the final VATA automaton is created
#define OPT_EARLY_EVALUATION 			true	// < Evaluates early interesection of products
#define OPT_EARLY_PARTIAL_SUB			true	// < Postpone the partially subsumed terms
#define OPT_CONT_ONLY_WHILE_UNSAT		true	// < Generate continuation only if there wasn't found (un)satisfying (counter)example yet
#define OPT_PRUNE_EMPTY					true	// < Prune terms by empty set
#define OPT_REDUCE_FIXPOINT_EVERYTIME	false	// < Prune the fixpoint everytime any iterator is invalidated
#define OPT_REDUCE_PREFIXPOINT			true	// < Prune the fixpoint when returning pre (i.e. fixpoint - symbol)
#define OPT_FIND_POSTPONED_CANDIDATE	true	// < Chose better candidate from list of postponed subsumption testing pairs
#define OPT_REDUCE_FULL_FIXPOINT		true	// < Prune the fixpoint by subsumption
#define OPT_CACHE_RESULTS 				true	// < Cache results of intersectnonempty(term, symbol)
#define OPT_CACHE_SUBSUMES				true	// < Cache the results of subsumption testing between terms
#define OPT_CACHE_SUBSUMED_BY			true	// < Cache the results of term subsumption by fixpoints
#define OPT_SMARTER_MONA_CONVERSION		false	// < Use faster conversion from MONA to VATA (courtesy of PJ)
#define OPT_CREATE_TAGGED_AUTOMATA		false	// < Use tags to create a specific subformula to automaton
#define OPT_EXTRACT_MORE_AUTOMATA		true	// < Calls detagger to heuristically convert some subformulae to automata
#define OPT_UNIQUE_TRIMMED_SYMBOLS		false	// < Will guarantee that there will not be a collisions between symbols after trimming

#endif
