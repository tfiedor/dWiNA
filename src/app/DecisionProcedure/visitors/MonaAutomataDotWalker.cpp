//
// Created by Raph on 27/01/2016.
//

#include "MonaAutomataDotWalker.h"
#include "DotWalker.h"
#include "../../Frontend/dfa.h"
#include "../automata.hh"

TimeType MonaAutomataDotWalker::_constructAutomaton(ASTForm* form) {
    DFA *monaAutomaton = nullptr;
    toMonaAutomaton(form, monaAutomaton, true);
    assert(monaAutomaton != nullptr);
    size_t monaStates = monaAutomaton->ns;

    /*DFA *temp = dfaCopy(monaAutomaton);
    dfaUnrestrict(temp);
    monaAutomaton = dfaMinimize(temp);

    // Clean up
    dfaFree(temp);*/
    size_t minimizedStates = monaAutomaton->ns;

    dfaFree(monaAutomaton);
    return std::make_pair(minimizedStates, monaStates);
}

void MonaAutomataDotWalker::visit(ASTForm_And* form) {
    TimeType automatonSize = this->_constructAutomaton(form);
    this->_dotFile << "\t" << (uintptr_t) form << " [label=\"\u2227\\n" << std::to_string(automatonSize.first) << "/" << std::to_string(automatonSize.second) << "\"];\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f1 << ";\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f2 << ";\n";
}

void MonaAutomataDotWalker::visit(ASTForm_Or* form) {
    TimeType automatonSize = this->_constructAutomaton(form);
    this->_dotFile << "\t" << (uintptr_t) form << " [label=\"\u2228\\n" << std::to_string(automatonSize.first) << "/" << std::to_string(automatonSize.second) << "\"];\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f1 << ";\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f2 << ";\n";
}

void MonaAutomataDotWalker::visit(ASTForm_Impl* form) {
    TimeType automatonSize = this->_constructAutomaton(form);
    this->_dotFile << "\t" << (uintptr_t) form << " [label=\"\u21D2\\n" << std::to_string(automatonSize.first) << "/" << std::to_string(automatonSize.second) << "\"];\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f1 << ";\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f2 << ";\n";
}

void MonaAutomataDotWalker::visit(ASTForm_Biimpl* form) {
    TimeType automatonSize = this->_constructAutomaton(form);
    this->_dotFile << "\t" << (uintptr_t) form << " [label=\"\u21D4\\n" << std::to_string(automatonSize.first) << "/" << std::to_string(automatonSize.second) << "\"];\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f1 << ";\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f2 << ";\n";
}

void MonaAutomataDotWalker::visit(ASTForm_Not* form) {
    TimeType automatonSize = this->_constructAutomaton(form);
    this->_dotFile << "\t" << (uintptr_t) form << " [label=\"\u00AC\\n" << std::to_string(automatonSize.first) << "/" << std::to_string(automatonSize.second) << "\"];\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f << ";\n";
}

template<class ExistClass>
void MonaAutomataDotWalker::_existsToDot(ExistClass* form) {
    TimeType automatonSize = this->_constructAutomaton(form);
    this->_dotFile << "\t" << (uintptr_t) form << " [label=\"\u2203";
    for(auto it = form->vl->begin(); it != form->vl->end(); ++it) {
        this->_dotFile << (*it) << ", ";
    }
    this->_dotFile << "\\n" << std::to_string(automatonSize.first) << "/" << std::to_string(automatonSize.second) << "\"];\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f << ";\n";
}

void MonaAutomataDotWalker::visit(ASTForm_Ex1* form) {
    this->_existsToDot<ASTForm_Ex1>(form);
}
void MonaAutomataDotWalker::visit(ASTForm_Ex2* form) {
    this->_existsToDot<ASTForm_Ex2>(form);
}

template<class ForallClass>
void MonaAutomataDotWalker::_forallToDot(ForallClass* form) {
    TimeType automatonSize = this->_constructAutomaton(form);
    this->_dotFile << "\t" << (uintptr_t) form << " [label=\"\u2200";
    for(auto it = form->vl->begin(); it != form->vl->end(); ++it) {
        this->_dotFile << (*it) << ", ";
    }
    this->_dotFile << "\\n" << std::to_string(automatonSize.first) << "/" << std::to_string(automatonSize.second) << "\"];\n";
    this->_dotFile << "\t" << (uintptr_t) form << " -- " << (uintptr_t) form->f << ";\n";
}

void MonaAutomataDotWalker::visit(ASTForm_All1* form) {
    this->_forallToDot<ASTForm_All1>(form);
}

void MonaAutomataDotWalker::visit(ASTForm_All2* form) {
    this->_forallToDot<ASTForm_All2>(form);
}

void MonaAutomataDotWalker::_atomicToDot(ASTForm* form) {
    TimeType automatonSize = this->_constructAutomaton(form);
    this->_dotFile << "\t" << (uintptr_t) form << " [label=\"" << form->ToString() << "\\n" << std::to_string(automatonSize.first) << "/" << std::to_string(automatonSize.second) << "\"];\n";
}


void MonaAutomataDotWalker::visit(ASTForm_True* form) {
    this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_False* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_In* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_Notin* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_RootPred* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_EmptyPred* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_FirstOrder* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_Sub* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_Equal1* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_Equal2* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_NotEqual1* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_NotEqual2* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_Less* form) {
	this->_atomicToDot(form);
}

void MonaAutomataDotWalker::visit(ASTForm_LessEq* form) {
	this->_atomicToDot(form);
}
