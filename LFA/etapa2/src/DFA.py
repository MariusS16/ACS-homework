from collections.abc import Callable
from dataclasses import dataclass
from itertools import product
# import pandas as pd
from typing import TypeVar
from functools import reduce

STATE = TypeVar('STATE')

@dataclass
class DFA[STATE]:
    S: set[str]
    K: set[STATE]
    q0: STATE
    d: dict[tuple[STATE, str], STATE]
    F: set[STATE]
    

    def accept(self, word: str) -> bool:
        cur_state = self.q0
        for symbol in word:
            if (cur_state, symbol) in self.d:
                cur_state = self.d[(cur_state, symbol)]
            else:
                return False
        return cur_state in self.F

    def remap_states[OTHER_STATE](self, f: Callable[[STATE], 'OTHER_STATE']) -> 'DFA[OTHER_STATE]':
        # optional, but might be useful for subset construction and the lexer to avoid state name conflicts.
        # this method generates a new dfa, with renamed state labels, while keeping the overall structure of the
        # automaton.

        # for example, given this dfa:

        # > (0) -a,b-> (1) ----a----> ((2))
        #               \-b-> (3) <-a,b-/
        #                   /     ⬉
        #                   \-a,b-/

        # applying the x -> x+2 function would create the following dfa:

        # > (2) -a,b-> (3) ----a----> ((4))
        #               \-b-> (5) <-a,b-/
        #                   /     ⬉
        #                   \-a,b-/

        pass
    
    
    def minimize(self) -> 'DFA[STATE]':
        states = list(self.K)
        n = len(states)
        index = {}
        stack = []
        for i in range(n):
            index[states[i]] = i

        # Initializez matricea cu indistinguishable
        matrix = []
        for i in range(n):
            row = [False] * n
            matrix.append(row)

        # Initializez matricea cu distinghishable pentru stari finale si non-finale
        for i in range(n):
            for j in range(i):
                state_i = states[i] in self.F
                state_j = states[j] in self.F
                if state_i != state_j:
                    matrix[i][j] = True
                    matrix[j][i] = True
                    stack.append((i, j))

        while stack:
            i, j = stack.pop()
            for symbol in self.S:
                pred_i = []
                pred_j = []

                for state in states:
                    if (state, symbol) in self.d:
                        if self.d[(state, symbol)] == states[i]:
                            pred_i.append(state)
                        if self.d[(state, symbol)] == states[j]:
                            pred_j.append(state)
                
                if len(pred_i) > 0 and len(pred_j) > 0:
                    for state in pred_i:
                        for state2 in pred_j:
                            if matrix[index[state]][index[state2]] == False:
                                stack.append((index[state], index[state2]))
                                matrix[index[state]][index[state2]] = True
                                matrix[index[state2]][index[state]] = True

        # Fiecare pereche de stare (i, j) care are matrix[i][j] == False sunt indistinguishable si pot fi unite
        new_states = {}
        for i in range(n):
            ok = 0              #in caz ca nu gasesc o echivalenta
            for j in range(i):
                if matrix[i][j] == False:
                    if states[j] in new_states:
                        new_states[states[i]] = new_states[states[j]]
                    ok = 1
                    break
            if ok == 0:
                new_states[states[i]] = states[i]

        
        # Construiesc noile tranzitii
        d2 = {}
        for (state, symbol), dest in self.d.items():
            d2[(new_states[state], symbol)] = new_states[dest]

        # Construiesc noile stari finale
        F2 = set()
        for state in self.F:
            F2.add(new_states[state])
    
        return DFA(
            S=self.S,
            K=set(new_states.values()),
            q0=new_states[self.q0],
            d=d2,
            F=F2
        )
        
        
        